import clang.cindex as cc
from clang.cindex import CursorKind as CK
from glob import glob
import re
import json
import api_generator.error as error
from api_generator.parser_types import *

GLOB_BLACK_LIST = (
    "api_sym_lookup_table.h",
    "extra/full_trace.h"
)



def parse(folder: str) -> ParseResult:
    args = generate_args(folder)

    result = ParseResult()
    for filename in glob(folder+'/**/*.h', recursive=True):
        f = filename[len(folder)+1:]

        if f in GLOB_BLACK_LIST:
            continue

        index = cc.Index.create()
        translation_unit = index.parse(filename, args=args)

        for cursor in translation_unit.cursor.get_children():
            r = parse_function_with_comments(cursor, filename)
            if r:
                result.extend(r)

        r = manual_parse(filename)
        if r:
            result.extend(r)


    #print(json.dumps(dict(result), indent=4))
    return result


def generate_args(folder: str) -> list:
    args = [
        '-x', 'c',  # Язык - C
        '-std=c99',  # Стандарт C
        # '-Wno-everything',  # Игнорируем предупреждения,
        #"-I/home/ghost/.xmake/packages/l/libsdl3/3.2.16/ded4ef7c20ba41f8992385c2a98a6623/include", "-I/home/ghost/.xmake/packages/o/opengl-headers/2024.01.04/59ed79d7030a4b298d033d457b08e5ff/include", "-I/home/ghost/.xmake/packages/e/egl-headers/2023.12.16/cd777b21b8c84268bf5e21c2813eae03/include",
        "-Isrc", "-Ideps",
        "-DHE_FS_SDL3", "-DHE_WS_SDL3", "-DHE_MEM_TRACK", "-DHE_MEM_TRACK_TRACE"
    ]
    return args


def parse_function_with_comments(cursor: cc.Cursor, filename: str):
    cursor_filename = cursor.location.file
    if cursor_filename == None or str(cursor_filename) != filename:
        return

    comment = cursor.raw_comment
    if comment == None:
        return

    doxygen = doxygen_comment_parse(comment)

    # print(doxygen)

    api_list = [d[1] for d in doxygen if d[0] == "api"]
    if len(api_list) == 0:
        return
    
    if len(api_list) != 1:
        error.error(f"Function {cursor.spelling} [{filename}:{cursor.location.line}] has multiple api tags")
        return
    
    api_config_list = [d[1] for d in doxygen if d[0] == "api_config"]
    if len(api_config_list) > 1:
        error.error(f"Function {cursor.spelling} [{filename}:{cursor.location.line}] has multiple api_config tags")
        return
    
    api_config = []
    if len(api_config_list) != 0:
        try:
            api_config = json.loads(api_config_list[0])
        except:
            error.error(f"Function {cursor.spelling} [{filename}:{cursor.location.line}] has invalid api_config tag")

    result = ParseResult()
    if cursor.kind == CK.FUNCTION_DECL:
        result.add_function(parse_function_decl(cursor, comment))
        return result
    
    elif cursor.kind == CK.STRUCT_DECL:
        s = parse_struct_decl(cursor, api_list[0], api_config)
        if s:
            result.extend(s)
        return result
    
    elif cursor.kind == CK.TYPEDEF_DECL:
        s = parse_typedef_decl(cursor)
        if s:
            result.extend(s)
        return result



def parse_function_decl(cursor: cc.Cursor, doc: str) -> Function:
    func_name = cursor.spelling
    return_type = cursor.result_type.spelling
    args: list[FunctionArg] = []

    # Парсим параметры
    for arg in cursor.get_arguments():
        arg_type = arg.type.spelling
        awrg_name = arg.spelling or ""
        args.append(FunctionArg(arg_type, awrg_name))

    return Function(func_name, return_type, args, str(cursor.location.file), int(cursor.location.line), doc)

def parse_struct_decl(cursor: cc.Cursor, generator_mode: str, config: dict) -> ParseResult:
    result = ParseResult()
    struct_name = cursor.spelling
    
    if generator_mode == "":
        result.add_struct(Struct(struct_name, str(cursor.location.file), int(cursor.location.line), cursor.raw_comment))
    elif generator_mode == "forward":
        #error.warn(f"[{cursor.location.file}:{cursor.location.line}] Forward declaration not supported yet")
        fields = []
        for child in cursor.get_children():
            if child.kind == CK.FIELD_DECL:
                field_type = child.type.spelling
                field_name = child.spelling
                fields.append(StructForwardField(field_name, field_type))
        result.add_forward_struct(StructForward(struct_name, fields, str(cursor.location.file), int(cursor.location.line), cursor.raw_comment))
    elif generator_mode == "server":
        result.add_struct(Struct(struct_name, str(cursor.location.file), int(cursor.location.line), cursor.raw_comment))

        fn_prefix = config.get("fn_prefix")
        if fn_prefix == None:
            error.error(f"[{cursor.location.file}:{cursor.location.line}] Missing fn_prefix in api_config")
        
        init_method = config.get("init_method")
        if init_method == None:
            error.error(f"[{cursor.location.file}:{cursor.location.line}] Missing init_method in api_config")
        
        global_server_backend_var = config.get("global_server_backend_var")
        if global_server_backend_var == None:
            error.error(f"[{cursor.location.file}:{cursor.location.line}] Missing global_server_backend_var in api_config")
        
        functions = []

        for child in cursor.get_children():
            if child.kind == CK.FIELD_DECL:
                field_type = child.type
                
                # Проверка на указатель на функцию
                if field_type.kind == cc.TypeKind.POINTER:
                    pointee = field_type.get_pointee()
                    if pointee.kind == cc.TypeKind.FUNCTIONPROTO or pointee.kind == cc.TypeKind.FUNCTIONNOPROTO:
                        fn_name = child.spelling
                        return_type = pointee.get_result().spelling
                        doc = child.raw_comment or ""
                        filename = str(child.location.file)
                        line = int(child.location.line)
                        args = []
                        
                        
                        # Получаем аргументы
                        for arg in child.get_children():
                            if arg.kind != cc.CursorKind.PARM_DECL:
                                continue
                            args.append(FunctionArg(arg.type.spelling, arg.spelling))

                        functions.append(Function(fn_name, return_type, args, filename, line, doc))

        result.add_server(Server(struct_name, fn_prefix, init_method, global_server_backend_var, functions, str(cursor.location.file), int(cursor.location.line), doc))
    else:
        error.error(f"[{cursor.location.file}:{cursor.location.line}] Unknown generator mode: {generator_mode}")

    return result
    #return Struct(struct_name, api_data, str(cursor.location.file), int(cursor.location.line))


def parse_typedef_decl(cursor: cc.Cursor) -> ParseResult:
    result = ParseResult()

    children = list(cursor.get_children())

    if len(children) == 0:
        result.add_typedef(Typedef(cursor.spelling, cursor.underlying_typedef_type.spelling, str(cursor.location.file), int(cursor.location.line), cursor.raw_comment))
    elif len(children) == 1:
        if children[0].kind == CK.STRUCT_DECL:
            return result
        
        if cursor.underlying_typedef_type.spelling.startswith("struct"):
            result.add_struct(Struct(cursor.spelling, str(cursor.location.file), int(cursor.location.line), cursor.raw_comment))
        else:
            result.add_typedef(Typedef(cursor.spelling, cursor.underlying_typedef_type.spelling, str(cursor.location.file), int(cursor.location.line), cursor.raw_comment))
    else:
        error.error(f"Invalid typedef {cursor.spelling} [{cursor.location.file}:{cursor.location.line}]")

    return result



def manual_parse(filename: str) -> ParseResult:
    result = ParseResult()

    code = open(filename).read()
    
    # Api enums /* API ENUM { json } */
    re_enums = re.findall(r"\s*\/\*\s*API\s+ENUM\s+({[\s\w\d=\'\"\,\.\[\]\{\}\\\/&\*=\/+-:;]*})\s*\*\/", code, flags=re.MULTILINE)

    for enum in re_enums:
        try:
            j = json.loads(enum)
            
            name = j["name"]
            if name == None:
                error.error(f"Invalid api enum in {filename}, missing name : {enum}")
            
            data_type = j["type"]
            if data_type == None:
                error.error(f"Invalid api enum in {filename}, missing type : {enum}")
            
            values = j["values"]
            if values == None:
                error.error(f"Invalid api enum in {filename}, missing values : {enum}")

            r_values = []
            for value in values:
                if len(value) != 2:
                    error.error(f"Invalid api enum in {filename}, invalid value : {enum}")

                r_values.append(ApiEnumValue(value[0], value[1]))

            result.add_api_enum(ApiEnum(name, data_type, r_values, str(filename), 0))
                
        except json.JSONDecodeError as exception:
            error.error(f"Invalid api enum in {filename} : {enum}\n{exception}")


    return result





def doxygen_comment_parse(comment: str):
    clean_comment = re.sub(r'^\s*/\*+|\s*\*/\s*$', '',
                           comment, flags=re.MULTILINE)

    blocks = []
    tmp_arr = []
    for line in clean_comment.splitlines():
        l = re.sub(r"^\s*\**\s*|\s*$", '', line, flags=re.MULTILINE).strip()

        if l == "":
            if len(tmp_arr) > 0:
                blocks.append(tmp_arr)
                tmp_arr = []
        else:
            tmp_arr.append(l)

    if len(tmp_arr) > 0:
        blocks.append(tmp_arr)

    # Если линия не начинается с символа @, то она должна конкатенироваться с предыдущей
    lines = []
    for block in blocks:
        tmp_str = ""
        for line in block:
            if line.startswith("@"):
                if tmp_str != "":
                    lines.append(tmp_str)
                # lines.append(line)
                tmp_str = line
            else:
                tmp_str += " " + line

        if tmp_str != "":
            lines.append(tmp_str)

    result = []
    for line in lines:
        matches = re.findall(r"^@(\S+)([ \t]+([^\n]*))?$", line)
        # arr = result.get(matches[0][0], [])
        # arr.append(matches[0][2])
        # result[matches[0][0]] = arr
        if len(matches) > 0:
            result.append((matches[0][0], matches[0][2]))
        else:
            result.append(("", line.strip()))

    return result

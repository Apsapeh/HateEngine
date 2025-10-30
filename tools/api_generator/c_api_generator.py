from api_generator.parser_types import *
from . import general
from glob import glob

API_HEADER = """
#pragma once
// clang-format off
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define PUBLIC __attribute__((dllexport))
    #else
      #define PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define PUBLIC __attribute__((dllimport))
    #else
      #define PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define LOCAL
#else
  #if __GNUC__ >= 4
    #define PUBLIC __attribute__((visibility("default")))
    #define LOCAL  __attribute__((visibility("hidden")))
  #else
    #define PUBLIC
    #define LOCAL
  #endif
#endif

INCLUDE_TYPES

INCLUDE_ERROR

INCLUDE_MACROS

TYPES

#if defined(HEAPI_COMPILE_TIME)

#error "HEAPI_COMPILE_TIME not supported yet"
CT_FN_DECL

#else

RAW_FN_PTRS_DECL

#if !defined(HEAPI_FULL_TRACE)
    NORM_FN_PTRS_DECL
#endif

#if defined(HEAPI_LOAD_IMPL)
    RAW_FN_PTRS_IMPL

    #if !defined(HEAPI_FULL_TRACE)
        NORM_FN_PTRS_IMPL
    #endif

    PUBLIC void ___hate_engine_runtime_init(void* (*proc_addr)(const char* name)) {
        RAW_FN_PTRS_LOAD

        #if !defined(HEAPI_FULL_TRACE)
            NORM_FN_PTRS_LOAD
        #endif
    }

SERVERS_INIT
#endif
#endif

#if defined(HEAPI_FULL_TRACE)
TRACE_DECL

#if defined(HEAPI_LOAD_IMPL)
    TRACE_IMPL
#endif

TRACE_DEFINES
#endif

// clang-format on

"""


def run(data: ParseResult):
    api_header = API_HEADER

    types = ""
    for struct in data.structs:
        if struct.doc != "":
            types += f"{struct.doc}\n"
        name = struct.name
        if name.endswith(" *"):
            name = name[:-2]
        types += f"typedef struct {struct.name} {name};\n\n"
        pass

    for forward_struct in data.forward_structs:
        if forward_struct.doc != "":
            types += f"{forward_struct.doc}\n"
        types += f"typedef struct {forward_struct.name} {{\n"
        for field in forward_struct.fields:
            types += f"    {general.get_correct_variable(field.type, field.name)};\n"
        types += f"}} {forward_struct.name};\n\n"
        pass

    for typedef in data.typedefs:
        if typedef.doc != "":
            types += f"{typedef.doc}\n"
        types += f"typedef {general.get_correct_variable(typedef.type, typedef.name)};\n\n"
        pass

    for typedef_function in data.typedef_functions:
        if typedef_function.doc != "":
            types += f"{typedef_function.doc}\n"
        args = ", ".join([general.get_correct_variable(arg._type, arg.name) for arg in typedef_function.args])
        if args == "":
            args = "void"
        types += f"typedef {typedef_function.return_type} (*{typedef_function.name})({args});\n\n"
        pass

    for typedef_struct_pointer in data.typedef_struct_pointers:
        if typedef_struct_pointer.doc != "":
            types += f"{typedef_struct_pointer.doc}\n"
        types += f"typedef struct {typedef_struct_pointer.name}* {typedef_struct_pointer.name};\n\n"
        pass

    for enum in data.api_enums:
        for value in enum.values:
            name = camel_to_upper_snake_case(enum.name + value.name)
            types += f"#define {name} {value.value}\n\n"
            pass


    fn_ptrs_decl = ""
    fn_ptrs_impl = ""
    fn_ptrs_load = ""
    norm_fn_ptrs_decl = ""
    norm_fn_ptrs_impl = ""
    norm_fn_ptrs_load = ""
    trace_decl = ""
    trace_impl = ""
    trace_defines = ""
    for function in data.functions:
        if function.doc != "":
            fn_ptrs_decl += f"{function.doc}\n"
            norm_fn_ptrs_decl += f"{function.doc}\n"

        args = ", ".join([general.get_correct_variable(arg._type, arg.name) for arg in function.args])
        if args == "":
            args = "void"

        function_name = "raw_" + function.name
        fn_ptrs_decl += f"extern {function.return_type} (*{function_name})({args});\n\n"
        fn_ptrs_impl += f"    {function.return_type} (*{function_name})({args});\n"
        cast_args = ", ".join([f"{arg._type}" for arg in function.args])
        if cast_args == "":
            cast_args = "void"
        fn_ptrs_load += f"        {function_name} = ({function.return_type} (*)({cast_args}))proc_addr(\"{function.name}\");\n"

        norm_fn_ptrs_decl += f"extern {function.return_type} (*{function.name})({args});\n\n"
        norm_fn_ptrs_impl += f"    {function.return_type} (*{function.name})({args});\n"
        norm_fn_ptrs_load += f"            {function.name} = {function_name};\n"

        if function.name == "__he_update_full_trace_info":
            continue

        trace_impl += full_trace_impl_generator(function.return_type, function.name, function.args)
        trace_decl += full_trace_header_generator(function.return_type, function.name, function.args)
        trace_defines += full_trace_define_generator(function.name, function.args)

        


    servers_init = ""
    for server in data.servers:
        servers_init += f"    PUBLIC void {server.init_method}({server.name}* backend) {{\n"

        norm_methods_init = ""

        for method in server.methods:
            if method.doc != "":
                fn_ptrs_decl += f"{method.doc}\n"
                norm_fn_ptrs_decl += f"{method.doc}\n"

            args = ", ".join([general.get_correct_variable(arg._type, arg.name) for arg in method.args])
            if args == "":
                args = "void"

            method_name = "raw_" + server.fn_prefix + method.name
            fn_ptrs_decl += f"extern {method.return_type} (*{method_name})({args});\n\n"
            fn_ptrs_impl += f"    {method.return_type} (*{method_name})({args});\n"

            norm_method_name = server.fn_prefix + method.name
            norm_fn_ptrs_decl += f"extern {method.return_type} (*{norm_method_name})({args});\n\n"
            norm_fn_ptrs_impl += f"    {method.return_type} (*{norm_method_name})({args});\n"

            cast_args = ", ".join([f"{arg._type}" for arg in method.args])
            if cast_args == "":
                cast_args = "void"
        
            servers_init += f"        {method_name} = ({method.return_type} (*)({cast_args}))raw_{server.fn_prefix}backend_get_function(backend, \"{method.name}\");\n"
            norm_methods_init += f"            {norm_method_name} = {method_name};\n"


            trace_impl += full_trace_impl_generator(method.return_type, norm_method_name, method.args)
            trace_decl += full_trace_header_generator(method.return_type, norm_method_name, method.args)
            trace_defines += full_trace_define_generator(norm_method_name, method.args)

        servers_init += f"        #if !defined(HEAPI_FULL_TRACE)\n"
        servers_init += norm_methods_init
        servers_init += f"        #endif\n"
        
        servers_init += f"    }}\n\n"
            

    include_macros = ""
    for filename in glob('src/**/*.h', recursive=True):
        #f = filename[len('src')+1:]
        include_macros += include_file_macros(filename) + "\n"

    to_replace = (
        ("INCLUDE_TYPES", include_file("src/types/types.h")),
        ("INCLUDE_ERROR", include_file("src/error.h")),
        ("INCLUDE_MACROS", include_macros),
        ("TYPES", types),
        ("CT_FN_DECL", ""),
        ("RAW_FN_PTRS_DECL", fn_ptrs_decl),
        ("NORM_FN_PTRS_DECL", norm_fn_ptrs_decl),
        ("RAW_FN_PTRS_IMPL", fn_ptrs_impl),
        ("NORM_FN_PTRS_IMPL", norm_fn_ptrs_impl),
        ("RAW_FN_PTRS_LOAD", fn_ptrs_load),
        ("NORM_FN_PTRS_LOAD", norm_fn_ptrs_load),
        ("SERVERS_INIT", servers_init),
        ("TRACE_DECL", trace_decl),
        ("TRACE_IMPL", trace_impl),
        ("TRACE_DEFINES", trace_defines),
    )

    for old, new in to_replace:
        api_header = api_header.replace(old, new, 1)

    with open("include/HateEngineAPI.h", "w") as f:
        f.write(api_header)



def full_trace_header_generator(return_type, name, args) -> str:
    functions = ""
    functions += f"{return_type} full_trace_{name}(const char* ___file___, uint32_t ___line___"
    if len(args) > 0:
        functions += ", "
    functions += ", ".join([f"{arg._type}" for arg in args])
    functions += ");\n"

    return functions

def full_trace_impl_generator(return_type, name, args) -> str:
    functions = ""
    functions += f"inline {return_type} full_trace_{name}(const char* ___file___, uint32_t ___line___"
    if len(args) > 0:
        functions += ", "
    
    for arg in args:
        functions += general.get_correct_variable(arg._type, arg.name)
                    
        if arg != args[-1]:
            functions += ", "
    functions += ") {\n"
    functions += f"    raw___he_update_full_trace_info(\"{name}\", ___file___, ___line___);\n";


    func_call = f"raw_{name}({', '.join([f'{arg.name}' for arg in args])});"
    if return_type == "void":
        functions += f"    {func_call}\n"
    else:
        functions += f"    {general.get_correct_variable(return_type, 'result')} = {func_call}\n"

    functions += "    raw___he_update_full_trace_info(\"\", \"\", -1);\n"

    if return_type != "void":
        functions += f"    return result;\n"
    functions += "}\n\n"

    return functions

def full_trace_define_generator(name, args):
    result = ""
    if len(args) > 0:
        result += f"#define {name}({', '.join([f'{arg.name}' for arg in args])}) full_trace_{name}(__FILE__, __LINE__, {', '.join([f'{arg.name}' for arg in args])})\n"
    else:
        result += f"#define {name}() full_trace_{name}(__FILE__, __LINE__)\n"
    return result


def include_file(file) -> str:
    with open(file, "r") as f:
        code = f.read().splitlines()
        n = 0
        result: list[str] = []
        is_in_api: bool = False
        for i, c in enumerate(code):
            if c.startswith("// API START") or c.startswith("// API BEGIN"):
                is_in_api = True
            elif c.startswith("// API END"):
                is_in_api = False
            elif is_in_api:
                result.append(c)

        return "\n".join(result)
    
def include_file_macros(file) -> str:
    with open(file, "r") as f:
        code = f.read().splitlines()
        n = 0
        result: list[str] = []
        is_in_api: bool = False
        for i, c in enumerate(code):
            if c.startswith("// MACROS API START") or c.startswith("// MACROS API BEGIN"):
                is_in_api = True
            elif c.startswith("// MACROS API END"):
                is_in_api = False
            elif is_in_api:
                result.append(c)

        return "\n".join(result)
    
def camel_to_upper_snake_case(name):
    result = []
    n = len(name)
    
    for i in range(n):
        current_char = name[i]
        prev_char = name[i-1] if i > 0 else ''
        next_char = name[i+1] if i < n-1 else ''
        
        should_add_underscore = False
        
        if current_char.isupper():
            if prev_char.islower():
                should_add_underscore = True
            elif prev_char == '_':
                should_add_underscore = True
            elif i > 0 and next_char.islower():
                should_add_underscore = True
        
        if should_add_underscore:
            result.append('_')
        
        result.append(current_char)
    
    return ''.join(result).upper()
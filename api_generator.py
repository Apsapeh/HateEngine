#!/usr/bin/env python3
import re
import os
from glob import glob

search_path = "src"

api_header = """
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//INCLUDE_ERROR

//TYPES

#if defined(HEAPI_COMPILE_TIME)

//CT_FN_DECL

#else

//FN_PTRS_DECL

#if defined(HEAPI_LOAD_IMPL)
    //FN_PTRS_IMPL

    void ___hate_engine_runtime_init(void* (*proc_addr)(const char* name)) {
        //FN_PTRS_LOAD
    }
#endif
#endif

#if defined(HEAPI_FULL_TRACE)
//TRACE_DEFINES
#else
//RAW_DEFINES
#endif

"""

api_fn_lookup_table = """
#pragma once
#define HE_MEM_NO_MACRO
#include <extra/full_trace.h>

//LOOKUP_INCLUDES

typedef struct {
    const char* name;
    void* ptr;
} APIFunctionLookupTable;

APIFunctionLookupTable api_function_lookup_table[] = {
    //FN_PTRS_LOOKUP
};

"""


####### FULL TRACE ######

full_trace_c = """
#include <log.h>
#include "full_trace.h"

//F_TRACE_IMPL
"""

full_trace_h = """
#pragma once
//INCLUDES

//F_TRACE_DECL
"""


class APIFunction:
    def __init__(self, mode, doc, rtype, name, args):
        self.mode = mode
        self.doc = doc
        self.rtype = rtype
        self.name = name
        self.args = args

class APIType:
    def __init__(self, _type, mode, doc, definition, name):
        self.type = _type
        self.mode = mode
        self.doc = doc
        self.definition = definition
        self.name = name



def parse_api_entries(source: str):
    global multi_line_comments
    lines = source.splitlines()
    entries = []
    i = 0

    while i < len(lines):
        line = lines[i].strip()

        # Match annotation
        if re.match(r'^//\s*\[\[API Generator(?::\s*([a-zA-Z_][a-zA-Z0-9_]*))?\]\]', line):
            generator_mode = re.findall(r'^//\s*\[\[API Generator(?::\s*([a-zA-Z_][a-zA-Z0-9_]*))?\]\]', line)[0] or "default"
            i += 1

            # Optional ~~NUMBER~~
            number = None
            if i < len(lines) and re.match(r'^\s*~~(\d+)~~', lines[i]):
                number = int(re.findall(r'\d+', lines[i])[0])
                i += 1

            code = ""
            trimmed = lines[i].strip()
            if trimmed.startswith("typedef") or trimmed.startswith("struct") or trimmed.startswith("union") or trimmed.startswith("enum"):
                depth = 0;
                while i < len(lines):
                    line = lines[i]
                    trimmed = line.strip()
                    depth += line.count("{") - line.count("}")

                    code += line

                    i += 1
                    if depth == 0 and trimmed.endswith(";"):
                        code = code[:-1]
                        i -= 1
                        break
                    else:
                        code += "\n"
            else:
                while i < len(lines):
                    line = lines[i]
                    trimmed = line.strip()
                    code += line
                    i += 1
                    if trimmed.endswith(";"):
                        code = code[:-1]
                        i -= 1
                        break
                    else:
                        code += "\n"

            #print(code)
            #print()

            trimmed = code.strip()
            #print(trimmed)
            if trimmed.startswith("typedef") or trimmed.startswith("struct") or trimmed.startswith("union") or trimmed.startswith("enum"):
                words = trimmed.split()
                name = words[-1]
                code += ";"
                _type = words[0]
                if _type == "typedef" and len(words) > 1 and words[1] in ["struct", "union", "enum"]:
                    _type += " " + words[1]
                doc = multi_line_comments[number] if number is not None else None
                entries.append(APIType(_type, generator_mode, doc, code, name))
            elif trimmed.startswith("DECLARE_RESULT"):
                doc = multi_line_comments[number] if number is not None else None
                code += ";"
                entries.append(APIType("", "forward", doc, code, ""))
            else:
                pattern = re.compile(r"[ \t]*([a-zA-Z_][a-zA-Z0-9_\*\s]*?)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^;{]*)\)\s*", re.MULTILINE)
                match = pattern.match(code)
                if match:
                    rtype = match.group(1)
                    name = match.group(2)
                    args = match.group(3)
                    doc = multi_line_comments[number] if number is not None else None
                    entries.append(APIFunction(generator_mode, doc, rtype, name, args))
                else:
                    print(f"\033[31mFailed to parse: {code}\n\033[0m")

        i += 1

    return entries



def main():
    global search_path
    global api_header
    global api_fn_lookup_table
    global full_trace_c
    global full_trace_h

    while not os.path.exists(search_path):
        search_path = "../" + search_path

    types = ""
    ct_fn_decl = ""
    fn_ptrs_decl = ""
    fn_ptrs_impl = ""
    fn_ptrs_load = ""
    lookup_includes = ""
    fn_ptrs_lookup = ""
    trace_defines = ""
    raw_defines = ""

    f_trace_impl = ""
    f_trace_decl = ""

    for filename in glob(search_path+'/**/*.h', recursive=True):
        f = filename[len(search_path)+1:]

        with open(filename, 'r') as f:
            code = find_multi_line_comments(f.read())
            entries = parse_api_entries(code)

            for e in entries:
                if isinstance(e, APIFunction):
                    fn_ptrs_lookup += f"    {{\"{e.name}\", (void*){e.name}}},\n"
                    fn_ptrs_lookup += f"    {{\"t_{e.name}\", (void*)full_trace_{e.name}}},\n"
                    doc = "" if e.doc is None else f"{e.doc}\n"
                    ct_fn_decl += f"{doc} {e.rtype} {e.name} ({e.args});\n\n"
                    #fn_ptrs_decl += f"{doc}extern {e.rtype} (*{e.name})({e.args});\n\n"
                    fn_ptrs_decl += f"{doc}extern {e.rtype} (*raw_{e.name})({e.args});\n\n"
                    fn_ptrs_impl += f"    {e.rtype} (*raw_{e.name})({e.args});\n"
                    fn_ptrs_load += f"    raw_{e.name} = ({e.rtype} (*)({e.args}))proc_addr(\"{e.name}\");\n"
                    #fn_ptrs_load += f"    trace_{e.name} = ({e.rtype} (*)({e.args}))proc_addr(\"trace_{e.name}\");\n"

                    t_args = e.args.strip()
                    if t_args == "" or t_args == "void":
                        t_args = "const char *___file___, int ___line___"
                    else:
                        t_args = f"const char *___file___, int ___line___, {e.args}"
                    fn_ptrs_decl += f"{doc}extern {e.rtype} (*trace_{e.name})({t_args});\n\n"
                    fn_ptrs_impl += f"    {e.rtype} (*trace_{e.name})({t_args});\n"
                    fn_ptrs_load += f"    trace_{e.name} = ({e.rtype} (*)({t_args}))proc_addr(\"t_{e.name}\");\n"

                    if e.args.strip() == "" or e.args.strip() == "void":
                        f_trace_impl += f"inline {e.rtype} full_trace_{e.name}(const char *___file___, int ___line___) {{\n"
                        f_trace_decl += f"{e.rtype} full_trace_{e.name}(const char *___file___, int ___line___);\n"
                    else:
                        f_trace_impl += f"inline {e.rtype} full_trace_{e.name}(const char *___file___, int ___line___, {e.args}) {{\n"
                        f_trace_decl += f"{e.rtype} full_trace_{e.name}(const char *___file___, int ___line___, {e.args});\n"
                    f_trace_impl += f"    he_update_full_trace_info(\"{e.name}\", ___file___, ___line___);\n"

                    spltted_args = e.args.split(",")
                    clear_args = ""
                    for i, arg in enumerate(spltted_args):
                        s = arg.strip().split()[-1]
                        clear_args += s
                        if i < len(spltted_args) - 1:
                            clear_args += ", "
                    if clear_args == "void":
                        clear_args = ""

                    if e.rtype == "void":
                        f_trace_impl += f"    {e.name}({clear_args});\n"
                    else:
                        f_trace_impl += f"    {e.rtype} result = {e.name}({clear_args});\n"

                    f_trace_impl += "    he_update_full_trace_info(\"\", \"\", -1);\n"
                    if e.rtype != "void":
                        f_trace_impl += f"    return result;\n"
                    f_trace_impl += f"}}\n\n"


                    trace_defines += f"{doc}#define {e.name}({clear_args}) trace_{e.name}(__FILE__, __LINE__{clear_args and ', '} {clear_args})\n\n"
                    raw_defines += f"{doc}#define {e.name}({clear_args}) raw_{e.name}({clear_args})\n\n"


                elif isinstance(e, APIType):
                    doc = "" if e.doc is None else f"{e.doc}\n"
                    if e.mode == "forward":
                        types += f"{doc}{e.definition}\n\n"
                    elif e.mode == "default":
                        if e.type == "typedef struct":
                            types += f"{doc}typedef struct {e.name} {e.name};\n\n"
                        elif e.type == "typedef union":
                            types += f"{doc}typedef union {e.name} {e.name};\n\n"
                        elif e.type == "typedef":
                            #types += f"{doc}typedef {e.type} {e.name};\n"
                            print(f"\033[31mError [{filename}]: '{e.definition}' must be forward annotated\n\033[0m")
                        else:
                            types += f"{doc}{e.type} {e.name};\n\n"

            if len(entries) > 0:
                filename = filename[len(search_path)+1:]
                lookup_includes += f"#include \"{filename}\"\n"


    # api_header = api_header.replace("//INCLUDE_RESULT", include_file("src/types/result.h"))
    to_replace = (
        ("//INCLUDE_ERROR", include_file("src/error.h")),
        ("//TYPES", types),
        ("//CT_FN_DECL", ct_fn_decl),
        ("//FN_PTRS_DECL", fn_ptrs_decl),
        ("//FN_PTRS_IMPL", fn_ptrs_impl),
        ("//FN_PTRS_LOAD", fn_ptrs_load),
        ("//TRACE_DEFINES", trace_defines),
        ("//RAW_DEFINES", raw_defines),
    )

    for old, new in to_replace:
        api_header = api_header.replace(old, new)


    api_fn_lookup_table = api_fn_lookup_table.replace("//LOOKUP_INCLUDES", lookup_includes)
    api_fn_lookup_table = api_fn_lookup_table.replace("//FN_PTRS_LOOKUP", fn_ptrs_lookup)

    full_trace_c = full_trace_c.replace("//F_TRACE_IMPL", f_trace_impl)
    #full_trace_c = full_trace_c.replace("//INCLUDES", lookup_includes)

    full_trace_h = full_trace_h.replace("//F_TRACE_DECL", f_trace_decl)
    full_trace_h = full_trace_h.replace("//INCLUDES", lookup_includes)

    #print(api_header)
    #return
    with open("include/HateEngineRuntimeAPI.h", "w") as f:
        f.write(api_header)

    with open("src/api_sym_lookup_table.h", "w") as f:
        f.write(api_fn_lookup_table)


    with open("src/extra/full_trace.c", "w") as f:
        f.write(full_trace_c)

    with open("src/extra/full_trace.h", "w") as f:
        f.write(full_trace_h)

multi_line_comments = []
def find_multi_line_comments(file):
    global multi_line_comments
    new_file = ""
    is_comment = False
    comment = ""
    for i, ch in enumerate(list(file)):
        if ch == "/" and file[i+1] == "*":
            is_comment = True
            comment = str(ch)
        elif ch == "/" and file[i-1] == "*":
            is_comment = False
            comment += str(ch)
            new_file += f"~~{len(multi_line_comments)}~~"
            multi_line_comments.append(comment)
            comment = ""
        elif is_comment:
            comment += str(ch)
        else:
            new_file += str(ch)
    return new_file

def include_file(file) -> str:
    with open(file, "r") as f:
        code = f.read().splitlines()
        n = 0
        for i, c in enumerate(code):
            if c.startswith("// API START"):
                n = i + 1
                break

        if n == len(code):
            n = 0

        code = code[n:]
        return "\n".join(code)


if __name__ == "__main__":
    main()

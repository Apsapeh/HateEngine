import re
import os
from glob import glob

search_path = "src"

api_header = """
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//TYPES

//FN_PTRS_DECL

#if defined(HERAPI_LOAD_IMPL)
    //FN_PTRS_IMPL

    void ___hate_engine_runtime_init(void* (*proc_addr)(const char* name)) {
        //FN_PTRS_LOAD
    }
#endif
"""

api_fn_lookup_table = """
//LOOKUP_INCLUDES

typedef struct {
    const char* name;
    void* ptr;
} APIFunctionLookupTable;

APIFunctionLookupTable api_function_lookup_table[] = {
    //FN_PTRS_LOOKUP
};
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
                        break
                    else:
                        code += "\n"

            #print(code)
            #print()

            trimmed = code.strip()
            if trimmed.startswith("typedef") or trimmed.startswith("struct") or trimmed.startswith("union") or trimmed.startswith("enum"):
                words = trimmed.split()
                name = words[-1]
                code += ";"
                _type = words[0]
                if _type == "typedef" and len(words) > 1 and words[1] in ["struct", "union", "enum"]:
                    _type += " " + words[1]
                doc = multi_line_comments[number] if number is not None else None
                entries.append(APIType(_type, generator_mode, doc, code, name))
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

    while not os.path.exists(search_path):
        search_path = "../" + search_path

    types = ""
    fn_ptrs_decl = ""
    fn_ptrs_impl = ""
    fn_ptrs_load = ""
    lookup_includes = ""
    fn_ptrs_lookup = ""

    for filename in glob(search_path+'/**/*.h', recursive=True):
        f = filename[len(search_path)+1:]
        
        with open(filename, 'r') as f:
            code = find_multi_line_comments(f.read())
            entries = parse_api_entries(code)

            for e in entries:
                if isinstance(e, APIFunction):
                    fn_ptrs_lookup += f"    {{\"{e.name}\", (void*){e.name}}},\n"
                    doc = "" if e.doc is None else f"{e.doc}\n"
                    fn_ptrs_decl += f"{doc}extern {e.rtype} (*{e.name})({e.args});\n\n"
                    fn_ptrs_impl += f"    {e.rtype} (*{e.name})({e.args});\n"
                    fn_ptrs_load += f"    {e.name} = ({e.rtype} (*)({e.args}))proc_addr(\"{e.name}\");\n"
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
                


    api_header = api_header.replace("//TYPES", types)
    api_header = api_header.replace("//FN_PTRS_DECL", fn_ptrs_decl)
    api_header = api_header.replace("//FN_PTRS_IMPL", fn_ptrs_impl)
    api_header = api_header.replace("//FN_PTRS_LOAD", fn_ptrs_load)

    api_fn_lookup_table = api_fn_lookup_table.replace("//LOOKUP_INCLUDES", lookup_includes)
    api_fn_lookup_table = api_fn_lookup_table.replace("//FN_PTRS_LOOKUP", fn_ptrs_lookup)


    #print(api_header)
    with open("include/HateEngineRuntimeAPI.h", "w") as f:
        f.write(api_header)

    with open("src/api_sym_lookup_table.h", "w") as f:
        f.write(api_fn_lookup_table)



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


if __name__ == "__main__":
    main()
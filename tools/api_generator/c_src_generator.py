from api_generator.parser_types import *
from . import general
import re


api_fn_lookup_table_header_file = """
#pragma once
// clang-format off
#define HE_MEM_NO_MACRO
//#include <extra/full_trace.h>

INCLUDES

typedef struct {
    const char* name;
    void* ptr;
} APIFunctionLookupTable;

"""

api_fn_lookup_table = """
APIFunctionLookupTable NAME[] = {
FN_PTRS
};
"""

API_FN_LOOKUP_INCLUDES_PATH_PREFIX_REMOVE = "src/"


def run(data: ParseResult):
    api_fn_lookup_table_generator(data)
    #full_trace_header_generator(data)
    #full_trace_impl_generator(data)

def api_fn_lookup_table_generator(data: ParseResult):
    includes = set()

    api_function_lookup_table_main = ""
    sorted_functions = sorted(data.functions, key=lambda f: f.name)
    for function in sorted_functions:
        includes.add(function.filename.removeprefix(API_FN_LOOKUP_INCLUDES_PATH_PREFIX_REMOVE))
        api_function_lookup_table_main += f"    {{\"{function.name}\", (void*){function.name}}},\n"
        #api_function_lookup_table_main += f"    {{\"t_{function.name}\", (void*)full_trace_{function.name}}},\n"
    if len(api_function_lookup_table_main) > 0:
        api_function_lookup_table_main = api_function_lookup_table_main[:-2]

    includes_str = ""
    for include in includes:
        includes_str += f"#include \"{include}\"\n"

    
    table = api_fn_lookup_table
    to_replace = (
        ("NAME", "g_apiFunctionLookupTable"),
        ("FN_PTRS", api_function_lookup_table_main),
    )

    for old, new in to_replace:
        table = table.replace(old, new)
   
    header_file = api_fn_lookup_table_header_file
    header_file = header_file.replace("INCLUDES", includes_str)
    header_file = header_file + table + "\n"
    header_file += "// clang-format on\n"

    with open("src/api_sym_lookup_table.h", "w") as f:
        f.write(header_file)


"""
    void ___hate_engine_runtime_init_window_server(WindowSeverBackend* backend) {
        create_window = window_server_backend_get(backend, "create_window");
        destroy_window = window_server_backend_get(backend, "destroy_window");
        update_window = window_server_backend_get(backend, "update_window");
    }
"""



def full_trace_header_generator(data: ParseResult):
    includes = set()

    functions = ""
    for function in data.functions:
        includes.add(function.filename.removeprefix(API_FN_LOOKUP_INCLUDES_PATH_PREFIX_REMOVE))

        functions += f"{function.return_type} full_trace_{function.name}(const char* ___file___, uint32_t ___line___"
        if len(function.args) > 0:
            functions += ", "
        functions += ", ".join([f"{arg._type}" for arg in function.args])
        functions += ");\n"

    includes_str = ""
    for include in includes:
        includes_str += f"#include \"{include}\"\n"

    data = f"#pragma once\n{includes_str}\n{functions}"

    with open("src/extra/full_trace.h", "w") as f:
        f.write(data)


def full_trace_impl_generator(data):
    functions = ""
    for function in data.functions:
        functions += f"inline {function.return_type} full_trace_{function.name}(const char* ___file___, uint32_t ___line___"
        if len(function.args) > 0:
            functions += ", "
        
        for arg in function.args:
            functions += general.get_correct_variable(arg._type, arg.name)
                        
            if arg != function.args[-1]:
                functions += ", "
        functions += ") {\n"
        functions += f"    he_update_full_trace_info(\"{function.name}\", ___file___, ___line___);\n";

        func_call = f"{function.name}({', '.join([f'{arg.name}' for arg in function.args])});"
        if function.return_type == "void":
            functions += f"    {func_call}\n"
        else:
            functions += f"    {general.get_correct_variable(function.return_type, 'result')} = {func_call}\n"

        functions += "    he_update_full_trace_info(\"\", \"\", -1);\n"

        if function.return_type != "void":
            functions += f"    return result;\n"
        functions += "}\n\n"

    data = f"#include <log.h>\n#include \"full_trace.h\"\n\n{functions}"

    with open("src/extra/full_trace.c", "w") as f:
        f.write(data)

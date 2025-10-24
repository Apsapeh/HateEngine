#!/usr/bin/env python3

from pathlib import Path

import create_class

# create_class node/PARRENT hello

BASE_PATH = "src/object/"

TYPES_FILE = """#pragma once

// clang-format off
enum ObjectType {
TYPES_TREE
};
// clang-format on
"""

AUTO_FREE = """#include "object.h"
INCLUDES

#include "platform/memory.h"

typedef void (*free_func)(void* object);

#define CAST_FREE(func) (free_func) func

// clang-format off
static free_func g_freeTable[] = {
FREE_TABLE
};
// clang-format on

void auto_free(Object* object) {
    free_table[object->type](object);
    tfree(object);
}
"""

def main():
    dirs = [p for p in Path(BASE_PATH).rglob('*') if p.is_dir()]
    dirs_stripped = [str(p).replace(BASE_PATH, '', 1) for p in dirs]
    dirs_stripped.sort()
    
    types_tree = []
    includes = []
    free_table_raw = []           
    free_table_raw_max_size = -1      
    for dir in dirs_stripped:
        depth = dir.count("/") + 1
        name = dir.split("/")[-1]
        enum_type = f"ObjectType{create_class.to_camel_case(name)}"
        # Types 
        types_tree.append("    " * depth + f"// {"->".join(create_class.to_camel_case(d) for d in ("object/" + dir).split("/"))}\n" + "    " * depth + enum_type)
        # Auto free
        includes.append(f"#include \"{dir}/{name}.h\"")
        free_table_raw.append((f"        [{enum_type}]", f" = CAST_FREE({name}_free)"))
        if len(free_table_raw[len(free_table_raw)-1][0]) > free_table_raw_max_size:
            free_table_raw_max_size = len(free_table_raw[len(free_table_raw)-1][0])
        
    free_table = []
    for e in free_table_raw:
        free_table.append(e[0] + " " * (free_table_raw_max_size - len(e[0])) + e[1])
        
        
    types_tree = ",\n".join(types_tree)
    includes = "\n".join(includes)
    free_table = ",\n".join(free_table)
    
    types_file = TYPES_FILE.replace("TYPES_TREE", types_tree)
    
    auto_free = AUTO_FREE.replace("INCLUDES", includes)
    auto_free = auto_free.replace("FREE_TABLE", free_table)
    
    with open("src/object/object_types.h", "w") as file:
        file.write(types_file)
        
    with open("src/object/object_auto_free.c", "w") as file:
        file.write(auto_free)
        

if __name__ == "__main__":
    main()
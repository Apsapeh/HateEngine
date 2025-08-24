#!/usr/bin/env python3
import sys
import os
import re

import update_class_tree

# create_class node/PARRENT hello

BASE_PATH = "src/object/"

CLASS_HEADER = """#pragma once

#include "../PARRENT.h"

// [[API Generator]]
typedef struct {
    PARRENT_CAMEL PARRENT;
    // Add your custom fields here
} NAME_CAMEL;

PARRENT_INIT;

void NAME_free(NAME_CAMEL* this);

// [[API Generator]]
NAME_CAMEL* NAME_new(/* Add your args here */);
"""

CLASS_IMPL = """#include "NAME.h"
#include "platform/memory.h"

PARRENT_INIT {
    PARRENT_init(PARRENT_INIT_CALL_ARGS);
    // Add your custom initialization here
}

void NAME_free(NAME_CAMEL* this) {
    PARRENT_free(&this->PARRENT);
    // Add your custom cleanup here
}

NAME_CAMEL* NAME_new(/* Add your args here */) {
    NAME_CAMEL* NAME = tmalloc(sizeof(NAME_CAMEL));
    NAME_init(NAME, ObjectTypeNAME_CAMEL, ...);
    return NAME;
}
"""

def main():
    args = sys.argv

    if len(args) < 3:
        print("Error")
        return 1
        
    if args[1] != ".":
        class_path = BASE_PATH + args[1]
    else:
        class_path = BASE_PATH[:-1]
        
    if class_path[-1] == "/":
        class_path = class_path[:-1]
    
    if os.path.exists(class_path):
        new_class_path = class_path + "/" + args[2]
        if not os.path.exists(new_class_path):
            os.mkdir(new_class_path)
        
        parrent = class_path.split("/")[-1]
        name = args[2]
        parrent_camel = to_camel_case(parrent)
        name_camel = to_camel_case(name)
        
        with open(class_path + "/" + parrent + ".h", "r") as file:
            parrent_header = file.read()
            
        parrent_init = ""
        parrent_init_call_args = ""
        for ex in parrent_header.split(";"):
            ex = ex.strip()
            bababooey = re.split(r'\(|\s', ex)
            
            if parrent + "_init" in bababooey:
                parrent_init = ex
                
                ex = re.findall(parrent + "_init" + r"\((.*)\)", ex)[0].split(",")
                ex = [s.strip().split()[-1] for s in ex]
                parrent_init_call_args = ', '.join(ex)
                parrent_init_call_args = parrent_init_call_args.replace("this", f"&this->{parrent}")
                print(parrent_init_call_args)
                # for i, w in enumerate(ex):
                #     parrent_init_call_args += word
                #     if i != len(ex) - 1:
                        
        
        if parrent_init == "":
            print("Error")
            return 1
            
        parrent_init = parrent_init.replace(parrent_camel, "NAME_CAMEL", count=1)
        parrent_init = parrent_init.replace(parrent, "NAME", count=1)
        
        to_replace = (
            ("PARRENT_INIT_CALL_ARGS", parrent_init_call_args),
            ("PARRENT_INIT", parrent_init),
            ("PARRENT_CAMEL", parrent_camel),
            ("PARRENT", parrent),
            ("NAME_CAMEL", name_camel),
            ("NAME", name),
        )
        header = CLASS_HEADER
        impl = CLASS_IMPL
        for r in to_replace:
            header = header.replace(r[0], r[1])
            impl = impl.replace(r[0], r[1])
            
        with open(new_class_path + "/" + name + ".h", "w") as file:
           file.write(header) 
        
        with open(new_class_path + "/" + name + ".c", "w") as file:
           file.write(impl)
           
        update_class_tree.main()
    else:
        print(f'Path "{class_path}"  not exist')
        
def to_camel_case(string):
    s = ''.join(word.capitalize() for word in string.split('_'))
    
    change_to = (
        ("3d", "3D"),
        ("2d", "2D"),
    )
    
    for c in change_to:
        s = s.replace(c[0], c[1])
    
    return s

if __name__ == "__main__":
    main()
import re

def get_correct_variable(var_type, name):
    # Function pointers type (*name)(args)
    match = re.findall(r"([\w_\d]+)\s*\((\*+)\)\s*(\(.*\))", var_type)
    if len(match) == 1:
        return f"{match[0][0]} ({match[0][1]} {name}) {match[0][2]}"

    # Array type name[size]
    match = re.findall(r"([\w_\d]+)\s*\[\s*(\d+)\s*\]", var_type)
    if len(match) == 1:
        return f"{match[0][0]} {name}[{match[0][1]}]"
    
    return f"{var_type} {name}"
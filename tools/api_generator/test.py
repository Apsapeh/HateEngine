import re

def doxygen_comment_parse(comment: str):
    clean_comment = re.sub(r'^\s*/\*+|\s*\*/\s*$', '', comment, flags=re.MULTILINE)

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
                #lines.append(line)
                tmp_str = line
            else:
                tmp_str += " " + line

        if tmp_str != "":
            lines.append(tmp_str)

    print(lines)
    result = {}
    for line in lines:
        matches = re.findall(r"^@(\S+)([ \t]+([^\n]*))?$", line)
        arr = result.get(matches[0][0], [])
        arr.append(matches[0][2])
        result[matches[0][0]] = arr

    return result

    
    
comment = """/*
 * @brief Get a function pointer for a backend
 * @return "InvalidArgument" if backend is NULL or name is NULL or function is NULL
 * @return "NotFound" if a function with the given name is not registered
 * 
 * @api
 */"""


print(doxygen_comment_parse(comment))
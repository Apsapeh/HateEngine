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


# VIBE
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

# Тестирование
test_cases = [
    "FSSeekFrom_Start",  # FS_SEEK_FROM_START
    "FileName",          # FILE_NAME  
    "HTTPRequest",       # HTTP_REQUEST (две большие рядом - одно слово)
    "XMLParser",         # XML_PARSER (две большие рядом - одно слово)
    "MyVariableName",    # MY_VARIABLE_NAME
    "userID",            # USER_ID
    "HTMLParser",        # HTML_PARSER (три большие рядом - одно слово)
    "FS_Seek_From_Start", # FS_SEEK_FROM_START
    "ABC",               # ABC (все большие - одно слово)
    "Abc",               # ABC (одно слово)
    "aBC",               # A_BC (разделяем, т.к. предыдущая маленькая)
]

print("Преобразование CamelCase/PascalCase в UPPER_SNAKE_CASE:")
print("-" * 50)
for case in test_cases:
    result = camel_to_upper_snake_case(case)
    print(f"{case:20} -> {result}")
file = """
// [[API Generator]]
~~1~~
void* he_malloc(size_t size);

// [[API Generator: forward]]
~~2~~
void* he_realloc(void* ptr, size_t size);

// [[API Generator]]
~~3~~
void he_free(void* ptr);

// [[API Generator: forward]]
typedef union {
    struct {
        float m00;
        float m01;
        float m02;
        float m03;
        float m10;
        float m11;
        float m12;
        float m13;
        float m20;
        float m21;
        float m22;
        float m23;
        float m30;
        float m31;
        float m32;
        float m33;
    } el;
    float data[16];
} Mat4;
"""

def find_multi_line_comments(file):
    multi_line_comments = []
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
            multi_line_comments.append(comment)
            new_file += f"~~{len(multi_line_comments)}~~"
            comment = ""
        elif is_comment:
            comment += str(ch)
        else:
            new_file += str(ch)
    return (new_file, multi_line_comments)

#new_file, multi_line_comments = find_multi_line_comments(file)
#print(new_file)
#print(multi_line_comments)


import re 

class APIFunction:
    def __init__(self, mode, doc, rtype, name, args):
        self.mode = mode
        self.doc = doc
        self.rtype = rtype
        self.name = name
        self.args = args

class APITypedef:
    def __init__(self, mode, doc, definition, name):
        self.mode = mode
        self.doc = doc
        self.definition = definition
        self.name = name

class APIStruct:
    def __init__(self, mode, doc, definition, name):
        self.mode = mode
        self.doc = doc
        self.definition = definition
        self.name = name

class APIUnion:
    def __init__(self, mode, doc, definition, name):
        self.mode = mode
        self.doc = doc
        self.definition = definition
        self.name = name

    

def parse_api_entries(source: str):
    lines = source.splitlines()
    entries = []
    i = 0

    while i < len(lines):
        line = lines[i].strip()

        # Match annotation
        if re.match(r'^//\s*\[\[API Generator(?::\s*([a-zA-Z_][a-zA-Z0-9_]*))?\]\]', line):
            generator_mode = re.findall(r'^//\s*\[\[API Generator(?::\s*([a-zA-Z_][a-zA-Z0-9_]*))?\]\]', line)[0] or "normal"
            i += 1

            # Optional ~~NUMBER~~
            number = None
            if i < len(lines) and re.match(r'^\s*~~(\d+)~~', lines[i]):
                number = int(re.findall(r'\d+', lines[i])[0])
                i += 1

            code = ""
            trimmed = lines[i].strip()
            if trimmed.startswith("typedef") or trimmed.startswith("struct") or trimmed.startswith("union"):
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
            if trimmed.startswith("typedef") or trimmed.startswith("struct") or trimmed.startswith("union"):
                words = trimmed.split()
                name = words[-1]
                code += ";"
                if trimmed.startswith("typedef"):
                    entries.append(APITypedef(generator_mode, None, code, name))
                elif trimmed.startswith("struct"):
                    entries.append(APIStruct(generator_mode, None, code, name))
                elif trimmed.startswith("union"):
                    entries.append(APIUnion(generator_mode, None, code, name))
            else:
                pattern = re.compile(r"[ \t]*([a-zA-Z_][a-zA-Z0-9_\*\s]*?)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^;{]*)\)\s*", re.MULTILINE)
                match = pattern.match(code)
                if match:
                    rtype = match.group(1)
                    name = match.group(2)
                    args = match.group(3)
                    entries.append(APIFunction(generator_mode, None, rtype, name, args))
                else:
                    print(f"\033[31mFailed to parse: {code}\n\033[0m")
                
        i += 1

    return entries


entries = parse_api_entries(file)
print(entries)
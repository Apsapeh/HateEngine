class FunctionArg:
    def __init__(self, _type: str, name: str):
        self._type = _type
        self.name = name

    def __str__(self):
        return f"{self._type} {self.name}"

    def __iter__(self):
        yield 'type', self._type
        yield 'name', self.name


class Function:
    def __init__(self, name: str, return_type: str, args: list[FunctionArg], filename: str, line: int, doc: str):
        self.name = name
        self.return_type = return_type
        self.args = args
        self.filename = filename
        self.line = line
        self.doc = doc

    def __str__(self):
        return f"{self.return_type} {self.name}({', '.join([f'{arg._type} {arg.name}' for arg in self.args])})"

    def __iter__(self):
        yield 'name', self.name
        yield 'return_type', self.return_type
        yield 'args', [dict(arg) for arg in self.args]
        yield 'filename', self.filename
        yield 'line', self.line
        yield 'doc', self.doc
        
class Struct:
    def __init__(self, name: str, filename: str, line: int, doc: str):
        self.name = name
        self.filename = filename
        self.line = line
        self.doc = doc
        
    def __str__(self):
        return f"struct {self.name}"
        
    def __iter__(self):
        yield 'name', self.name
        yield 'filename', self.filename
        yield 'line', self.line
        yield 'doc', self.doc


class StructForwardField:
    def __init__ (self, name: str, type: str):
        self.name = name
        self.type = type

    def __iter__(self):
        yield 'name', self.name
        yield 'type', self.type

class StructForward:
    def __init__ (self, name: str, fields: list[StructForwardField], filename: str, line: int, doc: str):
        self.name = name
        self.fields = fields
        self.filename = filename
        self.line = line
        self.doc = doc

    def __iter__(self):
        yield 'name', self.name
        yield 'fields', [dict(field) for field in self.fields]
        yield 'filename', self.filename
        yield 'line', self.line
        yield 'doc', self.doc

class Server:
    def __init__ (self, name: str, fn_prefix: str, init_method: str, methods: list[Function], filename: str, line: int, doc: str):
        self.name = name
        self.fn_prefix = fn_prefix
        self.init_method = init_method
        self.methods = methods
        self.filename = filename
        self.line = line
        self.doc = doc

    def __iter__(self):
        yield 'name', self.name
        yield 'fn_prefix', self.fn_prefix
        yield 'init_method', self.init_method
        yield 'methods', [dict(method) for method in self.methods]
        yield 'filename', self.filename
        yield 'line', self.line
        yield 'doc', self.doc


class Typedef:
    def __init__(self, name: str, type: str, filename: str, line: int, doc: str):
        self.name = name
        self.type = type
        self.filename = filename
        self.line = line
        self.doc = doc

    def __iter__(self):
        yield 'name', self.name
        yield 'type', self.type
        yield 'filename', self.filename
        yield 'line', self.line
        yield 'doc', self.doc

class ApiEnumValue:
    def __init__(self, name: str, value: str):
        self.name = name
        self.value = value

    def __iter__(self):
        yield 'name', self.name
        yield 'value', self.value

class ApiEnum:
    def __init__ (self, name: str, data_type: str, values: list[ApiEnumValue], filename: str, line: int):
        self.name = name
        self.data_type = data_type
        self.values = values
        self.filename = filename
        self.line = line
    
    def __iter__(self):
        yield 'name', self.name
        yield 'data_type', self.data_type
        yield 'values', [dict(value) for value in self.values]
        yield 'filename', self.filename
        yield 'line', self.line


class ParseResult:
    def __init__(self):
        self.functions: list[Function] = []
        self.structs: list[Struct] = []
        self.forward_structs: list[StructForward] = []
        self.servers: list[Server] = []
        self.typedefs: list[Typedef] = []
        self.api_enums: list[ApiEnum] = []

    def extend(self, other):
        self.functions.extend(other.functions)
        self.structs.extend(other.structs)
        self.forward_structs.extend(other.forward_structs)
        self.servers.extend(other.servers)
        self.typedefs.extend(other.typedefs)
        self.api_enums.extend(other.api_enums)

    def add_function(self, function: Function):
        self.functions.append(function)
        
    def add_struct(self, struct: Struct):
        self.structs.append(struct)

    def add_forward_struct(self, forward_struct: StructForward):
        self.forward_structs.append(forward_struct)

    def add_server(self, server: Server):
        self.servers.append(server)

    def add_typedef(self, typedef: Typedef):
        self.typedefs.append(typedef)

    def add_api_enum(self, api_enum: ApiEnum):
        self.api_enums.append(api_enum)

    def __iter__(self):
        yield 'functions', [dict(function) for function in self.functions]
        yield 'structs', [dict(struct) for struct in self.structs]
        yield 'forward_structs', [dict(forward_struct) for forward_struct in self.forward_structs]
        yield 'servers', [dict(server) for server in self.servers]
        yield 'typedefs', [dict(typedef) for typedef in self.typedefs]
        yield 'api_enums', [dict(api_enum) for api_enum in self.api_enums]

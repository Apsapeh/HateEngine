# import api_generator.aboba as aboba
from . import parser, c_src_generator, c_api_generator
import sys
import json

def help():
    print("Usage: api_generator [--json]")
    print("  --json   Output the parsed API as JSON to stdout")

def main():
    if "--help" in sys.argv or "-h" in sys.argv:
        help()
        return
    
    # aboba.main()
    # aboba.parse_file(sys.argv[1])
    #parser.parse(sys.argv[1])
    parse_result = parser.parse()
    c_src_generator.run(parse_result)
    c_api_generator.run(parse_result)

    if "--json" in sys.argv:
        print(json.dumps(dict(parse_result), indent=4))

    return


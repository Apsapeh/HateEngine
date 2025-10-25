# import api_generator.aboba as aboba
from . import parser, c_src_generator, c_api_generator
import sys

def main():
    # aboba.main()
    # aboba.parse_file(sys.argv[1])
    #parser.parse(sys.argv[1])
    parse_result = parser.parse()
    c_src_generator.run(parse_result)
    c_api_generator.run(parse_result)
    return
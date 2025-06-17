import os
from glob import glob

include_path = "include/HateEngine"

def gen_AllHateEngine_hpp():
    global include_path

    while not os.path.exists(include_path):
        include_path = "../" + include_path

    file_data = "#pragma once \n"

    for filename in glob(include_path+'/**/*.hpp', recursive=True):
        if filename == include_path + "/FullHateEngine.hpp":
            continue

        print(filename[len(include_path)+1:])
        file_data += "#include \"" + filename[len(include_path)+1:] + "\"\n"

    with open(include_path + "/FullHateEngine.hpp", "w") as f:
        f.write(file_data)  




if __name__ == "__main__":
    gen_AllHateEngine_hpp()

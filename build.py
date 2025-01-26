#!/usr/bin/env python3
import subprocess
import sys
import os
import shutil

def main():
    parse_args()

def parse_args():
    args = sys.argv[1:]
    
    if len(args) < 1:
        print("You need to specify a target: all, mingw, linux, mac")
        return
    
    xmake_get_config()
    target = args[0]

    verbose = False
    if len(args) > 1:
        i = 1
        while i < len(args):
            if args[i] == "-v":
                verbose = True
            i += 1

    if target == "all":
        build_mingw(verbose)
        build_linux(verbose)
        build_mac(verbose)
    elif target == "mingw":
        build_mingw(verbose)
    elif target == "linux":
        build_linux(verbose)
    elif target == "mac":
        build_mac(verbose)
    else:
        print("Unknown target: " + target)
        return

    xmake_set_config()


def build_mingw(verbose):
    build("Mingw", "mingw", "i386", verbose)
    copy_files("build/mingw/i386/release", "build/all/mingw-32", ["libHateEngine.dll", "libHateEngine.dll.a", "libHateEngine-static.a"])
    # Copy recompiled stdlib
    copy_files("extern_resources", "build/all/mingw-32", ["libgcc_s_dw2-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll"])

    build("Mingw", "mingw", "x86_64", verbose)
    copy_files("build/mingw/x86_64/release", "build/all/mingw-64", ["libHateEngine.dll", "libHateEngine.dll.a", "libHateEngine-static.a"])
    

def build_linux(verbose):
    build("Linux", "linux", "x86_64", verbose)
    copy_files("build/linux/x86_64/release", "build/all/linux-64", ["libHateEngine.so", "libHateEngine-static.a"])

def build_mac(verbose):
    # Check OS is MacOS
    if not sys.platform == "darwin":
        return
    build("Mac", "mac", "arm64", verbose, ["--target_minver=11.0"])
    build("Mac", "mac", "x86_64", verbose, ["--target_minver=10.7"])


def build(target_name: str, platform: str, arch: str, verbose: bool, additional_flags: list[str] = []) -> bool:
    print(f"Building {target_name} [{arch}]")
    args = ["xmake", "f", "-m", "release", "-p", platform, "-a", arch] + additional_flags
    result = subprocess.call(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result != 0:
        print(f"{target_name} [{arch}] configure failed")
        return False

    result = subprocess.call(["xmake"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result != 0:
        print(f"{target_name} [{arch}] build failed")
        return False
    return True

def copy_files(src: str, dst: str, files: list[str]):
    if not os.path.exists(dst):
        os.makedirs(dst)
    for file in files:
        shutil.copyfile(src + "/" + file, dst + "/" + file)

###### Saving and after restoring a xmake user-defined config
def xmake_get_config():
    subprocess.call(["xmake", "f", "--export=build/config.back"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

def xmake_set_config():
    subprocess.call(["xmake", "f", "--import=build/config.back"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)


if __name__ == "__main__":
    main()
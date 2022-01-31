import os, shutil, platform
# import subprocess as sp
from typing import Literal

from gendll import gendll

def assemble(platform: Literal["win", "mac", "lin"], source_dir: str, package_dir: str, dlls: bool = False):
    BINARY_FOR = {
        "win": "dcore.exe",
        "mac": "dcore",
        "lin": "dcore"
    }

    SOURCE_DIR = source_dir
    BINARY_PATH = os.path.join(SOURCE_DIR, "bin/release/" + BINARY_FOR[platform])
    DATA_DIR = os.path.join(SOURCE_DIR, "data")
    SAVES_DIR = os.path.join(SOURCE_DIR, "saves")
    PACKAGE_DIR = package_dir
    GEN_DLLS = dlls

    print("Began the assembly process...")
    if not os.path.exists(PACKAGE_DIR):
        os.mkdir(PACKAGE_DIR)
    else:
        print("Directory already exists, aboring.")
        exit(1)
        
    print("Copying the executable...")
    shutil.copyfile(BINARY_PATH, os.path.join(PACKAGE_DIR, BINARY_FOR[platform]))

    if platform == "win":
        if GEN_DLLS:
            print("Generating DLL files...")
            gendll(os.path.join(PACKAGE_DIR, "dcore.exe"), PACKAGE_DIR)
            
        # print("Copying DLL Files...")
        # for f in os.listdir(DLL_DIR):
        #     if f.endswith(".dll"):
        #         print(f)
        #         shutil.copy(os.path.join(DLL_DIR, f), os.path.join(PACKAGE_DIR, f))
    
    print("Creating the saves folder...")
    os.mkdir(os.path.join(PACKAGE_DIR, "saves"))
    shutil.copyfile(os.path.join(SAVES_DIR, "preferences.json"), os.path.join(PACKAGE_DIR, "saves/preferences.json"))

    print("Copying the data folder...")
    shutil.copytree(DATA_DIR, os.path.join(PACKAGE_DIR, "data"))

    print("Creating a zip archive...")
    shutil.make_archive(PACKAGE_DIR, "zip", PACKAGE_DIR)

    print("Removing the package directory")
    shutil.rmtree(PACKAGE_DIR)

    print("Done!")

    # DONE: Create zip archive of the PACKAGE_DIR and delete PACKAGE_DIR.
    #       The installer will download the generated zip and unpack it
    #       where needed on the user's PC.


def assemble_win(source_dir: str, package_dir: str, dlls: bool):
    SOURCE_DIR = source_dir
    BINARY_PATH = os.path.join(SOURCE_DIR, "bin/release/dcore.exe")
    DATA_DIR = os.path.join(SOURCE_DIR, "data")
    SAVES_DIR = os.path.join(SOURCE_DIR, "saves")
    PACKAGE_DIR = package_dir
    GEN_DLLS = dlls

    print("Began the assembly process...")
    if not os.path.exists(PACKAGE_DIR):
        os.mkdir(PACKAGE_DIR)
    else:
        print("Directory already exists, aboring.")
        exit(1)
        
    print("Copying the executable...")
    shutil.copyfile(BINARY_PATH, os.path.join(PACKAGE_DIR, "dcore.exe"))

    if GEN_DLLS:
        print("Generating DLL files...")
        gendll(os.path.join(PACKAGE_DIR, "dcore.exe"), PACKAGE_DIR)

    print("Creating the saves folder...")
    os.mkdir(os.path.join(PACKAGE_DIR, "saves"))
    shutil.copyfile(os.path.join(SAVES_DIR, "preferences.json"), os.path.join(PACKAGE_DIR, "saves/preferences.json"))

    print("Copying the data folder...")
    shutil.copytree(DATA_DIR, os.path.join(PACKAGE_DIR, "data"))

    print("Creating a zip archive...")
    shutil.make_archive(PACKAGE_DIR, "zip", PACKAGE_DIR)

    print("Removing the package directory")
    shutil.rmtree(PACKAGE_DIR)

    print("Done!")

if __name__ == '__main__':
    user_system = platform.system()
    SYSTEM_MAP = {
        "Darwin": "mac",
        "Linux": "lin",
        "Windows": "win"
    } # TODO: Test if this is enough...
    tmp = input(f"Target System ({user_system}) [{'/'.join(SYSTEM_MAP.keys())}]: ")
    system = SYSTEM_MAP[tmp if len(tmp) else user_system]

    source_dir = input("Source directory path: ")
    package_dir = input("Package directory path: ")

    if system == "win":
        tmp = input("Generate DLLs? [Y/n]: ").strip()
	if tmp:
        	gen_dlls = tmp.lower()[0] == 'y'
	else:
		gen_dlls = True
    else:
        gen_dlls = False

    assemble(system, source_dir, package_dir, gen_dlls)

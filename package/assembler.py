import sys, os, shutil
import subprocess as sp

from gendll import gendll

def assemble(source_dir: str, package_dir: str, dlls: bool):
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

if __name__ == '__main__':
    source_dir = input("Source directory path: ")
    package_dir = input("Package directory path: ")
    tmp = input("Generate DLLs? [Y/n]: ")
    gen_dlls = len(tmp) and tmp.lower()[0] == 'y'
    assemble(source_dir, package_dir, gen_dlls)

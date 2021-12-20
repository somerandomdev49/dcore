import sys, os, shutil
import subprocess as sp

SOURCE_DIR = input("SOURCE DIRECTORY PATH: ")
BINARY_PATH = os.path.join(SOURCE_DIR, "bin/release/dcore.exe")
DLL_DIR = os.path.join(SOURCE_DIR, "bin/release")
DATA_DIR = os.path.join(SOURCE_DIR, "data")
SAVES_DIR = os.path.join(SOURCE_DIR, "saves")
PACKAGE_DIR = input("PACKAGE DIRECTORY PATH: ")

print("Began Assembly Process...")
if not os.path.exists(PACKAGE_DIR):
    os.mkdir(PACKAGE_DIR)
else:
    print("Directory already exists, aboring.")
    exit(1)
    
print("Copying Executable...")
shutil.copyfile(BINARY_PATH, os.path.join(PACKAGE_DIR, "dcore.exe"))

print("Copying DLL Files...")
for f in os.listdir(DLL_DIR):
    if f.endswith(".dll"):
        print(f)
        shutil.copy(os.path.join(DLL_DIR, f), os.path.join(PACKAGE_DIR, f))

print("Creating the saves folder...")
os.mkdir(os.path.join(PACKAGE_DIR, "saves"))
shutil.copyfile(os.path.join(SAVES_DIR, "preferences.json"), os.path.join(PACKAGE_DIR, "saves/preferences.json"))

print("Copying the data folder...")
shutil.copytree(DATA_DIR, os.path.join(PACKAGE_DIR, "data"))

print("Creating a zip archive...")
shutil.make_archive(PACKAGE_DIR, "zip", PACKAGE_DIR)

print("Done!")

# DONE: Create zip archive of the PACKAGE_DIR and delete PACKAGE_DIR.
#       The installer will download the generated zip and unpack it
#       where needed on the user's PC.


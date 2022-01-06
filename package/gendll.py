'''
Copies necessary dll files based on lld output in stdin.
'''

# NOTE: Running on MSYS2 with mingw-w64:
# $ lld dcore.exe | python gendll.py
# NOTE: Tested only on MSYS2 with mingw-w64
#       but probably insn't needed elsewhere.

import sys, shutil, os
import subprocess as sp

def _gendll_impl(lines: list[str], outdir: str = '.'):
	for l in lines:
		if "mingw64" not in l: continue
		name, src = map(lambda s: s.strip(), l.split('=>'))
		src = src[:src.index('(')].strip()
		# shutil.copyfile(src, os.path.join(outdir, name))
		print(name)
		sp.run(["cp", src, os.path.join(outdir, name)])

def gendll(executable: str, outdir: str):
	p = sp.check_output(f"ldd {os.path.realpath(executable)}", shell=True)
	_gendll_impl(p.decode('utf-8').splitlines(), outdir)

if __name__ == "__main__":
	_gendll_impl(sys.stdin.readlines());

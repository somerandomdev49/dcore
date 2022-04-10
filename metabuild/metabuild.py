from dataclasses import dataclass
import os, sys, glob, re, os.path as path, typing
from itertools import islice

def chunk(it, size):
    it = iter(it)
    return iter(lambda: tuple(islice(it, size)), ())

class Env:
	def __init__(self):
		self.rules: list[dict[str, str]] = [] # [{ ... }, ...]
		self.exts: dict[str, int] = {} # { extension: index_in(self.rules), ... }
	
	def add_rule_for_ext(self, *exts: str, **kwargs: str):
		self.rules.append(kwargs)
		for ext in exts:
			self.exts[ext] = len(self.rules) - 1
	
	def generate(self, out: typing.TextIO, sources: list[str], outdir: str, binpath: str) -> str:
		for idx, rule in enumerate(self.rules):
			out.write("rule r" + str(idx) + "\n")
			for k, v in rule.items():
				out.write("  " + k + " = " + v + "\n")
			out.write("\n")
		
		outfiles: list[str] = []
		for source in sources:
			outfile = path.join(outdir, source) + ".o"
			outfiles.append(outfile)

			ext = path.basename(source).split('.')[-1]
			if ext not in self.exts:
				print("Could not compile '" + source + "': file extension '" + ext + "' is not supported.")
				continue
			
			out.write("build " + outfile + ": r" + str(self.exts[ext]) + " " + source + "\n")
		

		out.write("build " + binpath + ": r" + str(self.exts[""]) + " $\n"
			+ " $\n".join(map(lambda f: "  " + f, outfiles)) + "\n")
			
def get_project_sources(srcdir: str, exts: list[str]) -> list[str]:
	sources: list[str] = []
	for ext in set(exts):
		pattern = (srcdir[:-1] if srcdir.endswith('/') else srcdir) + "/**/*." + ext
		sources.extend(glob.glob(pattern, recursive=True))
	
	return sources

__all__ = [Env, get_project_sources]

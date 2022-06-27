from sys import stdout
import typing
import metabuild as M
import os, glob, os.path as path, configparser, subprocess, shlex

def get_platform() -> str:
	platforms = [
		'.'.join(path.basename(f).split('.')[:-1])
		for f in glob.glob("metabuild/platform/*.ninja")
	]
	return input(f"Please enter the platform to build to [{', '.join(platforms)}]: ")

def printed(*args):
	print(*args)
	return args

def flat(l):
	out = []
	for i in l:
		if type(i) == list:
			out += flat(i)
		else: out.append(i)
	return out

def read_config(fname: str) -> dict[str, str]:
	cfg = configparser.ConfigParser()
	cfg.read(fname)

	if "data" not in cfg: return {}
	if "exports" not in cfg: return {}
	
	data = {}
	if "_base" in cfg["data"]:
		data.update(read_config(cfg["data"]["_base"]))
	CMDS = {
		"map": map,
		"str": str,
		"int": int,
		"list": lambda *args: list(args),
		"flat": lambda *args: flat(args),
		"sh": lambda *args: subprocess.run(args, stdout=subprocess.PIPE)
			.stdout.decode('utf-8').strip()
	}

	def run(s: str, env: dict[str, str]) -> typing.Any:
		cmd, *args = shlex.split(s, comments=False)
		if cmd == "->":
			return lambda *args2: run(shlex.join(args), {
				**env,
				**dict(map(lambda x: (str(x[0]), x[1]), enumerate(args2)))
			})

		new_args = []
		for arg in args:
			if arg.startswith("$"):
				new_args.append(env[arg[1:]])
			else:
				new_args.append(arg)
		
		return CMDS[cmd](*new_args)

	for k, v in cfg["data"].items():
		if k.startswith("_"): continue
		data[k] = run(v, data)
	
	exports = {}
	for k, v in cfg["exports"].items():
		exports[k] = data[v]
	
	return exports

if __name__ == "__main__":
	platform = os.environ["DCORE_PLATFORM"] if "DCORE_PLATFORM" in os.environ else get_platform()
	with open("build.ninja", "w") as f:
		platform_basepath = "metabuild/platform/" + platform
		if os.path.exists(platform_basepath + ".ini"):
			data = read_config(platform_basepath + ".ini")
			for k, v in data.items():
				f.write(k + " = ")
				try: iter(v)
				except TypeError:
					f.write(str(v))
				else:
					f.write(' '.join(map(str, v)))
				f.write("\n")
					
		f.write("\ninclude " + platform_basepath + ".ninja\n\n")

		platform_specific = []
		if platform == "msys2":
			platform_specific = ["rc"]

		sources = M.get_project_sources("src", ["cpp", "c"] + platform_specific)
		env = M.Env()

		env.add_rule_for_ext("",
			command = "$linker $in -o $out -fdiagnostics-color $ld_flags")

		env.add_rule_for_ext("cpp", "cxx", "cc",
			command = "$compiler_cpp -c $in -o $out -fdiagnostics-color $cpp_flags -MD -MF $out.d",
			depfile = "$out.d")

		env.add_rule_for_ext("c",
			command = "$compiler_c -c $in -o $out -fdiagnostics-color $c_flags -MD -MF $out.d",
			depfile = "$out.d")

		env.add_rule_for_ext("rc",
			command = "$winres $in -O coff -o $out")
		
		env.generate(f, sources, "build", "bin/release/dcore")

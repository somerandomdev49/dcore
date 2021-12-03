# Deps

* `glfw` - `brew install glfw` - cross platform window/input management.
* `glm` - `brew install glm` - mathematics
* `freetype` - `brew install freetype` - font rendering
* `json` - `brew install nlohmann_json` - json serialization/deserialization
* `bullet3` - `git clone https://github.com/bulletphysics/bullet3` - bullet3 physics

## Specific installation steps

### Installing Bullet3 (!important!)

> Requirements: `cmake`, `make`

```shell
cd <library-path> # Not `dcore/`!
git clone https://github.com/bulletphysics/bullet3
cd bullet3
mkdir build
cd build
cmake .. -D CMAKE_C_COMPILER=<C++ COMPILER> -D CMAKE_CXX_COMPILER=<C++ COMPILER> # Generates the Makefiles
make # Builds the necessary Bullet3 modules.
sudo make install # Installs the libraries.
```

## vscode

### If vscode can't find sutff in `/usr/include`, do this

make a symlink for json

```shell
$ pkg-config "library name" --cflags                    # Should work, the compiler uses this to find headers.
-I"PATH_TO_JSON"                                        # Will print this, with maybe some other flags.
$ sudo ln -s "PATH_TO_JSON" /usr/include/"library name" # Creates a symlink to the install location in /usr/include.
```

# Deps

* `entt` - `brew install skypjack/entt/entt` - ECS
* `glfw` - `brew install glfw` - cross platform window/input management.
* `glm` - `brew install glm` - mathematics
* `freetype` - `brew install freetype` - font rendering
* `json` - `brew install nlohmann_json` - json serialization/deserialization
	
## Specific installation steps

### If Vscode can't find sutff in `/usr/include`, do this

make a symlink for json

```shell
$ pkg-config "library name" --cflags                    # Should work, the compiler uses this to find headers.
-I"PATH_TO_JSON"                                        # Will print this, with maybe some other flags.
$ sudo ln -s "PATH_TO_JSON" /usr/include/"library name" # Creates a symlink to the install location in /usr/include.
```

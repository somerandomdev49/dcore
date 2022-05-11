<h1 align="center">DragonCore</h1>
<p align="center">Dragon Core Game/Engine/Editor</p>

## Building

```
python3 metabuild/generate.py
ninja
bin/release/dcore # running
```

## Important!

- Have a look in `CODESTYLE.md`
- everything engine-related is in the `dcore` namespace/sub-namespace.
- everything game-related is in the `dg` namespace/sub-namespace.

## Features:

- Custom standard library, for literally no reason:
  - Better suits the codestyle, more utility functions.
  - Custom allocator.

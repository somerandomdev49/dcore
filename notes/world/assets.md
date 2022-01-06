# Assets

## Format

### Asset location:
- Path to asset: `data/dcore/main/<TYPE>/<CATEGORY>/<SUB-CATEGORY>/.../<ASSET>`. 
- `<TYPE>`: `model`, `texture`, etc.
- `<CATEGORY>`:
  - For `model`: `building`, `details`, `characters`, ...
  - For `texture`: `terrain`, `model`, `other`
- `<SUB-CATEGORY>`:
  - For `model`:
    - `buidling` category: `house`, `church`, `castle`, `inside` ...
    - `details` category: `path`, `city`, ...
  - For `texture`:
    - `terrain` category: `plains`, other zones/biomes (todo)
    - `model` category: path to model the texture is for
- `<SUB-SUB-CATEGORY>`:
  - For `model`:
    - `castle` sub-category: `wall`, `tower`, `other`, ...
    - `house` sub-category: `outside`, `inside`, `other`
    - `inside` sub-category: `door`, `bench`, `table`, `stool`, `chair`, ...
    

### Model asset format:

Each model is inside of it's own folder:
```haskell
+- AssetName -- Asset folder
   |
   +- AssetName.(blend/3ds/...) -- Source file
   |
   +- AssetName.gltf -- Exported .gltf file
   |
   +- AssetName.refs -- References/Suggestions file
   |
   +- AssetName.info -- Automatically generated after first editor run, do not edit. (not implemented yet)
```

#### Exporting .gltf files

When exporting to .gltf in Blender:
- Model:
  - Use the `Triangulate` modifier, play with the settings for better results
  - Have different materials for different textures
  - Make sure that **ALL** objects have a material (Will be fixed!)
  - *Note: the materials themselves are ignored, only the names are saved*
  - Apply all of the transforms. (May change in the future.)
    - This can be done by `Ctrl+A > All Transforms` or `Object > Apply > All Transforms`
    - This can be reversed by using `Object > Set Origin > Origin To Geometry` for each object
- In export settings on the right pane:
  - For `Format`, select `glTF Separate (.gltf + .bin + textures)`
  - Expand `Transform`, check `+Y Up`
  - Expand `Geometry`, check
    - `Apply Modifiers`
    - `UVs`
    - `Normals`
    - **NOT** `Tangents`
    - **NOT** `Vertex Colors`
    - `Materials`
  - *[Optional]* You may also check `Remember export settings` for blender to remember the settings.

#### [DEPRECATED, IGNORE] Exporting .obj files

The resulting Wavefront OBJ file should only have triangles, no materials, normals, UVs and no curves.
Preferably under 1k vertices. 

- Blender:
  - Expand `Geometry` on the right panel
  - Uncheck `Write Materials`
  - Uncheck `Object as OBJ Objects` in the `Include` panel
  - Check `Triangulate Faces`
  - Make sure `Write Normals` is checked
  - Make sure `Include UVs` is checked
- TODO: Add more software, if needed.

#### References/Suggestions file:

A reference file contains the ids of assets that are useful for the model.

##### The file's format:

```haskell
ReferenceName0: TargetId0
ReferenceName1: TargetId1
 . . . . . .  :  . . . .
```

##### Example for a tavern

##### References file:
```yaml
Door: DCore.Model.Main.Building.Inside.Door.Door01
DoorOpenSoundPack: DCore.SoundPack.Main.Building.Inside.DoorOpen.Wooden
DoorCloseSoundPack: DCore.SoundPack.Main.Building.Inside.DoorClose.Wooden
Table0: DCore.Sound.Main.Building.Inside.Table.DiningTable.02
Table1: DCore.Sound.Main.Building.Inside.Table.DiningTable.14
```

## Resource Map

File at `data/ResourceMap.ini` maps the file path to it's id. When you add a new
asset, add the id to it. The id should be of the following format:

```haskell
DCore."TYPE".Main."CATEGORY"."SUB-CATEGORY"."..."."ASSET"."SPECIFIC"
```

Note that the type/category/everything should be in PascalCase. The `."SPECIFIC"`
is not needed when there is only one such asset, otherwise it is the name of the
asset or it's number. For example, we could have 10 doors, their `SPECIFIC` would
be `00, 01, 02, ..., 10` and the folders that contain the models would be
`Door00, Door01, Door02, ..., Door10`. Or for example, you have the model for the
Goldhills castle, which means that the castle's model `SPECIFIC` is `Goldhills` and
the folder which contains the castle would be `WallGoldhills`, `TowerGoldhills`, etc. 

When exporting a `.gltf` asset, the path should be to
the folder containing the `.gltf` and `.bin` files. All other assets' paths must
be to the files themselves and not to the folders. This may change in the future.

### ResourceMap file:

```ini
...
DCore.Texture.Main.Grass = dcore/main/texture/grass.png
DCore.Texture.Main.Dirt = dcore/main/texture/dirt.png
DCore.Texture.Main.Stone = dcore/main/texture/stone.png
...
DCore.Model.Main.Building.House.Outside.Inn.01 = dcore/main/model/building/house/outside/Inn01
...
DCore.Shader.FontShader = dcore/main/shader/font.glsl
DCore.Heightmap.World1 = dcore/main/maps/terrain/world1.jpg
DCore.Font.Debug = dcore/main/fonts/courbd.ttf:32
...
```

Note that the path is just to the folder that contains the `.bin` and the `.gltf` files

## Manifest

This specifes what assets are loaded at the start of the program.

In general, the manifest already has everything needed, but in case it doesn't, the
format is the following:

```bash
# Comment
TYPE$ID
```

If the `ID` contains a `*`, it works like [glob](https://google.com/search?q=glob+unix).

**For example:**

If the resource map has the following ids:
1. `A.B.C.D.E`
2. `A.B.C.F.E`
3. `A.B.C.F.C`
4. `A.X.C.F.C`

Then:
- `A.B.*` matches: `1`, `2`, `3`
- `A.*.C` matches: `1`, `2`, `3`, `4`
- `*.C` matches: `1`, `2`, `3`, `4`
- `A.*.C.F.C` matches: `3`, `4`
- `A.*.C.F.*` matches: `2`, `3`, `4`

> Note: a `*` at the end always works, but `*`s in the middle aren't tested!

### Example

```bash

Texture$DCore.Texture.Main.*
# Texture$DCore.Texture.Gui.*
Audio$DCore.Audio.Main.*
# Mesh$DCore.Mesh.Main.*
Mesh$DCore.Mesh.*
Model$DCore.Model.*
Shader$DCore.Shader.*
Heightmap$DCore.Heightmap.World1

Font$DCore.Font.Debug

```

## World

### Inspiration
- [WoW's Stormwind][stormwind]

- Buildings
  - Houses:
    - Higher-class, 2-4 floors. [Example][houses.middle-class.example]
    - Middle-class, 2-3 floors. [Example][houses.higher-class.example]
    - Lower-class, 1-2 floors. [Example][houses.lower-class.example], but with less curves and detail.
    - + Same as above but with openings on first floors for shops and such. (less models overall)
    - + Same as above but with insides modeled. (less models overall)
  - Churches:
    - Higher-class, high gothic building, much stone. [Example][church.higher-class.example]
    - Middle-class, medium gothic building, stone in the base. [Example][church.middle-class.example]
  - Castles:
    - See [Stormwind][stormwind]
- City Details:
  - Fountains
  - Parks (?)
  - Bridges
  - Canals
  - Streets (Stone streets)
- Path Details:
  - Fences
  - Lamp posts (not tall, thicc stone base)

[stormwind]: https://www.google.com/search?q=world+of+warcraft+classic+stormwind&tbm=isch
[houses.middle-class.example]: https://www.pinterest.ru/pin/350647520989843174/
[houses.higher-class.example]: https://en.wikipedia.org/wiki/Brick_Gothic#/media/File:00_Bruges_JPG6.jpg
[houses.lower-class.example]:  https://wow.zamimg.com/uploads/screenshots/normal/877374-wcbuild.jpg
[church.higher-class.example]: https://wowpedia.fandom.com/wiki/Tranquil_Gardens_Cemetery?file=Tranquil_Gardens_Cemetery.jpg
[church.middle-class.example]: https://thumbs.dreamstime.com/z/medieval-stone-church-wooden-roof-voss-norway-46971883.jpg

## Living Things:

- Races:
  - Human: (Playable)
    - Normal
    - Gnome: Very short humans
  - Orc: (Playable) Buffer humans with low foreheads and no hair
    - Grey (Common)
    - Green (Rare)
  - Elves: (Playable) Tall humans with long ears, moon and forest have differnet ear shapes
    - Forest
    - Moon
  - Goblin: Green orkish gnomes
  - Werewolf: (maybe playable)
    
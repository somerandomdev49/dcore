# Assets

## Resource Map

File at `data/ResourceMap.ini` maps the file path to it's id. When you add a new asset, add the id to it.

## Format

### Asset location:
- Path to asset: `data/dcore/main/<TYPE>/<CATEGORY>/<SUB-CATEGORY>/.../<ASSET>`. 
- `<TYPE>`: `mesh`, `texture`, etc.
- `<CATEGORY>`:
  - For `mesh`: `building`, `details`, `characters`, ...
  - For `texture`: `terrain`, `mesh`, `other`
- `<SUB-CATEGORY>`:
  - For `mesh`:
    - `buidling` category: `house`, `church`, `castle`, `inside` ...
    - `details` category: `path`, `city`, ...
  - For `texture`:
    - `terrain` category: `plains`, other zones/biomes (todo)
    - `mesh` category: path to mesh the texture is for
- `<SUB-SUB-CATEGORY>`:
  - For `mesh`:
    - `castle` sub-category: `wall`, `tower`, `other`, ...
    - `house` sub-category: `outside`, `inside`, `other`
    - `inside` sub-category: `door`, `bench`, `table`, `stool`, `chair`, ...
    

### Mesh asset format:

Each mesh is inside of it's own folder:
```haskell
+- AssetName -- Asset folder
   |
   +- AssetName.(blend/3ds/...) -- Source file
   |
   +- AssetName.obj -- Exported .obj file
   |
   +- AssetName.refs -- References file
   |
   +- AssetName.srefs -- Suggestions file (optional)
```

#### Exporting .obj files:

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

A reference file contains the ids of assets that are used in the model,
for example a texture, another mesh (e.g. a door), a sound (maybe).

##### The file's format:

```haskell
ReferenceName0: TargetId0
ReferenceName1: TargetId1
 . . . . . .  :  . . . .
```

##### Example for a tavern

##### References file:
```yaml
# Only this door is fit for this building
Door: DCore.Mesh.Main.Building.Inside.Door.Door01
```

##### Suggestions file:
```yaml
# This file can be modified through the editor by placing assets inside of the house.
DoorOpenSoundPack: DCore.SoundPack.Main.Building.Inside.DoorOpen.Wooden
DoorCloseSoundPack: DCore.SoundPack.Main.Building.Inside.DoorClose.Wooden
Table0: DCore.Sound.Main.Building.Inside.Table.DiningTable.02
Table1: DCore.Sound.Main.Building.Inside.Table.DiningTable.14
Stool0: DCore.Sound.Main.Building.Inside.Stool.64
Stool1: DCore.Sound.Main.Building.Inside.Stool.02
Stool2: DCore.Sound.Main.Building.Inside.Stool.38
Chair0: DCore.Sound.Main.Building.Inside.Chair.Simple.03
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
    

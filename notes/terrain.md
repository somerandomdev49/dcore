# Implementation Notes: Terrain

Out terrain is split into 32x32 (for now) chunks, which make rendering and navigation easier.
We also have a heightmap (a texture that shows how high the terrain at a certain point should be)
The terrain also manages "active" chunks (the ones that are rendered and simulated)
Important: entities are only simulated if they are in range of the player.

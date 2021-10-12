# Implementation Notes: AI

Basic AI will work based on a "logic tree" which basically instructs the ai to d osomething based on some conditions.
Not hard to implement, there are probably many articles on how to do such stuff (There's probably a way to optimize the
tree into something more sensible for performance)

Pathfinding is harder though. The map is already split into chunks, so we can do an A* search per-chunk. We can also
have pre-written paths for entites to take (like a patrol path for guards). Most likely, only enemies will need to do
pathfinding. For example when you hit something, it pathfinds towards you, and (somehow) recalculating the path only
when the attacker moves more than a certain limit (e.g. when the enemy can't reach the player). We should probably have
something like a preferred minimum distance (so for example ranged enemies will have some distance). Should *not* be
that hard to implement once the entity and physics systems exist.

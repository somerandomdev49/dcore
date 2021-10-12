# Implementation Notes: Physics

## Collision Detection:

- *Sphere* vs *Plane* and *Cylinder* vs *Plane* (Maybe even *Capsule* vs *Plane*)
  For stairs and other similar stuff we will use planes. Because a character is a capsule,
  the bottom part is a sphere and thus we only need to check whether a sphere and a sphere collide.
  [Collision][sphere_plane] and [Reponse][sphere_plane_response]

- *Circle* vs *Polygon*
  Our world's collision shape is mostly defined by 2d polygons that have a uniform height.
  So we can simply check (again) for sphere and plane collision. We can optimise (if sphere
  vs plane is slower) and check for player y coordinate and the polygon's height and if it is
  inside of the polygon, we need to only check circle and polygon. [Reddit Thread][circle_triangle]


- *Sphere* vs *Heightmap*
  I'm not sure for now. Maybe convert to triangles? N-tree?
  [Article 1][terrain_1],
  [Article 2][terrain_2],
  [Article 3][terrain_3],
  [Article 4][terrain_4],
  [Article 5][terrain_5].

- *Capsule* vs *Convex*
  If there's no other choice, we can do this... [How this could be implemented][capsule_convex]
  
  
[capsule_convex]: https://github.com/DanielChappuis/reactphysics3d/blob/master/src/collision/narrowphase/CapsuleVsConvexPolyhedronAlgorithm.cpp

[sphere_plane]: https://stackoverflow.com/questions/22093749/c-plane-sphere-collision-detection
[sphere_plane_response]:https://gamedev.stackexchange.com/questions/23840/how-do-i-calculate-collision-response-between-a-sphere-and-a-plane

[circle_triangle]: https://www.reddit.com/r/gamedev/comments/xtry1/circlepolygon_collison_using_sat/

[terrain_1]: https://stackoverflow.com/questions/58739275/how-to-provide-efficient-collision-detection-and-response-for-spherical-objects
[terrain_2]: https://gamedev.stackexchange.com/questions/31844/checking-for-collisions-on-a-3d-heightmap
[terrain_3]: https://stackoverflow.com/questions/22432983/3d-terrain-model-collision
[terrain_4]: https://gamedev.stackexchange.com/questions/165111/terrain-collision-with-sphere-and-obb
[terrain_5]: https://gamedev.net/forums/topic/675055-3d-model-terrain-collision/5272986/

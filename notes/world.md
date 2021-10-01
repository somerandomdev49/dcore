# Implementation Notes: World

## World Events Management

A world-wide event is something that changes a lot of stuff in the world, and anything should be able to notified
when a world event happens. These events are saved and define the progress you have on completing the game.

## Save System

Shouldn't be *very* hard to implement, just that each manager that wants it's data to be saved does something like this:
```c++
void TheManager::Initialize()
{
    ...
    SaveSystem::Instance()->RegisterSave<TheManager>(&OnSave);
    SaveSystem::Instance()->RegisterLoad<TheManager>(&OnLoad);
    ...
}
...
void TheManager::OnSave(Saver &saver) const
{
    saver.Write(Stuff_.size());
    for(const auto &x : Stuff_)
    {
        saver.Write(x.foo);
        saver.Write(x.bar);
        saver.Save<ComplexThing>(x.comp);
    }
}

void TheManager::OnLoad(Loader &loader) const
{
    size_t size = -1; loader.Read(size);
    Stuff_.reserve(size);
    for(size_t i = 0; i < size; ++i)
    {
        glm::vec3 foo = glm::vec3(0, 0, 0); loader.Load(foo);
        int bar = 0; loader.Load(bar);
        ComplexThing comp = loader.Load<ComplexThing>(comp);
        Stuff.push_back(Thing(bar, foo, comp));
    }
}
```

## Loot Management
Should be pretty easy to do, just a table of entitytype -> `Loot`, which is a list of `Item` that contain a string id
and some simple data like `level`, `cost`.

## Spawn Management
Somehow define regions in files and load them in structures like this:

```c++
class SpawnRegion
{
public:
    
    /** Returns which type of actors the pool spawns.*/
    ActorType *GetType() const; 

    /** The actual amount of entities that are inside of the region */
    int GetPoolSize() const;

    /** The amount of entities that should be present in the region. */
    int GetPoolAllocated() const;
   
    /** Called when entities need spawning. Spawns at most `spawnCount` entities at a time. */
    void OnRandomSpawnTick(int spawnCount = DEFAULT_ENTITY_SPAWN_COUNT) const;

    constexpr int DEFAULT_ENTITY_SPAWN_COUNT = 2;
};
```

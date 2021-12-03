# Implementation Notes: Entity Management

The engine has an ECS, but we need something more specific for an rpg.
> Note: all of the code below should be copied only partially.

An actor component has all of the basic info for living things (e.g. health, effects...)

```c++
namespace dg::entity
{
    class ActorComponent : dcore::world::ComponentBase<ActorComponent>
    {
    public:
        CharacterModel *GetCharacterModel() const;

        float GetMaxHealth() const;
        float GetHealth() const;
        
        const std::vector<Effect> &GetEffects() const;
        void AddEffect(const Effect &newEffect);
        
        void Update(const dcore::world::EntityHandle &self);
        ...
    };
}
```

An effect is something that affects an actor during a period of time.

```c++
class dg::entity::Effect
{
public:
    const float GetTimeSinceBegin() const;
    const float GetEndTime() const;
    const std::string &GetName() const;
    
    void Apply(ActorComponent *target, float timeSinceLastApply);
};
```

EntityManager the class that creates the `dcore::world::Entity` entities.

```c++
class EntityManager
{
    // ActorType = { long uuid; string name; CharacterModel baseModel; List<...> modelVariations; Map<...> stats; ... }
    // SpawnPoint = { glm::vec3 position; glm::quat rotation; }
    void Spawn(ActorType *type, const SpawnPoint &where);
};
```

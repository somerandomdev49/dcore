class dg::entity::ActorComponent
{
public:
    float GetMaxHealth() const;
    float GetHealth() const;
    
    const std::vector<Effect> &GetEffects() const;
    void AddEffect(const Effect &newEffect);

    CharacterModel *GetCharacterModel() const;
    
    static void Update(ActorComponent *self);
}
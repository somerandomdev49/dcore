class dg::entity::ActorComponent {
public:
	float GetMaxHealth() const;
	float GetHealth() const;

	const std::vector<Effect> &GetEffects() const;
	void AddEffect(const Effect &newEffect);

	CharacterModel *GetCharacterModel() const;

	static void Update(ActorComponent *self);
}

class dg::entity::Effect {
public:
	const float GetTimeSinceBegin() const;
	const float GetEndTime() const;
	const std::string &GetName() const;

	void Apply(ActorComponent *target, float timeSinceLastApply);
};
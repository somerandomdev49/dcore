#pragma once
#include <dcore/World/Entity.hpp>

#include <dg/Entity/Effect.hpp>

#include <vector>

namespace dg::entity
{
	class ActorComponent : public dcore::world::ComponentBase<ActorComponent>
	{
	public:
		float GetMaxHealth() const;
		float GetHealth() const;
		float GetBaseDamage() const;

		const std::vector<Effect> &GetEffects() const;
		void AddEffect(const Effect &newEffect);

		// CharacterModel *GetCharacterModel() const;

		void Update(dcore::world::EntityHandle &self);

	private:
		float MaxHealth_, Health_, BaseDamage_;
	};
} // namespace dg::entity

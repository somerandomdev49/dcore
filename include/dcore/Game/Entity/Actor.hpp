#include <vector>
#include <dcore/World/Entity.hpp>
#include <dcore/Game/Entity/Effect.hpp>

namespace dg::entity
{
	class ActorComponent
	{
	public:
		float GetMaxHealth() const;
		float GetHealth() const;
		float GetBaseDamage() const;

		const std::vector<Effect> &GetEffects() const;
		void AddEffect(const Effect &newEffect);

		// CharacterModel *GetCharacterModel() const;

		void Update(dcore::world::Entity &entity);

	private:
		float MaxHealth_, Health_, BaseDamage_;
	};
} // namespace dg::entity

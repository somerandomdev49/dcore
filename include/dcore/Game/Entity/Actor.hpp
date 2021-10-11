#include <vector>
#include <dcore/Game/Entity/Effect.hpp>

namespace dg::entity
{
	class ActorComponent
	{
	public:
		float GetMaxHealth() const;
		float GetHealth() const;

		const std::vector<Effect> &GetEffects() const;
		void AddEffect(const Effect &newEffect);

		// CharacterModel *GetCharacterModel() const;

		static void Update(ActorComponent *self);

	private:
		float MaxHealth_, Health_;
	};
} // namespace dg::entity

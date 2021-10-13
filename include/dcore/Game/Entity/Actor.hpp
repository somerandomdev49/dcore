#include <vector>
#include <dcore/Game/Entity/Effect.hpp>

namespace dg::entity
{
	class ActorComponent
	{
	public:
		float GetMaxHealth() const;
		float GetHealth() const;
		float GetBasicDamage() const;

		const std::vector<Effect> &GetEffects() const;
		void AddEffect(const Effect &newEffect);

		// CharacterModel *GetCharacterModel() const;

		static void Update(ActorComponent *self);

	private:
		float MaxHealth_, Health_, BasicDamage_;

		float GetMaxHealth() {
			return MaxHealth_;
		}

		float GetHealth() {
			return Health_;
		}

		float BasicDamage() {
			return BasicDamage_;
		}
	};
} // namespace dg::entity

#include <string>

namespace dg::entity
{
	class ActorComponent;

	class Effect
	{
	public:
		float GetTimeSinceBegin() const;
		float GetEndTime() const;
		const std::string &GetName() const;

		void Apply(ActorComponent *target, float timeSinceLastApply);
	};
} // namespace dg::entity

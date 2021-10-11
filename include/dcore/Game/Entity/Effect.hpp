#include <string>

namespace dg::entity
{
	class ActorComponent;

	class Effect
	{
	public:
		const float GetTimeSinceBegin() const;
		const float GetEndTime() const;
		const std::string &GetName() const;

		void Apply(ActorComponent *target, float timeSinceLastApply);
	};
} // namespace dg::entity

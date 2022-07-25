#include <dcore/Core/Type.hpp>
#include <dcore/Renderer/RModel.hpp>
#include <dcore/AI/BehaviourTree.hpp>

namespace dg::content
{
	using ActorId = dstd::USize;

	struct ActorInfo {
		ActorId Id;
		const char *Name;
		dcore::graphics::Model *Model;
		dcore::ai::RBehaviourTree *Behaviour;
	};

	class ActorRegistry
	{
	public:
		ActorInfo &Get(ActorId id);

		ActorId RegisterActor(const char *displayName);
	};
}

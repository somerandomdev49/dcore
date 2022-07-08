#include <dcore/World/World.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
	class ColliderComponent : world::ComponentBase<ColliderComponent>
	{
	public:
		template<typename T>
		ColliderComponent(Collider *collider, typename T::ResourceType *resource)
			: Collider_(collider), Resource_(resource) { }

		void Start(const dcore::world::EntityHandle &self);
		void End(const dcore::world::EntityHandle &self);
		
		Collider *GetCollider() const { return Collider_; }
	private:
		Collider *Collider_;
		void *Resource_;
	};
}

#include <dcore/World/World.hpp>
#include <dcore/Physics/Collider.hpp>

namespace dcore::physics
{
	/**
	 * @brief Holds a @ref dcore::physics::Collider
	 * 
	 */
	class ColliderComponent : world::ComponentBase<ColliderComponent>
	{
	public:
		/**
		 * @brief Constructs a collider with a given collider resource.
		 * 
		 * @tparam T The collider type. Must have a ResourceType.
		 * @param collider The source collider object.
		 * @param resource The collider data resource.
		 */
		template<typename T>
		ColliderComponent(Collider *collider, typename T::ResourceType *resource)
		    : Collider_(collider), Resource_(resource)
		{
		}

		void Start(const dcore::world::EntityHandle &self);
		void End(const dcore::world::EntityHandle &self);

		Collider *GetCollider() const { return Collider_; }

	private:
		Collider *Collider_;
		void *Resource_;
	};
} // namespace dcore::physics

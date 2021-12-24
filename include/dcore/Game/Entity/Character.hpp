#pragma once
#include <dcore/Game/Entity/Inventory.hpp>
#include <dcore/Game/Entity/Actor.hpp>
#include <dcore/World/Entity.hpp>
#include <dcore/Game/Quest.hpp>
#include <dcore/Uni.hpp>
#include <vector>

#define DCORE_INV_BASE_SLOTS       16
#define DCORE_INV_SMALL_BAG_SLOTS  8
#define DCORE_INV_MEDIUM_BAG_SLOTS 16
#define DCORE_INV_BIG_BAG_SLOTS    32

namespace dg::entity
{
	class CharacterComponent : dcore::world::ComponentBase<CharacterComponent>
	{
	public:
		Quest *GetAssignedQuest(int index) const;
		const std::vector<Quest *> &GetAssignedQuests() const;

	private:
		std::vector<Spell*> Spells_;
		std::vector<Inventory> Bags_;
		std::vector<Quest *> AssignedQuests_;
	};
} // namespace dg::entity
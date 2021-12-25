#pragma once
#include <dcore/World/Entity.hpp>
#include <dcore/Uni.hpp>

#include <dg/Entity/Inventory.hpp>
#include <dg/Entity/Actor.hpp>
#include <dg/Spell.hpp>
#include <dg/Quest.hpp>

#include <vector>

#define DCORE_INV_BASE_SLOTS       16
#define DCORE_INV_SMALL_BAG_SLOTS  8
#define DCORE_INV_MEDIUM_BAG_SLOTS 16
#define DCORE_INV_BIG_BAG_SLOTS    32

namespace dg::entity
{
	class CharacterComponent : public dcore::world::ComponentBase<CharacterComponent>
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
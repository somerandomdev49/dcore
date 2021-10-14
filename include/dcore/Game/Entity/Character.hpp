#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <dcore/Game/Entity/Actor.hpp>
#include <cinttypes>

namespace dg::entity
{
	const int MAX_INVENTORY_SIZE = 5;

	class Character : public Actor
	{
	public:
		Quest *GetAssignedQuest(int index) const;
		const std::vector<Quest> &GetAssignedQuests() const;

	private:
		Inventory Inventory_;
		uint32_t ItemType;
		uint16_t Mods;
		std::vector<Quest *> AssignedQuests_;
	}
} // namespace dg::entity
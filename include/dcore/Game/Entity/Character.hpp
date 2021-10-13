#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <dcore/Game/Entity/Actor.hpp>

namespace dg::entity {
    const MAX_INVENTORY_SIZE = 5;

    struct Item {
        std::string name;
    }

    struct Inventory {
        int size;
        Item items[MAX_INVENTORY_SIZE];
    }

    struct Quest {
        int id;
        std::string name;
    }

    class Character: public Actor {
        public:
            Inventory GetInventory() const;
            int GetInventorySize const;
            
        private:
            Inventory inventory;
            Quest quests[];

            Inventory GetInventory() {
                return Inventory.items;
            }

            int GetInventorySize() {
                return Inventory.size;
            }

            Quest GetQuest() {
                return Quests[0];
            }
    }
}
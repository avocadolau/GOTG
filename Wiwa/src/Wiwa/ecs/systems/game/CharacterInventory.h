#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/Systems.h>
#include "Wiwa/ecs/components/game/items/Item.h"

namespace Wiwa
{
    // This system only takes care of the pickup system
    class WI_API CharacterInventory : public System
    {
    private:
        bool buyItem = false;
        bool shopActive = false;
        bool releasingShop = false;
        Item* currentItem;
    public:
        CharacterInventory() = default;
        virtual ~CharacterInventory() = default;

        void OnUpdate() override;

        void ShopElement(Item* item);

        void OnCollisionEnter(Object* body1, Object* body2) override;

        bool GetShopActive() { return shopActive; }

        Item* GetCurrentShopItem() { return currentItem; }
    };
}
REGISTER_SYSTEM(Wiwa::CharacterInventory);
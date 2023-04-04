#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/Systems.h>

namespace Wiwa
{
    // This system only takes care of the pickup system
    class WI_API CharacterInventory : public System
    {
    public:
        CharacterInventory() = default;
        virtual ~CharacterInventory() = default;

        void OnCollisionEnter(Object* body1, Object* body2) override;
    };
}
    
REGISTER_SYSTEM(Wiwa::CharacterInventory);
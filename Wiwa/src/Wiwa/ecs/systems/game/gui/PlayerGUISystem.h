#pragma once
#include <Wiwa/core/Core.h>
#include <Wiwa/ecs/Systems.h>

namespace Wiwa
{
    // This system only takes care of the pickup system
    class WI_API PlayerGUISystem : public System
    {
    public:
        PlayerGUISystem() = default;
        virtual ~PlayerGUISystem() = default;

        void OnUpdate() override;

    };
}

REGISTER_SYSTEM(Wiwa::PlayerGUISystem);
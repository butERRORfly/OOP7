#pragma once
#include <memory>

namespace game::core {
    class Npc;
}

namespace game::events {
    class FightObserver {
    public:
        virtual ~FightObserver() = default;
        virtual void on_fight(std::shared_ptr<core::Npc> killer,
                           std::shared_ptr<core::Npc> victim, 
                           bool win) = 0;
    };
}
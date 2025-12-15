#pragma once
#include "npc.h"
#include <memory>

namespace game::core {
    class NpcFactory {
    public:
        static std::shared_ptr<Npc> createNpc(NpcType type, const std::string &name, const Position &pos);
    };
}

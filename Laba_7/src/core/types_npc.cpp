#include "../../include/core/types_npc.h"

namespace game::core {
    std::string npcTypeToString(NpcType type) {
        switch (type) {
            case NpcType::Slaver: return "Slaver";
            case NpcType::KnightErrant: return "KnightErrant";
            case NpcType::Squirrel: return "Squirrel";
            default: return "Unknown";
        }
    }

    NpcType stringToNpcType(const std::string& typeStr) {
        if (typeStr == "Slaver") return NpcType::Slaver;
        if (typeStr == "KnightErrant") return NpcType::KnightErrant;
        if (typeStr == "Squirrel") return NpcType::Squirrel;
        return NpcType::Unknown;
    }
}
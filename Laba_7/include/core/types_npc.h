#pragma once
#include <string>

namespace game::core {
    enum class NpcType {
        Unknown = 0,
        Slaver = 1,
        KnightErrant = 2,
        Squirrel = 3,
    };
    
    std::string npcTypeToString(NpcType type);
    NpcType stringToNpcType(const std::string& typeStr);
}
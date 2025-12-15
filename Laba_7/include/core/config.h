#pragma once

namespace game::core {
    constexpr int MAP_WIDTH{20};
    constexpr int MAP_HEIGHT{20};
    constexpr int MAP_INTERVAL_UPDATE{1000}; // 1 second
    constexpr int GAME_DURATION{30000}; // 30 second
    constexpr int NPC_COUNT{50};

    constexpr int KNIGHT_ERRANT_MOVEMENT{30};
    constexpr int SLAVER_MOVEMENT{10};
    constexpr int SQUIRREL_MOVEMENT{5};

    constexpr int KNIGHT_ERRANT_DISTANCE_KILL{10};
    constexpr int SLAVER_DISTANCE_KILL{10};
    constexpr int SQUIRREL_DISTANCE_KILL{5};
};

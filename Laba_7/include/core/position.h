#pragma once
#include <cmath>
#include "config.h"

namespace game::core {
    struct Position {
        int x, y;

        Position(int x = 0, int y = 0) : x(x), y(y) {
        };

        [[nodiscard]] double distanceTo(const Position &other) const {
            return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
        }

        [[nodiscard]] bool isValid() const {
            return x >= 0 && x <= MAP_WIDTH && y >= 0 && y <= MAP_HEIGHT;
        }
    };
}

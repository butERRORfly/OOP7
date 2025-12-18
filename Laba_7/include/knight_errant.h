#pragma once
#include "core/npc.h"

namespace game::core {
    class KnightErrant : public Npc {
    public:
        KnightErrant(const std::string &name, const Position &pos);

        bool fight(std::shared_ptr<Slaver> other) override;

        bool fight(std::shared_ptr<KnightErrant> other) override;

        bool fight(std::shared_ptr<Squirrel> other) override;

        void print() override;

        void move(int max_x, int max_y) override;

        bool accept(std::shared_ptr<Npc> visitor) override;
    };
}

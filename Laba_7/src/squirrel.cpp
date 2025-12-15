#include "../include/squirrel.h"
#include "../include/knight_errant.h"
#include "../include/slaver.h"
#include <random>
#include <cstdlib>
#include <ctime>

namespace game::core {
    Squirrel::Squirrel(const std::string &name, const Position &pos)
        : Npc(NpcType::Squirrel, name, pos) {
    }

    void Squirrel::print() {
        std::cout << *this;
    }

    bool Squirrel::fight(std::shared_ptr<Slaver> other) {
        fight_notify(other, false);
        return false;
    }

    bool Squirrel::fight(std::shared_ptr<KnightErrant> other) {
        fight_notify(other, false);
        return false;
    }

    bool Squirrel::fight(std::shared_ptr<Squirrel> other) {
        fight_notify(other, true);
        return true;
    }

    bool Squirrel::accept(std::shared_ptr<Npc> visitor) {
        return fight(std::shared_ptr<Squirrel>(this, [](Squirrel *) {
        }));
    }

    void Squirrel::move(int max_x, int max_y) {
        std::lock_guard<std::mutex> lock(mtx_);

        if (!alive_) return;

        static thread_local std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(
            -SQUIRREL_MOVEMENT,
            SQUIRREL_MOVEMENT
        );

        int dx = dist(gen);
        int dy = dist(gen);

        Position new_pos = position_;
        new_pos.x += dx;
        new_pos.y += dy;

        if (new_pos.x >= 0 && new_pos.x < max_x &&
            new_pos.y >= 0 && new_pos.y < max_y) {
            position_ = new_pos;
        }
    }
}

#include "../include/slaver.h"
#include "../include/knight_errant.h"
#include "../include/squirrel.h"
#include <random>

namespace game::core {
    Slaver::Slaver(const std::string &name, const Position &pos)
        : Npc(NpcType::Slaver, name, pos) {
    }

    void Slaver::print() {
        std::cout << *this;
    }

    bool Slaver::fight(std::shared_ptr<Slaver> other) {
        fight_notify(other, false);
        return false;
    }

    bool Slaver::fight(std::shared_ptr<KnightErrant> other) {
        fight_notify(other, false);
        return false;
    }

    bool Slaver::fight(std::shared_ptr<Squirrel> other) {
        fight_notify(other, true);
        return true;
    }

    bool Slaver::accept(std::shared_ptr<Npc> visitor) {
        return fight(std::shared_ptr<Slaver>(this, [](Slaver *) {
        }));
    }

    void Slaver::move(int max_x, int max_y) {
        std::lock_guard<std::mutex> lock(mtx_);

        if (!alive_) return;

        static thread_local std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(
            -SLAVER_MOVEMENT,
            SLAVER_MOVEMENT
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

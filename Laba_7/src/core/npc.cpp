#include "../../include/core/npc.h"

#include <shared_mutex>
#include <utility>

#include "../../include/events/fight_observer.h"

namespace game::core {
    Npc::Npc(NpcType type, std::string name, const Position &pos)
        : type_(type), name_(std::move(name)), position_(pos) {
    }

    void Npc::subscribe(const std::shared_ptr<events::FightObserver> &observer) {
        observers_.push_back(observer);
    }

    void Npc::fight_notify(const std::shared_ptr<Npc> &defender, bool win) {
        for (auto &observer: observers_) {
            observer->on_fight(std::shared_ptr<Npc>(this, [](Npc *) {
            }), defender, win);
        }
    }

    bool Npc::is_close(const std::shared_ptr<Npc> &other, int distance) {
        std::lock_guard<std::mutex> lck(mtx_);

        return position_.distanceTo(other->position_) <= distance;
    }

    std::ostream &operator<<(std::ostream &os, const Npc &npc) {
        os << "'" << npc.name_ << "' ";
        return os;
    }

    bool Npc::is_alive() const {
        return alive_;
    }

    void Npc::must_die() {
        std::lock_guard<std::mutex> lck(mtx_);
        alive_ = false;
    }

    NpcType Npc::get_type() const {
        return type_;
    }

    Position Npc::get_position() const {
        return position_;
    }

    std::string Npc::get_name() const {
        return name_;
    }
}

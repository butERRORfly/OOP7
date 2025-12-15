#pragma once
#include "types_npc.h"
#include "position.h"
#include "../events/fight_observer.h"
#include <memory>
#include <iostream>
#include <mutex>
#include <vector>

namespace game::events {
    class BattleVisitor;
}

namespace game::core {
    class KnightErrant;
    class Slaver;
    class Squirrel;

    class Npc {
        friend std::ostream &operator<<(std::ostream &os, const Npc &npc);

    protected:
        std::mutex mtx_;

        NpcType type_;
        Position position_;
        std::string name_;
        bool alive_{true};

        std::vector<std::shared_ptr<events::FightObserver> > observers_;

    public:
        Npc(NpcType type, std::string name, const Position &pos);

        virtual ~Npc() = default;

        void subscribe(const std::shared_ptr<events::FightObserver> &observer);

        void fight_notify(const std::shared_ptr<Npc> &defender, bool win);

        [[nodiscard]] bool is_close(const std::shared_ptr<Npc> &other, int distance);

        virtual bool fight(std::shared_ptr<Slaver> other) = 0;

        virtual bool fight(std::shared_ptr<KnightErrant> other) = 0;

        virtual bool fight(std::shared_ptr<Squirrel> other) = 0;

        virtual bool accept(std::shared_ptr<Npc> visitor) = 0;

        virtual void print() = 0;

        virtual void move(int max_x, int max_y) = 0;

        [[nodiscard]] NpcType get_type() const;

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] Position get_position() const;

        [[nodiscard]] bool is_alive() const;

        void must_die();
    };
}

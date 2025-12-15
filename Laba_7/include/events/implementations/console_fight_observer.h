#pragma once

#include <memory>
#include "../../core/npc.h"
#include "../../events/fight_observer.h"

class ConsoleObserver : public game::events::FightObserver {
private:
    ConsoleObserver() = default;

public:
    static std::shared_ptr<game::events::FightObserver> get();

    void on_fight(std::shared_ptr<game::core::Npc> attacker,
                  std::shared_ptr<game::core::Npc> defender,
                  bool win) override;
};

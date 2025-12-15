#include "../../../include/events/fight_observer.h"
#include "../../../include/events/implementations/console_fight_observer.h"
#include <iostream>
#include <mutex>
#include <memory>

namespace {
    std::mutex print_mutex;
}

std::shared_ptr<game::events::FightObserver> ConsoleObserver::get() {
    static ConsoleObserver instance;
    return {
        &instance, [](game::events::FightObserver *) {
        }
    };
}

void ConsoleObserver::on_fight(const std::shared_ptr<game::core::Npc> attacker,
                               const std::shared_ptr<game::core::Npc> defender,
                               bool win) {
    if (win) {
        std::lock_guard<std::mutex> lck(print_mutex);
        std::cout << std::endl
                << "Murder --------";
        defender->print();
    }
}

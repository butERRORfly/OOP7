#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <random>

#include "include/core/factory_npc.h"
#include "include/editor/game_map.h"
#include "include/core/types_npc.h"
#include "include/core/position.h"
#include "include/core/config.h"
#include <set>
#include <queue>
#include <optional>

using namespace std::chrono_literals;
using set_t = std::set<std::shared_ptr<game::core::Npc> >;
std::mutex cout_mutex;

int random_int(const int min, const int max) {
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void clear_screen() {
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
}

struct FightEvent {
    std::shared_ptr<game::core::Npc> attacker;
    std::shared_ptr<game::core::Npc> defender;
};

class FightManager {
private:
    std::queue<FightEvent> events;
    std::atomic<bool> running_{false};

    FightManager() = default;

    std::mutex mtx;

public:
    static FightManager &get() {
        static FightManager instance;
        return instance;
    }

    void add_event(FightEvent &&event) {
        std::lock_guard<std::mutex> lck(mtx);
        events.push(event);
    }


    void start() {
        running_ = true;
    }

    void stop() {
        running_ = false;
    }

    void operator()() {
        while (running_) {
            std::optional<FightEvent> event;

            {
                std::lock_guard<std::mutex> lck(mtx);
                if (!events.empty()) {
                    event = std::move(events.front());
                    events.pop();
                }
            }

            if (event) {
                if (event->attacker->is_alive() &&
                    event->defender->is_alive() &&
                    event->defender->accept(event->attacker)) {
                    int attack_power = random_int(1, 6);
                    int defense_power = random_int(1, 6);

                    {
                        std::lock_guard<std::mutex> cout_lock(cout_mutex);
                        std::cout << "\n" << event->attacker->get_name() << " атакует "
                                << event->defender->get_name()
                                << " (атака: " << attack_power
                                << ", защита: " << defense_power << ")\n";
                    }


                    if (attack_power > defense_power) {
                        event->defender->must_die();

                        std::lock_guard<std::mutex> cout_lock(mtx);
                    } else {
                        std::lock_guard<std::mutex> cout_lock(mtx);
                    }
                }
            }

            std::this_thread::sleep_for(100ms);
        }
    }
};


int main() {
    try {
        auto game_map = std::make_shared<game::editor::GameMap>(
            game::core::MAP_WIDTH,
            game::core::MAP_HEIGHT
        );

        auto factory = std::make_unique<game::core::NpcFactory>();
        set_t all_npc;

        for (size_t i = 0; i < game::core::NPC_COUNT; ++i) {
            game::core::NpcType type;

            int random_type = random_int(0, 2) % 3;
            switch (random_type) {
                case 0: type = game::core::NpcType::KnightErrant;
                    break;
                case 1: type = game::core::NpcType::Slaver;
                    break;
                case 2: type = game::core::NpcType::Squirrel;
                    break;
            }

            int x = random_int(0, game::core::MAP_WIDTH) % game::core::MAP_WIDTH;
            int y = random_int(0, game::core::MAP_HEIGHT) % game::core::MAP_HEIGHT;

            std::string name;
            switch (type) {
                case game::core::NpcType::KnightErrant:
                    name = "Knight_" + std::to_string(i);
                    break;
                case game::core::NpcType::Slaver:
                    name = "Slaver_" + std::to_string(i);
                    break;
                case game::core::NpcType::Squirrel:
                    name = "Squirrel_" + std::to_string(i);
                    break;
            }

            auto npc = game::core::NpcFactory::createNpc(type, name, {x, y});

            if (npc) {
                game_map->add_npc(npc);
                all_npc.insert(npc);
            } else {
                std::cerr << "Failed to create NPC of type " << static_cast<int>(type)
                        << " at position (" << x << ", " << y << ")" << std::endl;
            }
        }

        std::cout << "=== НАЧАЛЬНАЯ КАРТА ===\n";
        std::cout << game_map->to_string();

        auto &fight_manager = FightManager::get();
        fight_manager.start();

        std::thread fight_thread(std::ref(fight_manager));

        std::atomic<bool> game_running{true};

        std::thread move_thread([&all_npc, &game_map, &fight_manager, &game_running]() {
            constexpr int MAX_X = game::core::MAP_WIDTH;
            constexpr int MAX_Y = game::core::MAP_HEIGHT;

            constexpr int KNIGHT_DISTANCE = game::core::KNIGHT_ERRANT_DISTANCE_KILL;
            constexpr int SLAVER_DISTANCE = game::core::SLAVER_DISTANCE_KILL;
            constexpr int SQUIRREL_DISTANCE = game::core::SQUIRREL_DISTANCE_KILL;

            while (game_running) {
                for (const auto &npc: all_npc) {
                    if (npc->is_alive()) {
                        int max_move = 0;
                        switch (npc->get_type()) {
                            case game::core::NpcType::KnightErrant:
                                max_move = game::core::KNIGHT_ERRANT_MOVEMENT;
                                break;
                            case game::core::NpcType::Slaver:
                                max_move = game::core::SLAVER_MOVEMENT;
                                break;
                            case game::core::NpcType::Squirrel:
                                max_move = game::core::SQUIRREL_MOVEMENT;
                                break;
                        }

                        int dx = random_int(-max_move, max_move);
                        int dy = random_int(-max_move, max_move);

                        auto pos = npc->get_position();
                        int new_x = pos.x + dx;
                        int new_y = pos.y + dy;

                        if (new_x < 0) new_x = 0;
                        if (new_x >= MAX_X) new_x = MAX_X - 1;
                        if (new_y < 0) new_y = 0;
                        if (new_y >= MAX_Y) new_y = MAX_Y - 1;

                        game_map->move_npc(npc, new_x, new_y);
                    }
                }

                for (const auto &npc: all_npc) {
                    if (!npc->is_alive()) continue;

                    for (const auto &other: all_npc) {
                        if (other == npc || !other->is_alive()) continue;

                        int kill_distance = 0;
                        switch (npc->get_type()) {
                            case game::core::NpcType::KnightErrant:
                                kill_distance = KNIGHT_DISTANCE;
                                break;
                            case game::core::NpcType::Slaver:
                                kill_distance = SLAVER_DISTANCE;
                                break;
                            case game::core::NpcType::Squirrel:
                                kill_distance = SQUIRREL_DISTANCE;
                                break;
                        }

                        if (npc->is_close(other, kill_distance)) {
                            fight_manager.add_event({npc, other});
                        }
                    }
                }

                std::this_thread::sleep_for(10ms);
            }
        });

        auto game_start_time = std::chrono::steady_clock::now();

        while (game_running) {
            std::this_thread::sleep_for(1s);

            auto elapsed = std::chrono::steady_clock::now() - game_start_time;
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

            clear_screen();
            std::cout << game_map->to_string();

            if (seconds >= game::core::GAME_DURATION / 1000) {
                game_running = false;
            }
        }


        fight_manager.stop();
        game_running = false;

        if (move_thread.joinable()) {
            move_thread.join();
        }

        if (fight_thread.joinable()) {
            fight_thread.join();
        }

        std::cout << "=== ФИНАЛЬНАЯ КАРТА ===\n";
        std::cout << game_map->to_string();

        std::cout << "\n=== ВЫЖИВШИЕ NPC ===\n";
        int survivors = 0;
        for (const auto &npc: all_npc) {
            if (npc->is_alive()) {
                std::cout << npc->get_name() << " ("
                        << npcTypeToString(npc->get_type())
                        << ") at (" << npc->get_position().x << ", "
                        << npc->get_position().y << ")\n";
                survivors++;
            }
        }
        std::cout << "Всего выжило: " << survivors << " из " << all_npc.size() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#pragma once
#include <vector>
#include <memory>
#include <string>
#include "../core/npc.h"


namespace game::editor {
    class GameMap {
    private:
        int width, height;
        std::vector<std::shared_ptr<core::Npc> > npcs{};
        mutable std::mutex mtx;

    public:
        GameMap(int w, int h) : width(w), height(h) {
        }

        void add_npc(const std::shared_ptr<core::Npc> &npc) {
            std::lock_guard lock(mtx);
            npcs.push_back(npc);
        }

        std::string to_string() const {
            std::lock_guard lock(mtx);

            std::vector<std::string> grid(height, std::string(width, ' '));

            for (const auto &npc: npcs) {
                if (npc->is_alive()) {
                    auto [x, y] = npc->get_position();
                    if (x >= 0 && x < width && y >= 0 && y < height) {
                        char symbol = '?';
                        switch (npc->get_type()) {
                            case core::NpcType::KnightErrant: symbol = 'K';
                                break;
                            case core::NpcType::Slaver: symbol = 'S';
                                break;
                            case core::NpcType::Squirrel: symbol = 'Q';
                                break;
                            default:
                                break;
                        }
                        grid[y][x] = symbol;
                    }
                }
            }

            std::string result = "\n Map\n";
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    char c = grid[y][x];
                    result += '[';
                    result += (c == ' ' ? ' ' : c);
                    result += ']';
                }
                result += '\n';
            }

            return result;
        }

        std::vector<std::shared_ptr<core::Npc> > get_all_npcs() const {
            std::lock_guard lock(mtx);
            return npcs;
        }

        bool move_npc(const std::shared_ptr<core::Npc> &npc, int new_x, int new_y) {
            std::lock_guard lock(mtx);

            if (!npc || !npc->is_alive()) {
                return false;
            }

            if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) {
                return false;
            }

            for (const auto &other: npcs) {
                if (other != npc && other->is_alive()) {
                    auto other_pos = other->get_position();
                    if (other_pos.x == new_x && other_pos.y == new_y) {
                        return false;
                    }
                }
            }

            npc->move(new_x, new_y);
            return true;
        }
    };
};

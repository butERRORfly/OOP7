#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <thread>

#include "../include/core/factory_npc.h"
#include "../include/editor/game_map.h"
#include "../include/core/types_npc.h"
#include "../include/core/position.h"
#include "../include/core/config.h"
#include "knight_errant.h"
#include "slaver.h"
#include "squirrel.h"

using namespace game::core;

class NpcTest : public ::testing::Test {
protected:
    void SetUp() override {
        knight = std::make_shared<KnightErrant>("Knight1", Position{0, 0});
        slaver = std::make_shared<Slaver>("Slaver1", Position{5, 5});
        squirrel = std::make_shared<Squirrel>("Squirrel1", Position{10, 10});
    }

    std::shared_ptr<KnightErrant> knight;
    std::shared_ptr<Slaver> slaver;
    std::shared_ptr<Squirrel> squirrel;
};

TEST_F(NpcTest, NPCInitialization) {
    EXPECT_EQ(knight->get_name(), "Knight1");
    EXPECT_EQ(knight->get_type(), NpcType::KnightErrant);
    EXPECT_EQ(knight->get_position().x, 0);
    EXPECT_EQ(knight->get_position().y, 0);
    EXPECT_TRUE(knight->is_alive());

    EXPECT_EQ(slaver->get_name(), "Slaver1");
    EXPECT_EQ(squirrel->get_name(), "Squirrel1");
}

TEST_F(NpcTest, NPCDeath) {
    EXPECT_TRUE(knight->is_alive());
    knight->must_die();
    EXPECT_FALSE(knight->is_alive());
}

TEST_F(NpcTest, IsCloseCalculation) {
    auto knight2 = std::make_shared<KnightErrant>("Knight2", Position{3, 4});

    EXPECT_TRUE(knight->is_close(knight2, 5));
    EXPECT_FALSE(knight->is_close(knight2, 4));
}

TEST(NpcFactoryTest, CreateNPC) {
    auto knight = NpcFactory::createNpc(
        NpcType::KnightErrant,
        "TestKnight",
        Position{1, 1}
    );
    EXPECT_NE(knight, nullptr);
    EXPECT_EQ(knight->get_type(), NpcType::KnightErrant);
    EXPECT_EQ(knight->get_name(), "TestKnight");

    auto slaver = NpcFactory::createNpc(
        NpcType::Slaver,
        "TestSlaver",
        Position{2, 2}
    );
    EXPECT_NE(slaver, nullptr);
    EXPECT_EQ(slaver->get_type(), NpcType::Slaver);

    auto squirrel = NpcFactory::createNpc(
        NpcType::Squirrel,
        "TestSquirrel",
        Position{3, 3}
    );
    EXPECT_NE(squirrel, nullptr);
    EXPECT_EQ(squirrel->get_type(), NpcType::Squirrel);
}

TEST(NpcFactoryTest, CreateInvalidNPC) {
    auto invalid = NpcFactory::createNpc(
        static_cast<NpcType>(999),
        "Invalid",
        Position{0, 0}
    );
    EXPECT_EQ(invalid, nullptr);
}

class GameMapTest : public ::testing::Test {
protected:
    void SetUp() override {
        game_map = std::make_shared<game::editor::GameMap>(
            MAP_WIDTH,
            MAP_HEIGHT
        );

        knight = std::make_shared<KnightErrant>("MapKnight", Position{0, 0});
        slaver = std::make_shared<Slaver>("MapSlaver", Position{5, 5});
    }

    std::shared_ptr<game::editor::GameMap> game_map;
    std::shared_ptr<KnightErrant> knight;
    std::shared_ptr<Slaver> slaver;
};

TEST_F(GameMapTest, StringRepresentation) {
    game_map->add_npc(knight);
    game_map->add_npc(slaver);

    auto str = game_map->to_string();
    EXPECT_FALSE(str.empty());
    EXPECT_GT(str.length(), 0);
}

TEST(FightLogicTest, KnightVsSquirrel) {
    auto knight = std::make_shared<KnightErrant>("Knight", Position{0, 0});
    auto squirrel = std::make_shared<Squirrel>("Squirrel", Position{1, 1});

    EXPECT_TRUE(squirrel->accept(knight));
}

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = std::make_unique<NpcFactory>();
        all_npc.clear();
    }

    std::unique_ptr<NpcFactory> factory;
    std::vector<std::shared_ptr<Npc> > all_npc;
};

TEST_F(IntegrationTest, FightSimulation) {
    auto knight = std::make_shared<KnightErrant>("Knight", Position{0, 0});
    auto squirrel = std::make_shared<Squirrel>("Squirrel", Position{1, 1});

    knight->move(0, 0);
    squirrel->move(0, 0);

    EXPECT_TRUE(squirrel->accept(knight));

    knight->must_die();
    EXPECT_FALSE(knight->is_alive());
    EXPECT_TRUE(squirrel->is_alive());
}

TEST(ConfigTest, ConstantsDefined) {
    EXPECT_GT(MAP_WIDTH, 0);
    EXPECT_GT(MAP_HEIGHT, 0);
    EXPECT_GT(NPC_COUNT, 0);
    EXPECT_GT(GAME_DURATION, 0);

    EXPECT_GT(KNIGHT_ERRANT_MOVEMENT, 0);
    EXPECT_GT(SLAVER_MOVEMENT, 0);
    EXPECT_GT(SQUIRREL_MOVEMENT, 0);

    EXPECT_GT(KNIGHT_ERRANT_DISTANCE_KILL, 0);
    EXPECT_GT(SLAVER_DISTANCE_KILL, 0);
    EXPECT_GT(SQUIRREL_DISTANCE_KILL, 0);
}

TEST(ThreadSafetyTest, ConcurrentAccess) {
    auto game_map = std::make_shared<game::editor::GameMap>(20, 20);
    auto knight = std::make_shared<KnightErrant>("ThreadKnight", Position{0, 0});

    game_map->add_npc(knight);

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([game_map, knight, i]() {
            game_map->move_npc(knight, i, i);
        });
    }

    for (auto &t: threads) {
        t.join();
    }

    EXPECT_TRUE(knight->is_alive());
    EXPECT_GE(knight->get_position().x, 0);
    EXPECT_GE(knight->get_position().y, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

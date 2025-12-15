#include "../../include/core/factory_npc.h"
#include "../../include/squirrel.h"
#include "../../include/slaver.h"
#include "../../include/knight_errant.h"
#include "../../include/events/implementations/console_fight_observer.h"


namespace game::core {
    std::shared_ptr<Npc> NpcFactory::createNpc(NpcType type, const std::string &name, const Position &pos) {
        std::shared_ptr<Npc> npc;
        switch (type) {
            case NpcType::KnightErrant:
                npc = std::make_shared<KnightErrant>(name, pos);
                break;
            case NpcType::Slaver:
                npc = std::make_shared<Slaver>(name, pos);
                break;
            case NpcType::Squirrel:
                npc = std::make_shared<Squirrel>(name, pos);
                break;
        }

        if (npc) {
            npc->subscribe(ConsoleObserver::get());
        }

        return npc;
    }
}

#ifndef PLAYER_FACTORY_HPP
#define PLAYER_FACTORY_HPP

#include <string>
#include "alien.hpp"
#include "doggo.hpp"
#include "human.hpp"
#include "player.hpp"
#include "zombie.hpp"

class PlayerFactory {
   public:
    static Player *make_player(int player_team, int player_order,
                               std::string player_type);
};

#endif

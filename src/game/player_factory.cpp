#include "player_factory.hpp"

Player *PlayerFactory::make_player(int player_team, int player_order,
                                   std::string player_type) {
    if (player_type == "human") {
        return new Human(player_team, player_order);
    } else if (player_type == "alien") {
        return new Alien(player_team, player_order);
    } else if (player_type == "zombie") {
        return new Zombie(player_team, player_order);
    } else if (player_type == "doggo") {
        return new Doggo(player_team, player_order);
    };
    return nullptr;
};

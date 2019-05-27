#include "alien.hpp"

const PlayerInfo ALIEN_INFO(4, 2, 3, 2, 1);

Alien::Alien(int player_team, int player_order)
    : Player(player_team, player_order, 'a', &ALIEN_INFO) {}

Appendage *Alien::recieve_tap(Appendage *my_appendage, Player *tapper,
                              Appendage *source_appendage) {
    return my_appendage;
}
Appendage *Alien::give_tap(Appendage *my_appendage, Player *tapped,
                           Appendage *target_appendage) {
    return my_appendage;
}

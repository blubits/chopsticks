#include "zombie.hpp"

const PlayerInfo ZOMBIE_INFO(1, 0, 4, 0, 2);

Zombie::Zombie(int player_team, int player_order)
    : Player(player_team, player_order, 'z', &ZOMBIE_INFO) {}

Appendage *Zombie::recieve_tap(Appendage *my_appendage, Player *tapper,
                               Appendage *source_appendage) {
    if (my_appendage->is_dead() && has_respawn) {
        hands.push_back(new Hand(1, player_info->num_fingers));
    }
    return my_appendage;
}
Appendage *Zombie::give_tap(Appendage *my_appendage, Player *tapped,
                            Appendage *target_appendage) {
    return my_appendage;
}

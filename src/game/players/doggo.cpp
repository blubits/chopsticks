#include "doggo.hpp"
#include "debug.hpp"

const PlayerInfo DOGGO_INFO(0, 4, 0, 4, 1);

Doggo::Doggo(int player_team, int player_order)
    : Player(player_team, player_order, 'd', &DOGGO_INFO) {}

Appendage *Doggo::recieve_tap(Appendage *my_appendage, Player *tapper,
                              Appendage *source_appendage) {
    // skip my turn if my foot died
    if (my_appendage->get_appendage_type() == 'F' && my_appendage->is_dead()) {
        if (DEBUG) {
            std::cout << "PLAYER:receive_tap() Target's foot has died. "
                         "Skipping next turn."
                      << std::endl;
        }
        skip_turn();
    }

    if (tapper->get_player_type() != 'd') {
        if (DEBUG) {
            std::cout << "PLAYER:receive_tap() Tapped a giant donggo. Skipping "
                         "next turn."
                      << std::endl;
        }
        tapper->skip_turn();
    }
    return my_appendage;
}
Appendage *Doggo::give_tap(Appendage *my_appendage, Player *tapped,
                           Appendage *target_appendage) {
    return my_appendage;
}

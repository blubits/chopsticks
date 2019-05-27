#include "human.hpp"
#include "debug.hpp"

const PlayerInfo HUMAN_INFO(2, 2, 5, 5, 1);

Human::Human(int player_team, int player_order)
    : Player(player_team, player_order, 'h', &HUMAN_INFO) {}

Appendage *Human::recieve_tap(Appendage *my_appendage, Player *tapper,
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
    return my_appendage;
}
Appendage *Human::give_tap(Appendage *my_appendage, Player *tapped,
                           Appendage *target_appendage) {
    return my_appendage;
}

#ifndef HUMAN_HPP
#define HUMAN_HPP

/**
 * A human player.
 *
 * No special attributes are assigned to this player.
 *
 * @author  Maded Batara III
 * @date    2019-05-27
 */

#include "player.hpp"

class Human : public Player {
   public:
    Human(int player_team, int player_order);

    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

#endif

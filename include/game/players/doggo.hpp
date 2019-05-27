#ifndef DOGGO_HPP
#define DOGGO_HPP

/**
 * A doggo player.
 *
 * Any non-doggo who taps a doggo skips his turn.
 *
 * @author  Maded Batara III
 * @date    2019-05-27
 */

#include "player.hpp"

class Doggo : public Player {
   public:
    Doggo(int player_team, int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

#endif

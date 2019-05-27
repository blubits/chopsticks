#ifndef ALIEN_HPP
#define ALIEN_HPP

/**
 * An alien player.
 *
 * The alien does not skip a turn if their foot died.
 *
 * @author  Maded Batara III
 * @date    2019-05-27
 */

#include "player.hpp"

class Alien : public Player {
   public:
    Alien(int player_team, int player_order);

    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

#endif

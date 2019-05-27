#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

/**
 * A zombie player.
 *
 * The zombie respawns a hand if their first hand died.
 *
 * @author  Maded Batara III
 * @date    2019-05-27
 */

#include "player.hpp"

class Zombie : public Player {
   private:
    bool has_respawn = true;

   public:
    Zombie(int player_team, int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

#endif

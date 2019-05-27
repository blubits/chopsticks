#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP

/**
 * Info on a player's appendages.
 *
 * @author  Maded Batara III
 * @date    2019-05-16
 */

struct PlayerInfo {
    int num_hands;
    int num_feet;
    int num_fingers;
    int num_toes;
    int actions_per_turn;
    PlayerInfo(int num_hands, int num_feet, int num_fingers, int num_toes,
               int actions_per_turn);
};

#endif

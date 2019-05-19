// @author Maded Batara III (blubits)
// @date 2019-05-16

#ifndef PLAYERINFO_H
#define PLAYERINFO_H

// Info on a player's appendages.
struct PlayerInfo {
    int num_hands;
    int num_feet;
    int num_fingers;
    int num_toes;
    int actions_per_turn;
    PlayerInfo(int num_hands, int num_feet, int num_fingers, int num_toes,
               int actions_per_turn);
};

PlayerInfo::PlayerInfo(int num_hands, int num_feet, int num_fingers,
                       int num_toes, int actions_per_turn)
    : num_hands(num_hands),
      num_feet(num_feet),
      num_fingers(num_fingers),
      num_toes(num_toes),
      actions_per_turn(actions_per_turn){};

const PlayerInfo HUMAN_INFO(2, 2, 5, 5, 1);
const PlayerInfo ALIEN_INFO(4, 2, 3, 2, 1);
const PlayerInfo ZOMBIE_INFO(1, 0, 4, 0, 2);
const PlayerInfo DOGGO_INFO(0, 4, 0, 4, 1);

#endif

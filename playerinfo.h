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
    int num_actions;
    PlayerInfo(int num_hands, int num_feet, int num_fingers, int num_toes, int num_actions);
};

PlayerInfo::PlayerInfo(int num_hands, int num_feet, int num_fingers,
                       int num_toes, int num_actions)
    : num_hands(num_hands),
      num_feet(num_feet),
      num_fingers(num_fingers),
      num_toes(num_toes),
      num_actions(num_actions){};

const PlayerInfo HUMAN_INFO(2, 2, 5, 5, 1);
const PlayerInfo ALIEN_INFO(4, 2, 3, 2, 1);
const PlayerInfo ZOMBIE_INFO(1, 0, 4, 0, 2);
const PlayerInfo DOGGO_INFO(0, 4, 0, 4, 1);

#endif
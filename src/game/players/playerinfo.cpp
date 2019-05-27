#include "playerinfo.hpp"

PlayerInfo::PlayerInfo(int num_hands, int num_feet, int num_fingers,
                       int num_toes, int actions_per_turn)
    : num_hands(num_hands),
      num_feet(num_feet),
      num_fingers(num_fingers),
      num_toes(num_toes),
      actions_per_turn(actions_per_turn){};

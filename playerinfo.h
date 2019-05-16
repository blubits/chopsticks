//
// Info on a player's appendages.
//
// @author Maded Batara III (blubits)
// @date 2019-05-16

struct PlayerInfo {
    int num_hands;
    int num_feet;
    int num_fingers;
    int num_toes;
    PlayerInfo(int num_hands, int num_feet, int num_fingers, int num_toes);
};

PlayerInfo::PlayerInfo(int num_hands, int num_feet, int fingers_max,
                       int toes_max)
    : num_hands(num_hands),
      num_feet(num_feet),
      num_fingers(num_fingers),
      num_toes(num_toes){};

const PlayerInfo HUMAN_INFO(2, 2, 5, 5);
const PlayerInfo ALIEN_INFO(4, 2, 3, 2);
const PlayerInfo ZOMBIE_INFO(1, 0, 4, 0);
const PlayerInfo DOGGO_INFO(0, 4, 0, 4);

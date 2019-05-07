#include <sstream>
#include <string>
#include <vector>
#include "hand.h"

class Player {
   private:
    int player_number;
    int player_team;
    char player_type;
    std::vector<Hand> hands;
    std::vector<Feet> feet;

   public:
    Player(int player_number, int player_team, char player_type) : player_number(player_number), player_team(player_team), player_type(){};
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);
    void attack(Player &other, char my_hand, char other_hand);
    bool is_dead();
    std::string to_string();
};

// void Player::transfer(int fingers_left, int fingers_right) {
//     if (is_dead()) return;
//     left.set_raised(fingers_left);
//     right.set_raised(fingers_right);
// }

// void Player::attack(Player &other, char my_hand, char other_hand) {
//     if (is_dead()) return;
//     if (my_hand == 'L') {
//         if (other_hand == 'L') {
//             left.tap(other.get_left_hand());
//         } else if (other_hand == 'R') {
//             left.tap(other.get_right_hand());
//         }
//     } else if (my_hand == 'R') {
//         if (other_hand == 'L') {
//             right.tap(other.get_left_hand());
//         } else if (other_hand == 'R') {
//             right.tap(other.get_right_hand());
//         }
//     }
// }

// bool Player::is_dead() { return left.is_dead() && right.is_dead(); }

// std::string Player::to_string() {
//     std::stringstream ans;
//     ans << "P" << player_number << " L:" << left.get_raised()
//         << " R:" << right.get_raised();
//     return ans.str();
// }

class Human : public Player {
};

class Alien : public Player {
};

class Zombie : public Player {
};

class Doggo : public Player {
};

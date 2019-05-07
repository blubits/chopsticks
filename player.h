#include <sstream>
#include <string>
#include "hand.h"

class Player {
   private:
    Hand left;
    Hand right;
    int player_number;

   public:
    Player(int player_number) : player_number(player_number), left(), right(){};
    const Hand get_left_hand();
    const Hand get_right_hand();
    void transfer(int fingers_left, int fingers_right);
    void attack(Player &other, char my_hand, char other_hand);
    bool is_dead();
    std::string to_string();
};

const Hand Player::get_left_hand() { return left; }
const Hand Player::get_right_hand() { return right; }

void Player::transfer(int fingers_left, int fingers_right) {
    if (is_dead()) return;
    left.set_fingers_up(fingers_left);
    right.set_fingers_up(fingers_right);
}

void Player::attack(Player &other, char my_hand, char other_hand) {
    if (is_dead()) return;
    if (my_hand == 'L') {
        if (other_hand == 'L') {
            left.tap(other.get_left_hand());
        } else if (other_hand == 'R') {
            left.tap(other.get_right_hand());
        }
    } else if (my_hand == 'R') {
        if (other_hand == 'L') {
            right.tap(other.get_left_hand());
        } else if (other_hand == 'R') {
            right.tap(other.get_right_hand());
        }
    }
}

bool Player::is_dead() { return left.is_dead() && right.is_dead(); }

std::string Player::to_string() {
    std::stringstream ans;
    ans << "P" << player_number << " L:" << left.get_fingers_up()
        << " R:" << right.get_fingers_up();
    return ans.str();
}

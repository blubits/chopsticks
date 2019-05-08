#include <sstream>
#include <string>
#include <vector>
#include "hand.h"

class Player {
   private:
    int player_number;
    int player_team;
    char player_type;
    const int num_hands = 0;
    const int num_feet = 0;
    const int fingers_max = 0;
    const int toes_max = 0;
    bool turn_skipped;
    std::vector<Hand> hands;
    std::vector<Foot> feet;

   public:
    Player(int player_number, int player_team, char player_type) : player_number(player_number), player_team(player_team), player_type(player_type){};
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);
    void attack(Player &other, char my_hand, char other_hand);
    bool is_dead();
    std::string to_string();
};

void Player::distribute_hands(std::vector<int> input) {
    auto i = input.begin();
    auto j = hands.begin();
    while (i != input.end() || j != hands.end()) {
        j->set_raised((*i));
        i++;
        j++;
    }
}

void Player::distribute_feet(std::vector<int> input) {
    auto i = input.begin();
    auto j = feet.begin();
    while (i != input.end() || j != feet.end()) {
        j->set_raised((*i));
        i++;
        j++;
    }
}

void Player::attack(Player &other, char my_hand, char other_hand) {
    if (is_dead()) return;
    if (my_hand == 'L') {
        if (other_hand == 'L') {
            // left.tap(other.get_left_hand());
        } else if (other_hand == 'R') {
            // left.tap(other.get_right_hand());
        }
    } else if (my_hand == 'R') {
        if (other_hand == 'L') {
            // right.tap(other.get_left_hand());
        } else if (other_hand == 'R') {
            // right.tap(other.get_right_hand());
        }
    }
}

bool Player::is_dead() {
    for (auto i = hands.begin(); i != hands.end(); i++) {
        if (!(i->is_dead())) return false;
    }
    for (auto i = feet.begin(); i != feet.end(); i++) {
        if (!(i->is_dead())) return false;
    }
    return true;
}

std::string Player::to_string() {
    std::stringstream ans;
    // ans << "P" << player_number << " L:" << left.get_raised()
    //     << " R:" << right.get_raised();
    return ans.str();
}

class Human : public Player {
   private:
    const int num_hands = 2;
    const int num_feet = 2;
    const int fingers_max = 5;
    const int toes_max = 5;
};

class Alien : public Player {
   private:
    const int num_hands = 4;
    const int num_feet = 2;
    const int fingers_max = 3;
    const int toes_max = 2;
};

class Zombie : public Player {
   private:
    const int num_hands = 1;
    const int fingers_max = 4;
};

class Doggo : public Player {
   private:
    const int num_feet = 4;
    const int toes_max = 5;
};

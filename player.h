#include <sstream>
#include <string>
#include <vector>
#include "hand.h"

class Player {
   private:
    int player_number;
    int player_team;
    char player_type;
    int num_hands = 0;
    int num_feet = 0;
    int fingers_max = 0;
    int toes_max = 0;
    bool turn_skipped;

    std::vector<Hand> hands;
    std::vector<Foot> feet;

   public:
    Player();
    Player(int player_number, int player_team, char player_type);
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);
    bool is_dead();
    std::string to_string();
};

Player::Player(){};

Player::Player(int player_number, int player_team, char player_type) {
    this->player_number = player_number;
    this->player_team = player_team;
    this->player_type = player_type;
    for (int i = 0; i < num_hands; i++) {
        hands.push_back(Hand(fingers_max, 0));
    }
    for (int i = 0; i < num_feet; i++) {
        feet.push_back(Foot(toes_max, 0));
    }
}

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
    int num_hands = 2;
    int num_feet = 2;
    int fingers_max = 5;
    int toes_max = 5;

   public:
    Human(int player_number, int player_team) : Player(player_team, player_number, 'h'){};
};

class Alien : public Player {
   private:
    int num_hands = 4;
    int num_feet = 2;
    int fingers_max = 3;
    int toes_max = 2;

   public:
    Alien(int player_number, int player_team) : Player(player_team, player_number, 'a'){};
};

class Zombie : public Player {
   private:
    int num_hands = 1;
    int fingers_max = 4;

   public:
    Zombie(int player_number, int player_team) : Player(player_team, player_number, 'z'){};
};

class Doggo : public Player {
   private:
    int num_feet = 4;
    int toes_max = 5;

   public:
    Doggo(int player_number, int player_team) : Player(player_team, player_number, 'd'){};
};

class PlayerFactory {
   public:
    static Player make_player(int player_number, int player_team, std::string player_type);
};

Player PlayerFactory::make_player(int player_number, int player_team, std::string player_type) {
    if (player_type == "human") {
        return Human(player_number, player_team);
    } else if (player_type == "alien") {
        return Alien(player_number, player_team);
    } else if (player_type == "zombie") {
        return Zombie(player_number, player_team);
    } else if (player_type == "doggo") {
        return Doggo(player_number, player_team);
    };
    return Player();
};

class Team {
   private:
    int current_player_idx = 0;
    std::vector<Player *> players;

   public:
    Player *operator[](int index);
    Player *get_player(int index);
    Player *current_player();
    void push_player(Player *player);
    bool is_dead();
};

Player *Team::operator[](int index) {
    return players[index];
}

Player *Team::get_player(int index) {
    return players[index];
}

Player *Team::current_player() {
    Player *current_player = players[current_player_idx];
    current_player_idx++;
    return current_player;
}

void Team::push_player(Player *player) {
    players.push_back(player);
}

bool Team::is_dead() {
    for (auto i : players) {
        if (!(i->is_dead())) {
            return false;
        }
    }
    return true;
}
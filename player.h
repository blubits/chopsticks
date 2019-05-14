#include <ostream>
#include <string>
#include <vector>
#include "hand.h"
#include "playerinfo.h"

class Player {
   private:
    int player_number;
    char player_type;
    const PlayerInfo *player_info;

    std::vector<Hand *> hands;
    std::vector<Foot *> feet;

    bool turn_skipped = false;

   public:
    Player(int player_number, char player_type, const PlayerInfo *player_info);
    Hand &get_hand(int i);
    Foot &get_foot(int i);
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);
    bool is_dead();

    // Abstract functions: override in subclasses
    virtual void get_tapped_by(Appendage &my_appendage, Player &player,
                               Appendage &player_appendage) = 0;

    friend std::ostream &operator<<(std::ostream &os, const Player &dt);
    std::string to_string();
};

giant_donggo.get_tapped_by(giant_donggo.get_foot(5), zombie,
                           zombie.get_hand(5));

Player::Player(int player_number, char player_type,
               const PlayerInfo *player_info)
    : player_number(player_number),
      player_type(player_type),
      player_info(player_info) {
    for (int i = 0; i < player_info->num_hands; i++) {
        hands.push_back(new Hand(player_info->num_fingers, 0));
    }
    for (int i = 0; i < player_info->num_feet; i++) {
        feet.push_back(new Foot(player_info->num_toes, 0));
    }
}

void Player::distribute_hands(std::vector<int> input) {
    int i = 0;
    for (auto &hi : input) {
        hands.at(i++)->set_raised(hi);
    }
}

void Player::distribute_feet(std::vector<int> input) {
    int i = 0;
    for (auto &hi : input) {
        feet.at(i++)->set_raised(hi);
    }
}

bool Player::is_dead() {
    for (auto &h : hands) {
        if (!h->is_dead()) return false;
    }
    for (auto &f : feet) {
        if (!f->is_dead()) return false;
    }
    return true;
}

std::ostream &operator<<(std::ostream &os, const Player &dt) {
    os << "P" << dt.player_number << dt.player_type << " (";
    for (auto &h : dt.hands) os << h;
    os << ":";
    for (auto &f : dt.feet) os << f;
    os << ")";
    return os;
}

class Human : public Player {
   public:
    Human(int player_number);
    void get_tapped_by(Player &player, Appendage &appendage);
};

Human::Human(int player_number) : Player(player_number, 'h', &HUMAN_INFO){};

Human::get_tapped_by(Player &player, Appendage &appendage) {}

class Alien : public Player {
   public:
    Alien(int player_number) : Player(player_number, 'a', &ALIEN_INFO){};
};

class Zombie : public Player {
   public:
    Zombie(int player_number) : Player(player_number, 'z', &ZOMBIE_INFO){};
};

class Doggo : public Player {
   public:
    Doggo(int player_number) : Player(player_number, 'd', &DOGGO_INFO){};
};

class PlayerFactory {
   public:
    static Player *make_player(int player_number, std::string player_type);
};

Player *PlayerFactory::make_player(int player_number, std::string player_type) {
    if (player_type == "human") {
        return new Human(player_number);
    } else if (player_type == "alien") {
        return new Alien(player_number);
    } else if (player_type == "zombie") {
        return new Zombie(player_number);
    } else if (player_type == "doggo") {
        return new Doggo(player_number);
    };
    return nullptr;
};

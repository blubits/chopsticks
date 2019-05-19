//
// Classes for implementing game players and player types.
//
// @author Maded Batara III (blubits)
// @date 2019-05-16

#ifndef PLAYER_H
#define PLAYER_H

#include <ostream>
#include <string>
#include <vector>
#include "hand.h"
#include "playerinfo.h"

// Represents a game player.
class Player {
   protected:
    int player_team;
    int player_order;
    char player_type;
    const PlayerInfo *player_info;

    std::vector<Hand *> hands;
    std::vector<Foot *> feet;

    bool turn;
    int actions_remaining;

   public:
    Player(int player_order, char player_type,
           const PlayerInfo *player_info);
    Hand *get_hand(int i);
    Foot *get_foot(int i);
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);
    bool is_dead();
    bool has_turn();
    void set_turn(bool t);
    bool has_actions();
    void use_action();

    void update_tapper(Player *tapper);
    void update_tapped(Player *tapped);

    // Abstract functions: override in subclasses

    friend std::ostream &operator<<(std::ostream &os, const Player &dt);
    std::string to_string();
};

Player::Player(int player_order, char player_type,
               const PlayerInfo *player_info)
    : player_order(player_order),
      player_type(player_type),
      player_info(player_info),
      turn(true),
      actions_remaining(player_info->num_actions) {
    for (int i = 0; i < player_info->num_hands; i++) {
        hands.push_back(new Hand(1, player_info->num_fingers));
    }
    for (int i = 0; i < player_info->num_feet; i++) {
        feet.push_back(new Foot(1, player_info->num_toes));
    }
}

void Player::update_tapper(Player *tapper) {
    ;
}

void Player::update_tapped(Player *tapped) {
    ;
}

Hand *Player::get_hand(int i) {
    return hands[i];
}

Foot *Player::get_foot(int i) {
    return feet[i];
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
    os << "P" << dt.player_order << dt.player_type << " (";
    for (auto &h : dt.hands) os << (*h);
    os << ":";
    for (auto &f : dt.feet) os << (*f);
    os << ")";
    return os;
}

bool Player::has_turn() {
    return turn;
}

void Player::set_turn(bool t) {
    turn = t;
}

bool Player::has_actions() {
    return actions_remaining != 0;
}

void Player::use_action() {
    if (!has_turn()) return;
    actions_remaining--;
    if (actions_remaining == 0) {
        actions_remaining = player_info->num_actions;
    }
}

class Human : public Player {
   public:
    Human(int player_order);
};

Human::Human(int player_order) : Player(player_order, 'h', &HUMAN_INFO){};

class Alien : public Player {
   public:
    Alien(int player_order) : Player(player_order, 'a', &ALIEN_INFO){};
};

class Zombie : public Player {
   private:
    bool has_respawn = true;

   public:
    Zombie(int player_order) : Player(player_order, 'z', &ZOMBIE_INFO){};
    void update_tapped(Player *player);
};

void Zombie::update_tapped(Player *player) {
    if (this->get_hand(0)->is_dead() && has_respawn) {
        has_respawn = false;
        hands.push_back(new Hand(1, 4));
    }
}

class Doggo : public Player {
   public:
    Doggo(int player_order) : Player(player_order, 'd', &DOGGO_INFO){};
    void update_tapper(Player *tapper);
};

void Doggo::update_tapper(Player *tapper) {
    tapper->set_turn(false);
};

class PlayerFactory {
   public:
    static Player *make_player(int player_order, std::string player_type);
};

Player *PlayerFactory::make_player(int player_order, std::string player_type) {
    if (player_type == "human") {
        return new Human(player_order);
    } else if (player_type == "alien") {
        return new Alien(player_order);
    } else if (player_type == "zombie") {
        return new Zombie(player_order);
    } else if (player_type == "doggo") {
        return new Doggo(player_order);
    };
    return nullptr;
};

#endif
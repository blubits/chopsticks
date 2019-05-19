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
    char player_type;
    const PlayerInfo *player_info;

    std::vector<Hand *> hands;
    std::vector<Foot *> feet;

    int actions_remaining;

   public:
    Player(char player_type, const PlayerInfo *player_info);

    char get_player_type() const;
    Hand *get_hand(int i);
    Foot *get_foot(int i);
    Appendage *get_appendage(string i);

    bool is_dead();
    bool has_turn();
    void use_action();
    void give_turn();
    void skip_turn();

    // Distributes digits to hands and/or feet.
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);

    // Taps a player's appendage.
    void tap(string my_appendage, Player *player, string target_appendage);

    // Recieves a tap from a player's appendage. Returns a reference to this
    // player's appendage that was tapped. Call after the tap has
    // already proceeded.
    virtual void Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                                        Appendage *source_appendage) = 0;

    // Gives a tap to a player's appendage. Returns a reference to this
    // player's appendage that tapped. Call after the tap has
    // already proceeded.
    virtual void Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                                     Appendage *target_appendage) = 0;

    // Directs player state to an output stream.
    friend std::ostream &operator<<(std::ostream &os, const Player &dt);
};

Player::Player(char player_type, const PlayerInfo *player_info)
    : player_type(player_type),
      player_info(player_info),
      turn(true),
      actions_remaining(0) {
    for (int i = 0; i < player_info->num_hands; i++) {
        hands.push_back(new Hand(1, player_info->num_fingers));
    }
    for (int i = 0; i < player_info->num_feet; i++) {
        feet.push_back(new Foot(1, player_info->num_toes));
    }
}

char Player::get_player_type() const { return player_type; }

Hand *Player::get_hand(int i) { return hands.get(i); }

Foot *Player::get_foot(int i) { return feet.get(i); }

Appendage *Player::get_appendage(string i) {
    if (i.size() != 2) return nullptr;
    int idx = i[1] - 'A';
    if (i[0] == 'H') return get_hand(idx);
    if (i[0] == 'F') return get_foot(idx);
    return nullptr;
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

bool Player::has_turn() { return actions_remaining != 0; }

void Player::use_action() {
    if (!has_turn()) return;
    actions_remaining--;
}

void Player::give_turn() { actions_remaining = player_info->actions_per_turn; }

void Player::skip_turn() { actions_remaining = 0; }

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

void Player::tap(string my_appendage, Player *player, string target_appendage) {
    Appendage *mine = get_appendage(my_appendage);
    Appendage *theirs = player->get_appendage(target_appendage);

    // if either of above returns nullptr, the appendage DNE
    if (mine == nullptr || theirs == nullptr) return;

    // if either of the above are already dead, tapping cannot proceed
    if (mine->is_dead() || theirs->is_dead()) return;

    // proceed with tapping
    theirs->add_digits(mine->get_digits_raised());

    // call tap recievers to prepare for the tap
    Appendage *new_mine = give_tap(mine, player, theirs);
    Appendage *new_theirs = player->recieve_tap(theirs, this, mine);
}

std::ostream &operator<<(std::ostream &os, const Player &dt) {
    os << "P" << dt.player_order << dt.player_type << " (";
    for (auto &h : dt.hands) os << (*h);
    os << ":";
    for (auto &f : dt.feet) os << (*f);
    os << ")";
    return os;
}

// A human player.
//
// No special attributes are assigned to this player.
class Human : public Player {
   public:
    Human(int player_order);
    void Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                                Appendage *source_appendage) override;
    void Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                             Appendage *target_appendage) override;
};

Human::Human() : Player('h', &HUMAN_INFO) {}

void Appendage *Human::recieve_tap(Appendage *my_appendage, Player *tapper,
                                   Appendage *source_appendage) {
    // skip my turn if my foot died
    if (my_appendage->get_appendage_type() == 'F' && my_appendage->is_dead()) {
        skip_turn();
    }
    return my_appendage;
}
void Appendage *Human::give_tap(Appendage *my_appendage, Player *tapped,
                                Appendage *target_appendage) {
    return my_appendage;
}

// An alien player.
//
// The alien does not skip a turn if their foot died.
class Alien : public Player {
   public:
    Alien(int player_order);
    void recieve_tap(Player *tapper, Appendage *source_appendage) override;
    void give_tap(Player *tapped, Appendage *target_appendage) override;
};

Alien::Alien() : Player('a', &ALIEN_INFO) {}

void Appendage *Alien::recieve_tap(Appendage *my_appendage, Player *tapper,
                                   Appendage *source_appendage) {
    return my_appendage;
}
void Appendage *Alien::give_tap(Appendage *my_appendage, Player *tapped,
                                Appendage *target_appendage) {
    return my_appendage;
}

// A zombie player.
//
// The zombie respawns a hand if their first hand died.
class Zombie : public Player {
   private:
    bool has_respawn = true;

   public:
    Zombie();
    void recieve_tap(Player *tapper, Appendage *source_appendage) override;
    void give_tap(Player *tapped, Appendage *target_appendage) override;
};

Zombie::Zombie() : Player('z', &ZOMBIE_INFO) {}

void Appendage *Zombie::recieve_tap(Appendage *my_appendage, Player *tapper,
                                    Appendage *source_appendage) {
    if (my_appendage->is_dead() && has_respawn) {
        hands.push_back(new Hand(1, player_info->num_fingers));
    }
    return my_appendage;
}
void Appendage *Zombie::give_tap(Appendage *my_appendage, Player *tapped,
                                 Appendage *target_appendage) {
    return my_appendage;
}

// A doggo player.
//
// Any non-doggo who taps a doggo skips his turn.
class Doggo : public Player {
   public:
    Doggo();
    void recieve_tap(Player *tapper, Appendage *source_appendage) override;
    void give_tap(Player *tapped, Appendage *target_appendage) override;
};

Doggo::Doggo() : Player('d', &DOGGO_INFO) {}

void Appendage *Doggo::recieve_tap(Appendage *my_appendage, Player *tapper,
                                   Appendage *source_appendage) {
    if (player->get_player_type() != 'd') {
        player->skip_turn();
    }
    return my_appendage;
}
void Appendage *Doggo::give_tap(Appendage *my_appendage, Player *tapped,
                                Appendage *target_appendage) {
    return my_appendage;
}

// A factory for Player objects.
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

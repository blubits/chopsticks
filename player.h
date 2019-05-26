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
#include "debug.h"
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

    bool is_skipped;
    int actions_remaining;

   public:
    Player(int player_team, int player_order, char player_type,
           const PlayerInfo *player_info);

    int get_player_team() const;
    char get_player_type() const;
    int get_player_order() const;
    vector<Hand *> *get_hands();
    vector<Feet *> *get_feet();
    Hand *get_hand(int i);
    Foot *get_foot(int i);
    Appendage *get_appendage(std::string i);

    bool is_dead();
    bool has_turn();
    bool turn_skipped();
    bool has_action();
    void reset_actions();
    void use_action();
    bool in_play();
    void give_turn();
    void skip_turn();
    void unskip_turn();
    char get_type();

    // Distributes digits to hands and/or feet.
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);

    // Taps a player's appendage.
    void tap(std::string my_appendage, Player *player,
             std::string target_appendage);

    // Recieves a tap from a player's appendage. Returns a reference to this
    // player's appendage that was tapped. Call after the tap has
    // already proceeded.
    virtual Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                                   Appendage *source_appendage) = 0;

    // Gives a tap to a player's appendage. Returns a reference to this
    // player's appendage that tapped. Call after the tap has
    // already proceeded.
    virtual Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                                Appendage *target_appendage) = 0;

    // Directs player state to an output stream.
    friend std::ostream &operator<<(std::ostream &os, Player &dt);
};

Player::Player(int player_team, int player_order, char player_type,
               const PlayerInfo *player_info)
    : player_team(player_team),
      player_order(player_order),
      player_type(player_type),
      player_info(player_info),
      actions_remaining(0),
      is_skipped(false) {
    for (int i = 0; i < player_info->num_hands; i++) {
        hands.push_back(new Hand(1, player_info->num_fingers));
    }
    for (int i = 0; i < player_info->num_feet; i++) {
        feet.push_back(new Foot(1, player_info->num_toes));
    }
}

int Player::get_player_order() const { return player_order; }

int Player::get_player_team() const { return player_team; }

char Player::get_player_type() const { return player_type; }

vector<Hand *> *get_hands(int i) { return &hands; }
vector<Feet *> *get_feet(int i) { return &feet; }

Hand *Player::get_hand(int i) { return hands.at(i); }

Foot *Player::get_foot(int i) { return feet.at(i); }

Appendage *Player::get_appendage(std::string i) {
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

bool Player::has_turn() { return !is_skipped; }

bool Player::turn_skipped() { return is_skipped; }

void Player::use_action() { actions_remaining--; }

void Player::reset_actions() {
    actions_remaining = player_info->actions_per_turn;
}

bool Player::has_action() { return actions_remaining > 0; }

void Player::give_turn() {
    if (turn_skipped()) return;
    actions_remaining = player_info->actions_per_turn;
}

bool Player::in_play() {
    return actions_remaining != player_info->actions_per_turn;
}

void Player::skip_turn() { is_skipped = true; }

void Player::unskip_turn() {
    if (turn_skipped()) {
        is_skipped = false;
        actions_remaining = 0;
    }
}

void Player::distribute_hands(std::vector<int> input) {
    int i = 0;
    for (auto &hi : input) {
        hands.at(i++)->set_digits_raised(hi);
    }
}

void Player::distribute_feet(std::vector<int> input) {
    int i = 0;
    for (auto &hi : input) {
        feet.at(i++)->set_digits_raised(hi);
    }
}

void Player::tap(std::string my_appendage, Player *player,
                 std::string target_appendage) {
    Appendage *mine = get_appendage(my_appendage);
    Appendage *theirs = player->get_appendage(target_appendage);

    if (DEBUG) {
        std::cout << "PLAYER:tap() " << *this << " is tapping " << *player
                  << std::endl;
    }

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

char Player::get_type() { return player_type; }

std::ostream &operator<<(std::ostream &os, Player &dt) {
    os << "P" << dt.player_order << dt.player_type;
    if (DEBUG) {
        os << " [";
        os << dt.actions_remaining;
        os << "]";
        os << "[";
        os << !dt.is_skipped;
        os << "]";
    }
    os << " (";
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
    Human(int player_team, int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Human::Human(int player_team, int player_order)
    : Player(player_team, player_order, 'h', &HUMAN_INFO) {}

Appendage *Human::recieve_tap(Appendage *my_appendage, Player *tapper,
                              Appendage *source_appendage) {
    // skip my turn if my foot died
    if (my_appendage->get_appendage_type() == 'F' && my_appendage->is_dead()) {
        if (DEBUG) {
            std::cout << "PLAYER:receive_tap() Target's foot has died. "
                         "Skipping next turn."
                      << std::endl;
        }
        skip_turn();
    }
    return my_appendage;
}
Appendage *Human::give_tap(Appendage *my_appendage, Player *tapped,
                           Appendage *target_appendage) {
    return my_appendage;
}

// An alien player.
//
// The alien does not skip a turn if their foot died.
class Alien : public Player {
   public:
    Alien(int player_team, int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Alien::Alien(int player_team, int player_order)
    : Player(player_team, player_order, 'a', &ALIEN_INFO) {}

Appendage *Alien::recieve_tap(Appendage *my_appendage, Player *tapper,
                              Appendage *source_appendage) {
    return my_appendage;
}
Appendage *Alien::give_tap(Appendage *my_appendage, Player *tapped,
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
    Zombie(int player_team, int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Zombie::Zombie(int player_team, int player_order)
    : Player(player_team, player_order, 'z', &ZOMBIE_INFO) {}

Appendage *Zombie::recieve_tap(Appendage *my_appendage, Player *tapper,
                               Appendage *source_appendage) {
    if (my_appendage->is_dead() && has_respawn) {
        hands.push_back(new Hand(1, player_info->num_fingers));
    }
    return my_appendage;
}
Appendage *Zombie::give_tap(Appendage *my_appendage, Player *tapped,
                            Appendage *target_appendage) {
    return my_appendage;
}

// A doggo player.
//
// Any non-doggo who taps a doggo skips his turn.
class Doggo : public Player {
   public:
    Doggo(int player_team, int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Doggo::Doggo(int player_team, int player_order)
    : Player(player_team, player_order, 'd', &DOGGO_INFO) {}

Appendage *Doggo::recieve_tap(Appendage *my_appendage, Player *tapper,
                              Appendage *source_appendage) {
    // skip my turn if my foot died
    if (my_appendage->get_appendage_type() == 'F' && my_appendage->is_dead()) {
        if (DEBUG) {
            std::cout << "PLAYER:receive_tap() Target's foot has died. "
                         "Skipping next turn."
                      << std::endl;
        }
        skip_turn();
    }

    if (tapper->get_player_type() != 'd') {
        if (DEBUG) {
            std::cout << "PLAYER:receive_tap() Tapped a giant donggo. Skipping "
                         "next turn."
                      << std::endl;
        }
        tapper->skip_turn();
    }
    return my_appendage;
}
Appendage *Doggo::give_tap(Appendage *my_appendage, Player *tapped,
                           Appendage *target_appendage) {
    return my_appendage;
}

// A factory for Player objects.
class PlayerFactory {
   public:
    static Player *make_player(int player_team, int player_order,
                               std::string player_type);
};

Player *PlayerFactory::make_player(int player_team, int player_order,
                                   std::string player_type) {
    if (player_type == "human") {
        return new Human(player_team, player_order);
    } else if (player_type == "alien") {
        return new Alien(player_team, player_order);
    } else if (player_type == "zombie") {
        return new Zombie(player_team, player_order);
    } else if (player_type == "doggo") {
        return new Doggo(player_team, player_order);
    };
    return nullptr;
};

#endif

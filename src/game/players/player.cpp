#include "player.hpp"
#include "debug.hpp"

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

const PlayerInfo *Player::get_player_info() const { return player_info; }

std::vector<Hand *> *Player::get_hands() { return &hands; }
std::vector<Foot *> *Player::get_feet() { return &feet; }

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
        while (hands.at(i++)->is_dead())
            ;
        hands.at(i - 1)->set_digits_raised(hi);
    }
}

void Player::distribute_feet(std::vector<int> input) {
    int i = 0;
    for (auto &hi : input) {
        while (feet.at(i++)->is_dead())
            ;
        feet.at(i - 1)->set_digits_raised(hi);
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
    mine->tap(theirs);

    // call tap recievers to prepare for the tap
    Appendage *new_mine = give_tap(mine, player, theirs);
    Appendage *new_theirs = player->recieve_tap(theirs, this, mine);
}

char Player::get_type() { return player_type; }

std::ostream &operator<<(std::ostream &os, Player &dt) {
    os << "P" << dt.player_order << dt.player_type;
    if (dt.is_dead()) {
        os << " [dead]";
        return os;
    }
    if (dt.is_skipped) os << " [skipped]";
    os << " (";
    for (auto &h : dt.hands) os << (*h);
    os << ":";
    for (auto &f : dt.feet) os << (*f);
    os << ")";
    return os;
}

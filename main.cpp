// CS 12 Lab 12b - Chopsticks v2

#include <bits/stdc++.h>

/***
 * hand.h
 */

//
// Classes for implementing tappable appendages and appendage appendage_types.
//
// @author Maded Batara III (blubits)
// @date 2019-05-16



// Represents the appendages of a certain player.
//
// The Appendage class is a semi-abstract class; appendage subclasses
// (e.g. hands and feet) are expected to implement add_digits() and is_dead(),
// as the logic for these functions will vary across appendage appendage_types.

class Appendage {
   protected:
    char appendage_type;
    int digits_raised;
    int digits_max;

   public:
    Appendage();
    Appendage(int digits_raised, int digits_max);
    Appendage(int digits_raised, int digits_max, char appendage_type);
    char get_appendage_type();
    int get_digits_raised() const;
    void set_digits_raised(int digits);
    int get_digits_max() const;
    void tap(Appendage* tap_target);

    // Add additional digits_raised digits to the appendage. Can only
    // be done as long as the appendage is still alive.
    virtual void add_digits(int digits) = 0;

    // Checks if the appendage is dead.
    virtual bool is_dead() const = 0;

    // Directs appendage state to an output stream.
    friend std::ostream& operator<<(std::ostream& os, const Appendage& dt);
};

Appendage::Appendage() : digits_raised(1), digits_max(5), appendage_type('A') {}

Appendage::Appendage(int digits_raised, int digits_max)
    : digits_raised(digits_raised),
      digits_max(digits_max),
      appendage_type('A') {}

Appendage::Appendage(int digits_raised, int digits_max, char appendage_type)
    : digits_raised(digits_raised),
      digits_max(digits_max),
      appendage_type(appendage_type) {}

void Appendage::tap(Appendage* tap_target) {
    if (tap_target->is_dead()) return;
    tap_target->add_digits(digits_raised);
}

char Appendage::get_appendage_type() { return appendage_type; }

int Appendage::get_digits_max() const { return digits_max; }

int Appendage::get_digits_raised() const { return digits_raised; }

void Appendage::set_digits_raised(int digits) {
    if (is_dead()) return;
    digits_raised = digits;
}

std::ostream& operator<<(std::ostream& os, const Appendage& dt) {
    if (dt.is_dead()) {
        os << "X";
    } else {
        os << dt.digits_raised;
    }
    return os;
}

// A player's hands.
//
// Note that a player's hand obeys standard rules of Chopsticks; fingers
// wrap around, and the hand remains alive until all fingers are digits_raised.

class Hand : public Appendage {
   public:
    Hand(int digits_raised, int digits_max);
    void add_digits(int digits) override;
    bool is_dead() const override;
};

Hand::Hand(int digits_raised, int digits_max)
    : Appendage(digits_raised, digits_max, 'H') {}

void Hand::add_digits(int digits) {
    if (is_dead()) return;
    digits_raised = (digits_raised - 1 + digits) % digits_max + 1;
}

bool Hand::is_dead() const { return digits_raised == digits_max; }

// A player's feet.
//
// Toes in this implementation of the game do not wrap around and are simply
// added until all possible toes have been digits_raised. Note that a player
// skips a turn if one of his feet die.

class Foot : public Appendage {
   public:
    Foot(int digits_raised, int digits_max);
    void add_digits(int digits) override;
    bool is_dead() const override;
};

Foot::Foot(int digits_raised, int digits_max)
    : Appendage(digits_raised, digits_max, 'F') {}

void Foot::add_digits(int digits) {
    if (is_dead()) return;
    digits_raised += digits;
}

bool Foot::is_dead() const { return digits_raised >= digits_max; }



/***
 * playerinfo.h
 */

// @author Maded Batara III (blubits)
// @date 2019-05-16


// Info on a player's appendages.
struct PlayerInfo {
    int num_hands;
    int num_feet;
    int num_fingers;
    int num_toes;
    int actions_per_turn;
    PlayerInfo(int num_hands, int num_feet, int num_fingers, int num_toes,
               int actions_per_turn);
};

PlayerInfo::PlayerInfo(int num_hands, int num_feet, int num_fingers,
                       int num_toes, int actions_per_turn)
    : num_hands(num_hands),
      num_feet(num_feet),
      num_fingers(num_fingers),
      num_toes(num_toes),
      actions_per_turn(actions_per_turn){};

const PlayerInfo HUMAN_INFO(2, 2, 5, 5, 1);
const PlayerInfo ALIEN_INFO(4, 2, 3, 2, 1);
const PlayerInfo ZOMBIE_INFO(1, 0, 4, 0, 2);
const PlayerInfo DOGGO_INFO(0, 4, 0, 4, 1);



/***
 * player.h
 */

//
// Classes for implementing game players and player types.
//
// @author Maded Batara III (blubits)
// @date 2019-05-16



// Represents a game player.
class Player {
   protected:
    int player_order;
    char player_type;
    const PlayerInfo *player_info;

    std::vector<Hand *> hands;
    std::vector<Foot *> feet;

    int actions_remaining;

   public:
    Player(int player_order, char player_type, const PlayerInfo *player_info);

    char get_player_type() const;
    Hand *get_hand(int i);
    Foot *get_foot(int i);
    Appendage *get_appendage(std::string i);

    bool is_dead();
    bool has_turn();
    bool turn_skipped();
    void use_action();
    void give_turn();
    void skip_turn();
    void unskip_turn();

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

Player::Player(int player_order, char player_type,
               const PlayerInfo *player_info)
    : player_order(player_order),
      player_type(player_type),
      player_info(player_info),
      actions_remaining(0) {
    for (int i = 0; i < player_info->num_hands; i++) {
        hands.push_back(new Hand(1, player_info->num_fingers));
    }
    for (int i = 0; i < player_info->num_feet; i++) {
        feet.push_back(new Foot(1, player_info->num_toes));
    }
}

char Player::get_player_type() const { return player_type; }

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

bool Player::has_turn() { return actions_remaining != 0; }

bool Player::turn_skipped() { return actions_remaining == -1; }

void Player::use_action() {
    if (!has_turn()) return;
    actions_remaining--;
}

void Player::give_turn() { actions_remaining = player_info->actions_per_turn; }

void Player::skip_turn() { actions_remaining = -1; }

void Player::unskip_turn() { actions_remaining = 0; }

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

    // std::cout << *player << " (" << *theirs << ":"
    //           << theirs->get_appendage_type() << ") is getting tapped by "
    //           << *this << " (" << *mine << ":" << mine->get_appendage_type()
    //           << ")" << std::endl;

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

std::ostream &operator<<(std::ostream &os, Player &dt) {
    os << "P" << dt.player_order << dt.player_type;
    // os << " [";
    // if (dt.has_turn()) {
    //     os << "!";
    // } else if (dt.turn_skipped()) {
    //     os << "X";
    // } else {
    //     os << ".";
    // }
    // os << "]";
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
    Human(int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Human::Human(int player_order) : Player(player_order, 'h', &HUMAN_INFO) {}

Appendage *Human::recieve_tap(Appendage *my_appendage, Player *tapper,
                              Appendage *source_appendage) {
    // skip my turn if my foot died
    if (my_appendage->get_appendage_type() == 'F' && my_appendage->is_dead()) {
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
    Alien(int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Alien::Alien(int player_order) : Player(player_order, 'a', &ALIEN_INFO) {}

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
    Zombie(int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Zombie::Zombie(int player_order) : Player(player_order, 'z', &ZOMBIE_INFO) {}

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
    Doggo(int player_order);
    Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                           Appendage *source_appendage) override;
    Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                        Appendage *target_appendage) override;
};

Doggo::Doggo(int player_order) : Player(player_order, 'd', &DOGGO_INFO) {}

Appendage *Doggo::recieve_tap(Appendage *my_appendage, Player *tapper,
                              Appendage *source_appendage) {
    if (tapper->get_player_type() != 'd') {
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



/***
 * team.h
 */



class Team {
   private:
    int current_player_idx;
    std::vector<Player *> players;

   public:
    Team();
    int size();
    Player *operator[](int index);
    Player *get_player(int index);
    void add_player(Player *player);
    bool is_dead();
    bool is_skipped();

    Player *get_current_player();
    bool go_to_next_player();

    // Directs team state to an output stream.
    friend std::ostream &operator<<(std::ostream &os, const Team &dt);
};

Team::Team() : current_player_idx(0) {}

int Team::size() { return players.size(); }

Player *Team::operator[](int index) { return players[index]; }

Player *Team::get_player(int index) { return players.at(index); }

void Team::add_player(Player *player) { players.push_back(player); }

bool Team::is_dead() {
    for (auto i : players) {
        if (!(i->is_dead())) {
            return false;
        }
    }
    return true;
}

bool Team::is_skipped() {
    for (auto i : players) {
        if (!i->turn_skipped()) return false;
    }
    return true;
}

Player *Team::get_current_player() {
    if (is_dead()) return nullptr;
    return players.at(current_player_idx);
}

bool Team::go_to_next_player() {
    if (is_dead()) return false;
    for (int di = 1; di < size(); di++) {
        current_player_idx = (current_player_idx + 1) % size();
        Player *possible_player = get_player(current_player_idx);
        if (possible_player->is_dead()) continue;
        if (possible_player->turn_skipped()) {
            possible_player->unskip_turn();
            continue;
        }
        possible_player->give_turn();
        return true;
    }
    current_player_idx = (current_player_idx + 1) % size();
    if (get_current_player()->turn_skipped()) {
        get_current_player()->unskip_turn();
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const Team &dt) {
    for (auto &i : dt.players) {
        os << (*i);
        if (i != dt.players.back()) {
            os << " | ";
        }
    }
    return os;
}



/***
 * game.h
 */



class Game {
   private:
    std::vector<Team *> teams;
    std::vector<Player *> players;

    bool ongoing;
    int turn_number;
    int current_team_idx;
    int team_idx_won;

   public:
    Game();
    int num_teams();
    Team *operator[](int index);
    Team *get_team(int index);
    bool is_ongoing();
    int who_won();

    void push_player(int player_team, std::string player_type);
    void push_team();

    void start_game();
    Player *get_current_player();
    void go_to_next_player();

    void move(std::string command);

    void check_win_condition();

    // Directs game state to an output stream.
    friend std::ostream &operator<<(std::ostream &os, Game &dt);
};

Game::Game()
    : ongoing(true), turn_number(0), current_team_idx(0), team_idx_won(-1) {}

int Game::num_teams() { return teams.size(); }

Team *Game::operator[](int index) { return teams[index]; }

Team *Game::get_team(int index) { return teams.at(index); }

bool Game::is_ongoing() { return ongoing; }

int Game::who_won() { return team_idx_won; }

void Game::push_player(int player_team, std::string player_type) {
    Player *new_player =
        PlayerFactory::make_player(players.size() + 1, player_type);
    players.push_back(new_player);
    teams[player_team]->add_player(new_player);
};

void Game::push_team() { teams.push_back(new Team()); };

void Game::start_game() { get_current_player()->give_turn(); }

Player *Game::get_current_player() {
    return get_team(current_team_idx)->get_current_player();
}

void Game::go_to_next_player() {
    get_team(current_team_idx)->go_to_next_player();
    for (int dt = 1; dt <= num_teams(); dt++) {
        current_team_idx = (current_team_idx + 1) % num_teams();
        Team *possible_team = get_team(current_team_idx);
        if (possible_team->is_skipped()) {
            possible_team->go_to_next_player();
            continue;
        }
        if (possible_team->get_current_player() != nullptr) {
            possible_team->get_current_player()->give_turn();
            break;
        }
    }
}

void Game::move(std::string command) {
    Player *current_player = get_current_player();
    if (current_player == nullptr || !is_ongoing()) return;

    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;

    if (cmd == "tap") {
        std::string source_appendage_idx, target_appendage_idx;
        int target_player_idx;

        ss >> source_appendage_idx >> target_player_idx >> target_appendage_idx;

        Player *target_player = players[target_player_idx - 1];
        current_player->tap(source_appendage_idx, target_player,
                            target_appendage_idx);
    } else if (cmd == "disthands") {
        int x;
        std::vector<int> ix;
        while (ss >> x) {
            ix.push_back(x);
        }
        current_player->distribute_hands(ix);
    } else if (cmd == "distfeet") {
        int x;
        std::vector<int> ix;
        while (ss >> x) {
            ix.push_back(x);
        }
        current_player->distribute_feet(ix);
    }
    current_player->use_action();
    check_win_condition();
    if (!is_ongoing()) return;
    if (!current_player->has_turn()) {
        go_to_next_player();
    }
}

void Game::check_win_condition() {
    int no_teams_alive = 0, team_alive_idx = 0;
    for (int i = 0; i < num_teams(); i++) {
        if (!get_team(i)->is_dead()) {
            no_teams_alive++;
            team_alive_idx = i;
        }
    }
    if (no_teams_alive == 1) {
        ongoing = false;
        team_idx_won = team_alive_idx;
    }
}

std::ostream &operator<<(std::ostream &os, Game &dt) {
    for (int i = 0; i < dt.num_teams(); i++) {
        // if (dt.current_team_idx == i) os << "> ";
        os << "Team " << i + 1 << ": " << *(dt.get_team(i)) << std::endl;
    }
    return os;
}



/***
 * chopsticks.cpp
 */

// CS12 S - Batara, Salinas


using namespace std;

int main() {
    int p;
    int t;
    cin >> p >> t;

    Game g;
    for (int i = 0; i < t; i++) {
        g.push_team();
    }

    for (int i = 1; i < p + 1; i++) {
        string player_type;
        int player_team;
        cin >> player_type >> player_team;
        g.push_player(player_team - 1, player_type);
    }

    cin.ignore();
    g.start_game();
    cout << g << endl;

    string command;
    while (getline(cin, command)) {
        g.move(command);
        cout << g << endl;
        if (!g.is_ongoing()) {
            cout << "Team " << g.who_won() + 1 << " wins!" << endl;
            break;
        }
    }
}



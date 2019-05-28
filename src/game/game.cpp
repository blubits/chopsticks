#include "game.hpp"

Game::Game()
    : ongoing(true), turn_number(0), current_team_idx(0), team_idx_won(-1) {}

int Game::num_teams() { return teams.size(); }

Team *Game::operator[](int index) { return teams[index]; }

Team *Game::get_team(int index) { return teams.at(index); }

Player *Game::get_player(int index) { return players.at(index); }

bool Game::is_ongoing() { return ongoing; }

int Game::who_won() { return team_idx_won; }

void Game::push_player(int player_team, std::string player_type) {
    Player *new_player = PlayerFactory::make_player(
        player_team, players.size() + 1, player_type);
    players.push_back(new_player);
    teams[player_team]->add_player(new_player);
};

void Game::push_team() { teams.push_back(new Team()); };

void Game::start_game() {
    for (auto t : teams) {
        t->get_player(0)->give_turn();
    }
}

Player *Game::get_current_player() {
    Team *current_team = get_current_team();
    Player *current_player = current_team->get_current_player();
    if (current_player->has_action() && current_player->in_play()) {
        if (DEBUG) {
            std::cout << "GAME:get_current_player() Current player still has "
                         "action. Returning current player."
                      << std::endl;
        }
        return current_player;
    }

    if (current_player->is_dead() || current_player->turn_skipped()) {
        if (!current_player->in_play()) {
            current_player->unskip_turn();
        }
        // DEBUG CODE
        if (DEBUG && current_player->turn_skipped()) {
            std::cout << "GAME:get_current_player() Current player is skipped. "
                         "Finding new player."
                      << std::endl;
        }
        if (DEBUG && current_player->is_dead()) {
            std::cout << "GAME:get_current_player() Current player is dead. "
                         "Finding new player."
                      << std::endl;
        }
        // END DEBUG CODE
        current_player->unskip_turn();
        current_team->go_to_next_player();
        current_player = current_team->get_current_player();
    }
    // DEBUG CODE
    if (DEBUG) {
        std::cout << "GAME:get_current_player() Getting player "
                  << *current_player << std::endl;
    }
    // END DEBUG CODE
    return current_player;
}

Team *Game::get_current_team() {
    Team *current_team = get_team(current_team_idx);
    Player *current_player = current_team->get_current_player();
    if (current_player->has_action()) {
        return current_team;
    }
    if (!(current_team->can_play()) || current_team->is_skipped() ||
        current_team->is_dead()) {
        if (DEBUG && current_team->is_skipped()) {
            std::cout << "GAME:get_current_team() Current team is skipped. "
                         "Finding new team."
                      << std::endl;
        } else if (DEBUG && current_team->is_dead()) {
            std::cout << "GAME:get_current_team() Current team is dead. "
                         "Finding new team."
                      << std::endl;
        } else if (!(current_team->can_play())) {
            std::cout << "GAME:get_current_team() Current team cannot play. "
                         "Finding new team."
                      << std::endl;
        }
        go_to_next_team();
        current_team = get_team(current_team_idx);
    }
    // DEBUG CODE
    if (DEBUG) {
        std::cout << "GAME:get_current_team() Getting team "
                  << current_team_idx + 1 << std::endl;
    }
    // END DEBUG CODE
    return current_team;
}

void Game::go_to_next_team() {
    current_team_idx = (current_team_idx + 1) % num_teams();
    Team *current_team = get_team(current_team_idx);
    while (!(current_team->can_play()) || current_team->is_skipped() ||
           current_team->is_dead()) {
        // DEBUG CODE
        if (DEBUG && current_team->is_skipped()) {
            std::cout << "GAME:go_to_next_team() Team " << current_team_idx + 1
                      << " is skipped. Looking for new team." << std::endl;
        } else if (DEBUG && current_team->is_dead()) {
            std::cout << "GAME:go_to_next_team() Team " << current_team_idx + 1
                      << " is dead. Looking for new team." << std::endl;
        } else if (DEBUG && !(current_team->can_play())) {
            std::cout << "GAME:go_to_next_team() Team " << current_team_idx + 1
                      << " cannot play. Looking for new team." << std::endl;
        }
        // END DEBUG CODE
        if (current_team->is_skipped() || !(current_team->can_play())) {
            current_team->clear_skips();
        }
        current_team_idx = (current_team_idx + 1) % num_teams();
        current_team = get_team(current_team_idx);
    }
    // DEBUG CODE
    if (DEBUG) {
        std::cout << "GAME:go_to_next_team() Team " << current_team_idx + 1
                  << " is next to play." << std::endl;
    }
    // END DEBUG CODE
}

void Game::go_to_next_player() {
    Team *current_team = get_team(current_team_idx);
    Player *current_player = current_team->get_current_player();
    current_player->use_action();
    if (current_player->has_action()) {
        // DEBUG CODE
        if (DEBUG) {
            std::cout << "GAME:go_to_next_player() Current player still has "
                         "action. Consuming action and continuing."
                      << std::endl;
        }
        // END DEBUG CODE
        return;
    }
    go_to_next_team();
    current_team->go_to_next_player();
    // if (!current_team->can_play()) {
    //     current_team->clear_skips();
    // }
}

void Game::move(std::string command) {
    // DEBUG CODE
    // if (DEBUG) {
    //     std::cout << "GAME:move() Attempting to get current player" <<
    //     std::endl;
    // }
    // END DEBUG CODE

    Player *current_player = get_current_player();
    if (current_player == nullptr || !is_ongoing()) {
        // DEBUG CODE
        if (DEBUG && current_player == nullptr) {
            std::cout << "GAME:move() Current player is null, returning"
                      << std::endl;
        }
        if (DEBUG && !is_ongoing()) {
            std::cout << "GAME:move() Game is not ongoing, returning"
                      << std::endl;
        }
        // END DEBUG CODE

        return;
    }

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
    check_win_condition();
    if (!is_ongoing()) return;
    go_to_next_player();
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
    Player *current_player = dt.get_current_player();
    if (current_player->get_type() == 'z' && current_player->in_play()) {
        return os;
    }
    for (int i = 0; i < dt.num_teams(); i++) {
        if (dt.current_team_idx == i) {
            os << "> ";
        }
        os << "Team " << i + 1 << ": " << *(dt.get_team(i)) << std::endl;
    }
    return os;
}

bool Game::is_valid_command(Player *player, std::string command) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    if (cmd == "tap") {
        std::string source_appendage_idx, target_appendage_idx;
        int target_player_idx;
        ss >> source_appendage_idx >> target_player_idx >> target_appendage_idx;
        Player *target_player = players[target_player_idx - 1];

        // friendly fire
        if (player->get_player_team() == target_player->get_player_team())
            return false;
        // target player is dead
        if (target_player->is_dead()) return false;
        // appendage check
        try {
            Appendage *source_appendage =
                player->get_appendage(source_appendage_idx);
            Appendage *target_appendage =
                target_player->get_appendage(target_appendage_idx);
            // source or target appendage is dead
            if (source_appendage->is_dead() || target_appendage->is_dead())
                return false;
        } catch (const std::out_of_range &e) {
            // source or target appendage does not exist
            return false;
        }
        return true;
    } else if (cmd == "disthands") {
        int x, i = 0, sum_x = 0, sum_hands = 0;
        while (ss >> x) {
            // the disthands command distributes fingers even to
            // already-dead hands. thus, only add to the sum
            // of fingers on a player if the hand being checked
            // is still alive, since those are the relevant fingers.
            if (!(player->get_hand(i++)->is_dead())) sum_x += x;
        }
        for (auto &h : *(player->get_hands())) {
            if (!h->is_dead()) sum_hands += h->get_digits_raised();
        }
        // number of fingers up does not match hands distributed
        if (sum_x == sum_hands)
            return true;
        else
            return false;
    } else if (cmd == "distfeet") {
        int x, i = 0, sum_x = 0, sum_feet = 0;
        while (ss >> x) {
            // see comment on disthands
            if (!(player->get_foot(i++)->is_dead())) sum_x += x;
        }
        // number of toes up does not match hands distributed
        for (auto &f : *(player->get_feet())) {
            if (!f->is_dead()) sum_feet += f->get_digits_raised();
        }
        if (sum_x == sum_feet)
            return true;
        else
            return false;
    }
    return false;
}

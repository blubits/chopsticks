#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "team.h"

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

#endif

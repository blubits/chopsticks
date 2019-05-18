#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "team.h"

class Game {
   private:
    std::vector<Player *> players;
    std::vector<Team *> teams;
    bool ongoing;
    int turn;
    int team;
    int won;

   public:
    Game() : ongoing(true), turn(0), team(0), won(-1){};
    void push_player(int player_number, int player_team, std::string player_type);
    void push_team();
    Player *get_current_player();
    void move(std::string command);
    bool is_ongoing();
    int who_won();
    std::string to_string();
};

void Game::push_player(int player_number, int player_team, std::string player_type) {
    Player *new_player = PlayerFactory::make_player(player_number, player_type);
    players.push_back(new_player);
    teams[player_team - 1]->push_player(players.back());
};

void Game::push_team() {
    teams.push_back(new Team());
};

Player *Game::get_current_player() {
    Team *current_team = teams[team];
    Player *current_player;
    int i = 0;
    for (; i < current_team->size(); i++) {
        current_player = current_team->get_current_player();
        if (current_player->has_turn()) {
            break;
        } else {
            current_player->set_turn(true);
        }
    }
    team++;
    if (team == teams.size()) {
        team = 0;
    }
    if (i == current_team->size()) {
        return get_current_player();
    } else {
        return current_player;
    }
}

void Game::move(std::string command) {
    Player *current_player = get_current_player();

    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;

    if (cmd == "tap") {
        int source_appendage_idx, target_player_idx, target_appendage_idx;
        char source_appendage_type, source_appendage_char, target_appendage_type, target_appendage_char;

        ss >> source_appendage_type >> source_appendage_char;
        ss >> target_player_idx;
        target_player_idx--;
        ss >> target_appendage_type >> target_appendage_char;

        source_appendage_idx = int(source_appendage_char) - 65;
        target_appendage_idx = int(target_appendage_char) - 65;

        Player *target_player = players[target_player_idx];

        Appendage *target_appendage;
        if (target_appendage_type == 'H') {
            target_appendage = target_player->get_hand(target_appendage_idx);
        } else if (target_appendage_type == 'F') {
            target_appendage = target_player->get_foot(target_appendage_idx);
        }

        Appendage *source_appendage;
        if (source_appendage_type == 'H') {
            source_appendage = current_player->get_hand(source_appendage_idx);
        } else if (target_appendage_type == 'F') {
            source_appendage = current_player->get_foot(source_appendage_idx);
        }

        source_appendage->tap(target_appendage);
        current_player->update_tapped(target_player);
        target_player->update_tapper(current_player);

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
}

std::string Game::to_string() {
    std::stringstream ans;
    for (int i = 0; i < teams.size(); i++) {
        ans << "Team " << i + 1 << ": " << teams[i]->to_string() << std::endl;
    }
    return ans.str();
}

bool Game::is_ongoing() { return ongoing; }

int Game::who_won() { return won; }

#endif

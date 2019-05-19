#ifndef TEAM_H
#define TEAM_H

#include <iostream>
#include <sstream>
#include "player.h"

class Team {
   private:
    int current_player_idx = 0;
    std::vector<Player *> players;

   public:
    Player *operator[](int index);
    Player *get_player(int index);
    Player *get_current_player();
    void push_player(Player *player);
    bool is_dead();
    int size();

    std::string to_string();
};

Player *Team::operator[](int index) { return players[index]; }

Player *Team::get_player(int index) { return players[index]; }

Player *Team::get_current_player() {
    Player *current_player = players[current_player_idx];
    if (!(current_player->has_actions())) {
        current_player_idx++;
    }
    if (current_player_idx == players.size()) {
        current_player_idx = 0;
    }
    for (int i = 0; i < players.size(); i++) {
        if (current_player->has_turn()) {
            break;
        }
        current_player = players[current_player_idx];
        if (!(current_player->has_actions())) {
            current_player_idx++;
        }
        if (current_player_idx == players.size()) {
            current_player_idx = 0;
        }
    }

    return current_player;
}

int Team::size() {
    return players.size();
}

void Team::push_player(Player *player) { players.push_back(player); }

bool Team::is_dead() {
    for (auto i : players) {
        if (!(i->is_dead())) {
            return false;
        }
    }
    return true;
}

std::string Team::to_string() {
    std::stringstream ans;
    for (auto i : players) {
        ans << (*i);
        if (i != players.back()) {
            ans << " | ";
        }
    }
    return ans.str();
}

#endif
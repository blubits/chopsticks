#ifndef TEAM_H
#define TEAM_H

#include <iostream>
#include <sstream>
#include "player.h"

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

#endif

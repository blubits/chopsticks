#ifndef TEAM_H
#define TEAM_H

#include <iostream>
#include <sstream>
#include "debug.h"
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
    void clear_skips();

    Player *get_current_player();
    void go_to_next_player();

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
        if (!(i->turn_skipped())) return false;
    }
    return true;
}

Player *Team::get_current_player() {
    if (is_dead()) return nullptr;
    return players.at(current_player_idx);
}

void Team::clear_skips() {
    bool return_game_play = false;
    if (is_skipped()) return_game_play = true;
    for (int i = 0; i < size(); i++) {
        get_player(i)->unskip_turn();
        if (return_game_play && i == current_player_idx)
            get_player(i)->give_turn();
        // if (return_game_play && i == 0) get_player(i)->give_turn();
    }
}

void Team::go_to_next_player() {
    if (is_dead() || get_current_player()->has_turn()) return;
    if (DEBUG) std::cout << "Finding next player... " << std::endl;
    for (int _ = 1; _ <= size(); _++) {
        current_player_idx = (current_player_idx + 1) % size();
        Player *possible_player = get_player(current_player_idx);
        if (possible_player->is_dead()) {
            if (DEBUG)
                std::cout << *possible_player << " is dead ("
                          << current_player_idx << ")" << std::endl;
            continue;
        }
        if (possible_player->turn_skipped()) {
            if (DEBUG)
                std::cout << *possible_player << "'s turn is skipped ("
                          << current_player_idx << ")" << std::endl;
            possible_player->unskip_turn();
            continue;
        }
        possible_player->give_turn();
        if (DEBUG)
            std::cout << "It's " << *possible_player << "'s turn ("
                      << current_player_idx << ")" << std::endl;
        break;
    }
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

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
    bool can_play();
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
        if (!(i->turn_skipped())) {
            return false;
        }
    }
    return true;
}

bool Team::can_play() {
    for (auto i : players) {
        if (!(i->is_dead()) && !(i->turn_skipped())) {
            return true;
        }
    }
    return false;
}

Player *Team::get_current_player() {
    if (is_dead()) {
        // DEBUG CODE
        if (DEBUG) {
            std::cout << "TEAM:get_current_player() Current player for team is dead, returning nullptr" << std::endl;
        }
        // END DEBUG CODE
        return nullptr;
    }
    Player *current_player = players.at(current_player_idx);
    // DEBUG CODE
    if (DEBUG) {
        std::cout << "TEAM:get_current_player() Returning player " << *current_player << " of current team" << std::endl;
    }
    // END DEBUG CODE
    return current_player;
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
    if (is_dead()) {
        // DEBUG CODE
        if (DEBUG && is_dead()) {
            std::cout << "TEAM:go_to_next_player() Team is dead, cannot change players." << std::endl;
        } else if (DEBUG) {
            std::cout << "TEAM:go_to_next_player() Current player still has actions, cannot change players" << std::endl;
        }
        // END DEBUG CODE
        return;
    }

    // if (!can_play()) {
    //     // DEBUG CODE
    //     if (DEBUG) {
    //         std::cout << "TEAM:go_to_next_player() Team is skipped. Clearing skips." << std::endl;
    //     }
    //     // END DEBUG CODE
    //     clear_skips();
    //     return;
    // }

    // if (get_current_player()->turn_skipped()) {
    //     get_current_player()->unskip_turn();
    // }

    int _ = 1;
    for (; _ <= size(); _++) {
        current_player_idx = (current_player_idx + 1) % size();
        Player *possible_player = get_player(current_player_idx);
        if (possible_player->is_dead()) {
            // DEBUG CODE
            if (DEBUG) {
                std::cout << "TEAM:go_to_next_player() Player " << *possible_player << " is dead. Iterating." << std::endl;
            }
            // END DEBUG CODE
            continue;
        }
        if (possible_player->turn_skipped() && !possible_player->in_play()) {
            // DEBUG CODE
            if (DEBUG) {
                std::cout << "TEAM:go_to_next_player() Player " << *possible_player << " has turn skipped. Resetting and iterating." << std::endl;
            }
            // END DEBUG CODE
            possible_player->unskip_turn();
            continue;
        }
        possible_player->give_turn();
        // DEBUG CODE
        if (DEBUG) {
            std::cout << "TEAM:go_to_next_player() Player " << *possible_player << " is next to play for current team." << std::endl;
        }
        // END DEBUG CODE
        break;
    }
    // if (_ == size()) {
    //     if (DEBUG) {
    //         std::cout << "TEAM:go_to_next_player() Team has wipped. Resetting team." << std::endl;
    //     }
    //     current_player_idx = 0;
    // }
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

#ifndef TEAM_HPP
#define TEAM_HPP

#include <iostream>
#include <sstream>
#include "debug.hpp"
#include "player.hpp"

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

#endif

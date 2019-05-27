#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "debug.hpp"
#include "player_factory.hpp"
#include "team.hpp"

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
    Player *get_player(int index);
    bool is_ongoing();
    int who_won();

    void push_team();
    void push_player(int player_team, std::string player_type);

    void start_game();

    Team *get_current_team();
    void go_to_next_team();
    Player *get_current_player();
    void go_to_next_player();

    bool is_valid_command(Player *player, std::string command);

    void move(std::string command);

    void check_win_condition();

    // Directs game state to an output stream.
    friend std::ostream &operator<<(std::ostream &os, Game &dt);
};

#endif

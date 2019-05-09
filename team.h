#include <sstream>
#include "player.h"

class Team {
   private:
    int current_player_idx = 0;
    std::vector<Player *> players;

   public:
    Player *operator[](int index);
    Player *get_player(int index);
    Player *current_player();
    void push_player(Player *player);
    bool is_dead();
    std::string to_string();
};

Player *Team::operator[](int index) { return players[index]; }

Player *Team::get_player(int index) { return players[index]; }

Player *Team::current_player() {
    Player *current_player = players[current_player_idx];
    current_player_idx++;
    return current_player;
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
        ans << i->to_string();
        if (i != players.back()) {
            ans << " | ";
        }
    }
    return ans.str();
}

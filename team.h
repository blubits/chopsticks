#include <vector>
#include "player.h"

class Team {
   private:
    std::vector<Player *> players;

   public:
    Player *operator[](int index);
    Player *get_player(int index);
    void push_back(Player *player);
    bool is_dead();
};

Player *Team::operator[](int index) {
    return players[index];
}

Player *Team::get_player(int index) {
    return players[index];
}

void Team::push_back(Player *player) {
    players.push_back(player);
}

bool Team::is_dead() {
    for (auto i : players) {
        if (!(i->is_dead())) {
            return false;
        }
    }
    return true;
}
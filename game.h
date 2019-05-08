#include <sstream>
#include <string>
#include "player.h"

class Game {
   private:
    std::vector<Player> players;
    std::vector<Team> teams;
    bool ongoing;
    int turn;
    int won;

   public:
    Game() : ongoing(true), turn(0), won(-1){};
    void push_player(int player_number, int player_team, int player_type);
    void push_team();
    void move(std::string command);
    bool is_ongoing();
    int who_won();
    std::string to_string();
};

void Game::push_player(int player_number, int player_team, int player_type){

};

void Game::push_team() {
    teams.push_back(Team());
}

void Game::move(std::string command) {
    ;
}

bool Game::is_ongoing() { return ongoing; }
int Game::who_won() { return won; }

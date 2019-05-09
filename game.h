#include <sstream>
#include <string>
#include "team.h"

class Game {
   private:
    std::vector<Player *> players;
    std::vector<Team *> teams;
    bool ongoing;
    int turn;
    int won;

   public:
    Game() : ongoing(true), turn(0), won(-1){};
    void push_player(int player_number, int player_team, std::string player_type);
    void push_team();
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
}

void Game::move(std::string command) {
    ;
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

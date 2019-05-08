#include <sstream>
#include <string>
#include "player.h"

class Game {
   private:
    Player p1;
    Player p2;
    bool ongoing;
    int turn;
    int won;

   public:
    Game() : p1(1, 1, 'H'), p2(2, 2, 'H'), ongoing(true), turn(0), won(-1){};
    void move(std::string command);
    bool is_ongoing();
    int who_won();
    std::string to_string() { return p1.to_string() + " | " + p2.to_string(); }
};

void Game::move(std::string command) {
    if (!ongoing) return;
    std::stringstream ss(command);
    std::string action;
    ss >> action;
    if (action == "attack") {
        char a, b;
        ss >> a >> b;
        if (turn % 2 == 0) {
            p2.attack(p1, b, a);
        } else {
            p1.attack(p2, b, a);
        }
    } else if (action == "change") {
        int a, b;
        ss >> a >> b;
        if (turn % 2 == 0) {
            // p1.transfer(a, b);
        } else {
            // p2.transfer(a, b);
        }
    }
    if (p1.is_dead()) {
        won = 2;
        ongoing = false;
    } else if (p2.is_dead()) {
        won = 1;
        ongoing = false;
    }
    turn++;
}

bool Game::is_ongoing() { return ongoing; }
int Game::who_won() { return won; }

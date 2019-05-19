// CS12 S - Batara, Salinas

#include <iostream>
#include <string>
#include <vector>
#include "game.h"

using namespace std;

int main() {
    int p;
    int t;
    cin >> p >> t;

    Game g;
    for (int i = 0; i < t; i++) {
        g.push_team();
    }

    for (int i = 1; i < p + 1; i++) {
        string player_type;
        int player_team;
        cin >> player_type >> player_team;
        g.push_player(player_team - 1, player_type);
    }

    cin.ignore();
    g.start_game();
    cout << g << endl;

    string command;
    while (getline(cin, command)) {
        g.move(command);
        cout << g << endl;
        if (!g.is_ongoing()) {
            cout << "Team " << g.who_won() + 1 << " wins!" << endl;
            break;
        }
    }
}

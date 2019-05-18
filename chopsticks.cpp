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
        g.push_player(i, player_team, player_type);
    }

    string command;
    cin.ignore();
    cout << g.to_string() << endl;

    while (getline(cin, command)) {
        g.move(command);
        cout << command << endl;
        cout << g.to_string() << endl;
        if (!g.is_ongoing()) {
            cout << "Player " << g.who_won() << " wins" << endl;
            break;
        }
    }
}

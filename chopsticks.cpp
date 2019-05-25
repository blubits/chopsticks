// CS12 S - Batara, Salinas

#include <iostream>
#include "aux.h"
#include "server.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Attempt to create a server
        if (is_valid_port(argv[0])) {
            Server *server = new Server();
            server.start();
        } else {
            std::cout << "Please chosoe a valid port between 1024-65535" << std::endl
        }

    } else if (argc == 2) {
        // Attempt to create a client
    }

    // int p;
    // int t;
    // cin >> p >> t;

    // Game g;
    // for (int i = 0; i < t; i++) {
    //     g.push_team();
    // }

    // for (int i = 1; i < p + 1; i++) {
    //     string player_type;
    //     int player_team;
    //     cin >> player_type >> player_team;
    //     g.push_player(player_team - 1, player_type);
    // }

    // cin.ignore();
    // g.start_game();
    // cout << g << endl;

    // int i = 1;
    // string command;
    // while (getline(cin, command)) {
    //     if (DEBUG) {
    //         cout << "MAIN:main() Turn #" << i << " | " << command << endl;
    //         i++;
    //     }

    //     g.move(command);
    //     cout << g << endl;
    //     if (!g.is_ongoing()) {
    //         cout << "Team " << g.who_won() + 1 << " wins!" << endl;
    //         break;
    //     }
    // }
}

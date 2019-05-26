#include <sstream>
#include <string>
#include <vector>
#include "game.h"
#include "protocol.h"
#include "socketstream/socketstream.hh"

#ifndef SERVER_H
#define SERVER_H

class Server {
   private:
    swoope::socketstream *server;
    std::vector<swoope::socketstream *> clients;
    Game *game;
    int num_players;

    void init_players();
    void start_game();

   public:
    Server();
    void start(char *port);
};

Server::Server() : num_players(1) {}

void Server::init_players() {
    int code;
    std::string line;

    // Request the number of players
    // Input must be between 2 and 6 inclusive
    while (true) {
        std::cout << "Please input number of players:" << std::endl;
        getline(std::cin, line);
        if (is_number(line)) {
            num_players = atol(line.c_str());
            if (num_players > 1 && num_players < 7) {
                break;
            }
        }
    }

    // Let num_players - 1 clients connect to the server
    for (int i = 1; i < num_players; i++) {
        std::cout << "Waiting for players..." << std::endl;
        clients.push_back(new swoope::socketstream());
        swoope::socketstream *new_client = clients.back();
        server->accept(*new_client);
        std::cout << "Accepted connection from " << clients.back()->remote_address() << std::endl;

        // Broadcast to clients that a new player has connected
        // Does not broadcast to the last player
        if (i != num_players - 1) {
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "A new player has connected. Waiting for more players..." << std::endl;
            }
        }
    }

    std::string player_types[num_players];

    // Notify clients that player type selection has begun
    std::cout << "Requesting player type from players. Please wait." << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "Requesting player type from players. Please wait." << std::endl;
    }

    // Request each player type from each player
    while (true) {
        std::cout << "Please enter a player type (human, zombie, alien, doggo): " << std::endl;
        getline(std::cin, line);
        if (is_valid_player_type(line)) {
            player_types[0] = line;
            break;
        }
    }
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "Player " << 1 << " has elected to be a " << player_types[0] << "." << std::endl;
    }
    for (int i = 1; i < num_players; i++) {
        swoope::socketstream *current_client = clients[i - 1];

        // Request the new player's type
        while (true) {
            *current_client << static_cast<int>(CODES::REQUEST_NEW_INPUT) << std::endl;
            *current_client << "Please enter a player type (human, zombie, alien, doggo): " << std::endl;
            *current_client >> code;
            current_client->ignore();
            if (code == static_cast<int>(CODES::NEW_INPUT)) {
                while (!getline(*current_client, line)) {
                };
                if (is_valid_player_type(line)) {
                    player_types[i] = line;
                    break;
                }
            }
        }
        // Broadcast the current client's information to other clients
        std::cout << "Player " << i + 1 << " has elected to be a " << player_types[i] << "." << std::endl;
        for (auto client : clients) {
            *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
            *client << "Player " << i + 1 << " has elected to be a " << player_types[i] << "." << std::endl;
        }
    }

    int player_teams[num_players];

    // Notify clients that team selection has begun
    std::cout << "Requesting player teams from players. Please wait." << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "Requesting player teams from players. Please wait." << std::endl;
    }

    while (true) {
        while (true) {
            int team_number = -1;
            std::cout << "Please input a team number between 1 and " << num_players << "." << std::endl;
            getline(std::cin, line);
            if (is_number(line)) {
                team_number = atol(line.c_str());
                if (team_number > 0 && team_number <= num_players) {
                    player_teams[0] = team_number;
                    break;
                }
            }
        }
        for (int i = 1; i < num_players; i++) {
            while (true) {
                swoope::socketstream *current_client = clients[i - 1];
                int team_number = -1;
                *current_client << static_cast<int>(CODES::REQUEST_NEW_INPUT) << std::endl;
                *current_client << "Please input a team number between 1 and " << num_players << "." << std::endl;
                *current_client >> code;
                current_client->ignore();
                if (code == static_cast<int>(CODES::NEW_INPUT)) {
                    while (!getline(*current_client, line)) {
                    }
                    if (is_number(line)) {
                        team_number = atol(line.c_str());
                        if (team_number > 0 && team_number <= num_players) {
                            player_teams[i] = team_number;
                            break;
                        }
                    }
                }
            }
        }

        // Check validity of team configuration
        // Check if there are at least two teams with players
        // and that no valid team comes after a team with zero players
        bool is_valid = true;
        int team_count[num_players] = {};
        for (int i = 0; i < num_players; i++) {
            team_count[player_teams[i] - 1]++;
        }
        int num_valid_teams = 0;
        if (team_count[0] > 0) {
            num_valid_teams++;
        }
        for (int i = 1; i < num_players; i++) {
            if (team_count[i] > 0) {
                num_valid_teams++;
            }
            if (team_count[i] != 0 && team_count[i - 1] == 0) {
                is_valid = false;
                break;
            }
        }
        if (num_valid_teams > 1 && is_valid) {
            for (int j = 0; j < num_valid_teams; j++) {
                game->push_team();
            }
            for (int j = 0; j < num_players; j++) {
                game->push_player(player_teams[j] - 1, player_types[j]);
            }
            break;
        } else {
            std::cout << "An invalid team configuration was entered. Restarting team selection." << std::endl;
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "An invalid team configuration was entered. Restarting team selection." << std::endl;
            }
        }
    }

    std::cout << "All players have connected!!! Starting game." << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "All players have connected!!! Starting game." << std::endl;
    }
}

void Server::start_game() {
    int code;
    std::string line;

    game->start_game();

    // Print the current state of the game
    std::stringstream ss;
    ss << game << std::endl;
    while (!getline(ss, line)) {
        std::cout << line << std::endl;
    }
    ss.clear();
    for (auto client : clients) {
        ss << game << std::endl;
        while (!getline(ss, line)) {
            *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
            *client << line << std::endl;
        }
        ss.clear();
    }
    line = "";

    while (true) {
        Player *current_player = game->get_current_player();
        int current_player_idx = current_player->get_player_order() - 1;
        if (current_player_idx == 0) {
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "Waiting for player " << current_player_idx + 1 << " to move." << std::endl;
            }
            while (!game->is_valid_command(current_player, line)) {
                std::cout << "Please enter your move." << std::endl;
                getline(std::cin, line);
            }
            game->move(line);
        } else {
            std::cout << "Waiting for player " << current_player_idx + 1 << " to move." << std::endl;
            for (int i = 0; i < clients.size(); i++) {
                if (i != current_player_idx - 1) {
                    *clients[i] << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                    *clients[i] << "Waiting for player " << current_player_idx + 1 << " to move." << std::endl;
                }
            }
            swoope::socketstream *current_client = clients[current_player_idx - 1];

            while (true) {
                *current_client << static_cast<int>(CODES::REQUEST_NEW_INPUT) << std::endl;
                *current_client << "Please enter your move." << std::endl;
                *current_client >> code;
                current_client->ignore();
                if (code == static_cast<int>(CODES::NEW_INPUT)) {
                    while (!getline(*current_client, line)) {
                    }
                    if (game->is_valid_command(current_player, line))
                        break;
                }
            }
            game->move(line);
        }

        // Print the current player's move
        std::cout << "Player " << current_player_idx + 1 << ": " << line << std::endl;

        // Print the current state of the game
        std::stringstream ss;
        ss << game << std::endl;
        while (!getline(ss, line)) {
            std::cout << line << std::endl;
        }
        ss.clear();
        for (auto client : clients) {
            ss << game << std::endl;
            while (!getline(ss, line)) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << line << std::endl;
            }
            ss.clear();
        }
        line = "";

        if (!game->is_ongoing()) {
            std::cout << "Team " << game->who_won() + 1 << " wins!" << std::endl;
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "Team " << game->who_won() + 1 << " wins!" << std::endl;
            }
            break;
        }
    }
}

void Server::start(char *port) {
    std::cout << "Attempting to start server" << std::endl;

    server = new swoope::socketstream();
    server->open(port, 4);
    game = new Game();

    std::cout << "Started server succesfully" << std::endl;

    init_players();
    start_game();
}

#endif
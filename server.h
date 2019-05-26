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

   public:
    Server();
    void start(char *port);
};

Server::Server() : num_players(1) {}

void Server::init_players() {
    int code;
    std::string line;

    while (true) {
        std::cout << "Please input number of players:" << std::endl;
        getline(std::cin, line);
        if (is_number(line)) {
            num_players = atol(line.c_str());
            if (num_players > 1 || num_players < 7) {
                break;
            }
        }
    }

    std::string player_types[num_players];
    int player_teams[num_players];

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

    // Request each player's team type
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

    // // Request each player's team number
    // int team_number = -1;
    // while (team_number < 2 || team_number > 6) {
    //     std::cout << "Please input number of players:" << std::endl;
    //     getline(std::cin, line);
    //     if (is_number(line) && atol(line.c_str())) {
    //         num_players = atol(line.c_str());
    //     }
    // }

    std::cout << "All players have connected!!!" << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "All players have connected!!!" << std::endl;
    }
}

void Server::start(char *port) {
    std::cout << "Attempting to start server" << std::endl;

    server = new swoope::socketstream();
    server->open(port, 4);
    game = new Game();

    std::cout << "Started server succesfully" << std::endl;

    init_players();
}

#endif
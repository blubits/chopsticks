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
    Game* game;
    int num_players;

   public:
    Server();
    void start(char *port);
};

Server::Server() : num_players(1) {}

void Server::start(char *port) {
    std::cout << "Attempting to start server" << std::endl;

    server = new swoope::socketstream();
    server->open(port, 4);
    game = Game();
    int code;
    std::string line;

    std::cout << "Started server succesfully" << std::endl;

    while (num_players < 2 || num_players > 6) {
        std::cout << "Please input number of players:" << std::endl;
        std::cin >> num_players;
    }

    for (int i = 1; i < num_players; i++) {
        std::cout << "Waiting for players..." << std::endl;
        clients.push_back(new swoope::socketstream());
        swoope::socketstream *new_client = clients.back();
        server->accept(*new_client);
        std::cout << "Accepted connection from " << clients.back()->remote_address() << std::endl;

        // Broadcast to clients that a new player has connected
        if (i != num_players - 1) {
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "Waiting for players..." << std::endl;
            }
        }

        // Request the new player's type
        while (true) {
            *new_client >> code;
            new_client->ignore();
            if (code == static_cast<int>(CODES::NEW_INPUT)) {
                while (!getline(*new_client, line)) {
                };
            }
        }
    }

    std::cout << "All players have connected!!!" << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "All players have connected!!!" << std::endl;
    }
}

#endif
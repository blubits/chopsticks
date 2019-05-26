// CS12 S - Batara, Salinas

#include <iostream>
#include "client.h"
#include "helpers.h"
#include "server.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        // Attempt to create a server
        if (is_valid_port(argv[1])) {
            Server *server = new Server();
            server->start(argv[1]);
        } else {
            std::cout << "Please chose a valid port between 1024-65535 to start a server" << std::endl;
            return -1;
        }

    } else if (argc == 3) {
        // Attempt to create a client
        if (is_valid_port(argv[2])) {
            Client *client = new Client();
            client->start(argv[1], argv[2]);
        } else {
            std::cout << "Please chose a valid port between 1024-65535 to start a client" << std::endl;
            return -1;
        }
    } else {
        std::cout << "Please provide either a port to start a server or a valid ip and a port to start a client." << std::endl;
        return -1;
    }

    return 0;
}

// CS12 S - Batara, Salinas

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include "client.hpp"
#include "server.hpp"

bool is_valid_port(char *port) {
    int i = 0;
    while (port[i] != '\0') {
        if (!isdigit(port[i])) {
            return false;
        }
        i++;
    }

    long long int port_long;
    port_long = atol(port);

    if (port_long >= 1024 && port_long <= 65535) {
        return true;
    }

    return false;
}

int main(int argc, char *argv[]) {
    // start messages
    const std::string HELP_MESSAGE =
        "Usage: chopsticks [--help] [-c|--client ADDRESS PORT] [-s|--server "
        "PORT]\n"
        "The children's game Chopsticks. Played over the network.";
    const std::string INVALID_ARGUMENT_MESSAGE =
        "chopsticks: invalid argument\n"
        "Try 'chopsticks --help' for more information.";
    const std::string INVALID_PORT_MESSAGE =
        "chopsticks: invalid port\n"
        "Please choose a valid port between 1024-65535.";

    if (argc == 3 &&
        (strcmp(argv[1], "--server") == 0 || strcmp(argv[1], "-s") == 0)) {
        // Attempt to create a server
        if (is_valid_port(argv[2])) {
            Server *server = new Server();
            server->start(argv[2]);
        } else {
            std::cout << INVALID_PORT_MESSAGE << std::endl;
            return -1;
        }
    } else if (argc == 4 && (strcmp(argv[1], "--client") == 0 ||
                             strcmp(argv[1], "-c") == 0)) {
        // Attempt to create a client
        if (is_valid_port(argv[3])) {
            Client *client = new Client();
            client->start(argv[2], argv[3]);
        } else {
            std::cout << INVALID_PORT_MESSAGE << std::endl;
            return -1;
        }
    } else if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        std::cout << HELP_MESSAGE << std::endl;
    } else {
        std::cout << INVALID_ARGUMENT_MESSAGE << std::endl;
        return -1;
    }

    return 0;
}

// CS12 S - Batara, Salinas

#include <iostream>
#include "aux.h"
#include "server.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        // Attempt to create a server
        if (is_valid_port(argv[1])) {
            Server *server = new Server();
            server->start(argv[1]);
        } else {
            std::cout << "Please chosoe a valid port between 1024-65535" << std::endl;
        }

    } else if (argc == 3) {
        // Attempt to create a client
    } else {
        std::cout << "Please provide either a port to start a server or a valid ip and a port to start a client." << std::endl;
        return -1;
    }

    return 0;
}

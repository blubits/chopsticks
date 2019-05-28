#include <client.hpp>

int Client::get_player_number() const { return player_number; }

void Client::play() {
    while (true) {
        int code = -1;
        std::string line;

        *socket >> code;
        socket->ignore();
        if (code != -1) {
            while (!(getline(*socket, line))) {
            }
        }

        if (code == static_cast<int>(CODES::NEW_BROADCAST)) {
            std::cout << line << std::endl;
        }
        if (code == static_cast<int>(CODES::REQUEST_NEW_INPUT)) {
            std::cout << line << std::endl << "> ";
            getline(std::cin, line);
            *socket << static_cast<int>(CODES::NEW_INPUT) << std::endl;
            *socket << line << std::endl;
        }
        if (code == static_cast<int>(CODES::GAME_END)) {
            std::cout << line << std::endl;
            socket->close();
            break;
        }
        if (!socket->is_open()) {
            break;
        }
    }

    std::cout << "Terminating connection with server." << std::endl;
}

void Client::start(char* ip, char* port) {
    const std::string WELCOME_MESSAGE =
        "+++++++++++++++++++++++++++++++++++++\n"
        "+                                   +\n"
        "+             CHOPSTICKS            +\n"
        "+      Batara - Salinas - CS12      +\n"
        "+                                   +\n"
        "+++++++++++++++++++++++++++++++++++++";
    socket = new swoope::socketstream();
    socket->open(ip, port);

    if (!socket->is_open()) {
        std::cout << "chopsticks: Failed to connect to server" << std::endl;
        std::cout << "Check if the server is running or if your server IP and "
                     "port are correct."
                  << std::endl;
        return;
    }
    std::cout << "Connected to server at " << socket->remote_address()
              << " successfully. " << std::endl;

    std::cout << WELCOME_MESSAGE << std::endl << std::endl;

    play();
}

#include <iostream>
#include "protocol.h"
#include "socketstream/socketstream.hh"

class Client {
   private:
    swoope::socketstream* socket;
    int player_number;
    int player_team;

   public:
    Client();
    void start(char* ip, char* port);
};

void Client::start(char* ip, char* port) {
    socket = new swoope::socketstream();
    socket->open(ip, port);

    if (!socket->is_open()) {
        std::cout << "Failed to connect to server" << std::endl;
        return;
    }
    std::cout << "Connected to server at " << socket->remote_address() << std::endl;
}
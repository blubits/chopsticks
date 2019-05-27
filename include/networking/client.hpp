#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include "protocol.hpp"
#include "socketstream/socketstream.hh"

class Client {
   private:
    swoope::socketstream* socket;
    int player_number;
    int player_team;
    void play();

   public:
    // Client();
    int get_player_number() const;
    void start(char* ip, char* port);
};

#endif

#include <sstream>
#include <string>
#include <vector>
#include "game.hpp"
#include "protocol.hpp"
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

#endif

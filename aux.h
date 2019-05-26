#include <string>
#include "ctype.h"
#include "stdlib.h"

#ifndef AUX_h
#define AUX_H

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

bool is_valid_player_type(std::string type) {
    if (type == "human" || type == "alien" || type == "zombie" || type == "doggo") {
        return true;
    }
    return false;
}

#endif
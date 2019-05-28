#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

enum class CODES : int {
    NEW_BROADCAST = 0,
    REQUEST_NEW_INPUT = 100,
    NEW_INPUT = 101,
    GAME_END = 200,
    TERMINATE_CONNECTION = 400,
    PLAYER_DISCONNECT = 401
};

#endif
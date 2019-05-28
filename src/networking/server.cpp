#include <server.hpp>

bool is_valid_player_type(std::string type) {
    if (type == "human" || type == "alien" || type == "zombie" ||
        type == "doggo") {
        return true;
    }
    return false;
}

bool is_number(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

std::string ordinal(int num) {
    std::stringstream ss;
    ss << num;
    if (num % 10 == 1 && num % 100 != 11)
        ss << "st";
    else if (num % 10 == 2 && num % 100 != 12)
        ss << "nd";
    else if (num % 10 == 3 && num % 100 != 13)
        ss << "rd";
    else
        ss << "th";
    return ss.str();
}

std::string translate(std::string command) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    if (cmd == "tap") {
        std::string source_appendage_idx, target_appendage_idx;
        std::string target_player_idx;
        ss >> source_appendage_idx >> target_player_idx >> target_appendage_idx;
        int saidx = int(source_appendage_idx[1] - 'A') + 1,
            taidx = int(target_appendage_idx[1] - 'A') + 1;
        std::string source_appendage =
            ordinal(saidx) + " " +
            (source_appendage_idx[0] == 'H' ? "hand" : "foot");
        std::string target_appendage =
            ordinal(taidx) + " " +
            (target_appendage_idx[0] == 'H' ? "hand" : "foot");
        return "tapped player " + target_player_idx + "'s " + target_appendage +
               " with his " + source_appendage;
    } else if (cmd == "disthands") {
        return "shuffled his fingers.";
    } else if (cmd == "distfeet") {
        return "shuffled his toes.";
    }
    return "";
}

Server::Server() : num_players(1) {}

void Server::init_players() {
    int code;
    std::string line;

    // Request the number of players
    // Input must be between 2 and 6 inclusive
    while (true) {
        std::cout << "How many people are playing?" << std::endl << "> ";
        getline(std::cin, line);
        if (is_number(line)) {
            num_players = atol(line.c_str());
            if (num_players > 1 && num_players < 7) {
                break;
            } else {
                std::cout << "Too many players, sorry!" << std::endl;
            }
        } else {
            std::cout << "That's not a number, sorry!" << std::endl;
        }
    }

    std::cout << std::endl;

    // Let num_players - 1 clients connect to the server
    for (int i = 1; i < num_players; i++) {
        std::cout << "Got it! Waiting for players..." << std::endl;
        clients.push_back(new swoope::socketstream());
        swoope::socketstream *new_client = clients.back();
        server->accept(*new_client);
        std::cout << "Player " << i + 1 << " has connected from "
                  << clients.back()->remote_address() << std::endl;

        // Broadcast to clients that a new player has connected
        // Does not broadcast to the last player
        if (i != num_players - 1) {
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "Player " << i + 1
                        << " has connected. Waiting for more players..."
                        << std::endl;
            }
        }
    }

    std::string player_types[num_players];

    // Notify clients that player type selection has begun
    std::cout << "All players have connected!" << std::endl << std::endl;
    std::cout << "Requesting player type from players. Please wait."
              << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "All players have connected!" << std::endl;
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << std::endl;
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "Requesting player type from players. Please wait."
                << std::endl;
    }

    // Request each player type from each player
    while (true) {
        std::cout
            << "Please enter a player type (human, zombie, alien, doggo): "
            << std::endl
            << "> ";
        getline(std::cin, line);
        if (is_valid_player_type(line)) {
            player_types[0] = line;
            break;
        }
    }
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "Player " << 1 << " has elected to be a " << player_types[0]
                << "." << std::endl;
    }
    for (int i = 1; i < num_players; i++) {
        swoope::socketstream *current_client = clients[i - 1];

        // Request the new player's type
        while (true) {
            *current_client << static_cast<int>(CODES::REQUEST_NEW_INPUT)
                            << std::endl;
            *current_client
                << "Please enter a player type (human, zombie, alien, doggo): "
                << std::endl;
            *current_client >> code;
            current_client->ignore();
            if (code == static_cast<int>(CODES::NEW_INPUT)) {
                while (!getline(*current_client, line)) {
                };
                if (is_valid_player_type(line)) {
                    player_types[i] = line;
                    break;
                }
            }
        }
        // Broadcast the current client's information to other clients
        std::cout << "Player " << i + 1 << " has elected to be a "
                  << player_types[i] << "." << std::endl;
        for (auto client : clients) {
            *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
            *client << "Player " << i + 1 << " has elected to be a "
                    << player_types[i] << "." << std::endl;
        }
    }

    int player_teams[num_players];

    // Notify clients that team selection has begun
    std::cout << "Requesting player teams from players. Please wait."
              << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "Requesting player teams from players. Please wait."
                << std::endl;
    }

    while (true) {
        while (true) {
            int team_number = -1;
            std::cout << "Please input a team number between 1 and "
                      << num_players << "." << std::endl;
            getline(std::cin, line);
            if (is_number(line)) {
                team_number = atol(line.c_str());
                if (team_number > 0 && team_number <= num_players) {
                    player_teams[0] = team_number;
                    break;
                }
            }
        }
        for (int i = 1; i < num_players; i++) {
            while (true) {
                swoope::socketstream *current_client = clients[i - 1];
                int team_number = -1;
                *current_client << static_cast<int>(CODES::REQUEST_NEW_INPUT)
                                << std::endl;
                *current_client << "Please input a team number between 1 and "
                                << num_players << "." << std::endl;
                *current_client >> code;
                current_client->ignore();
                if (code == static_cast<int>(CODES::NEW_INPUT)) {
                    while (!getline(*current_client, line)) {
                    }
                    if (is_number(line)) {
                        team_number = atol(line.c_str());
                        if (team_number > 0 && team_number <= num_players) {
                            player_teams[i] = team_number;
                            break;
                        }
                    }
                }
            }
        }

        // Check validity of team configuration
        // Check if there are at least two teams with players
        // and that no valid team comes after a team with zero players
        bool is_valid = true;
        int team_count[num_players];
        for (int i = 0; i < num_players; i++) {
            team_count[i] = 0;
        }
        for (int i = 0; i < num_players; i++) {
            team_count[player_teams[i] - 1]++;
        }
        int num_valid_teams = 0;
        if (team_count[0] > 0) {
            num_valid_teams++;
        }
        for (int i = 1; i < num_players; i++) {
            if (team_count[i] > 0) {
                num_valid_teams++;
            }
            if (team_count[i] != 0 && team_count[i - 1] == 0) {
                is_valid = false;
                break;
            }
        }
        if (num_valid_teams > 1 && is_valid) {
            for (int j = 0; j < num_valid_teams; j++) {
                game->push_team();
            }
            for (int j = 0; j < num_players; j++) {
                game->push_player(player_teams[j] - 1, player_types[j]);
            }
            break;
        } else {
            std::cout << "An invalid team configuration was entered. "
                         "Restarting team selection."
                      << std::endl;
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "An invalid team configuration was entered. "
                           "Restarting team selection."
                        << std::endl;
            }
        }
    }

    std::cout << "All players have connected! Starting game." << std::endl;
    for (auto client : clients) {
        *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
        *client << "All players have connected! Starting game." << std::endl;
    }
}

void Server::start_game() {
    int code;
    std::string line;

    game->start_game();

    // Print the current state of the game
    std::cout << *game << std::endl;
    std::stringstream ss;
    for (auto client : clients) {
        ss << *game << std::endl;
        while (getline(ss, line)) {
            *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
            *client << line << std::endl;
        }
        ss.clear();
    }
    line = "";

    while (true) {
        Player *current_player = game->get_current_player();
        int current_player_idx = current_player->get_player_order() - 1;
        if (current_player_idx == 0) {
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << "Waiting for player " << current_player_idx + 1
                        << " to move." << std::endl;
            }
            int tries = 0;
            while (!game->is_valid_command(current_player, line)) {
                if (tries == 0)
                    std::cout << "Please enter your move." << std::endl;
                else
                    std::cout << "Invalid input, please try again."
                              << std::endl;
                getline(std::cin, line);
                tries++;
            }
            game->move(line);
        } else {
            std::cout << "Waiting for player " << current_player_idx + 1
                      << " to move." << std::endl;
            for (int i = 0; i < clients.size(); i++) {
                if (i != current_player_idx - 1) {
                    *clients[i] << static_cast<int>(CODES::NEW_BROADCAST)
                                << std::endl;
                    *clients[i] << "Waiting for player "
                                << current_player_idx + 1 << " to move."
                                << std::endl;
                }
            }
            std::cout << "current client idx: " << current_player_idx - 1
                      << std::endl;
            swoope::socketstream *current_client =
                clients[current_player_idx - 1];

            int tries = 0;
            while (true) {
                *current_client << static_cast<int>(CODES::REQUEST_NEW_INPUT)
                                << std::endl;
                if (tries == 0)
                    *current_client << "Please enter your move." << std::endl;
                else
                    *current_client << "Invalid input, please try again."
                                    << std::endl;
                *current_client >> code;
                current_client->ignore();
                if (code == static_cast<int>(CODES::NEW_INPUT)) {
                    while (!getline(*current_client, line)) {
                    }
                    if (game->is_valid_command(current_player, line)) break;
                }
                tries++;
            }
            game->move(line);
        }

        // Print the current player's move
        std::cout << "Player " << current_player_idx + 1 << " "
                  << translate(line) << std::endl;

        // Print the current state of the game
        std::cout << *game << std::endl;
        for (auto client : clients) {
            ss << *game << std::endl;
            while (getline(ss, line)) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                *client << line << std::endl;
            }
            ss.clear();
        }
        line = "";

        if (!game->is_ongoing()) {
            if (game->get_player(0)->get_player_team() == game->who_won()) {
                std::cout << "Your team wins!" << std::endl;
            } else {
                std::cout << "Your team lost :(" << std::endl;
                std::cout << "Team " << game->who_won() + 1 << " wins!"
                          << std::endl;
            }
            int p = 1;
            for (auto client : clients) {
                *client << static_cast<int>(CODES::NEW_BROADCAST) << std::endl;
                if (game->get_player(p++)->get_player_team() ==
                    game->who_won()) {
                    *client << "Your team wins!" << std::endl;
                } else {
                    *client << "Your team lost :(" << std::endl;
                    *client << static_cast<int>(CODES::NEW_BROADCAST)
                            << std::endl;
                    *client << "Team " << game->who_won() + 1 << " wins!"
                            << std::endl;
                }
                *client << static_cast<int>(CODES::GAME_END) << std::endl;
            }
            break;
        }
    }
}

void Server::start(char *port) {
    const std::string WELCOME_MESSAGE =
        "+++++++++++++++++++++++++++++++++++++\n"
        "+                                   +\n"
        "+             CHOPSTICKS            +\n"
        "+      Batara - Salinas - CS12      +\n"
        "+                                   +\n"
        "+++++++++++++++++++++++++++++++++++++";

    std::cout << "Attempting to start server..." << std::endl;

    server = new swoope::socketstream();
    server->open(port, 4);
    game = new Game();

    std::cout << "Started server succesfully." << std::endl << std::endl;
    std::cout << WELCOME_MESSAGE << std::endl << std::endl;

    init_players();
    start_game();
}

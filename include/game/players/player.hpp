#ifndef PLAYER_HPP
#define PLAYER_HPP

/**
 * Represents a game player.
 *
 * @author  Maded Batara III
 * @author  Jose Enrico Salinas
 * @date    2019-05-27
 */

#include <iostream>
#include <string>
#include <vector>
#include "foot.hpp"
#include "hand.hpp"
#include "playerinfo.hpp"

class Player {
   protected:
    int player_team;
    int player_order;
    char player_type;
    const PlayerInfo *player_info;

    std::vector<Hand *> hands;
    std::vector<Foot *> feet;

    bool is_skipped;
    int actions_remaining;

   public:
    Player(int player_team, int player_order, char player_type,
           const PlayerInfo *player_info);

    int get_player_team() const;
    char get_player_type() const;
    int get_player_order() const;
    const PlayerInfo *get_player_info() const;
    std::vector<Hand *> *get_hands();
    std::vector<Foot *> *get_feet();
    Hand *get_hand(int i);
    Foot *get_foot(int i);
    Appendage *get_appendage(std::string i);

    bool is_dead();
    bool has_turn();
    bool turn_skipped();
    bool has_action();
    void reset_actions();
    void use_action();
    bool in_play();
    void give_turn();
    void skip_turn();
    void unskip_turn();
    char get_type();

    // Distributes digits to hands and/or feet.
    void distribute_hands(std::vector<int> input);
    void distribute_feet(std::vector<int> input);

    // Taps a player's appendage.
    void tap(std::string my_appendage, Player *player,
             std::string target_appendage);

    // Recieves a tap from a player's appendage. Returns a reference to this
    // player's appendage that was tapped. Call after the tap has
    // already proceeded.
    virtual Appendage *recieve_tap(Appendage *my_appendage, Player *tapper,
                                   Appendage *source_appendage) = 0;

    // Gives a tap to a player's appendage. Returns a reference to this
    // player's appendage that tapped. Call after the tap has
    // already proceeded.
    virtual Appendage *give_tap(Appendage *my_appendage, Player *tapped,
                                Appendage *target_appendage) = 0;

    // Directs player state to an output stream.
    friend std::ostream &operator<<(std::ostream &os, Player &dt);
};

#endif

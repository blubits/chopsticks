//
// Classes for implementing tappable appendages and appendage appendage_types.
//
// @author Maded Batara III (blubits)
// @date 2019-05-16

#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <ostream>
#include <string>
#include "player.h"

// Represents the appendages of a certain player.
//
// The Appendage class is a semi-abstract class; appendage subclasses
// (e.g. hands and feet) are expected to implement add_digits() and is_dead(),
// as the logic for these functions will vary across appendage appendage_types.

class Appendage {
   protected:
    char appendage_type;
    int digits_raised;
    int digits_max;

   public:
    Appendage();
    Appendage(int digits_raised, int digits_max);
    Appendage(int digits_raised, int digits_max, char appendage_type);
    char get_appendage_type();
    int get_digits_raised() const;
    void set_digits_raised(int digits);
    int get_digits_max() const;
    void tap(Appendage* tap_target);

    // Add additional digits_raised digits to the appendage. Can only
    // be done as long as the appendage is still alive.
    virtual void add_digits(int digits) = 0;

    // Checks if the appendage is dead.
    virtual bool is_dead() const = 0;

    // Directs appendage state to an output stream.
    friend std::ostream& operator<<(std::ostream& os, const Appendage& dt);
};

Appendage::Appendage() : digits_raised(1), digits_max(5), appendage_type('A') {}

Appendage::Appendage(int digits_raised, int digits_max)
    : digits_raised(digits_raised),
      digits_max(digits_max),
      appendage_type('A') {}

Appendage::Appendage(int digits_raised, int digits_max, char appendage_type)
    : digits_raised(digits_raised),
      digits_max(digits_max),
      appendage_type(appendage_type) {}

void Appendage::tap(Appendage* tap_target) {
    if (tap_target->is_dead()) return;
    tap_target->add_digits(digits_raised);
}

char Appendage::get_appendage_type() { return appendage_type; }

int Appendage::get_digits_max() const { return digits_max; }

int Appendage::get_digits_raised() const { return digits_raised; }

void Appendage::set_digits_raised(int digits) {
    if (is_dead()) return;
    digits_raised = digits;
}

std::ostream& operator<<(std::ostream& os, const Appendage& dt) {
    if (dt.is_dead()) {
        os << "X";
    } else {
        os << dt.digits_raised;
    }
    return os;
}

// A player's hands.
//
// Note that a player's hand obeys standard rules of Chopsticks; fingers
// wrap around, and the hand remains alive until all fingers are digits_raised.

class Hand : public Appendage {
   public:
    Hand(int digits_raised, int digits_max);
    void add_digits(int digits) override;
    bool is_dead() const override;
};

Hand::Hand(int digits_raised, int digits_max)
    : Appendage(digits_raised, digits_max, 'H') {}

void Hand::add_digits(int digits) {
    if (is_dead()) return;
    digits_raised = (digits_raised - 1 + digits) % digits_max + 1;
}

bool Hand::is_dead() const { return digits_raised == digits_max; }

// A player's feet.
//
// Toes in this implementation of the game do not wrap around and are simply
// added until all possible toes have been digits_raised. Note that a player
// skips a turn if one of his feet die.

class Foot : public Appendage {
   public:
    Foot(int digits_raised, int digits_max);
    void add_digits(int digits) override;
    bool is_dead() const override;
};

Foot::Foot(int digits_raised, int digits_max)
    : Appendage(digits_raised, digits_max, 'F') {}

void Foot::add_digits(int digits) {
    if (is_dead()) return;
    digits_raised += digits;
}

bool Foot::is_dead() const { return digits_raised >= digits_max; }

#endif

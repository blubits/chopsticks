#include "hand.hpp"

Hand::Hand(int digits_raised, int digits_max)
    : Appendage(digits_raised, digits_max, 'H') {}

void Hand::add_digits(int digits) {
    if (is_dead()) return;
    // digits in the hand wrap around
    digits_raised = (digits_raised - 1 + digits) % digits_max + 1;
}

bool Hand::is_dead() const { return digits_raised == digits_max; }

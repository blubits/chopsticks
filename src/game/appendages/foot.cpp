#include "foot.hpp"

Foot::Foot(int digits_raised, int digits_max)
    : Appendage(digits_raised, digits_max, 'F') {}

void Foot::add_digits(int digits) {
    if (is_dead()) return;
    digits_raised += digits;
}

bool Foot::is_dead() const { return digits_raised >= digits_max; }

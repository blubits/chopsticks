#include "appendage.hpp"

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

char Appendage::get_appendage_type() const { return appendage_type; }

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

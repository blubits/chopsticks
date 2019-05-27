#ifndef APPENDAGE_HPP
#define APPENDAGE_HPP

/**
 * Represents the appendages of a certain player.
 *
 * The Appendage class is a semi-abstract class; appendage subclasses
 * (e.g. hands and feet) are expected to implement add_digits() and is_dead(),
 * as the logic for these functions will vary across appendage appendage_types.
 *
 * @author  Maded Batara III
 * @author  Jose Enrico Salinas
 * @date    2019-05-19
 */

#include <ostream>

class Appendage {
   protected:
    // The appendage type, either hand or foot.
    char appendage_type;
    // The number of raised digits on the appendage.
    int digits_raised;
    // The maximum number of raised digits on the appendage.
    int digits_max;

   public:
    Appendage();
    Appendage(int digits_raised, int digits_max);
    Appendage(int digits_raised, int digits_max, char appendage_type);

    char get_appendage_type() const;
    int get_digits_raised() const;
    void set_digits_raised(int digits);
    int get_digits_max() const;

    // Taps another hand, thereby adding digits to it.
    // Can only be done as long as both appendages are still alive.
    void tap(Appendage* tap_target);

    // Add additional digits to the appendage. Can only
    // be done as long as the appendage is still alive.
    // Implemented in subclasses.
    virtual void add_digits(int digits) = 0;

    // Checks if the appendage is dead.
    // Implemented in subclasses.
    virtual bool is_dead() const = 0;

    // Directs the appendage state to an output stream.
    friend std::ostream& operator<<(std::ostream& os, const Appendage& dt);
};

#endif

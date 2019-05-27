#ifndef FOOT_HPP
#define FOOT_HPP

/**
 * A player's feet.
 *
 * Note that toes do not wrap around and are simply added until all possible
 * toes have been digits_raised. Note that a player skips a turn if one of
 * his feet die.
 *
 * @author  Maded Batara III
 * @date    2019-05-19
 */

#include "appendage.hpp"

class Foot : public Appendage {
   public:
    Foot(int digits_raised, int digits_max);

    void add_digits(int digits) override;
    bool is_dead() const override;
};

#endif

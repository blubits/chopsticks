#ifndef HAND_HPP
#define HAND_HPP

/**
 * A player's hands.
 *
 * Note that a player's hand obeys standard rules of Chopsticks; fingers
 * wrap around, and the hand remains alive until all fingers are digits_raised.
 *
 * @author  Maded Batara III
 * @date    2019-05-19
 */

#include "appendage.hpp"

class Hand : public Appendage {
   public:
    Hand(int digits_raised, int digits_max);
    void add_digits(int digits) override;
    bool is_dead() const override;
};

#endif

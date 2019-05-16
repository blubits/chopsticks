//
// Classes for implementing tappable appendages and appendage types.
//
// @author Maded Batara III (blubits)
// @date 2019-05-16

#include <ostream>
#include <string>

// Represents the appendages of a certain player.
// The Appendage class is a semi-abstract class; appendage subclasses
// (e.g. hands and feet) are expected to implement add_digits() and is_dead(),
// as the logic for these functions will vary across appendage types.
class Appendage {
   protected:
    int raised;
    int max;

   public:
    Appendage();
    Appendage(int raised, int max);
    std::string get_name() const;
    int get_raised() const;
    int get_max() const;

    // Add additional raised digits to the appendage. Can only
    // be done as long as the appendage is still alive.
    virtual void add_digits(int digits) = 0;

    // Checks if the appendage is dead.
    virtual bool is_dead() const = 0;

    // Directs program state to an output stream.
    friend std::ostream& operator<<(std::ostream& os, const Appendage& dt);
};

Appendage::Appendage() : raised(1), max(5){};

Appendage::Appendage(int raised, int max) : raised(raised), max(max){};

int Appendage::get_max() const { return max; }

int Appendage::get_raised() const { return raised; }

std::ostream& operator<<(std::ostream& os, const Appendage& dt) {
    if (dt.is_dead()) {
        os << "X";
    } else {
        os << dt.raised;
    }
    return os;
}

// A player's hands.
// Note that a player's hand obeys standard rules of Chopsticks; fingers
// wrap around, and the hand remains alive until all fingers are raised.
class Hand : public Appendage {
   public:
    using Appendage::Appendage;
    void add_digits(int digits) override;
    bool is_dead() const override;
};

void Hand::add_digits(int digits) {
    if (is_dead()) return;
    raised = (raised - 1 + other.get_raised()) % max + 1;
}

bool Hand::is_dead() const { return raised == max; }

// A player's feet.
// Toes in this implementation of the game do not wrap around and are simply
// added until all possible toes have been raised. Note that a player skips
// a turn if one of his feet dies.
class Foot : public Appendage {
   public:
    using Appendage::Appendage;
    void add_digits(int digits) override;
    bool is_dead() const override;
};

void Foot::add_digits(int digits) {
    if (is_dead()) return;
    raised += digits;
}

bool Foot::is_dead() const { return raised >= max; }

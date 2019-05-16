#include "hand.h"
#include "player.h"

class Tapper {
   public:
    void tap(Human *h, Appendage &a);
    void tap(Alien *h, Appendage &a);
    void tap(Zombie *h, Appendage &a);
    void tap(Doggo *h, Appendage &a);
};

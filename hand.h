class Appendage {
   protected:
    int max;
    int raised;

   public:
    Appendage();
    Appendage(int max, int raised);
    int get_max() const;
    int get_raised() const;
    void set_raised(int raised);
    virtual void tap(const Appendage& other) = 0;
    virtual bool is_dead() = 0;
};

Appendage::Appendage() : max(5), raised(1){};

Appendage::Appendage(int max, int raised) : max(max), raised(raised){};

int Appendage::get_max() const { return max; }

int Appendage::get_raised() const { return raised; }

void Appendage::set_raised(int raised) { this->raised = raised; }

class Hand : public Appendage {
   public:
    virtual void tap(const Appendage& other);
    virtual bool is_dead();
};

void Hand::tap(const Appendage& other) {
    if (is_dead()) return;
    raised = (raised - 1 + other.get_raised()) % max + 1;
}

bool Hand::is_dead() { return raised == max; }

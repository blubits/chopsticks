#include <ostream>
#include <string>

class Appendage {
   protected:
    int max;
    int raised;

   public:
    Appendage();
    Appendage(int max, int raised);
    std::string get_name() const;
    int get_max() const;
    int get_raised() const;
    void set_raised(int raised);

    // Abstract functions; override in subclasses
    virtual void tap(const Appendage& other) = 0;
    virtual bool is_dead() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Appendage& dt);
};

Appendage::Appendage() : max(5), raised(1){};

Appendage::Appendage(int max, int raised) : max(max), raised(raised){};

int Appendage::get_max() const { return max; }

int Appendage::get_raised() const { return raised; }

void Appendage::set_raised(int raised) { this->raised = raised; }

std::ostream& operator<<(std::ostream& os, const Appendage& dt) {
    if (dt.is_dead()) {
        os << "X";
    } else {
        os << dt.raised;
    }
    return os;
}

class Hand : public Appendage {
   public:
    using Appendage::Appendage;
    void tap(const Appendage& other) override;
    bool is_dead() const override;
};

void Hand::tap(const Appendage& other) {
    if (is_dead()) return;
    raised = (raised - 1 + other.get_raised()) % max + 1;
}

bool Hand::is_dead() const { return raised == max; }

class Foot : public Appendage {
   public:
    using Appendage::Appendage;
    void tap(const Appendage& other) override;
    bool is_dead() const override;
};

void Foot::tap(const Appendage& other) {
    if (is_dead()) return;
    raised += other.get_raised();
}

bool Foot::is_dead() const { return raised >= max; }

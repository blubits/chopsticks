#include <string>

class Appendage {
   protected:
    std::string name;
    int max;
    int raised;

   public:
    Appendage(std::string name);
    Appendage(std::string name, int max, int raised);
    std::string get_name() const;
    int get_max() const;
    int get_raised() const;
    void set_raised(int raised);
    virtual void tap(const Appendage& other) = 0;
    virtual bool is_dead() = 0;
};

Appendage::Appendage(std::string name) : name(name), max(5), raised(1){};

Appendage::Appendage(std::string name, int max, int raised)
    : name(name), max(max), raised(raised){};

std::string Appendage::get_name() const { return name; }

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

class Hand {
   private:
    int num_of_fingers;
    int fingers_up;

   public:
    Hand() : num_of_fingers(5), fingers_up(1){};
    Hand(int num_of_fingers, int fingers_up)
        : num_of_fingers(num_of_fingers), fingers_up(fingers_up){};
    int get_num_of_fingers();
    void set_fingers_up(int fingers);
    int get_fingers_up();
    void tap(const Hand& other);
    bool is_dead();
};

int Hand::get_num_of_fingers() { return num_of_fingers; }

void Hand::set_fingers_up(int fingers) { fingers_up = fingers; }

int Hand::get_fingers_up() { return fingers_up; }

void Hand::tap(const Hand& other) {
    if (is_dead()) return;
    fingers_up = (fingers_up - 1 + other.fingers_up) % num_of_fingers + 1;
}

bool Hand::is_dead() { return num_of_fingers == fingers_up; }

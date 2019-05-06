// CS12 S - Batara, Salinas

#include <bits/stdc++.h>

using namespace std;

class Hand {
private:
    int num_of_fingers;
    int fingers_up;

public:
    Hand() : num_of_fingers(5), fingers_up(1) {};
    Hand(int num_of_fingers, int fingers_up) : num_of_fingers(num_of_fingers), fingers_up(fingers_up) {};
    int get_num_of_fingers();
    void set_fingers_up(int fingers);
    int get_fingers_up();
    void tap(const Hand &other);
    bool is_dead();
};

int Hand::get_num_of_fingers() {
    return num_of_fingers;
}

void Hand::set_fingers_up(int fingers) {
    fingers_up = fingers;
}

int Hand::get_fingers_up() {
    return fingers_up;
}

void Hand::tap(const Hand& other) {
    if (is_dead()) return;
    fingers_up = (fingers_up - 1 + other.fingers_up) % num_of_fingers + 1;
}

bool Hand::is_dead() {
    return num_of_fingers == fingers_up;
}

class Player {
private:
    Hand left;
    Hand right;
    int player_number;

public:
    Player(int player_number) : player_number(player_number), left(), right() {};
    const Hand get_left_hand();
    const Hand get_right_hand();
    void transfer(int fingers_left, int fingers_right);
    void attack(Player &other, char my_hand, char other_hand);
    bool is_dead();
    string to_string();
};

const Hand Player::get_left_hand() { return left; }
const Hand Player::get_right_hand() { return right; }

void Player::transfer(int fingers_left, int fingers_right) {
    if (is_dead()) return;
    left.set_fingers_up(fingers_left);
    right.set_fingers_up(fingers_right);
}

void Player::attack(Player &other, char my_hand, char other_hand) {
    if (is_dead()) return;
    if (my_hand == 'L') {
        if (other_hand == 'L') {
            left.tap(other.get_left_hand());
        } else if (other_hand == 'R') {
            left.tap(other.get_right_hand());
        }
    } else if (my_hand == 'R') {
        if (other_hand == 'L') {
            right.tap(other.get_left_hand());
        } else if (other_hand == 'R') {
            right.tap(other.get_right_hand());
        }
    }
}

bool Player::is_dead() {
    return left.is_dead() && right.is_dead();
}

string Player::to_string() {
    stringstream ans;
    ans << "P" << player_number << " L:" << left.get_fingers_up() << " R:" << right.get_fingers_up();
    return ans.str();
}

class Game {
private:
    Player p1;
    Player p2;
    bool ongoing;
    int turn;
    int won;

public:
    Game() : p1(1), p2(2), ongoing(true), turn(0), won(-1) {};
    void move(string command);
    bool is_ongoing();
    int who_won();
    string to_string() {
        return p1.to_string() + " | " + p2.to_string();
    }
};

void Game::move(string command) {
    if (!ongoing) return;
    stringstream ss(command);
    string action;
    ss >> action;
    if (action == "attack") {
        char a, b;
        ss >> a >> b;
        if (turn % 2 == 0) {
            p2.attack(p1, b, a);
        } else {
            p1.attack(p2, b, a);
        }
    } else if (action == "change") {
        int a, b;
        ss >> a >> b;
        if (turn % 2 == 0) {
            p1.transfer(a, b);
        } else {
            p2.transfer(a, b);
        }
    }
    if (p1.is_dead()) {
        won = 2;
        ongoing = false;
    } else if (p2.is_dead()) {
        won = 1;
        ongoing = false;
    }
    turn++;
}

bool Game::is_ongoing() { return ongoing; }
int Game::who_won() { return won; }

int main() {
    Game g;
    string command;
    cout << g.to_string() << endl;
    while (getline(cin, command)) {
        g.move(command);
        cout << g.to_string() << endl;
        if (!g.is_ongoing()) {
            cout << "Player " << g.who_won() << " wins" << endl; 
            break;
        }
    }
}
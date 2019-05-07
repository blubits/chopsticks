// CS12 S - Batara, Salinas

#include <game.h>
#include <iostream>
#include <string>

using namespace std;

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

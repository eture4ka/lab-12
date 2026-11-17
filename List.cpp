#include "List.h"
#include <iostream>

using namespace std;

List::List(const string& type, double gain, double maxCurrent) : data(type, gain, maxCurrent) {
    cout << "    + List: додано вузол \"" << type << "\"\n";
}

List::~List() {
    cout << "    - ~List: видалено вузол \"" << data.getType() << "\"\n";
}

shared_ptr<List> List::append(const shared_ptr<List>& tail,
                              const string& type, double gain, double maxCurrent) {
    auto node = make_shared<List>(type, gain, maxCurrent);
    if (tail) {
        tail->next = node;   // сильний зв'язок вперед
        node->prev = tail;   // слабкий зв'язок назад
    }
    return node;
}

void List::printForward(const shared_ptr<List>& head) {
    cout << "    Від голови до хвоста (через next):\n";
    int i = 1;
    for (shared_ptr<List> cur = head; cur; cur = cur->next) {
        cout << "      " << i++ << ". ";
        cur->data.print();
        cout << "   use_count = " << cur.use_count() << '\n';
    }
}

void List::printBackward(const shared_ptr<List>& tail) {
    cout << "    Від хвоста до голови (через prev.lock()):\n";
    int i = 1;
    for (shared_ptr<List> cur = tail; cur; cur = cur->prev.lock()) {
        cout << "      " << i++ << ". ";
        cur->data.print();
        cout << '\n';
    }
}

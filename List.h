#ifndef LIST_H
#define LIST_H

#include <memory>
#include "Transistor.h"

// Пункт 6: елемент двозв'язного списку транзисторів.
//   next — shared_ptr: елемент ВОЛОДІЄ наступним
//   prev — weak_ptr:   лише посилається на попередній, щоб не було циклу
class List {
public:
    Transistor data;
    std::shared_ptr<List> next;
    std::weak_ptr<List> prev;

    List(const std::string& type, double gain, double maxCurrent);
    ~List();

    // Додає елемент у кінець списку, що починається з head; повертає новий хвіст
    static std::shared_ptr<List> append(const std::shared_ptr<List>& tail,
                                        const std::string& type, double gain, double maxCurrent);
    static void printForward(const std::shared_ptr<List>& head);
    static void printBackward(const std::shared_ptr<List>& tail);
};

#endif

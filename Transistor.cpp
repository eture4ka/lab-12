#include "Transistor.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

Transistor::Transistor() : type("KT315B"), gain(200), maxCurrent(0.1) {
    cout << "    + Transistor(): створено \"" << type << "\" (за замовчуванням)\n";
}

Transistor::Transistor(const string& t, double g, double i) : type("?"), gain(1), maxCurrent(1) {
    // сетери перевіряють значення; якщо кинуть виняток, об'єкт не буде створено
    setType(t);
    setGain(g);
    setMaxCurrent(i);
    cout << "    + Transistor(): створено \"" << type << "\" (h = " << gain
         << ", Imax = " << maxCurrent << " А)\n";
}

Transistor::~Transistor() {
    cout << "    - ~Transistor(): знищено \"" << type << "\"\n";
}

string Transistor::getType() const { return type; }
double Transistor::getGain() const { return gain; }
double Transistor::getMaxCurrent() const { return maxCurrent; }

void Transistor::setType(const string& value) {
    if (value.empty()) throw invalid_argument("тип транзистора не може бути порожнім");
    type = value;
}

void Transistor::setGain(double value) {
    if (value <= 0 || value > 10000) {
        ostringstream msg;
        msg << "коефіцієнт підсилення " << value << " некоректний (0 < h <= 10000)";
        throw invalid_argument(msg.str());
    }
    gain = value;
}

void Transistor::setMaxCurrent(double value) {
    if (value <= 0 || value > 100) {
        ostringstream msg;
        msg << "струм " << value << " А некоректний (0 < I <= 100)";
        throw invalid_argument(msg.str());
    }
    maxCurrent = value;
}

double Transistor::power(double voltage) const { return voltage * maxCurrent; }

void Transistor::print() const {
    cout << left << setw(10) << type << right
         << " h = " << setw(7) << fixed << setprecision(1) << gain
         << "  Imax = " << setw(6) << setprecision(2) << maxCurrent << " А";
}

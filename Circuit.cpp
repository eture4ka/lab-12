#include "Circuit.h"
#include <iostream>

using namespace std;

// ---------- Amplifier ----------

Amplifier::Amplifier(const string& n) : name(n) {
    cout << "    + Amplifier(): створено підсилювач \"" << name << "\"\n";
}

Amplifier::~Amplifier() {
    cout << "    - ~Amplifier(): знищено підсилювач \"" << name << "\"\n";
}

void Amplifier::setStage(const shared_ptr<PowerStage>& s) { stage = s; }
string Amplifier::getName() const { return name; }

void Amplifier::show() const {
    cout << "    Підсилювач \"" << name << "\", вихідний каскад: ";
    if (stage) stage->getTransistor().print();
    else cout << "(немає)";
    cout << '\n';
}

// ---------- PowerStage ----------

PowerStage::PowerStage(const string& type, double gain, double maxCurrent)
    : transistor(type, gain, maxCurrent) {
    cout << "    + PowerStage(): створено каскад на \"" << type << "\"\n";
}

PowerStage::~PowerStage() {
    cout << "    - ~PowerStage(): знищено каскад на \"" << transistor.getType() << "\"\n";
}

void PowerStage::setOwner(const shared_ptr<Amplifier>& a) { owner = a; }
const Transistor& PowerStage::getTransistor() const { return transistor; }

void PowerStage::showOwner() const {
    if (shared_ptr<Amplifier> a = owner.lock())   // тимчасовий shared_ptr, якщо об'єкт ще живий
        cout << "    Каскад \"" << transistor.getType() << "\" належить підсилювачу \""
             << a->getName() << "\"\n";
    else
        cout << "    Каскад \"" << transistor.getType() << "\": власника вже не існує\n";
}

// ---------- Варіант з витоком ----------

int LeakyAmplifier::alive = 0;
int LeakyStage::alive = 0;

LeakyAmplifier::LeakyAmplifier() {
    ++alive;
    cout << "    + LeakyAmplifier() створено\n";
}

LeakyAmplifier::~LeakyAmplifier() {
    --alive;
    cout << "    - ~LeakyAmplifier() знищено\n";
}

LeakyStage::LeakyStage() {
    ++alive;
    cout << "    + LeakyStage() створено\n";
}

LeakyStage::~LeakyStage() {
    --alive;
    cout << "    - ~LeakyStage() знищено\n";
}

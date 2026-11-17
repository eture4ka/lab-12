#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <memory>
#include <string>
#include "Transistor.h"

// Пункт 5: два класи, що посилаються один на одного.
//   Amplifier  (ChildA) — підсилювач, ВОЛОДІЄ своїм вихідним каскадом (shared_ptr)
//   PowerStage (ChildB) — вихідний каскад на транзисторі, лише ЗНАЄ свій підсилювач (weak_ptr)

class PowerStage;

class Amplifier {
private:
    std::string name;
    std::shared_ptr<PowerStage> stage;   // сильне посилання: тримає об'єкт живим

public:
    explicit Amplifier(const std::string& name);
    ~Amplifier();
    void setStage(const std::shared_ptr<PowerStage>& s);
    std::string getName() const;
    void show() const;
};

class PowerStage {
private:
    Transistor transistor;
    std::weak_ptr<Amplifier> owner;      // слабке посилання: НЕ збільшує лічильник

public:
    PowerStage(const std::string& type, double gain, double maxCurrent);
    ~PowerStage();
    void setOwner(const std::shared_ptr<Amplifier>& a);
    const Transistor& getTransistor() const;
    void showOwner() const;              // доступ до власника через lock()
};

// Ті самі класи, але з shared_ptr в обидва боки (для демонстрації витоку пам'яті).
class LeakyStage;

class LeakyAmplifier {
public:
    std::shared_ptr<LeakyStage> stage;
    static int alive;                    // скільки об'єктів зараз існує
    LeakyAmplifier();
    ~LeakyAmplifier();
};

class LeakyStage {
public:
    std::shared_ptr<LeakyAmplifier> owner;   // ПОМИЛКА: має бути weak_ptr
    static int alive;
    LeakyStage();
    ~LeakyStage();
};

#endif

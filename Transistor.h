#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include <string>

// Клас варіанту 18: транзистор (тип, коефіцієнт підсилення, максимальний струм).
// Конструктор і деструктор виводять повідомлення, щоб було видно,
// коли розумні вказівники створюють і звільняють об'єкт.
class Transistor {
private:
    std::string type;    // тип (маркування)
    double gain;         // коефіцієнт підсилення
    double maxCurrent;   // максимальний струм, А

public:
    Transistor();
    Transistor(const std::string& type, double gain, double maxCurrent);
    ~Transistor();

    std::string getType() const;
    double getGain() const;
    double getMaxCurrent() const;

    // Сетери з перевіркою: кидають std::invalid_argument при некоректному значенні
    void setType(const std::string& value);
    void setGain(double value);
    void setMaxCurrent(double value);

    double power(double voltage) const;   // P = U * Imax, Вт
    void print() const;
};

#endif

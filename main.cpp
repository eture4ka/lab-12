#include <iostream>
#include <memory>
#include <stdexcept>
#include "Transistor.h"
#include "Circuit.h"
#include "List.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void header(const string& text) { cout << "\n========== " << text << " ==========\n"; }

void showPtr(const string& name, const unique_ptr<Transistor>& p) {
    cout << "    " << name << ".get() = " << p.get();
    if (p) { cout << "  -> "; p->print(); }
    else cout << "  (nullptr, нічим не володіє)";
    cout << '\n';
}

// ---------------------------------------------------------------------------
void demoUniquePtr() {
    header("3. std::unique_ptr");

    cout << "  Створення через make_unique<Transistor>():\n";
    unique_ptr<Transistor> item1 = make_unique<Transistor>();
    showPtr("item1", item1);

    // unique_ptr<Transistor> item2 = item1;
    // ^ ПОМИЛКА КОМПІЛЯЦІЇ: у unique_ptr конструктор копіювання видалено (= delete).
    //   Об'єктом може володіти лише ОДИН unique_ptr. Якби копія була можлива,
    //   два вказівники викликали б delete для того самого об'єкта (подвійне звільнення).
    cout << "  Рядок \"unique_ptr<Transistor> item2 = item1;\" не компілюється\n"
         << "  (копіювання unique_ptr заборонене, див. коментар у коді).\n";

    {
        cout << "\n  Переміщення: item2 = std::move(item1)\n";
        unique_ptr<Transistor> item2 = move(item1);
        showPtr("item1", item1);
        showPtr("item2", item2);
        cout << "    item1 == nullptr ? " << boolalpha << (item1 == nullptr) << '\n';
        cout << "    Потужність item2 при 12 В: " << item2->power(12) << " Вт\n";
        cout << "  Вихід з блоку -> item2 знищується, пам'ять звільняється автоматично:\n";
    }
    cout << "  Блок завершено, delete не викликали вручну.\n";

    cout << "\n  Перевірка валідності (виняток у конструкторі):\n";
    try {
        auto bad = make_unique<Transistor>("BAD", 100, -2);
    } catch (const invalid_argument& e) {
        cout << "    ! Помилка: " << e.what() << ". Об'єкт не створено, витоку немає.\n";
    }
}

// ---------------------------------------------------------------------------
void demoSharedPtr() {
    header("4. std::shared_ptr");

    auto sp1 = make_shared<Transistor>("IRF540N", 1500, 33);
    cout << "  Після make_shared:           use_count = " << sp1.use_count() << '\n';

    shared_ptr<Transistor> sp2 = sp1;
    shared_ptr<Transistor> sp3 = sp1;
    cout << "  Після двох копій (sp2, sp3): use_count = " << sp1.use_count() << '\n';

    {
        shared_ptr<Transistor> sp4 = sp1;
        cout << "  Всередині блоку (sp4):       use_count = " << sp1.use_count() << '\n';
    }
    cout << "  Після виходу з блоку:        use_count = " << sp1.use_count() << '\n';
    cout << "  (sp4 знищився і зменшив лічильник; об'єкт живий, поки лічильник > 0)\n";

    sp2.reset();
    sp3.reset();
    cout << "  Після sp2.reset(), sp3.reset(): use_count = " << sp1.use_count() << '\n';
    cout << "  Кінець функції -> sp1 знищується, лічильник 0, об'єкт видаляється:\n";
}

// ---------------------------------------------------------------------------
void demoWeakPtr() {
    header("5. std::weak_ptr проти циклічних посилань");

    cout << "  Правильно: Amplifier --shared_ptr--> PowerStage --weak_ptr--> Amplifier\n";
    {
        auto amp = make_shared<Amplifier>("УНЧ-1");
        auto stage = make_shared<PowerStage>("2N3055", 20, 15);
        amp->setStage(stage);
        stage->setOwner(amp);

        amp->show();
        stage->showOwner();
        cout << "    use_count(amp)   = " << amp.use_count()
             << "  (тільки змінна amp; weak_ptr не рахується)\n";
        cout << "    use_count(stage) = " << stage.use_count()
             << "  (змінна stage + поле в amp)\n";
        cout << "  Вихід з блоку:\n";
    }
    cout << "  Обидва деструктори викликані -> витоку немає.\n";

    cout << "\n  Неправильно: shared_ptr в обидва боки (цикл)\n";
    {
        auto amp = make_shared<LeakyAmplifier>();
        auto stage = make_shared<LeakyStage>();
        amp->stage = stage;
        stage->owner = amp;
        cout << "    use_count(amp)   = " << amp.use_count() << '\n';
        cout << "    use_count(stage) = " << stage.use_count() << '\n';
        cout << "  Вихід з блоку:\n";
    }
    cout << "    (жодного деструктора не викликано!)\n"
         << "    Живих LeakyAmplifier: " << LeakyAmplifier::alive
         << ", LeakyStage: " << LeakyStage::alive << "  -> ВИТІК ПАМ'ЯТІ\n"
         << "    Об'єкти тримають один одного, тому лічильник кожного лишається 1\n"
         << "    і ніколи не падає до 0.\n";
}

// ---------------------------------------------------------------------------
void demoList() {
    header("6. Двозв'язний список (next: shared_ptr, prev: weak_ptr)");

    auto head = List::append(nullptr, "KT315B", 200, 0.1);
    auto tail = head;
    tail = List::append(tail, "BC547", 110, 0.1);
    tail = List::append(tail, "KT815", 40, 1.5);
    tail = List::append(tail, "IRF540N", 1500, 33);

    cout << '\n';
    List::printForward(head);
    List::printBackward(tail);

    double total = 0;
    for (auto cur = head; cur; cur = cur->next) total += cur->data.getMaxCurrent();
    cout << "    Сумарний максимальний струм: " << total << " А\n";

    cout << "\n  Кінець функції -> head знищується і по ланцюжку next звільняє весь список:\n";
    tail.reset();   // хвіст тримає лише локальна змінна; відпускаємо її першою
}

// ---------------------------------------------------------------------------
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    cout << "Лабораторна робота. Варіант 18: Транзистор (тип, підсилення, макс. струм)\n";

    demoUniquePtr();
    demoSharedPtr();
    demoWeakPtr();
    demoList();

    cout << "\nПрограму завершено.\n";
    return 0;
}

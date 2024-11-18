#include <iostream>
#include <vector>
#include <fstream> //Чтение с файла для графика
#include <iomanip> //Выравнивание вывода
#include <chrono> //Время
#include <memory> // Для STL указателей
#include <cassert> //Ошибки

#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include "LinkedListUniquePtr.hpp"
#include "LinkedListSharedPtr.hpp"
#include "tests.hpp"

void testUnqPtrDereferencing() {
    UniquePtr<int> unqPtr(new int(42));
    assert(*unqPtr == 42);
    std::cout << "testUnqPtrDereferencing() - PASSED\n"; // Разыменование UnqPtr работает корректно
}

void testShrdPtrDereferencing() {
    UniquePtr<int> unqPtr(new int(42));
    SharedPtr<int> shrdPtr(unqPtr.release());
    assert(*shrdPtr == 42);
    std::cout << "testShrdPtrDereferencing() - PASSED\n"; // Разыменование ShrdPtr работает корректно
}

void testUnqPtrNullptr() {
    UniquePtr<int> unqPtr;
    assert(!unqPtr);
    std::cout << "testUnqPtrNullptr() - PASSED\n"; //Проверка UnqPtr на nullptr пройдена
}

void testShrdPtrNullptr() {
    SharedPtr<int> shrdPtr;
    assert(!shrdPtr);
    std::cout << "testShrdPtrNullptr() - PASSED\n"; //Проверка SharedPtr на nullptr пройдена
}

void testUnqPtrMove() {
    UniquePtr<int> unqPtr1(new int(100));
    UniquePtr<int> unqPtr2 = std::move(unqPtr1);
    assert(!unqPtr1 && unqPtr2);
    std::cout << "testUnqPtrMove() - PASSED\n"; //Семантика перемещения UnqPtr работает корректно
}

void testShrdPtrFromUnqPtr() {
    UniquePtr<int> unqPtr(new int(200));
    SharedPtr<int> shrdPtr(unqPtr.release());
    assert(*shrdPtr == 200 && !unqPtr);
    std::cout << "testShrdPtrFromUnqPtr() - PASSED\n"; //ShrdPtr корректно принял права собственности от UnqPtr
}

class BaseTest {
public:
    virtual ~BaseTest() = default;
    virtual void show() const {
        std::cout << "Base class\n";
    }
};

class DerivedTest : public BaseTest {
public:
    void show() const override {
        std::cout << "Derived class\n";
    }
};

void testPolymorphism() {
    UniquePtr<BaseTest> unqPtr(new DerivedTest());
    assert(dynamic_cast<DerivedTest*>(unqPtr.get()) != nullptr);  // Проверка приведения к DerivedTest

    SharedPtr<BaseTest> shrdPtr(unqPtr.release());
    assert(dynamic_cast<DerivedTest*>(shrdPtr.get()) != nullptr);  // Проверка приведения к DerivedTest
    
    std::cout << "testPolymorphism() - PASSED\n";
}

void testUniquePtrInheritance() {
    UniquePtr<BaseTest> basePtr(new BaseTest());
    UniquePtr<DerivedTest> derivedPtr(new DerivedTest());
    basePtr = std::move(derivedPtr);
    assert(!derivedPtr && basePtr);
    std::cout << "testUniquePtrInheritance() - PASSED\n"; // UniquePtr корректно перенес производный файл в базовый
}

void testSharedPtrInheritance() {
    SharedPtr<BaseTest> basePtr(new BaseTest());
    SharedPtr<DerivedTest> derivedPtr(new DerivedTest());
    basePtr = derivedPtr;
    assert(basePtr && derivedPtr);
    std::cout << "testSharedPtrInheritance() - PASSED\n"; // SharedPtr корректно перенес производный файл в базовый
}

void testArrayHandling() {
    UniquePtr<int[]> unqArray(new int[5]{1, 2, 3, 4, 5});
    assert(unqArray);
    std::cout << "testArrayHandling() - PASSED\n"; // Обработка массива для UnqPtr работает 
}

void testSharedPtrFunctionality() {

    // Тест 1: Преобразование SharedPtr<int> в SharedPtr<float>
    SharedPtr<int> intPtr = SharedPtr<int>(new int(42));
    SharedPtr<float> floatPtr = intPtr;  // Преобразование int в float
    assert(floatPtr.useCount() == intPtr.useCount()); 
    assert(*floatPtr == 42.0f); // Проверка значения после преобразования
    std::cout << floatPtr.useCount();
    std::cout << intPtr.useCount();

    // Тест 2: Преобразование SharedPtr<DerivedTest> в SharedPtr<BaseTest>
    SharedPtr<DerivedTest> derivedPtr = SharedPtr<DerivedTest>(new DerivedTest());
    SharedPtr<BaseTest> basePtr = derivedPtr; // Преобразование DerivedTest в BaseTest
    assert(basePtr.useCount() == derivedPtr.useCount());
    //basePtr->show(); // Полиморфное поведение (Derived class)

    // Тест 3: Проверка корректности счетчика ссылок после нескольких присвоений
    SharedPtr<float> floatPtrCopy = floatPtr;
    assert(floatPtr.useCount() == 3);
    assert(*floatPtrCopy == 42.0f); // Проверка значения копии
}

void zz() {
    // Должно компилироваться
    UniquePtr<int> uni ( new int(42));
    *uni = 100500;
    assert(*uni == 100500);
}

void functionalTest() {
    std::cout << "Функциональное тестирование запущено\n";
    
    testUnqPtrDereferencing();
    testShrdPtrDereferencing();
    testUnqPtrNullptr();
    testShrdPtrNullptr();
    testUnqPtrMove();
    testShrdPtrFromUnqPtr();
    testPolymorphism();
    testUniquePtrInheritance();
    testSharedPtrInheritance();
    testArrayHandling();
    testSharedPtrFunctionality();
    zz();

    std::cout << "Функциональное тестирование окончено\n";
}

// Для собственных UniquePtr и SharedPtr
double loadTestUniquePtr(int N) {
    std::vector<UniquePtr<int>> buff(N);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        if (i % 5 == 0) {
            buff[i] = UniquePtr<int>(new int(i));
        } else {
            buff[i] = std::move(buff[i - (i % 5)]);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    return duration.count();
}

double loadTestSharedPtr(int N) {
    std::vector<SharedPtr<int>> buff(N);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        if (i % 5 == 0) {
            buff[i] = SharedPtr<int>(new int(i));
        } else {
            buff[i] = buff[i - (i % 5)];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    return duration.count();
}

// Для стандартных STL UniquePtr и SharedPtr
double loadTestStdUniquePtr(int N) {
    std::vector<std::unique_ptr<int>> buff(N);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        if (i % 5 == 0) {
            buff[i] = std::make_unique<int>(i);
        } else {
            buff[i] = std::move(buff[i - (i % 5)]);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    return duration.count();
}

double loadTestStdSharedPtr(int N) {
    std::vector<std::shared_ptr<int>> buff(N);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        if (i % 5 == 0) {
            buff[i] = std::make_shared<int>(i);
        } else {
            buff[i] = buff[i - (i % 5)];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    return duration.count();
}

void runLoadTestsAndPlot() {
    const int step = 500'000;
    const int maxElements = 20 * step;

    // Открываем файл для записи результатов
    std::ofstream file("load_test_results.csv");
    if (!file.is_open()) {
        std::cerr << "Ошибка при открытии файла для записи!\n";
        return;
    }

    // Заголовок CSV
    file << "Elements,UniquePtr,StdUniquePtr,SharedPtr,StdSharedPtr\n";

    std::cout << std::setw(20) << "Elements"
            << std::setw(20) << "UniquePtr (s)"
            << std::setw(20) << "std::unique_ptr (s)"
            << std::setw(20) << "SharedPtr (s)"
            << std::setw(20) << "std::shared_ptr (s)"
            << std::setw(25) << "UniquePtr Speed (%)"
            << std::setw(25) << "SharedPtr Speed (%)" << std::endl;

    // Запускаем тесты и записываем результаты в файл
    for (int items = step; items <= maxElements; items += step) {
        double timeUnique = loadTestUniquePtr(items);
        double timeStdUnique = loadTestStdUniquePtr(items);
        double timeShared = loadTestSharedPtr(items);
        double timeStdShared = loadTestStdSharedPtr(items);

        file << items << ","
             << std::fixed << std::setprecision(7) << timeUnique << ","
             << timeStdUnique << ","
             << timeShared << ","
             << timeStdShared << "\n";

        double uniqueSpeed = (timeStdUnique / timeUnique - 1) * 100;
        double sharedSpeed = (timeStdShared / timeShared - 1) * 100;

        // Выводим на экран
        std::cout << std::setw(20) << items
                << std::setw(20) << timeUnique
                << std::setw(20) << timeStdUnique
                << std::setw(20) << timeShared
                << std::setw(20) << timeStdShared 
                << std::setw(25) << uniqueSpeed
                << std::setw(25) << sharedSpeed << std::endl;
    }
    file.close();
    std::cout << "Нагрузочное тестирование окончено, результаты сохранены в 'load_test_results.csv'\n";

    // Запускаем gnuplot
    int result = system("gnuplot plot.gp");
    if (result != 0) {
        std::cerr << "Ошибка при запуске gnuplot, убедитесь, что он установлен и доступен в PATH\n";
    } else {
        std::cout << "График построен и сохранен в 'load_test_plot.png'\n";
    }
}

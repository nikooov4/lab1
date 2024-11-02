#include <iostream>
#include <vector>
#include <iomanip> //Выравнивание вывода
#include <chrono> //Время
#include <memory> // Для STL указателей
#include <cassert> //Ошибки

#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include "LinkedListUniquePtr.hpp"
#include "LinkedListSharedPtr.hpp"
#include "tests.hpp"

// Тестирование разыменования указателей
void testUnqPtrDereferencing() {

    UniquePtr<int> unqPtr(new int(42));
    if (*unqPtr == 42) {
        std::cout << "testUnqPtrDereferencing() - PASSED\n"; //Разыменование UnqPtr работает корректно
    } else {
        std::cout << "testUnqPtrDereferencing() - ERROR\n"; //Не удалось выполнить разыменование UnqPtr
    }
}

void testShrdPtrDereferencing() {

    UniquePtr<int> unqPtr(new int(42));
    SharedPtr<int> shrdPtr(unqPtr.release()); // Передача владения сырого указателя
    
    if (*shrdPtr == 42) {
        std::cout << "testShrdPtrDereferencing() - PASSED\n"; //Разыменование ShrdPtr работает корректно
    } else {
        std::cout << "testShrdPtrDereferencing() - ERROR\n"; //Не удалось выполнить разыменование SharedPtr
    }
}

void testUnqPtrNullptr() {
    
    UniquePtr<int> unqPtr;
    if (!unqPtr) {
        std::cout << "testUnqPtrNullptr() - PASSED\n"; //Проверка UnqPtr на nullptr пройдена
    } else {
        std::cout << "testUnqPtrNullptr() - ERROR\n"; //Проверка UnqPtr на nullptr НЕ пройдена
    }
}

void testShrdPtrNullptr() {
    
    SharedPtr<int> shrdPtr;
    if (!shrdPtr) {
        std::cout << "testShrdPtrNullptr() - PASSED\n"; //Проверка SharedPtr на nullptr пройдена
    } else {
        std::cout << "testShrdPtrNullptr() - ERROR\n"; //Проверка SharedPtr на nullptr НЕ пройдена
    }
}

void testUnqPtrMove() {
    
    UniquePtr<int> unqPtr1(new int(100));
    UniquePtr<int> unqPtr2 = std::move(unqPtr1);
    
    if (!unqPtr1 && unqPtr2) {
        std::cout << "testUnqPtrMove() - PASSED\n"; //Семантика перемещения UnqPtr работает корректно
    } else {
        std::cout << "testUnqPtrMove() - ERROR\n"; //Семантика перемещения UnqPtr НЕ работает корректно
    }
}

void testShrdPtrFromUnqPtr() {

    UniquePtr<int> unqPtr(new int(200));
    SharedPtr<int> shrdPtr(unqPtr.release()); // Передача владения сырого указателя
    
    if (*shrdPtr == 200 && !unqPtr) {
        std::cout << "testShrdPtrFromUnqPtr() - PASSED\n"; //ShrdPtr корректно принял права собственности от UnqPtr
    } else {
        std::cout << "testShrdPtrFromUnqPtr() - ERROR\n"; //ShrdPtr НЕ корректно принял права собственности от UnqPtr
    }
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
    //unqPtr->show();  // Должен сказать Derived class

    SharedPtr<BaseTest> shrdPtr(unqPtr.release());
    //shrdPtr->show(); // Должен сказать Derived class

    std::cout << "testPolymorphism() - PASSED\n";
}

// Тест на единственную наследственность
void testUniquePtrInheritance() {

    // Создаем UniquePtr для базового и производного классов
    UniquePtr<BaseTest> basePtr(new BaseTest());
    UniquePtr<DerivedTest> derivedPtr(new DerivedTest());
/*
    std::cout << "Наследование:";
    basePtr->show();      // Должно вывести "Base class"
    std::cout << "Наследование:";
    derivedPtr->show();   // Должно вывести "Derived class"
*/
    basePtr = std::move(derivedPtr);

    if (!derivedPtr && basePtr) {
        std::cout << "testUniquePtrInheritance() - PASSED\n"; // UniquePtr корректно перенес производный файл в базовый
    } else {
        std::cout << "testUniquePtrInheritance() - ERROR\n"; // Ошибка в тесте наследования UniquePtr
    }

    //basePtr->show();  // Должно вывести "Derived class" после присваивания
}

// Тест на единственную наследственность
void testSharedPtrInheritance() {

    SharedPtr<BaseTest> basePtr(new BaseTest());
    SharedPtr<DerivedTest> derivedPtr(new DerivedTest());
/*
    basePtr->show();      // Должно вывести "Base class"
    derivedPtr->show();   // Должно вывести "Derived class"
*/
    basePtr = derivedPtr;

    if (basePtr && derivedPtr) {
        std::cout << "testSharedPtrInheritance() - PASSED\n"; // UniquePtr корректно перенес производный файл в базовый
    } else {
        std::cout << "testSharedPtrInheritance() - ERROR\n"; // Ошибка в тесте наследования UniquePtr
    }

    //basePtr->show();  // Должно вывести "Derived class" после присваивания
}


// Обработка тестового массива
void testArrayHandling() {
 
    UniquePtr<int[]> unqArray(new int[5]{1, 2, 3, 4, 5});
    if (unqArray) {
        std::cout << "testArrayHandling() - PASSED\n"; // Обработка массива для UnqPtr работает 
    } else {
        std::cout << "testArrayHandling() - ERROR\n"; // Обработка массива для UnqPtr НЕ работает 
    }
}

void testSharedPtrFunctionality() {

    // Тест 1: Преобразование SharedPtr<int> в SharedPtr<float>
    SharedPtr<int> intPtr = SharedPtr<int>(new int(42));
    SharedPtr<float> floatPtr = intPtr;  // Преобразование int в float
    assert(floatPtr.use_count() == intPtr.use_count()); 
    assert(*floatPtr == 42.0f); // Проверка значения после преобразования

    // Тест 2: Преобразование SharedPtr<DerivedTest> в SharedPtr<BaseTest>
    SharedPtr<DerivedTest> derivedPtr = SharedPtr<DerivedTest>(new DerivedTest());
    SharedPtr<BaseTest> basePtr = derivedPtr; // Преобразование DerivedTest в BaseTest
    assert(basePtr.use_count() == derivedPtr.use_count());
    //basePtr->show(); // Полиморфное поведение (Derived class)

    // Тест 3: Проверка корректности счетчика ссылок после нескольких присвоений
    SharedPtr<float> floatPtrCopy = floatPtr;
    assert(floatPtr.use_count() == 3);
    assert(*floatPtrCopy == 42.0f); // Проверка значения копии
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

void runLoadTests() {
    const int step = 1'000'000;
    std::cout << "Нагрузочное тестирование запущено\n";

    std::cout << std::setw(20) << "Elements"
              << std::setw(20) << "UniquePtr (s)"
              << std::setw(20) << "std::unique_ptr (s)"
              << std::setw(20) << "SharedPtr (s)"
              << std::setw(20) << "std::shared_ptr (s)"
              << std::setw(25) << "UniquePtr Speed (%)"
              << std::setw(25) << "SharedPtr Speed (%)" << std::endl;

    for (int items = step; items <= 20 * step; items += step) {
        double timeUnique = loadTestUniquePtr(items);
        double timeStdUnique = loadTestStdUniquePtr(items);
        double timeShared = loadTestSharedPtr(items);
        double timeStdShared = loadTestStdSharedPtr(items);

        double uniqueSpeed = (timeStdUnique / timeUnique - 1) * 100;
        double sharedSpeed = (timeStdShared / timeShared - 1) * 100;

        std::cout << std::setw(20) << items
                  << std::setw(20) << timeUnique
                  << std::setw(20) << timeStdUnique
                  << std::setw(20) << timeShared
                  << std::setw(20) << timeStdShared
                  << std::setw(25) << uniqueSpeed
                  << std::setw(25) << sharedSpeed
                  << std::endl;
    }
    std::cout << "Нагрузочное тестирование окончено\n";
}
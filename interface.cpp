#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>    // Для использования std::unique_ptr и std::shared_ptr 
#include <limits>  // для std::numeric_limits

#include "tests.hpp"
#include "interface.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"

//#include <matplotlibcpp.h>

//Для хранения указателей с именами
std::unordered_map<std::string, UniquePtr<int>> uniquePointersInt;
std::unordered_map<std::string, UniquePtr<std::string>> uniquePointersString;

std::unordered_map<std::string, SharedPtr<int>> sharedPointersInt;
std::unordered_map<std::string, SharedPtr<std::string>> sharedPointersString;

int getIntInput() {
    int value;

    while (true) {
        std::cin >> value;

        // Проверка на успешное считывание числа
        if (std::cin.fail()) {
            std::cin.clear(); // Сбрасываем состояние ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
            std::cout << "Ошибка: пожалуйста, введите корректное число.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем оставшийся ввод
            return value;
        }
    }
}

float getFloatInput() {
    float value;

    while (true) {
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: пожалуйста, введите корректное число.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

// Создание UniquePtr
void createUniquePtr() {
    std::string name;
    std::cout << "Введите имя для UniquePtr: ";
    std::cin >> name;

    std::cout << "Выберите тип данных для UniquePtr (1 - число, 2 - строка): ";
    int choice = getIntInput();

    if (choice == 1) {
        std::cout << "Введите значение для UniquePtr: ";
        int value = getIntInput();
        uniquePointersInt[name] = UniquePtr<int>(new int(value));
        std::cout << "UniquePtr с именем " << name << " для числа создан.\n";
    } else if (choice == 2) {
        std::string strValue;
        std::cout << "Введите строку для UniquePtr: ";
        std::cin.ignore();
        std::getline(std::cin, strValue);

        uniquePointersString[name] = UniquePtr<std::string>(new std::string(strValue));
        std::cout << "UniquePtr с именем " << name << " для строки создан.\n";
    } else {
        std::cout << "Неверный выбор!\n";
    }
}

// Создание SharedPtr
void createSharedPtr() {
    std::string name;
    std::cout << "Введите имя для SharedPtr: ";
    std::cin >> name;

    std::cout << "Выберите тип данных для SharedPtr (1 - число, 2 - строка): ";
    int choice = getIntInput();
    
    if (choice == 1) {
        std::cout << "Введите значение для SharedPtr: ";
        int value = getIntInput();
        sharedPointersInt[name] = SharedPtr<int>(new int(value));
        std::cout << "SharedPtr с именем " << name << " для числа создан.\n";
    } else if (choice == 2) {
        std::string strValue;
        std::cout << "Введите строку для SharedPtr: ";
        std::cin.ignore();
        std::getline(std::cin, strValue);
        sharedPointersString[name] = SharedPtr<std::string>(new std::string(strValue));
        std::cout << "SharedPtr с именем " << name << " для строки создан.\n";
    } else {
        std::cout << "Неверный выбор!\n";
    }
}

// Создание дубликата SharedPtr
void createDuplicateSharedPtr() {
    std::string existingName, newName;
    std::cout << "Введите имя существующего SharedPtr для дублирования: ";
    std::cin >> existingName;

    std::cout << "Введите имя для нового дубликата SharedPtr: ";
    std::cin >> newName;

    if (sharedPointersInt.find(existingName) != sharedPointersInt.end()) {
        sharedPointersInt[newName] = sharedPointersInt[existingName];
        std::cout << "Дубликат SharedPtr с именем " << newName << " создан для числового SharedPtr " << existingName << ".\n";
    } else if (sharedPointersString.find(existingName) != sharedPointersString.end()) {
        sharedPointersString[newName] = sharedPointersString[existingName];
        std::cout << "Дубликат SharedPtr с именем " << newName << " создан для строкового SharedPtr " << existingName << ".\n";
    } else {
        std::cout << "SharedPtr с именем " << existingName << " не найден!\n";
    }
}


// Удаление UniquePtr
void deleteUniquePtr() {
    std::string name;
    std::cout << "Введите имя UniquePtr для удаления: ";
    std::cin >> name;

    // Удаление числового указателя
    if (uniquePointersInt.erase(name)) {
        std::cout << "UniquePtr для числа с именем " << name << " удален.\n";
    } 
    // Удаление строкового указателя
    else if (uniquePointersString.erase(name)) {
        std::cout << "UniquePtr для строки с именем " << name << " удален.\n";
    } else {
        std::cout << "UniquePtr с именем " << name << " не найден!\n";
    }
}

// Удаление SharedPtr
void deleteSharedPtr() {
    std::string name;
    std::cout << "Введите имя SharedPtr для удаления: ";
    std::cin >> name;

    // Удаление числового указателя
    if (sharedPointersInt.erase(name)) {
        std::cout << "SharedPtr для числа с именем " << name << " удален.\n";
    } 
    // Удаление строкового указателя
    else if (sharedPointersString.erase(name)) {
        std::cout << "SharedPtr для строки с именем " << name << " удален.\n";
    } else {
        std::cout << "SharedPtr с именем " << name << " не найден!\n";
    }
}

// Отображение всех созданных указателей
void displayPointers() {
    std::cout << "\nСписок UniquePtr для чисел:\n";
    for (const auto& [name, ptr] : uniquePointersInt) {
        if (ptr) {
            std::cout << "Имя: " << name << ", Значение: " << *ptr << "\n";
        } else {
            std::cout << "Имя: " << name << ", Указатель освобожден\n";
        }
    }

    std::cout << "\nСписок UniquePtr для строк:\n";
    for (const auto& [name, ptr] : uniquePointersString) {
        if (ptr) {
            std::cout << "Имя: " << name << ", Значение: " << *ptr << "\n";
        } else {
            std::cout << "Имя: " << name << ", Указатель освобожден\n";
        }
    }

    std::cout << "\nСписок SharedPtr для чисел:\n";
    for (const auto& [name, ptr] : sharedPointersInt) {
        if (ptr) {
            std::cout << "Имя: " << name << ", Значение: " << *ptr << ", Счетчик ссылок: " << ptr.use_count() << "\n";
        } else {
            std::cout << "Имя: " << name << ", Указатель освобожден\n";
        }
    }

    std::cout << "\nСписок SharedPtr для строк:\n";
    for (const auto& [name, ptr] : sharedPointersString) {
        if (ptr) {
            std::cout << "Имя: " << name << ", Значение: " << *ptr << ", Счетчик ссылок: " << ptr.use_count() << "\n";
        } else {
            std::cout << "Имя: " << name << ", Указатель освобожден\n";
        }
    }
}

void testSubtypingConsole() {
    std::cout << "Тест подтипизации между SharedPtr<int> и SharedPtr<float>\n";
    std::cout << "1. Создать SharedPtr<int> и присвоить его SharedPtr<float>\n";
    std::cout << "2. Создать SharedPtr<float> и присвоить его SharedPtr<int>\n";
    std::cout << "Выберите опцию (1 или 2): ";
    int choice = getIntInput();

    if (choice == 1) {
        std::cout << "Введите значение для SharedPtr<int>: ";
        int value = getIntInput();

        SharedPtr<int> intPtr(new int(value));
        SharedPtr<float> floatPtr = intPtr;  
        
        std::cout << "SharedPtr<int> успешно присвоен SharedPtr<float>.\n";
        std::cout << "Значение floatPtr: " << *floatPtr << "\n";
        std::cout << "Счетчик ссылок floatPtr: " << floatPtr.use_count() << "\n";
    } 
    else if (choice == 2) {
        std::cout << "Введите значение для SharedPtr<float>: ";
        float value = getFloatInput();

        SharedPtr<float> floatPtr(new float(value));
        SharedPtr<int> intPtr = floatPtr;  
        
        std::cout << "SharedPtr<float> успешно присвоен SharedPtr<int>.\n";
        std::cout << "Значение intPtr: " << *intPtr << "\n";
        std::cout << "Счетчик ссылок intPtr: " << intPtr.use_count() << "\n";
    } 
    else {
        std::cout << "Неверный выбор! Попробуйте снова.\n";
    }
}

void displayMenu() {
    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Создать UniquePtr\n";
        std::cout << "2. Создать SharedPtr\n";
        std::cout << "3. Создать дубликат SharedPtr\n";
        std::cout << "4. Удалить UniquePtr\n";
        std::cout << "5. Удалить SharedPtr\n";
        std::cout << "6. Показать все указатели\n";
        std::cout << "7. Тесты\n";
        std::cout << "8. Тест подтипизации\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";
        choice = getIntInput();

        switch (choice) {
            case 1:
                createUniquePtr();
                break;
            case 2:
                createSharedPtr();
                break;
            case 3:
                createDuplicateSharedPtr();
                break;
            case 4:
                deleteUniquePtr();
                break;
            case 5:
                deleteSharedPtr();
                break;
            case 6:
                displayPointers();
                break;
            case 7:
                int value_for_test;
                std::cout << "Выберите Тесты\n";
                std::cout << "1. Функциональное тестирование\n";
                std::cout << "2. Нагрузочное тестирование\n";
                std::cout << "0. Выход\n";
                std::cout << "Ваш выбор:\n";
                value_for_test = getIntInput();
                
                switch(value_for_test) {
                    case 1: 
                        functionalTest();
                        break;
                    case 2:
                        runLoadTests();
                        break;
                    case 0:
                        break;
                    default:
                        std::cout << "Неверный выбор. Попробуйте снова\n";
                }
                break;
            case 8:
                testSubtypingConsole();
                break;
            case 0:
                std::cout << "Выход из программы.\n";
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0);
}
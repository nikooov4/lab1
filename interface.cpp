#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>    // Для использования std::unique_ptr и std::shared_ptr 
#include <limits>  // для std::numeric_limits

#include "tests.hpp"
#include "interface.hpp"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"

//Для хранения указателей с именами
std::unordered_map<std::string, UniquePtr<int>> uniquePointersInt;
std::unordered_map<std::string, UniquePtr<std::string>> uniquePointersString;

std::unordered_map<std::string, SharedPtr<int>> sharedPointersInt;
std::unordered_map<std::string, SharedPtr<std::string>> sharedPointersString;


template<typename T>
T getInput() {
    static_assert(std::is_same<T, int>::value || std::is_same<T, float>::value,
                  "This function only supports int or float types.");
    
    T value;
    while (true) {
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear(); // Сбросить флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистить входной буфер
            std::cerr << "Ошибка: ожидался ввод числа (int или float) Попробуйте еще раз\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Убедиться, что буфер пуст
            return value;
        }
    }
}

void createUniquePtr() {
    std::string name;
    std::cout << "Введите имя для UniquePtr: ";
    std::cin >> name;

    std::cout << "Выберите тип данных для UniquePtr (1 - число, 2 - строка, 3 - на основе существующего UniquePtr): ";
    int choice = getInput<int>();

    if (choice == 1) {
        std::cout << "Введите значение для UniquePtr: ";
        int value = getInput<int>();
        uniquePointersInt[name] = UniquePtr<int>(new int(value));
        std::cout << "UniquePtr с именем " << name << " для числа создан\n";
    } else if (choice == 2) {
        std::string strValue;
        std::cout << "Введите строку для UniquePtr: ";
        std::getline(std::cin, strValue);
        uniquePointersString[name] = UniquePtr<std::string>(new std::string(strValue));
        std::cout << "UniquePtr с именем " << name << " для строки создан\n";
        std::cout << "Создан UniquePtr для строки: " << *uniquePointersString[name] << "\n";
    } else if (choice == 3) {
        std::cout << "Выберите тип существующего UniquePtr (1 - число, 2 - строка): ";
        int subChoice = getInput<int>();

        if (subChoice == 1) {
            std::cout << "Введите имя существующего UniquePtr<int>: ";
            std::string existingName;
            std::cin >> existingName;

            if (uniquePointersInt.find(existingName) != uniquePointersInt.end()) {
                uniquePointersInt[name] = std::move(uniquePointersInt[existingName]);
                std::cout << "UniquePtr с именем " << name << " создан на основе " << existingName << ".\n";
            } else {
                std::cout << "UniquePtr с таким именем не найден!\n";
            }
        } else if (subChoice == 2) {
            std::cout << "Введите имя существующего UniquePtr<string>: ";
            std::string existingName;
            std::cin >> existingName;

            if (uniquePointersString.find(existingName) != uniquePointersString.end()) {
                uniquePointersString[name] = std::move(uniquePointersString[existingName]);
                std::cout << "UniquePtr с именем " << name << " создан на основе " << existingName << ".\n";
            } else {
                std::cout << "UniquePtr с таким именем не найден!\n";
            }
        } else {
            std::cout << "Неверный выбор!\n";
        }
    } else {
        std::cout << "Неверный выбор!\n";
    }
}

void createSharedPtr() {
    std::string name;
    std::cout << "Введите имя для SharedPtr: ";
    std::cin >> name;

    std::cout << "Выберите тип данных для SharedPtr (1 - число, 2 - строка, 3 - на основе существующего SharedPtr): ";
    int choice = getInput<int>();

    if (choice == 1) {
        std::cout << "Введите значение для SharedPtr: ";
        int value = getInput<int>();
        sharedPointersInt[name] = SharedPtr<int>(new int(value));
        std::cout << "SharedPtr с именем " << name << " для числа создан\n";
    } else if (choice == 2) {
        std::string strValue;
        std::cout << "Введите строку для SharedPtr: ";
        std::getline(std::cin, strValue);
        sharedPointersString[name] = SharedPtr<std::string>(new std::string(strValue));
        std::cout << "SharedPtr с именем " << name << " для строки создан\n";
    } else if (choice == 3) {
        std::cout << "Выберите тип существующего SharedPtr (1 - число, 2 - строка): ";
        int subChoice = getInput<int>();

        if (subChoice == 1) {
            std::cout << "Введите имя существующего SharedPtr<int>: ";
            std::string existingName;
            std::cin >> existingName;

            if (sharedPointersInt.find(existingName) != sharedPointersInt.end()) {
                sharedPointersInt[name] = sharedPointersInt[existingName];
                std::cout << "SharedPtr с именем " << name << " создан на основе " << existingName << ".\n";
            } else {
                std::cout << "SharedPtr с таким именем не найден!\n";
            }
        } else if (subChoice == 2) {
            std::cout << "Введите имя существующего SharedPtr<string>: ";
            std::string existingName;
            std::cin >> existingName;

            if (sharedPointersString.find(existingName) != sharedPointersString.end()) {
                sharedPointersString[name] = sharedPointersString[existingName];
                std::cout << "SharedPtr с именем " << name << " создан на основе " << existingName << ".\n";
            } else {
                std::cout << "SharedPtr с таким именем не найден!\n";
            }
        } else {
            std::cout << "Неверный выбор!\n";
        }
    } else {
        std::cout << "Неверный выбор!\n";
    }
}

// Удаление UniquePtr
void deleteUniquePtr() {
    std::string name;
    std::cout << "Введите имя UniquePtr для удаления: ";
    std::cin >> name;

    // Удаление числового указателя
    if (uniquePointersInt.erase(name)) {
        std::cout << "UniquePtr для числа с именем " << name << " удален\n";
    } 
    // Удаление строкового указателя
    else if (uniquePointersString.erase(name)) {
        std::cout << "UniquePtr для строки с именем " << name << " удален\n";
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
        std::cout << "SharedPtr для числа с именем " << name << " удален\n";
    } 
    // Удаление строкового указателя
    else if (sharedPointersString.erase(name)) {
        std::cout << "SharedPtr для строки с именем " << name << " удален\n";
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
            std::cout << "Имя: " << name << ", Значение: " << *ptr << ", Счетчик ссылок: " << ptr.useCount() << "\n";
        } else {
            std::cout << "Имя: " << name << ", Указатель освобожден\n";
        }
    }

    std::cout << "\nСписок SharedPtr для строк:\n";
    for (const auto& [name, ptr] : sharedPointersString) {
        if (ptr) {
            std::cout << "Имя: " << name << ", Значение: " << *ptr << ", Счетчик ссылок: " << ptr.useCount() << "\n";
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
    int choice = getInput<int>();

    if (choice == 1) {
        std::cout << "Введите значение для SharedPtr<int>: ";
        int value = getInput<int>();

        SharedPtr<int> intPtr(new int(value));
        SharedPtr<float> floatPtr = intPtr;  
        
        std::cout << "SharedPtr<int> успешно присвоен SharedPtr<float>.\n";
        std::cout << "Значение floatPtr: " << *floatPtr << "\n";
        std::cout << "Счетчик ссылок floatPtr: " << floatPtr.useCount() << "\n";
    } 
    else if (choice == 2) {
        std::cout << "Введите значение для SharedPtr<float>: ";
        float value = getInput<float>();

        SharedPtr<float> floatPtr(new float(value));
        SharedPtr<int> intPtr = floatPtr;  
        
        std::cout << "SharedPtr<float> успешно присвоен SharedPtr<int>.\n";
        std::cout << "Значение intPtr: " << *intPtr << "\n";
        std::cout << "Счетчик ссылок intPtr: " << intPtr.useCount() << "\n";
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
        std::cout << "3. Удалить UniquePtr\n";
        std::cout << "4. Удалить SharedPtr\n";
        std::cout << "5. Показать все указатели\n";
        std::cout << "6. Тесты\n";
        std::cout << "7. Тест подтипизации\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";
        choice = getInput<int>();

        switch (choice) {
            case 1:
                createUniquePtr();
                break;
            case 2:
                createSharedPtr();
                break;
            case 3:
                deleteUniquePtr();
                break;
            case 4:
                deleteSharedPtr();
                break;
            case 5:
                displayPointers();
                break;
            case 6:
                int value_for_test;
                std::cout << "Выберите Тесты\n";
                std::cout << "1. Функциональное тестирование\n";
                std::cout << "2. Нагрузочное тестирование\n";
                std::cout << "0. Выход\n";
                std::cout << "Ваш выбор:\n";
                value_for_test = getInput<int>();
                
                switch(value_for_test) {
                    case 1: 
                        functionalTest();
                        break;
                    case 2:
                        runLoadTestsAndPlot();
                        break;
                    case 0:
                        break;
                    default:
                        std::cout << "Неверный выбор. Попробуйте снова\n";
                }
                break;
            case 7:
                testSubtypingConsole();
                break;
            case 0:
                std::cout << "Выход из программы\n";
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0);
}
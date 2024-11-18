#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <type_traits>  // Для std::enable_if и std::is_base_of

template<typename T>
class UniquePtr {
private:
    T* pointer;

public:
    UniquePtr() : pointer(nullptr) {}

    explicit UniquePtr(T* p) : pointer(p) {}

    // Запрет копирования
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator = (const UniquePtr&) = delete;

    // Конструктор перемещения
    UniquePtr(UniquePtr&& other) noexcept : pointer(other.pointer) {
        other.pointer = nullptr;
    }

    // Оператор присваивания перемещением
    UniquePtr& operator = (UniquePtr&& other) noexcept {
        if (this != &other) {
            delete pointer;
            pointer = other.pointer;
            other.pointer = nullptr;
        }
        return *this;
    }

    // Конструктор перемещения для наследуемых типов
    template <typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
    UniquePtr(UniquePtr<U>&& other) noexcept : pointer(static_cast<T*>(other.release())) {}

    // Оператор присваивания для наследуемых типов
    template <typename U, typename = std::enable_if_t<std::is_convertible<U*, T*>::value>>
    UniquePtr& operator=(UniquePtr<U>&& other) noexcept {
        if (reinterpret_cast<void*>(this) != reinterpret_cast<void*>(&other)) {
            delete pointer;
            pointer = static_cast<T*>(other.release());
        }
        return *this;
    }

    T& operator*() {
        return *pointer;
    }

    const T& operator*() const {
        return *pointer;
    }

    T* operator->() {
        return pointer;
    }

    const T* operator->() const {
        return pointer;
    }

    T* get() const {
        return pointer;
    }

    T* release() {
        T* old_ptr = pointer;
        pointer = nullptr;
        return old_ptr;
    }

    void reset(T* new_ptr = nullptr) {
        delete pointer;
        pointer = new_ptr;
    }

    explicit operator bool() const {
        return pointer != nullptr;
    }

    ~UniquePtr() {
        delete pointer;
    }
};

// Для работы с массивами
template<typename T>
class UniquePtr<T[]> {
private:
    T* pointer;

public:
    UniquePtr() : pointer(nullptr) {}

    explicit UniquePtr(T* p) : pointer(p) {}

    // Запрещаем копирование
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Конструктор перемещения
    UniquePtr(UniquePtr&& other) noexcept : pointer(other.pointer) {
        other.pointer = nullptr;
    }

    // Оператор присваивания перемещением
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete[] pointer; 
            pointer = other.pointer;
            other.pointer = nullptr;
        }
        return *this;
    }

    // Доступ к элементам массива
    T& operator[](std::size_t index) {
        return pointer[index];
    }

    T* get() const {
        return pointer;
    }

    T* release() {
        T* old_ptr = pointer;
        pointer = nullptr;
        return old_ptr;
    }

    void reset(T* new_ptr = nullptr) {
        delete[] pointer;  
        pointer = new_ptr;
    }

    explicit operator bool() const {
        return pointer != nullptr;
    }

    ~UniquePtr() {
        delete[] pointer;  
    }
};

#endif

#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <type_traits>  // Для std::enable_if и std::is_arithmetic

template<typename T>
class SharedPtr {
private:
    T* ptr;
    int* ref_count;
    
    void release() {
        if (ref_count) { 
            if (--(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
        }
    }

public:
    template <typename U>
    friend class SharedPtr;

    // Конструктор по умолчанию
    SharedPtr() : ptr(nullptr), ref_count(new int(1)) {}

    // Инициализация с указателем на объект
    explicit SharedPtr(T* p) : ptr(p), ref_count(new int(1)) {}

    // Конструктор копирования
    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr), ref_count(other.ref_count) {
        if (ref_count) ++(*ref_count);
    }

    // Конструктор копирования для числовых типов и наследуемых классов
    template <typename U>
    SharedPtr(const SharedPtr<U>& other,
              typename std::enable_if<
                  std::is_convertible<U*, T*>::value ||
                  (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value), int>::type* = 0)
        : ptr(nullptr), ref_count(other.ref_count) {
        if (ref_count) {
            ++(*ref_count);
            if constexpr (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value) {
                ptr = new T(static_cast<T>(*other.get())); // Преобразование значений
            } else {
                ptr = static_cast<T*>(other.get());
            }
        }
    }

    // Оператор присваивания
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }

    // Оператор присваивания для числовых типов и наследуемых классов
    template <typename U>
    SharedPtr<T>& operator=(const SharedPtr<U>& other) {
        static_assert(std::is_convertible<U*, T*>::value ||
                      (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value),
                      "Поддерживаются только числовые типы или связанные типы");

        if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) {
            release();
            ref_count = other.ref_count;
            if (ref_count) {
                ++(*ref_count);
                if constexpr (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value) {
                    ptr = new T(static_cast<T>(*other.get()));
                } else {
                    ptr = static_cast<T*>(other.get());
                }
            }
        }
        return *this;
    }

    // Конструктор перемещения
    SharedPtr(SharedPtr&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count) {
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

    // Оператор присваивания перемещением
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    // Получить количество ссылок
    int useCount() const {
        return ref_count ? *ref_count : 0;
    }

    T* get() const {
        return ptr;
    }

    const T& operator*() const {
        return *ptr;
    }

    const T* operator->() const {
        return ptr;
    }

    // Сбросить указатель
    void reset(T* newPtr = nullptr) {
        release();
        ptr = newPtr;
        ref_count = new int(1);
    }

    // Приведение к bool для проверки наличия объекта
    explicit operator bool() const {
        return ptr != nullptr;
    }

    // Деструктор
    ~SharedPtr() {
        release();
    }
};

#endif

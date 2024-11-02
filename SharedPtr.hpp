#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <type_traits>  // Для std::enable_if и std::is_base_of

template<typename T>
class SharedPtr {
private:
    T* ptr;
    int* ref_count;

    void release() {
        if (ref_count) {
            *ref_count -= 1;

            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
        }
    }

public:
    SharedPtr() : ptr(nullptr), ref_count(nullptr) {}

    explicit SharedPtr(T* p) : ptr(p), ref_count(new int(1)) {}

    SharedPtr(const SharedPtr& other) {
        ptr = other.ptr;
        ref_count = other.ref_count;

        if (ref_count) {
            (*ref_count)++;
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();

            ptr = other.ptr;
            ref_count = other.ref_count;

            if (ref_count) {
                (*ref_count)++;
            }
        }

        return *this;
    }

    // Конструктор копирования для числовых типов и наследуемых классов
    template <typename U>
    SharedPtr(const SharedPtr<U>& other,
              typename std::enable_if<
                  std::is_convertible<U*, T*>::value ||
                  (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value), int>::type* = 0)
        : ptr(nullptr), ref_count(other.getRefCount()) {
        if (ref_count) {
            ++(*ref_count);
        }
        if constexpr (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value) {
            ptr = new T(static_cast<T>(*other.get())); // Преобразуем значение, если типы числовые
        } else {
            ptr = static_cast<T*>(other.get()); // Подтипизация
        }
    }

    // Оператор присваивания для числовых типов и наследуемых классов
    template <typename U>
    SharedPtr<T>& operator = (const SharedPtr<U>& other) {
        static_assert(std::is_convertible<U*, T*>::value ||
                      (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value),
                      "Поддерживаются только числовые типы или связанные типы");

        if (reinterpret_cast<void*>(this) != reinterpret_cast<const void*>(&other)) {
            release();
            ref_count = other.getRefCount();
            if (ref_count) {
                ++(*ref_count);
            }
            if constexpr (std::is_arithmetic<U>::value && std::is_arithmetic<T>::value) {
                ptr = new T(static_cast<T>(*other.get())); // Преобразуем значение, если типы числовые
            } else {
                ptr = static_cast<T*>(other.get()); // Подтипизация
            }
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept {
        ptr = other.ptr;
        ref_count = other.ref_count;
        other.ptr = nullptr;
        other.ref_count = nullptr;
    }

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

    explicit operator bool() const {
        return ptr != nullptr;  
    }

    int* getRefCount() const { 
        return ref_count; 
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

    void reset(T* newPtr = nullptr) {
        release(); 
        ptr = newPtr;
        if (newPtr) {
            ref_count = new int(1);  // Новый счетчик для нового объекта
        } else {
            ref_count = nullptr;     
        }
    }

    ~SharedPtr() {
        release();
    }

    int use_count() const {
        return ref_count ? *ref_count : 0;
    }
};

#endif
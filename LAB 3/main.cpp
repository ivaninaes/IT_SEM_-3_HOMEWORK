#include <iostream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <cstring>

class VectorException : public std::exception {
public:
    explicit VectorException(const char* message) : msg_(message) {}
    virtual const char* what() const noexcept override {
        return msg_;
    }
private:
    const char* msg_;
};

template<typename T>
class subvector {
public:
    T* mas;
    unsigned int top;
    unsigned int capacity;

public:
    subvector() : mas(nullptr), top(0), capacity(0) {}

    ~subvector() {
        delete[] mas;
    }

    subvector(const subvector& other) : top(other.top), capacity(other.capacity) {
        mas = new T[capacity];
        for (unsigned int i = 0; i < top; i++) {
            mas[i] = other.mas[i];
        }
    }

    subvector& operator=(const subvector& other) {
        if (this != &other) {
            subvector temp(other); // Используем временный объект
            swap(temp); // Обмениваемся данными
        }
        return *this;
    }

    subvector(subvector&& other) noexcept : mas(other.mas), top(other.top), capacity(other.capacity) {
        other.mas = nullptr;
        other.top = 0;
        other.capacity = 0;
    }

    subvector& operator=(subvector&& other) noexcept {
        if (this != &other) {
            delete[] mas;
            mas = other.mas;
            top = other.top;
            capacity = other.capacity;
            other.mas = nullptr;
            other.top = 0;
            other.capacity = 0;
        }
        return *this;
    }

    bool push_back(const T& d) {
        if (top >= capacity) {
            unsigned int new_capacity = (capacity == 0) ? 1 : capacity * 2;
            T* new_mas = nullptr;

            try {
                new_mas = new T[new_capacity];
            } catch (const std::bad_alloc&) {
                throw VectorException("Memory allocation failed in push_back.");
            }

            try {
                safe_copy(mas, new_mas, top);
            } catch (...) {
                delete[] new_mas;
                throw;
            }

            delete[] mas;
            mas = new_mas;
            capacity = new_capacity;
        }

        mas[top] = d;
        top++;
        return true;
    }

    T pop_back() {
        if (top == 0) {
            throw VectorException("Cannot pop from an empty subvector.");
        }
        top--;
        return mas[top];
    }

    bool resize(unsigned int new_capacity) {
        if (new_capacity < top) {
            throw VectorException("New capacity cannot be less than the current size.");
        }

        T* new_mas = nullptr;

        try {
            new_mas = new T[new_capacity];
        } catch (const std::bad_alloc&) {
            throw VectorException("Memory allocation failed in resize.");
        }

        try {
            safe_copy(mas, new_mas, top);
        } catch (...) {
            delete[] new_mas;
            throw;
        }

        delete[] mas;
        mas = new_mas;
        capacity = new_capacity;

        return true;
    }

    void shrink_to_fit() {
        if (capacity > top) {
            T* new_mas = new T[top];
            try {
                safe_copy(mas, new_mas, top);
            } catch (...) {
                delete[] new_mas;
                throw;
            }
            delete[] mas;
            mas = new_mas;
            capacity = top;
        }
    }

    void clear() {
        top = 0;
    }

private:
    void swap(subvector& other) noexcept {
        std::swap(mas, other.mas);
        std::swap(top, other.top);
        std::swap(capacity, other.capacity);
    }

    void safe_copy(T* src, T* dest, unsigned int count) {
        for (unsigned int i = 0; i < count; ++i) {
            dest[i] = src[i];
        }
    }
};

// Пример использования и демонстрация работы с исключениями
int main() {
    try {
        subvector<int> sv;
        sv.push_back(1);
        sv.push_back(2);
        sv.push_back(3);
        std::cout << "Elements added successfully." << std::endl;

        sv.pop_back();
        std::cout << "Element popped successfully." << std::endl;

        sv.resize(10);
        std::cout << "Resized successfully." << std::endl;

        sv.shrink_to_fit();
        std::cout << "Shrink to fit successfully." << std::endl;

        sv.clear();
        std::cout << "Cleared successfully." << std::endl;

        // Негативные сценарии
        try {
            sv.pop_back();
        } catch (const VectorException& e) {
            std::cerr << "Expected exception: " << e.what() << std::endl;
        }
        sv.push_back(1);
        sv.push_back(2);
        sv.push_back(3);
        try {
            sv.resize(2); // Попытка уменьшить емкость меньше текущего размера
        } catch (const VectorException& e) {
            std::cerr << "Expected exception: " << e.what() << std::endl;
        }

        try {
            subvector<int> sv2;
            for (int i = 0; i < 1000000000; ++i) {
                sv2.push_back(i); // Попытка вызвать исключение при выделении памяти
            }
        } catch (const VectorException& e) {
            std::cerr << "Expected exception: " << e.what() << std::endl;
        }

    } catch (const VectorException& e) {
        std::cerr << "VectorException: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
    }

    return 0;
}
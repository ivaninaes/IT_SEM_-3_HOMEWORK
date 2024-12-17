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
            delete[] mas;
            top = other.top;
            capacity = other.capacity;
            mas = new T[capacity];
            for (unsigned int i = 0; i < top; i++) {
                mas[i] = other.mas[i];
            }
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

            for (unsigned int i = 0; i < top; i++) {
                new_mas[i] = mas[i];
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

        for (unsigned int i = 0; i < top; i++) {
            new_mas[i] = mas[i];
        }

        delete[] mas;
        mas = new_mas;
        capacity = new_capacity;

        if (new_capacity < top) {
            top = new_capacity;
        }

        return true;
    }

    void shrink_to_fit() {
        if (capacity > top) {
            T* new_mas = new T[top];
            for (unsigned int i = 0; i < top; i++) {
                new_mas[i] = mas[i];
            }
            delete[] mas;
            mas = new_mas;
            capacity = top;
        }
    }

    void clear() {
        top = 0;
    }
};

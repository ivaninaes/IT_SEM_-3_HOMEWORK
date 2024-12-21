#ifndef SUBVECTOR_IMPL_H
#define SUBVECTOR_IMPL_H

#include "subvector.h"

template<typename T>
subvector<T>::subvector() : mas(nullptr), top(0), capacity(0) {}

template<typename T>
subvector<T>::subvector(const subvector& other) : top(other.top), capacity(other.capacity) {
    mas = new T[capacity];
    std::copy(other.mas, other.mas + top, mas);
}

template<typename T>
subvector<T>& subvector<T>::operator=(const subvector& other) {
    if (this != &other) {
        delete[] mas;
        top = other.top;
        capacity = other.capacity;
        mas = new T[capacity];
        std::copy(other.mas, other.mas + top, mas);
    }
    return *this;
}

template<typename T>
subvector<T>::~subvector() {
    delete[] mas;
}

template<typename T>
bool subvector<T>::push_back(T d) {
    if (top + 1 >= capacity) {
        unsigned int new_capacity = (capacity == 0) ? 1 : capacity * 2;
        T *new_mas = new T[new_capacity];
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

template<typename T>
T subvector<T>::pop_back() {
    if (top == 0) {
        return T();
    }
    top--;
    return mas[top];
}

template<typename T>
bool subvector<T>::resize(unsigned int new_capacity) {
    if (new_capacity == 0) {
        delete[] mas;
        mas = nullptr;
        capacity = 0;
        top = 0;
    } else {
        T *new_mas = new T[new_capacity];
        unsigned int new_top = std::min(top, new_capacity);
        for (unsigned int i = 0; i < new_top; i++) {
            new_mas[i] = mas[i];
        }

        delete[] mas;

        mas = new_mas;
        capacity = new_capacity;
        top = new_top;
    }
    return true;
}

template<typename T>
void subvector<T>::shrink_to_fit() {
    if (capacity > top) {
        T *new_mas = new T[top];

        for (unsigned int i = 0; i < top; i++) {
            new_mas[i] = mas[i];
        }

        delete[] mas;

        mas = new_mas;
        capacity = top;
    }
}

template<typename T>
void subvector<T>::clear() {
    top = 0;
}

template<typename T>
T& subvector<T>::operator[](unsigned int index) {
    return mas[index];
}

template<typename T>
const T& subvector<T>::operator[](unsigned int index) const {
    return mas[index];
}

template<typename T>
unsigned int subvector<T>::size() const {
    return top;
}

#endif // SUBVECTOR_IMPL_H

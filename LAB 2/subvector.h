#ifndef SUBVECTOR_H
#define SUBVECTOR_H

#include <algorithm>

template<typename T>
class subvector {
private:
    T *mas;
    unsigned int top;
    unsigned int capacity;

public:
    subvector();
    subvector(const subvector& other);
    subvector& operator=(const subvector& other);
    ~subvector();

    bool push_back(T d);
    T pop_back();
    bool resize(unsigned int new_capacity);
    void shrink_to_fit();
    void clear();

    T& operator[](unsigned int index);
    const T& operator[](unsigned int index) const;

    unsigned int size() const;
};

#include "subvector_impl.h"  // Включаем реализацию шаблонного класса

#endif // SUBVECTOR_H

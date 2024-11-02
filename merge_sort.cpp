#include <iostream>

using std::cout;

void my_sort(int n, int *array) {
    // База рекурсии
    if (n == 1) {
        return;
    }
    // Разделяем
    int a[n / 2];
    int b[n - n/2];
    for (int i = 0; i < n / 2; i++) {
        a[i] = array[i];
    } 
    for (int i = n / 2; i < n; i++) {
        b[i - n/2] = array[i];
    } 
    my_sort(n / 2, a); // Тут мы гарантируем что а и б отсортированы
    my_sort(n-(n/2), b);
    // Сливаем идя двумя указателями по масиву а и б
    int *a_p = a;
    int *b_p = b;
    for (int i = 0; i < n; i++) {
        if (a_p - a == n / 2) { // Если уже набрали из б все элементы то осталось только ф
            array[i] = *b_p;
            b_p++;
        } else if (b_p - b == n - n / 2) { // и наоборот
            array[i] = *a_p;
            a_p++;
        } else {
            if (*a_p < *b_p) { // а тут просто минимум из обоих частей берем и двигаем использованный указатель
                array[i] = *a_p;
                a_p++;
            } else {
                array[i] = *b_p;
                b_p++;
            }
        }
    }
}


// test
int main() {
    int a[] = {2, 5, 2, 1, 4, 2, 7, 9, 6, 36, 0, -1};
    my_sort(12, a);
    for (int i = 0; i < 12; i++) {
        cout << a[i] << ' ';
    }
}
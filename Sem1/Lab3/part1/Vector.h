// Vector.h - объявление класса Vector (вектор размерности n из чисел double)
#pragma once
#include <iostream>

class Vector
{
    double* p = nullptr; // указатель на динамический массив элементов
    int n = 0;           // размерность вектора (число элементов)

public:
    // Поток, куда конструкторы, деструктор и операции присваивания
    // пишут сообщения о своём вызове (по умолчанию консоль)
    static std::ostream* trace;

    Vector();                          // пустой вектор (n = 0)
    Vector(const double* arr, int n);  // вектор на основе обычного массива
    explicit Vector(int n);            // вектор из n нулей
    Vector(const Vector& v);           // конструктор копирования
    Vector(Vector&& v) noexcept;       // конструктор перемещения
    ~Vector();                         // деструктор

    int size() const { return n; }
    const double* data() const { return p; } // адрес массива (для демонстрации)

    double& operator[](int index);             // доступ к элементу с изменением
    const double& operator[](int index) const; // доступ к элементу константного вектора

    Vector& operator=(const Vector& v);     // присваивание с копированием
    Vector& operator=(Vector&& v) noexcept; // присваивание с перемещением

    // Вариант 1: сложение векторов одинаковой размерности
    friend Vector operator+(const Vector& v1, const Vector& v2);

    friend std::ostream& operator<<(std::ostream& os, const Vector& v); // вывод в поток
    friend std::istream& operator>>(std::istream& is, Vector& v);       // ввод из потока
};

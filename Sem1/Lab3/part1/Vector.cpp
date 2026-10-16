// Vector.cpp - реализация методов класса Vector
#include "Vector.h"
#include <stdexcept>

std::ostream* Vector::trace = &std::cout;

Vector::Vector()
{
    *trace << "  [Vector()] empty vector\n";
}

Vector::Vector(const double* arr, int n) : n(n)
{
    if (n < 0) throw std::invalid_argument("negative size");
    p = n ? new double[n] : nullptr;       // выделяем свою память
    for (int i = 0; i < n; i++) p[i] = arr[i]; // копируем элементы массива
    *trace << "  [Vector(const double*, int)] n=" << n << "\n";
}

Vector::Vector(int n) : n(n)
{
    if (n < 0) throw std::invalid_argument("negative size");
    p = n ? new double[n]() : nullptr; // () заполняет массив нулями
    *trace << "  [Vector(int)] n=" << n << "\n";
}

// Копирование: новый объект получает СВОЮ память и копию всех элементов
Vector::Vector(const Vector& v) : n(v.n)
{
    p = n ? new double[n] : nullptr;
    for (int i = 0; i < n; i++) p[i] = v.p[i];
    *trace << "  [Vector(const Vector&)] copy, n=" << n << "\n";
}

// Перемещение: новый объект забирает память у v, v становится пустым.
// Память не выделяется и элементы не копируются.
Vector::Vector(Vector&& v) noexcept : p(v.p), n(v.n)
{
    v.p = nullptr;
    v.n = 0;
    *trace << "  [Vector(Vector&&)] move, n=" << n << "\n";
}

Vector::~Vector()
{
    *trace << "  [~Vector()] n=" << n << "\n";
    delete[] p; // delete[] для nullptr ничего не делает
}

double& Vector::operator[](int index)
{
    if (index < 0 || index >= n) throw std::out_of_range("index out of range");
    return p[index]; // возвращаем ссылку, поэтому v[i] можно менять
}

const double& Vector::operator[](int index) const
{
    if (index < 0 || index >= n) throw std::out_of_range("index out of range");
    return p[index];
}

Vector& Vector::operator=(const Vector& v)
{
    *trace << "  [operator=(const Vector&)] copy assignment\n";
    if (this != &v) // защита от присваивания самому себе (a = a)
    {
        double* q = v.n ? new double[v.n] : nullptr; // сначала новая память,
        for (int i = 0; i < v.n; i++) q[i] = v.p[i];
        delete[] p;                                  // потом освобождаем старую
        p = q;
        n = v.n;
    }
    return *this; // ссылка на себя позволяет писать a = b = c
}

Vector& Vector::operator=(Vector&& v) noexcept
{
    *trace << "  [operator=(Vector&&)] move assignment\n";
    if (this != &v)
    {
        delete[] p;   // освобождаем свою старую память
        p = v.p;      // забираем память у v
        n = v.n;
        v.p = nullptr; // v остаётся пустым, его деструктор ничего не удалит
        v.n = 0;
    }
    return *this;
}

// Вариант 5: (Vector, double*) -> Vector. Результат имеет длину 2n:
// сначала идут элементы вектора v, затем элементы массива arr.
// Длина массива не передаётся, она считается равной длине вектора v.
Vector operator+(const Vector& v, const double* arr)
{
    if (v.n > 0 && arr == nullptr)
        throw std::invalid_argument("array is nullptr");
    Vector r(2 * v.n);
    for (int i = 0; i < v.n; i++)
    {
        r.p[i] = v.p[i];         // первая половина: элементы вектора
        r.p[v.n + i] = arr[i];   // вторая половина: элементы массива
    }
    return r;
}

// Формат вывода: {1, 2, 3}
std::ostream& operator<<(std::ostream& os, const Vector& v)
{
    os << '{';
    for (int i = 0; i < v.n; i++)
        os << (i ? ", " : "") << v.p[i];
    return os << '}';
}

// Формат ввода: размерность, затем элементы: 3  1 2 3
std::istream& operator>>(std::istream& is, Vector& v)
{
    int n;
    if (!(is >> n) || n < 0)
    {
        is.setstate(std::ios::failbit);
        return is;
    }
    double* q = n ? new double[n] : nullptr;
    for (int i = 0; i < n; i++)
        if (!(is >> q[i]))
        {
            delete[] q; // данных не хватило: вектор не меняем
            return is;
        }
    delete[] v.p; // ввод удался: заменяем содержимое вектора
    v.p = q;
    v.n = n;
    return is;
}

// Лабораторная работа №3, часть 2, вариант 1.
// Базовый класс "вектор на плоскости" и производный класс
// "вектор в трёхмерном пространстве". Начало вектора в точке (0, 0),
// поля задают координаты конца вектора.
//
// Чтобы показать два вида полиморфизма, иерархия написана дважды:
//   StaticPoly  - методы НЕ виртуальные (статическое связывание),
//   DynamicPoly - методы виртуальные (динамическое связывание).
// Больше эти две версии ничем не отличаются.
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

// ===================== Статический полиморфизм =====================
namespace StaticPoly
{
class Vector2D
{
protected:
    double x, y; // координаты конца вектора

public:
    Vector2D(double x, double y) : x(x), y(y) {}

    double length() const // длина вектора на плоскости
    {
        return sqrt(x * x + y * y);
    }
    void print() const // печать полей и длины
    {
        cout << "Vector2D (" << x << ", " << y << "), length = " << length() << endl;
    }
};

class Vector3D : public Vector2D
{
protected:
    double z; // дополнительная координата

public:
    Vector3D(double x, double y, double z)
        : Vector2D(x, y), z(z) // сначала вызывается конструктор базового класса
    {
    }

    double length() const // переопределённая функция (без virtual)
    {
        double l = Vector2D::length(); // длина проекции на плоскость XY
        return sqrt(l * l + z * z);
    }
    void print() const // переопределённая функция (без virtual)
    {
        cout << "Vector3D (" << x << ", " << y << ", " << z << "), length = " << length() << endl;
    }
};
} // namespace StaticPoly

// ===================== Динамический полиморфизм =====================
namespace DynamicPoly
{
class Vector2D
{
protected:
    double x, y;

public:
    Vector2D(double x, double y) : x(x), y(y)
    {
        cout << "  [Vector2D(" << x << ", " << y << ") constructor]" << endl;
    }
    // Виртуальный деструктор нужен, чтобы delete через указатель на базовый
    // класс вызывал и деструктор производного класса
    virtual ~Vector2D()
    {
        cout << "  [~Vector2D destructor]" << endl;
    }

    virtual double length() const
    {
        return sqrt(x * x + y * y);
    }
    virtual void print() const
    {
        cout << "Vector2D (" << x << ", " << y << "), length = " << length() << endl;
    }
};

class Vector3D : public Vector2D
{
protected:
    double z;

public:
    Vector3D(double x, double y, double z) : Vector2D(x, y), z(z)
    {
        cout << "  [Vector3D(" << x << ", " << y << ", " << z << ") constructor]" << endl;
    }
    ~Vector3D() override
    {
        cout << "  [~Vector3D destructor]" << endl;
    }

    double length() const override // override: компилятор проверит, что метод
    {                              // действительно переопределяет виртуальный
        double l = Vector2D::length();
        return sqrt(l * l + z * z);
    }
    void print() const override
    {
        cout << "Vector3D (" << x << ", " << y << ", " << z << "), length = " << length() << endl;
    }
};
} // namespace DynamicPoly

int main()
{
    cout << "===== 1. Static polymorphism (methods are NOT virtual) =====" << endl;
    {
        StaticPoly::Vector2D a(3, 4);
        StaticPoly::Vector3D b(1, 2, 2);

        cout << "Calls through objects:" << endl;
        cout << "a.print(): ";   a.print();
        cout << "b.print(): ";   b.print();
        cout << "a.length() = " << a.length() << ", b.length() = " << b.length() << endl;

        StaticPoly::Vector2D* p = &b; // указатель базового типа на объект производного
        cout << "Call through Vector2D* pointing to Vector3D b:" << endl;
        cout << "p->print(): ";  p->print();
        cout << "p->length() = " << p->length()
             << "  <- Vector2D::length() is called, z is ignored" << endl;
    }

    cout << endl << "===== 2. Dynamic polymorphism (methods are virtual) =====" << endl;
    {
        DynamicPoly::Vector2D a(3, 4);
        DynamicPoly::Vector3D b(1, 2, 2);

        cout << "Calls through objects:" << endl;
        cout << "a.print(): ";   a.print();
        cout << "b.print(): ";   b.print();
        cout << "a.length() = " << a.length() << ", b.length() = " << b.length() << endl;

        DynamicPoly::Vector2D* p = &b;
        cout << "Call through Vector2D* pointing to Vector3D b:" << endl;
        cout << "p->print(): ";  p->print();
        cout << "p->length() = " << p->length()
             << "  <- Vector3D::length() is called (type of the object decides)" << endl;
        cout << "End of block, objects are destroyed in reverse order:" << endl;
    }

    cout << endl << "===== 3. Vector of pointers to base and derived objects =====" << endl;
    vector<DynamicPoly::Vector2D*> list;
    list.push_back(new DynamicPoly::Vector2D(3, 4));
    list.push_back(new DynamicPoly::Vector3D(1, 2, 2));
    list.push_back(new DynamicPoly::Vector2D(6, 8));
    list.push_back(new DynamicPoly::Vector3D(2, 3, 6));

    cout << "Data of all objects:" << endl;
    double sum = 0;
    for (size_t i = 0; i < list.size(); i++)
    {
        cout << i + 1 << ") ";
        list[i]->print();          // вызывается print() класса самого объекта
        sum += list[i]->length();  // и length() тоже
    }
    cout << "Sum of lengths = " << sum << endl;

    // Для сравнения: те же векторы в иерархии без virtual. Объекты создаются
    // без new: удалять Vector3D через Vector2D* без виртуального деструктора нельзя
    StaticPoly::Vector2D s1(3, 4), s3(6, 8);
    StaticPoly::Vector3D s2(1, 2, 2), s4(2, 3, 6);
    vector<StaticPoly::Vector2D*> slist = { &s1, &s2, &s3, &s4 };
    double ssum = 0;
    for (StaticPoly::Vector2D* p : slist) ssum += p->length();
    cout << "Same vectors without virtual: sum = " << ssum
         << " (wrong, z coordinates are lost)" << endl;

    cout << endl << "Deleting objects through Vector2D* (virtual destructor):" << endl;
    for (DynamicPoly::Vector2D* p : list) delete p;
    list.clear();
    return 0;
}

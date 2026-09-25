// Лабораторная работа №1, часть 2. Тренировочная задача.
// Почему sizeof(Test) == 12 и как переписать структуру, чтобы она занимала 8 байт.
#include <iostream>
#include <cstddef> // offsetof
using namespace std;

// Исходная структура
struct Test
{
    short b;       // 2 байта (+ 2 байта выравнивания, чтобы int начинался с адреса, кратного 4)
    int a;         // 4 байта
    char ch1, ch2; // 1 + 1 байт (+ 2 байта в конце, чтобы размер был кратен 4)
};

// Переписанная структура: поля упорядочены по убыванию размера
struct Test2
{
    int a;         // 4 байта, смещение 0
    short b;       // 2 байта, смещение 4
    char ch1, ch2; // 1 + 1 байт, смещения 6 и 7
};

int main()
{
    cout << "sizeof(Test)  = " << sizeof(Test) << endl;
    cout << "  offset b = " << offsetof(Test, b)
         << ", a = " << offsetof(Test, a)
         << ", ch1 = " << offsetof(Test, ch1)
         << ", ch2 = " << offsetof(Test, ch2) << endl;

    cout << "sizeof(Test2) = " << sizeof(Test2) << endl;
    cout << "  offset a = " << offsetof(Test2, a)
         << ", b = " << offsetof(Test2, b)
         << ", ch1 = " << offsetof(Test2, ch1)
         << ", ch2 = " << offsetof(Test2, ch2) << endl;
    return 0;
}

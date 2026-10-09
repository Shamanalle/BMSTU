// Лабораторная работа №3, часть 1, вариант 1.
// Класс Vector: конструкторы, деструктор, перегрузка операций
// [], = (копирование), = (перемещение), <<, >> и + (сложение векторов).
// Входные данные читаются из input.txt, результаты пишутся в output.txt.
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <utility>
#include "Vector.h"
using namespace std;

static const char* yesNo(bool b) { return b ? "yes" : "no"; }

int main()
{
    ifstream fin("input.txt");
    if (!fin)
    {
        cerr << "Cannot open input.txt" << endl;
        return 1;
    }
    ofstream fout("output.txt");
    if (!fout)
    {
        cerr << "Cannot create output.txt" << endl;
        return 1;
    }
    Vector::trace = &fout; // сообщения конструкторов тоже идут в output.txt

    { // все векторы живут в этом блоке, деструкторы сработают до конца программы
        fout << "=== 1. Reading from input.txt (operator>>) ===\n";
        Vector a, b, c;
        if (!(fin >> a >> b >> c))
        {
            fout << "Error: wrong format of input.txt\n";
            cerr << "Wrong format of input.txt" << endl;
            return 1;
        }
        fout << "a = " << a << "  n=" << a.size() << "\n";
        fout << "b = " << b << "  n=" << b.size() << "\n";
        fout << "c = " << c << "  n=" << c.size() << "\n";

        fout << "\n=== 2. operator[] ===\n";
        fout << "a[0] = " << a[0] << ", a[3] = " << a[3] << "\n";
        try
        {
            double x = a[10]; // индекс вне диапазона: operator[] бросит исключение
            fout << "a[10] = " << x << "\n";
        }
        catch (const out_of_range& e)
        {
            fout << "a[10]: exception \"" << e.what() << "\"\n";
        }

        fout << "\n=== 3. Copy constructor: Vector d = a; ===\n";
        Vector d = a;
        d[0] = 100; // меняем только копию
        fout << "after d[0] = 100:\n";
        fout << "a = " << a << "\n";
        fout << "d = " << d << "\n";
        fout << "a and d use different memory: " << yesNo(a.data() != d.data()) << "\n";

        fout << "\n=== 4. Move constructor: Vector e = std::move(d); ===\n";
        const double* old = d.data();
        Vector e = std::move(d);
        fout << "d = " << d << "  n=" << d.size() << "  (moved-from, empty)\n";
        fout << "e = " << e << "\n";
        fout << "e took memory of d without copying: " << yesNo(e.data() == old) << "\n";

        fout << "\n=== 5. Copy assignment: f = b; ===\n";
        Vector f;
        f = b;
        f[1] = -20; // меняем только f
        fout << "after f[1] = -20:\n";
        fout << "b = " << b << "\n";
        fout << "f = " << f << "\n";
        fout << "b and f use different memory: " << yesNo(b.data() != f.data()) << "\n";

        fout << "\n=== 6. Move assignment: g = std::move(f); ===\n";
        Vector g;
        old = f.data();
        g = std::move(f);
        fout << "f = " << f << "  n=" << f.size() << "  (moved-from, empty)\n";
        fout << "g = " << g << "\n";
        fout << "g took memory of f without copying: " << yesNo(g.data() == old) << "\n";

        fout << "\n=== 7. Variant 1: operator+ (Vector + Vector -> Vector) ===\n";
        Vector s = a + b;
        fout << "s = a + b = " << a << " + " << b << "\n";
        fout << "s = " << s << "\n";

        fout << "\ns = b + b; (the result is a temporary, so move assignment is chosen)\n";
        s = b + b;
        fout << "s = " << s << "\n";

        fout << "\na + c (sizes " << a.size() << " and " << c.size() << "):\n";
        try
        {
            Vector bad = a + c;
            fout << "bad = " << bad << "\n";
        }
        catch (const invalid_argument& ex)
        {
            fout << "exception \"" << ex.what() << "\"\n";
        }

        fout << "\n=== 8. End of block: destructors ===\n";
    }
    fout << "Done.\n";
    cout << "Results are written to output.txt" << endl;
    return 0;
}

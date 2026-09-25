// Лабораторная работа №1, часть 2. Вариант 5.
// Структура «студент»: ФИО, массив элементов структуры «дисциплина» (название, оценка).
// Задача: определить, сколько студентов имеют неудовлетворительную оценку
// по заданному предмету.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int MIN_DISC = 4; // Минимальное число дисциплин в сессии

struct Discipline // Структура «дисциплина»
{
    string name; // Название дисциплины
    int mark;    // Оценка (2..5)
};

struct Student // Структура «студент»
{
    string fio;              // ФИО
    vector<Discipline> disc; // Результаты сдачи сессии
};

// Ввод одного студента с клавиатуры
void inputStudent(Student& s)
{
    cout << "FIO: ";
    getline(cin, s.fio);

    int k; // Число дисциплин
    do
    {
        cout << "Number of disciplines (>= " << MIN_DISC << "): ";
        cin >> k;
    } while (k < MIN_DISC);
    cin.ignore(); // После ввода числа в буфере остается символ '\n', его игнорируем

    s.disc.resize(k);
    for (int j = 0; j < k; j++)
    {
        cout << "  Discipline " << j + 1 << " name: ";
        getline(cin, s.disc[j].name);
        do
        {
            cout << "  Mark (2..5): ";
            cin >> s.disc[j].mark;
        } while (s.disc[j].mark < 2 || s.disc[j].mark > 5);
        cin.ignore();
    }
}

// Вывод одного студента на консоль
void printStudent(const Student& s)
{
    cout << s.fio << ":";
    for (const Discipline& d : s.disc)
        cout << "  " << d.name << " - " << d.mark << ";";
    cout << endl;
}

// Сколько студентов получили 2 по дисциплине subject
int countFailed(const vector<Student>& group, const string& subject)
{
    int count = 0;
    for (const Student& s : group)
        for (const Discipline& d : s.disc)
            if (d.name == subject && d.mark == 2)
            {
                count++;
                break; // Студент уже учтен, дальше его дисциплины не смотрим
            }
    return count;
}

int main()
{
    int n; // Число студентов в группе
    cout << "n = ";
    cin >> n;
    cin.ignore();

    vector<Student> group(n); // Массив студентов, размер известен только во время работы
    for (int i = 0; i < n; i++)
    {
        cout << endl << "Student N=" << i + 1 << endl;
        inputStudent(group[i]);
    }

    cout << endl << "Entered data:" << endl; // Вывод введенных данных для контроля ввода
    for (int i = 0; i < n; i++)
    {
        cout << i + 1 << ". ";
        printStudent(group[i]);
    }

    string subject;
    cout << endl << "Subject: ";
    getline(cin, subject);

    int k = countFailed(group, subject);
    if (k)
        cout << "Students with mark 2 in \"" << subject << "\": " << k << endl;
    else
        cout << "No students with mark 2 in \"" << subject << "\"" << endl;
    return 0;
}

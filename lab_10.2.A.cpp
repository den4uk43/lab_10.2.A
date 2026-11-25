#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
#include <Windows.h>
using namespace std;

enum Specialnist { KOMP_NAUKY, INFORMATYKA, MATEM_EKON, FIZ_INF, TRUD_NAVCH };

struct Student
{
    string      prizv;
    int         kurs;
    Specialnist spec;
    int         fizyka;
    int         matematyka;
    int         informatyka;
};

int InputInt(const string prompt, const int lo, const int hi);
double InputDouble(const string prompt, const double lo, const double hi);
string InputPrizv();
int Sum(const Student& s);
double Avg(const Student& s);
void Create(Student* p, const int N);
void Print(Student* p, const int N, const string* specStr);
void Sort(Student* p, const int N);
int BinSearch(Student* p, const int N, const string prizv, const int sumKey, const int matem);
int* IndexSort(Student* p, const int N);
void PrintIndexSorted(Student* p, int* I, const int N, const string* specStr);

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    string specStr[] = { "Комп'ютерні науки", "Інформатика", "Математика та економіка", "Фізика та інформатика", "Трудове навчання" };

    int N = InputInt("Введіть кількість студентів N: ", 1, 1000);

    Student* p = new Student[N];

    double avg;
    int matem;
    string prizv;
    int found;

    int menuItem;
    do {
        cout << endl << endl << endl;
        cout << "Виберіть дію:" << endl << endl;
        cout << " [1] - введення даних з клавіатури" << endl;
        cout << " [2] - вивід даних на екран" << endl;
        cout << " [3] - фізичне впорядкування даних" << endl;
        cout << " [4] - бінарний пошук студента за прізвищем," << endl;
        cout << "        середнім балом та оцінкою з математики" << endl;
        cout << " [5] - індексне впорядкування та вивід даних"
            << endl << endl;
        cout << " [0] - вихід та завершення роботи програми" << endl << endl;
        cout << "Введіть значення: ";
        if (!(cin >> menuItem))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            menuItem = -1;
        }
        cout << endl << endl << endl;

        switch (menuItem)
        {
        case 1:
            Create(p, N);
            break;
        case 2:
            Print(p, N, specStr);
            break;
        case 3:
            Sort(p, N);
            break;
        case 4:
            cout << "Введіть ключі пошуку (масив має бути впорядкований за п. 3):" << endl;
            avg = InputDouble("     середній бал (1-5): ", 1, 5);
            matem = InputInt("     оцінка з математики (1-5): ", 1, 5);
            cin.get();
            cin.sync();

            cout << "     прізвище: "; prizv = InputPrizv();
            cout << endl;
            // середній бал = сума / 3, тому порівнюємо цілі суми (без похибок double)
            if ((found = BinSearch(p, N, prizv, (int)floor(avg * 3 + 0.5), matem)) != -1)
                cout << "Студент навчається, позиція в списку: " << found + 1 << endl;
            else
                cout << "Шуканого студента не знайдено" << endl;
            break;
        case 5:
        {
            int* I = IndexSort(p, N);
            PrintIndexSorted(p, I, N, specStr);
            delete[] I;
            break;
        }
        case 0:
            break;
        default:
            cout << "Ви ввели помилкове значення! "
                "Слід ввести число - номер вибраного пункту меню" << endl;
        }
    } while (menuItem != 0);

    delete[] p;
    return 0;
}

int InputInt(const string prompt, const int lo, const int hi)
{
    int x;
    cout << prompt;
    while (!(cin >> x) || x < lo || x > hi)
    {
        if (cin.eof())
            exit(1);
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Помилка! Слід ввести ціле число від " << lo << " до " << hi << "." << endl;
        cout << prompt;
    }
    return x;
}

double InputDouble(const string prompt, const double lo, const double hi)
{
    double x;
    cout << prompt;
    while (!(cin >> x) || x < lo || x > hi)
    {
        if (cin.eof())
            exit(1);
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Помилка! Слід ввести число від " << lo << " до " << hi << "." << endl;
        cout << prompt;
    }
    return x;
}

string InputPrizv()
{
    string s;
    while (true)
    {
        getline(cin, s);
        if (!s.empty() && s.size() <= 15)
            return s;
        if (cin.eof())
            exit(1);
        cout << "Помилка! Прізвище має бути непорожнім та не довшим за 15 символів." << endl;
        cout << "     прізвище: ";
    }
}

int Sum(const Student& s)    // сума трьох оцінок
{
    return s.fizyka + s.matematyka + s.informatyka;
}

double Avg(const Student& s) // середній бал
{
    return Sum(s) / 3.0;
}

void Create(Student* p, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;

        cin.get();
        cin.sync();

        cout << "     прізвище: "; p[i].prizv = InputPrizv();
        p[i].kurs = InputInt("     курс (1-6): ", 1, 6);
        p[i].spec = (Specialnist)InputInt(
            "     спеціальність (0 - Комп'ютерні науки, 1 - Інформатика,\n"
            "                    2 - Математика та економіка, 3 - Фізика та інформатика,\n"
            "                    4 - Трудове навчання): ", 0, 4);
        p[i].fizyka = InputInt("     оцінка з фізики (1-5): ", 1, 5);
        p[i].matematyka = InputInt("     оцінка з математики (1-5): ", 1, 5);
        p[i].informatyka = InputInt("     оцінка з інформатики (1-5): ", 1, 5);
        cout << endl;
    }
}

void PrintHeader()
{
    cout << "====================================================================================================="
        << endl;
    cout << "| №   | Прізвище        | Курс | Спеціальність           | Фізика | Математика | Інформатика | Сер.бал |"
        << endl;
    cout << "-----------------------------------------------------------------------------------------------------"
        << endl;
}

void PrintRow(int num, const Student& s, const string* specStr)
{
    cout << "| " << setw(3) << right << num << " ";
    cout << "| " << setw(15) << left << s.prizv
        << " | " << setw(4) << right << s.kurs
        << " | " << setw(23) << left << specStr[s.spec]
        << " | " << setw(6) << right << s.fizyka
        << " | " << setw(10) << right << s.matematyka
        << " | " << setw(11) << right << s.informatyka
        << " | " << setw(7) << right << fixed << setprecision(2) << Avg(s) << " |" << endl;
}

void Print(Student* p, const int N, const string* specStr)
{
    cout << "Список студентів" << endl;
    PrintHeader();
    for (int i = 0; i < N; i++)
        PrintRow(i + 1, p[i], specStr);
    cout << "====================================================================================================="
        << endl << endl;
}

// фізичне впорядкування: середній бал -> оцінка з математики -> прізвище (за зростанням)
void Sort(Student* p, const int N)
{
    Student tmp;
    for (int i0 = 0; i0 < N - 1; i0++)
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if ((Sum(p[i1]) > Sum(p[i1 + 1]))
                ||
                (Sum(p[i1]) == Sum(p[i1 + 1]) &&
                    p[i1].matematyka > p[i1 + 1].matematyka)
                ||
                (Sum(p[i1]) == Sum(p[i1 + 1]) &&
                    p[i1].matematyka == p[i1 + 1].matematyka &&
                    p[i1].prizv > p[i1 + 1].prizv))
            {
                tmp = p[i1];
                p[i1] = p[i1 + 1];
                p[i1 + 1] = tmp;
            }
}

// бінарний пошук за (середній бал, оцінка з математики, прізвище)
int BinSearch(Student* p, const int N, const string prizv, const int sumKey, const int matem)
{
    int L = 0, R = N - 1, m;
    while (L <= R)
    {
        m = (L + R) / 2;
        if (Sum(p[m]) == sumKey && p[m].matematyka == matem && p[m].prizv == prizv)
            return m;
        if ((Sum(p[m]) < sumKey)
            ||
            (Sum(p[m]) == sumKey && p[m].matematyka < matem)
            ||
            (Sum(p[m]) == sumKey && p[m].matematyka == matem &&
                p[m].prizv < prizv))       // прізвища впорядковані за зростанням
        {
            L = m + 1;
        }
        else
        {
            R = m - 1;
        }
    }
    return -1;
}

// індексне впорядкування (вставками): середній бал -> математика -> прізвище
int* IndexSort(Student* p, const int N)
{
    int* I = new int[N];
    for (int i = 0; i < N; i++)
        I[i] = i;

    int i, j, value;
    for (i = 1; i < N; i++)
    {
        value = I[i];
        for (j = i - 1;
            j >= 0 && ((Sum(p[I[j]]) > Sum(p[value]))
                ||
                (Sum(p[I[j]]) == Sum(p[value]) &&
                    p[I[j]].matematyka > p[value].matematyka)
                ||
                (Sum(p[I[j]]) == Sum(p[value]) &&
                    p[I[j]].matematyka == p[value].matematyka &&
                    p[I[j]].prizv > p[value].prizv));
            j--)
        {
            I[j + 1] = I[j];
        }
        I[j + 1] = value;
    }
    return I;
}

void PrintIndexSorted(Student* p, int* I, const int N, const string* specStr)
{
    cout << "Список студентів (індексне впорядкування)" << endl;
    PrintHeader();
    for (int i = 0; i < N; i++)
        PrintRow(i + 1, p[I[i]], specStr);
    cout << "====================================================================================================="
        << endl << endl;
}
#include <windows.h>
#include <locale>
#include <iostream>
#include <cmath>
using namespace std;

double f(double x) {
    return 1.0 / sqrt(x * x + 3.2);
}

double leftRect(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += f(a + i * h);
    return sum * h;
}

double rightRect(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 1; i <= n; i++)
        sum += f(a + i * h);
    return sum * h;
}

double midRect(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += f(a + i * h + h / 2.0);
    return sum * h;
}

double trapezoid(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    for (int i = 1; i < n; i++)
        sum += 2 * f(a + i * h);
    return sum * (h / 2.0);
}

double simpson(double a, double b, int n) {
    if (n % 2 != 0) n++;
    double h = (b - a) / n;

    double sum = f(a) + f(b);
    for (int i = 1; i < n; i++) {
        if (i % 2 == 0) sum += 2 * f(a + i * h);
        else sum += 4 * f(a + i * h);
    }

    return sum * (h / 3.0);
}

double exactIntegral(double a, double b) {
    auto F = [](double x) {
        return log(x + sqrt(x * x + 3.2));
        };
    return F(b) - F(a);
}

double findN(double a, double b, double eps,
    double (*method)(double, double, int),
    int& bestN)
{
    double exact = exactIntegral(a, b);
    int n = 10;
    const int max_n = 1e7;

    while (true) {
        double approx = method(a, b, n);
        double err = fabs(approx - exact);

        if (err <= eps) {
            bestN = n;
            return approx;
        }

        n *= 2;
        if (n > max_n) {
            bestN = -1;
            return approx;
        }
    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "ukr");

    double a = 1.2, b = 2.7;
    double eps = 0.001;
    int choice;

    do {
        cout << "\n----- Меню -----\n";
        cout << "1. Метод лiвих прямокутникiв\n";
        cout << "2. Метод правих прямокутникiв\n";
        cout << "3. Метод центральних прямокутникiв\n";
        cout << "4. Метод трапецій\n";
        cout << "5. Метод Сiмпсона\n";
        cout << "0. Вихiд\n";
        cout << "Ваш вибiр: ";
        cin >> choice;

        if (choice == 0) break;

        int bestN;
        double result;

        switch (choice) {
        case 1:
            result = findN(a, b, eps, leftRect, bestN);
            break;
        case 2:
            result = findN(a, b, eps, rightRect, bestN);
            break;
        case 3:
            result = findN(a, b, eps, midRect, bestN);
            break;
        case 4:
            result = findN(a, b, eps, trapezoid, bestN);
            break;
        case 5:
            result = findN(a, b, eps, simpson, bestN);
            break;
        default:
            cout << "Невірний вибір!\n";
            continue;
        }

        if (bestN == -1) {
            cout << "Метод не досяг точності eps. Потрібне дуже велике n.\n";
        }
        else {
            cout << "Результат: " << result
                << "\nМінімальне n: " << bestN
                << "\nПохибка <= " << eps << endl;
        }

    } while (choice != 0);

    return 0;
}

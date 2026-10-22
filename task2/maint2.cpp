#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <iomanip>

using namespace std;

int s = 13;
double sigma = 0.5;
double du = 1.0;

random_device rd;
mt19937 gen(rd());
normal_distribution<double> norm(0.0, 1.0);

//  Помеха h ~ N(0, sigma)
double noise(double sigma)
{
    return norm(gen) * sigma;
}

double object(vector<double>& u, double sigma)
{
    double y = 0.0;
    for (int i = 0; i < u.size(); i++)
        y += u[i] * u[i];
    y += noise(sigma);
    return y;
}

int main() 
{
    // количество столбцов
    int n = 4; 
    
    // Количество строк (опытов)
    int N = 8; 
    int base_factors = 3; 

    // выделяем память под N строк и n столбцов
    int** x_all = new int*[N];
    for (int i = 0; i < N; i++)
        x_all[i] = new int[n](); 

    int* x = new int[base_factors]();

    // генерируем базовые 3 фактора x1, x2, x3 в кодировке 0, 1
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < base_factors; j++)
            x_all[i][j] = x[j];

        int j = 0;
        while (j < base_factors && x[j] == 1)
        {
            x[j] = 0;
            j++;
        }
        if (j < base_factors)
            x[j] = 1;
    }

    // Переводим базовые нули в реальные -1
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < base_factors; j++)
        {
            if (x_all[i][j] == 0)
            {
                x_all[i][j] = -1; 
            }
        }
    }

    // Вычисляем дополнительные столбцы в зависимости от выбранного n
    if (n == 4)
    {
        for (int i = 0; i < N; i++)
        {
            // x4 = x1 * x2 * x3
            x_all[i][3] = x_all[i][0] * x_all[i][1] * x_all[i][2]; 
        }
    }
    else if (n == 6)
    {
        for (int i = 0; i < N; i++)
        {
            x_all[i][3] = x_all[i][0] * x_all[i][1];         // x4 = x1 * x2
            x_all[i][4] = x_all[i][0] * x_all[i][2];         // x5 = x1 * x3
            x_all[i][5] = x_all[i][0] * x_all[i][1] * x_all[i][2]; // x6 = x1 * x2 * x3
        }
    }

    // Матрица u_all
    int** u_all = new int*[N];
    for (int i = 0; i < N; i++)
        u_all[i] = new int[n](); // Выделяем ровно n столбцов (4 или 6)

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < n; j++) // Идем по абсолютно ВСЕМ столбцам от 0 до n-1
        {
            u_all[i][j] = s + x_all[i][j] * du;
        }
    }

    // Массив y_all
    double* y_all = new double[N]();
    for (int i = 0; i < N; i++)
    {
        // Создаем временный вектор u 
        vector<double> u_vector(n); 
        for (int j = 0; j < n; j++)
            u_vector[j] = u_all[i][j];

        // Передаем весь вектор u в объект для расчета y
        y_all[i] = object(u_vector, sigma);
    }

    //шапка таблицы
    for (int j = 0; j < n; j++)
        cout << "x" << (j + 1) << "\t";
    for (int j = 0; j < n; j++)
        cout << "u" << (j + 1) << "\t";
    cout << "y" << "\t";
    cout << endl;
    
    // вывод данных 
    for (int i = N - 1; i >= 0; i--)
    {
        // Выводим все столбцы x
        for (int j = 0; j < n; j++) 
        {
            cout << x_all[i][j] << "\t"; 
        }
        // Выводим ВСЕ столбцы u
        for (int j = 0; j < n; j++)
        {
            cout << u_all[i][j] << "\t";
        }
        
        // Выводим посчитанный y
        cout << y_all[i] << endl;
    }

    //Находим коэф бетта 
    for (int j = 0; j <= n; j++)
    {
        double scalar = 0.0;
        for (int i = 0; i < N; i++)
        {
            double x_val;

            if (j == 0)
            {
                x_val = 1.0; 
            }
            else 
            {
                x_val = x_all[i][j - 1];
            }
            scalar += x_val * y_all[i];
        }
        
        double beta = scalar / N;
        cout << "beta_" << j << " = " << beta << " \talpha_" << j << " = " << beta << endl;
    }

    //  Освобождение памяти 
    for (int i = 0; i < N; i++)
        delete[] x_all[i];
    delete[] x_all;

    for (int i = 0; i < N; i++)
        delete[] u_all[i];
    delete[] u_all;

    delete[] y_all;
    delete[] x;

    return 0;
}

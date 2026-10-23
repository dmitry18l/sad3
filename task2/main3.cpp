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
    y += 0;
    // y += noise(sigma);
    return y;
}

int main() 
{   
    int k = 4;
    // количество столбцов матрицы X
    int n = 7; 
    
    // Количество строк (опытов)
    int N = 9; 
    int base_factors = 2; 

    // выделяем память под N строк и n столбцов
    double** x_all = new double*[N];
    for (int i = 0; i < N; i++)
        x_all[i] = new double[n](); 

    int* x = new int[base_factors]();

    // генерируем базовые факторы x1, x2 в кодировке 0, 1
    for (int i = 0; i < k; i++)
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
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < base_factors; j++)
        {
            if (x_all[i][j] == 0)
            {
                x_all[i][j] = -1.0; 
            }
        }
    }

    // считаем доп строки
    x_all[4][0] = 1.0;
    x_all[5][0] = -1.0;
    x_all[6][0] = 0;
    x_all[7][0] = 0;
    x_all[8][0] = 0;

    x_all[4][1] = 0;
    x_all[5][1] = 0;
    x_all[6][1] = 1.0;
    x_all[7][1] = -1.0;
    x_all[8][1] = 0;

    double sum_x1_sq = 0.0;
    double sum_x2_sq = 0.0;
    
    // считаем доп столбцы
    for (int i = 0; i < N; i++)
    {
        // x1x2
        x_all[i][2] = x_all[i][0] * x_all[i][1];
        
        // x1^2
        x_all[i][3] = x_all[i][0] * x_all[i][0]; 
        
        // x2^2
        x_all[i][4] = x_all[i][1] * x_all[i][1];

        sum_x1_sq += x_all[i][3]; // Сумма столбца x1^2
        sum_x2_sq += x_all[i][4]; // Сумма столбца x2^2
    }

    for (int i = 0; i < N; i++)
    {
        // x1^2 - ср
        x_all[i][5] = x_all[i][3] - (sum_x1_sq / double(N));
        // x2^2 - ср
        x_all[i][6] = x_all[i][4] - (sum_x2_sq / double(N));
    }

    // u_all
    double** u_all = new double*[N];
    for (int i = 0; i < N; i++)
        u_all[i] = new double[base_factors](); 

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < base_factors; j++) 
        {
            u_all[i][j] = s + x_all[i][j] * du;
        }
    }

    // Массив y_all
    double* y_all = new double[N]();
    for (int i = 0; i < N; i++)
    {
        vector<double> u_vector(base_factors); 
        for (int j = 0; j < base_factors; j++)
            u_vector[j] = u_all[i][j];

        y_all[i] = object(u_vector, sigma);
    }

    // Скорректированная под новый размер шапка таблицы
    cout << "x1\tx2\tx1x2\tx1^2\tx2^2\tx1'\tx2'\tu1\tu2\ty" << endl;
    
    // вывод первых k строк (в обратном порядке)
    for (int i = k - 1; i >= 0; i--)
    {
        for (int j = 0; j < n; j++) 
        {
            cout << fixed << setprecision(3) << x_all[i][j] << "\t"; 
        }
        for (int j = 0; j < base_factors; j++) 
        {
            cout << fixed << setprecision(3) << u_all[i][j] << "\t";
        }
        cout << fixed << setprecision(3) << y_all[i] << endl;
    }

    // Выводим оставшиеся строки в обычном прямом порядке
    for (int i = k; i < N; i++)
    {
        for (int j = 0; j < n; j++) 
        {
            cout << fixed << setprecision(3) << x_all[i][j] << "\t"; 
        }
        for (int j = 0; j < base_factors; j++) 
        {
            cout << fixed << setprecision(3) << u_all[i][j] << "\t";
        }
        cout << fixed << setprecision(3) << y_all[i] << endl;
    }

    double saved_beta_0 = 0.0;
    double saved_beta_6 = 0.0;
    double saved_beta_7 = 0.0;

    //Находим коэф бетта 
    for (int j = 0; j <= n; j++)
    {   
        if (j == 4 || j == 5)
        {
            continue;
        }

        double scalar1 = 0.0;
        double scalar2 = 0.0;
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
            scalar1 += x_val * y_all[i];
            scalar2 += x_val * x_val;
        }
        
        double beta = scalar1 / scalar2;
        cout << "beta_" << j << " = " << beta << " \talpha_" << j << " = " << beta << endl;

        if (j == 0) saved_beta_0 = beta;
        if (j == 6) saved_beta_6 = beta;
        if (j == 7) saved_beta_7 = beta;
    }

    double beta_0_real = saved_beta_0 - (saved_beta_6 * (sum_x1_sq / double(N))) - (saved_beta_7 * (sum_x1_sq / double(N)));
    cout << "beta_0_real = " << beta_0_real << endl;
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

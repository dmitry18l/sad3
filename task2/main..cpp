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

//  Объект: y = u1^2 + u2^2 + ... + un^2 + h 
double object(vector<double>& u, double sigma)
{
    double y = 0.0;
    for (int i = 0; i < u.size(); i++)
        y += u[i] * u[i];
    // y += 0;
    y += noise(sigma);
    return y;
}

int main() 
{
    int n = 2;
    int N = 1;
    for (int i = 0; i < n; i++)
        N = N * 2;

    //  Матрица x_all 
    int** x_all = new int*[N];
    for (int i = 0; i < N; i++)
        x_all[i] = new int[n]();

    int* x = new int[n]();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < n; j++)
            x_all[i][j] = x[j];

        int j = 0;
        while (j < n && x[j] == 1)
        {
            x[j] = 0;
            j++;
        }
        if (j < n)
            x[j] = 1;
    }

    //  Матрица u_all 
    int** u_all = new int*[N];
    for (int i = 0; i < N; i++)
        u_all[i] = new int[n]();

    int* u = new int[n]();

    // Матрица u_all
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double x_centered;
            if (x_all[i][j] == 0)
                x_centered = -1.0;
            else
                x_centered = 1.0;

            u_all[i][j] = s + x_centered * du;
        }
    }

    // Массив y_all 
    double* y_all = new double[N]();

    for (int i = 0; i < N; i++)
    {
        vector<double> u(n);
        for (int j = 0; j < n; j++)
            u[j] = u_all[i][j];

        y_all[i] = object(u, sigma);
    }

    //  Шапка 
    for (int j = 0; j < n; j++)
        cout << "x" << (j + 1) << "\t";
    for (int j = 0; j < n; j++)
        cout << "u" << (j + 1) << "\t";
    cout << "y" << "\t";
    cout << endl;

    // Данные 
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (x_all[i][j] == 0)
                cout << -1 << "\t";
            else
                cout << x_all[i][j] << "\t";
        }
        for (int j = 0; j < n; j++)
            cout << u_all[i][j] << "\t";
        cout << y_all[i] << "\t";
        cout << endl;
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
                if (x_all[i][j-1] == 0)
                {
                    x_val = -1.0;
                }
                else
                {
                    x_val = 1.0;
                }
            }
            scalar += x_val * y_all[i];
        }
        // cout << scalar << endl;
        double beta = scalar / N;
        cout << "beta_" << j << " = " << beta << " \talpha_" << j << " = " << beta << endl;
    }

    //  Освобождение 
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
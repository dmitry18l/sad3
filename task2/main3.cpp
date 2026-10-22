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
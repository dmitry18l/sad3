#include "iostream"
#include "fstream"
#include "cmath"

using namespace std;

int main() {
    // Number of rows and columns
    int rows = 105;
    int cols = 3; // 2 signs and 1 class

    // Number of classes
    int c = 3;

    // Number of sign
    int sign = cols - 1;


    // Open train file for reading
    ifstream fin("iris34_train.txt");

    // Check if the file was opened
    if (!fin.is_open()) {
        cout << "Error: File iris34_train.txt not found!" << endl;
        return 1;
    }


    // Allocate memory for train data
    double** a = new double*[rows];

    for (int i = 0; i < rows; i++) {
        a[i] = new double[cols];
    }


    // Read train data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fin >> a[i][j];
        }
    }

    fin.close();


    // Mathematical expectations

    double** math = new double*[c];

    for (int i = 0; i < c; i++) {
        math[i] = new double[sign];
    }
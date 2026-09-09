#include "iostream"
#include "fstream"

using namespace std;

int main() {
    //Open file for reading
    ifstream fin("iris34_train.txt");

    //Control: If the file is not next to the program, it will be closed
    if (!fin.is_open()) {
        cout << "Error: File iris34_train.txt not found!" << endl;
        return 1;
    }

    //Use pointers to allocate memory for a two-dimensional array
    int rows = 105;
    int cols = 3; // 3 cloumns: 2 sign and 1 class

    double** a = new double*[rows];
    for (int i = 0; i < rows; i++) {
        a[i] = new double[cols];
    }

    //Read data from the file into our array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fin >> a[i][j];
        }
    }
    fin.close();

    //Output the already populated array to the screen for verification.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }

    //Clear memory
    for (int i = 0; i < rows; i++) {
        delete[] a[i];
    }
    delete[] a;

    return 0;

}
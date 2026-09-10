#include "iostream"
#include "fstream"
#include "cmath"

using namespace std;

int main() {
    // Open file for reading
    ifstream fin("iris34_train.txt");

    // Check if the file was opened
    if (!fin.is_open()) {
        cout << "Error: File iris34_train.txt not found!" << endl;
        return 1;
    }

    // Number of rows and columns
    int rows = 105;
    int cols = 3; // 2 features and 1 class

    // Allocate memory for a two-dimensional array
    double** a = new double*[rows];

    for (int i = 0; i < rows; i++) {
        a[i] = new double[cols];
    }

    // Read data from the file into the array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fin >> a[i][j];
        }
    }

    fin.close();


    // Mathematical expectations

    double sum1 = 0;
    double sum2 = 0;

    int count0 = 0;
    int count1 = 0;
    int count2 = 0;


    // Class 0
    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 0) {
            sum1 += a[i][0];
            sum2 += a[i][1];
            count0++;
        }
    }

    double math0_1 = sum1 / count0;
    double math0_2 = sum2 / count0;


    // Reset sums
    sum1 = 0;
    sum2 = 0;


    // Class 1
    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 1) {
            sum1 += a[i][0];
            sum2 += a[i][1];
            count1++;
        }
    }

    double math1_1 = sum1 / count1;
    double math1_2 = sum2 / count1;


    // Reset sums
    sum1 = 0;
    sum2 = 0;


    // Class 2
    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 2) {
            sum1 += a[i][0];
            sum2 += a[i][1];
            count2++;
        }
    }

    double math2_1 = sum1 / count2;
    double math2_2 = sum2 / count2;


    // Standard deviations

    // Reset sums
    sum1 = 0;
    sum2 = 0;


    // Class 0
    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 0) {
            sum1 += pow(a[i][0] - math0_1, 2);
            sum2 += pow(a[i][1] - math0_2, 2);
        }
    }

    double sko0_1 = sqrt(sum1 / count0);
    double sko0_2 = sqrt(sum2 / count0);


    // Reset sums
    sum1 = 0;
    sum2 = 0;


    // Class 1
    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 1) {
            sum1 += pow(a[i][0] - math1_1, 2);
            sum2 += pow(a[i][1] - math1_2, 2);
        }
    }

    double sko1_1 = sqrt(sum1 / count1);
    double sko1_2 = sqrt(sum2 / count1);


    // Reset sums
    sum1 = 0;
    sum2 = 0;


    // Class 2
    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 2) {
            sum1 += pow(a[i][0] - math2_1, 2);
            sum2 += pow(a[i][1] - math2_2, 2);
        }
    }

    double sko2_1 = sqrt(sum1 / count2);
    double sko2_2 = sqrt(sum2 / count2);


    // Output mathematical expectations

    cout << "Class 0:" << endl;
    cout << "Feature 1 = " << math0_1 << endl;
    cout << "Feature 2 = " << math0_2 << endl;

    cout << "Feature 1 standard deviation = " << sko0_1 << endl;
    cout << "Feature 2 standard deviation = " << sko0_2 << endl;

    cout << endl;

    cout << "Class 1:" << endl;
    cout << "Feature 1 = " << math1_1 << endl;
    cout << "Feature 2 = " << math1_2 << endl;

    cout << "Feature 1 standard deviation = " << sko1_1 << endl;
    cout << "Feature 2 standard deviation = " << sko1_2 << endl;

    cout << endl;

    cout << "Class 2:" << endl;
    cout << "Feature 1 = " << math2_1 << endl;
    cout << "Feature 2 = " << math2_2 << endl;

    cout << "Feature 1 standard deviation = " << sko2_1 << endl;
    cout << "Feature 2 standard deviation = " << sko2_2 << endl;


    // Clear memory
    for (int i = 0; i < rows; i++) {
        delete[] a[i];
    }

    delete[] a;

    return 0;
}
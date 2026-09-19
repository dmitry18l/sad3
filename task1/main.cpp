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

    // Number of objects in each class
    int* count = new int[c];

    for (int k = 0; k < c; k++) {
        count[k] = 0;
    }

    // Calculate mathematical expectations
    for (int k = 0; k < c; k++) {

        for (int i = 0; i < rows; i++) {
            
            if (a[i][cols - 1] == k) {
                count[k]++;
            }
        }
        //cout << k << ' ' << count[k] << endl;

        for (int j = 0; j < sign; j++) {

            double sum = 0;

            for (int i = 0; i < rows; i++) {
                
                if (a[i][cols - 1] == k) {
                    sum += a[i][j];
                }
            }
            //cout << sum << endl;

            math[k][j] = sum/count[k];
            //cout << math[k][j] << endl;
        }

    }

    // Standard deviation
    double** sko = new double*[c];

    for (int i = 0; i < c; i++) {
        sko[i] = new double[sign];
    }

    // Calculate standard deviation
    for (int k = 0; k < c; k++) {

        for (int j = 0; j < sign; j++) {

            double sum = 0;

            for (int i = 0; i < rows; i++) {

                if (a[i][cols - 1] == k) {

                    sum += pow(a[i][j] - math[k][j], 2);
                }
            }

            double dispersion = sum/count[k];

            sko[k][j] = sqrt(dispersion);
            //cout << sko[k][j] << endl;
        }

    }

    // Output parameters

    for (int k = 0; k < c; k++) {

        cout << "Class " << k << ":" << endl;

        for (int j = 0; j < sign; j++) {

            cout << "Priznak " << j + 1
                 << ": math = " << math[k][j]
                 << ": sko = " << sko[k][j] << endl;
        }

        cout << endl;
    }

    // Bayesian classifier

    double pi = 3.14159265359;


    // Calculate training accuracy

    int correctTrain = 0;

    for (int i = 0; i < rows; i++) {

        // Bayes values for all classes
        double* bayes = new double[c];

        for (int k = 0; k < c; k++) {

            double p = 1;

            for (int j = 0; j < sign; j++) {

                double x = a[i][j];

                double p_x = 
                (1 / (sko[k][j] * sqrt(2 * pi))) * exp(-pow(x - math[k][j], 2) / (2 * pow(sko[k][j], 2)));
            
                p = p * p_x;

            }

            // Bayes value
            bayes[k] = p;
        }

        // Find the class with the largest Bayes value
        int predictedClass = 0;

        for (int k = 1; k < c; k ++) {

            if (bayes[k] > bayes[predictedClass]) {
                predictedClass = k;
            }
        }

        // Check the result
        if (predictedClass == a[i][cols - 1]) {
            correctTrain++;
        }

        delete[] bayes;
    }

    double accuracyTrain = (double)correctTrain / rows;

    cout << "Training accuracy = "
         << accuracyTrain * 100 << "%" << endl;

    // Clear train data

    for (int i = 0; i < rows; i++) {
        delete[] a[i];
    }

    delete[] a;

    // Open test file for reading

    ifstream finTest("iris34_test.txt");

    // Check if the file was opened
    if (!finTest.is_open()) {
        cout << "Error: File iris34_test.txt not found!" << endl;
        return 1;
    }


    // Number of test rows
    int testRows = 45;


    // Allocate memory for test data

    double** test = new double*[testRows];

    for (int i = 0; i < testRows; i++) {
        test[i] = new double[cols];
    }


    // Read test data

    for (int i = 0; i < testRows; i++) {
        for (int j = 0; j < cols; j++) {
            finTest >> test[i][j];
        }
    }

    finTest.close();


    // Calculate test accuracy

    int correctTest = 0;


    for (int i = 0; i < testRows; i++) {

        // Bayes values for all classes
        double* bayes = new double[c];


        for (int k = 0; k < c; k++) {

            double p = 1;


            // using math and sko from train

            for (int j = 0; j < sign; j++) {

                double x = test[i][j];

                double p_x =
                    (1.0 / (sko[k][j] * sqrt(2 * pi))) *
                    exp(-pow(x - math[k][j], 2) /
                    (2 * pow(sko[k][j], 2)));


                p = p * p_x;
            }


            // Bayes value
            bayes[k] = p;
        }


        // Find the class with the largest Bayes value

        int predictedClass = 0;

        for (int k = 1; k < c; k++) {

            if (bayes[k] > bayes[predictedClass]) {
                predictedClass = k;
            }
        }


        // Check the result

        if (predictedClass == test[i][cols - 1]) {
            correctTest++;
        }


        delete[] bayes;
    }


    double accuracyTest = (double)correctTest / testRows;


    // Output only test accuracy

    cout << "Test accuracy = "
         << accuracyTest * 100 << "%" << endl;


    // Clear test data

    for (int i = 0; i < testRows; i++) {
        delete[] test[i];
    }

    delete[] test;


    // Clear parameters

    for (int i = 0; i < c; i++) {
        delete[] math[i];
        delete[] sko[i];
    }

    delete[] math;
    delete[] sko;
    delete[] count;


    return 0;
    
}
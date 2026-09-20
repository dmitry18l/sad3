#include "iostream"
#include "fstream"
#include "cmath"
#include "cstdlib"
#include "ctime"

using namespace std;

int main() {

    // Number of rows and columns
    int rows = 1600;
    int cols = 1000; // 999 signs and 1 class

    // Number of classes
    int c = 2;

    // Number of signs
    int sign = cols - 1;

    // Class labels
    int classLabel[2] = {0, 1};

    // Open file for reading
    ifstream fin("GAMETES_Epistasis_2_Way_1000atts_0.4H_EDM_1_EDM_1_1.txt");

    // Check if the file was opened
    if (!fin.is_open()) {
        cout << "Error: File GAMETES_Epistasis_2_Way_1000atts_0.4H_EDM_1_EDM_1_1.txt not found!" << endl;
        return 1;
    }

    // Allocate memory for all data
    double** a = new double*[rows];

    for (int i = 0; i < rows; i++) {
        a[i] = new double[cols];
    }

    // Read data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fin >> a[i][j];
        }
    }

    fin.close();

    // Randomly mix the data
    srand(time(0));

    for (int i = 0; i < rows * 10; i++) {

        int r1 = rand() % rows;
        int r2 = rand() % rows;

        double* temp = a[r1];
        a[r1] = a[r2];
        a[r2] = temp;
    }

    // Split data into train and test
    int rows_train = rows * 0.8;
    int rows_test = rows - rows_train;

    // Allocate memory for train data
    double** train = new double*[rows_train];

    for (int i = 0; i < rows_train; i++) {
        train[i] = new double[cols];
    }

    // Allocate memory for test data
    double** test = new double*[rows_test];

    for (int i = 0; i < rows_test; i++) {
        test[i] = new double[cols];
    }

    // Copy data into train
    for (int i = 0; i < rows_train; i++) {

        for (int j = 0; j < cols; j++) {
            train[i][j] = a[i][j];
        }
    }

    // Copy data into test
    for (int i = 0; i < rows_test; i++) {

        for (int j = 0; j < cols; j++) {
            test[i][j] = a[rows_train + i][j];
        }
    }

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

        for (int i = 0; i < rows_train; i++) {

            if (train[i][cols - 1] == classLabel[k]) {
                count[k]++;
            }
        }

        for (int j = 0; j < sign; j++) {

            double sum = 0;

            for (int i = 0; i < rows_train; i++) {

                if (train[i][cols - 1] == classLabel[k]) {
                    sum += train[i][j];
                }
            }

            math[k][j] = sum / count[k];
        }
    }

    // A priori probability
    double* aprior = new double[c];

    for (int k = 0; k < c; k++) {
        aprior[k] = (double)count[k] / rows_train;
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

            for (int i = 0; i < rows_train; i++) {

                if (train[i][cols - 1] == classLabel[k]) {

                    sum += pow(train[i][j] - math[k][j], 2);
                }
            }

            double dispersion = sum / count[k];

            sko[k][j] = sqrt(dispersion);
        }
    }

    // Output parameters
    for (int k = 0; k < c; k++) {

        cout << "Class " << classLabel[k] << ":" << endl;

        cout << "aprior probability = " << aprior[k] << endl;

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

    for (int i = 0; i < rows_train; i++) {

        // Bayes values for all classes
        double* bayes = new double[c];

        for (int k = 0; k < c; k++) {

            // Start with 1
            double p = 1;

            // Calculate probability for each sign
            for (int j = 0; j < sign; j++) {

                double x = train[i][j];

                double p_x =
                    (1.0 / (sko[k][j] * sqrt(2 * pi))) *
                    exp(-pow(x - math[k][j], 2) /
                    (2 * pow(sko[k][j], 2)));

                p = p * p_x;
            }

            // Multiply by a priori probability
            p = p * aprior[k];

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
        if (classLabel[predictedClass] == train[i][cols - 1]) {
            correctTrain++;
        }

        delete[] bayes;
    }

    double accuracyTrain = (double)correctTrain / rows_train;

    cout << "Training accuracy = "
         << accuracyTrain * 100 << "%" << endl;


    // Calculate test accuracy
    int correctTest = 0;

    for (int i = 0; i < rows_test; i++) {

        // Bayes values for all classes
        double* bayes = new double[c];

        for (int k = 0; k < c; k++) {

            // Start with 1
            double p = 1;

            // Use math and sko from train
            for (int j = 0; j < sign; j++) {

                double x = test[i][j];

                double p_x =
                    (1.0 / (sko[k][j] * sqrt(2 * pi))) *
                    exp(-pow(x - math[k][j], 2) /
                    (2 * pow(sko[k][j], 2)));

                p = p * p_x;
            }

            // Multiply by a priori probability
            p = p * aprior[k];

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
        if (classLabel[predictedClass] == test[i][cols - 1]) {
            correctTest++;
        }

        delete[] bayes;
    }

    double accuracyTest = (double)correctTest / rows_test;

    cout << "Test accuracy = "
         << accuracyTest * 100 << "%" << endl;


    // Clear all data
    for (int i = 0; i < rows; i++) {
        delete[] a[i];
    }

    delete[] a;

    for (int i = 0; i < rows_train; i++) {
        delete[] train[i];
    }

    delete[] train;

    for (int i = 0; i < rows_test; i++) {
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
    delete[] aprior;

    return 0;
}

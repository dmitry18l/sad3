#include "iostream"
#include "fstream"
#include "cmath"

using namespace std;

int main() {
    // Open file for reading
    ifstream fin("iris34_test.txt");

    // Check if the file was opened
    if (!fin.is_open()) {
        cout << "Error: File iris34_test.txt not found!" << endl;
        return 1;
    }

    // Number of rows and columns
    int rows = 45;
    int cols = 3; // 2 features and 1 class

    // Allocate memory for a two-dimensional array
    double** a = new double*[rows];

    for (int i = 0; i < rows; i++) {
        a[i] = new double[cols];
    }

    // Read data from the file iris34_test.txt
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

    // Standard deviation
    
    // Class 0
    sum1 = 0;
    sum2 = 0;

    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 0) {
            sum1 += pow(a[i][0] - math0_1, 2);
            sum2 += pow(a[i][1] - math0_2, 2);
        }
    }

    double dispersion0_1 = sum1 / count0;
    double dispersion0_2 = sum2 / count0;

    double sko0_1 = sqrt(dispersion0_1);
    double sko0_2 = sqrt(dispersion0_2);


    // Class 1
    sum1 = 0;
    sum2 = 0;

    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 1) {
            sum1 += pow(a[i][0] - math1_1, 2);
            sum2 += pow(a[i][1] - math1_2, 2);
        }
    }

    double dispersion1_1 = sum1 / count1;
    double dispersion1_2 = sum2 / count1;

    double sko1_1 = sqrt(dispersion1_1);
    double sko1_2 = sqrt(dispersion1_2);


    // Class 2
    sum1 = 0;
    sum2 = 0;

    for (int i = 0; i < rows; i++) {
        if (a[i][2] == 2) {
            sum1 += pow(a[i][0] - math2_1, 2);
            sum2 += pow(a[i][1] - math2_2, 2);
        }
    }

    double dispersion2_1 = sum1 / count2;
    double dispersion2_2 = sum2 / count2;

    double sko2_1 = sqrt(dispersion2_1);
    double sko2_2 = sqrt(dispersion2_2);

    // Output parameters
    // cout << "Class 0:" << endl;
    // cout << "Feature 1: math = " << math0_1
    //      << ", sko = " << sko0_1 << endl;
    // cout << "Feature 2: math = " << math0_2
    //      << ", sko = " << sko0_2 << endl;

    // cout << endl;

    // cout << "Class 1:" << endl;
    // cout << "Feature 1: math = " << math1_1
    //      << ", sko = " << sko1_1 << endl;
    // cout << "Feature 2: math = " << math1_2
    //      << ", sko = " << sko1_2 << endl;

    // cout << endl;

    // cout << "Class 2:" << endl;
    // cout << "Feature 1: math = " << math2_1
    //      << ", sko = " << sko2_1 << endl;
    // cout << "Feature 2: math = " << math2_2
    //      << ", sko = " << sko2_2 << endl;


    // Bayesian classifier
    double pi = 3.14159265359;

    // Prior probabilities of classes
    double prior0 = (double)count0 / rows;
    double prior1 = (double)count1 / rows;
    double prior2 = (double)count2 / rows;


    cout << endl;
    // cout << "Bayesian classifier:" << endl;

    int correctTrain = 0;


    // Classify every object from the training sample
    for (int i = 0; i < rows; i++) {

        // X and Y of the current object
        double x = a[i][0];
        double y = a[i][1];

        // Class 0
        double p0_x =
            (1 / (sko0_1 * sqrt(2 * pi))) *
            exp(-pow(x - math0_1, 2) /
            (2 * pow(sko0_1, 2)));

        double p0_y =
            (1 / (sko0_2 * sqrt(2 * pi))) *
            exp(-pow(y - math0_2, 2) /
            (2 * pow(sko0_2, 2)));

        // Two-dimensional density
        double p0 = p0_x * p0_y;

        // Bayes value
        double bayes0 = p0 * prior0;

        // Class 1
        double p1_x =
            (1 / (sko1_1 * sqrt(2 * pi))) *
            exp(-pow(x - math1_1, 2) /
            (2 * pow(sko1_1, 2)));

        double p1_y =
            (1 / (sko1_2 * sqrt(2 * pi))) *
            exp(-pow(y - math1_2, 2) /
            (2 * pow(sko1_2, 2)));

        // Two-dimensional density
        double p1 = p1_x * p1_y;

        // Bayes value
        double bayes1 = p1 * prior1;

        // Class 2
        double p2_x =
            (1 / (sko2_1 * sqrt(2 * pi))) *
            exp(-pow(x - math2_1, 2) /
            (2 * pow(sko2_1, 2)));

        double p2_y =
            (1 / (sko2_2 * sqrt(2 * pi))) *
            exp(-pow(y - math2_2, 2) /
            (2 * pow(sko2_2, 2)));

        // Two-dimensional density
        double p2 = p2_x * p2_y;

        // Bayes value
        double bayes2 = p2 * prior2;

        // Find the class
        int predictedClass;

        if (bayes0 > bayes1 && bayes0 > bayes2) {
            predictedClass = 0;
        }
        else if (bayes1 > bayes0 && bayes1 > bayes2) {
            predictedClass = 1;
        }
        else {
            predictedClass = 2;
        }

        if (predictedClass == a[i][2]) {
            correctTrain++;
        }


        // Output result for current object
        // cout << "Object " << i + 1
        //      << ": x = " << x
        //      << ", y = " << y
        //      << ", predicted class = "
        //      << predictedClass << endl;

    }

    double accuracyTrain = (double)correctTrain / rows;

    //Output accuracy iris34_test.txt
    cout << endl;
    cout << "Testing accuracy = "
         << accuracyTrain * 100 << "%" << endl;

    // Clear memory
    for (int i = 0; i < rows; i++) {
        delete[] a[i];
    }

    delete[] a;

    return 0;
}
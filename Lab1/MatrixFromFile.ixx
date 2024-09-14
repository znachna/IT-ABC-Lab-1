export module MatrixFromFile;

import <iostream>;
import <vector>;
import <fstream>;

using namespace std;

export pair<vector<vector<double>>, double> ReadProblemVectorFromFile(const string& filename) {

    ifstream file(filename);

    int n;
    file >> n;

    vector<vector<double>> coefficients;

    for (int i = 0; i < n; i++) {
        vector<double> row;
        for (int j = 0; j < n; j++) {
            double value;
            file >> value;
            row.push_back(value);
        }
        coefficients.push_back(row);
    }

    vector<double> b;
    for (int i = 0; i < n; i++) {
        double value;
        file >> value;
        b.push_back(value);
    }

    double finalError;
    file >> finalError;

    vector<vector<double>> problemVector;

    for (int i = 0; i < n; i++) {
        double divider = coefficients[i][i];

        vector<double> row;

        row.push_back(b[i] / divider);

        for (int j = 0; j < n; j++) {
            if (i != j) {
                row.push_back(-coefficients[i][j] / divider);
            }
        }

        problemVector.push_back(row);
    }

    file.close();
    return { problemVector, finalError };
}

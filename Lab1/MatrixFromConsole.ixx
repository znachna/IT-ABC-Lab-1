export module MatrixFromConsole;

import <string>;
import <vector>;
import <iostream>;

using namespace std;

double GetVar(string message) {
    double var;

    cout << message << " ";
    cin >> var;
    return var;
}

vector <vector <double>> GetMatrixFromConsole (int matrixLength) {

    vector <vector <double> > matrix;

    for (int i = 0; i < matrixLength; i++) {

        vector <double> row(matrixLength);

        for (int j = 0; j < matrixLength; j++) {
            cout << "Row " << i + 1 << " Collumn " << j + 1 << endl;
            cin >> row[j];
        }

        matrix.push_back(row);
    }

    return matrix;
}

vector <double> GetVectorFromConsole(int vectorLength) {

    vector <double> equationVector(vectorLength);

    for (int i = 0; i < vectorLength; i++) {
        cout << "System constant # " << i+1 << " ";
        cin >> equationVector[i];
    }

    return equationVector;
}

vector <vector <double>> CreateProblemVector(int n, vector<vector <double>> matrix, vector<double> b) {

    vector<vector<double>> problemVector;
    for (int i = 0; i < matrix.size(); i++) {

        vector <double> row;
        double devider = matrix[i][i];

        row.push_back(b[i] / devider);
        for (int j = 0; j < matrix[i].size(); j++) {
            if (i == j) continue;
            row.push_back(-matrix[i][j] / devider);
        }
        problemVector.push_back(row);
    }

    return problemVector;
}




export pair<vector<vector<double>>, double> ReadProblemVectorFromConsole() {
    
    const int n = GetVar("input matrix length: ");
    const vector < vector < double > > matrix = GetMatrixFromConsole(n);
    const vector <double> b = GetVectorFromConsole(n);
    const double finalError = GetVar("input error: ");

    vector <vector <double>> problemVector = CreateProblemVector(n, matrix, b);

    return { problemVector, finalError };
}
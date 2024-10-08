﻿#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


import MatrixFromFile;
import MatrixFromConsole;

using namespace std;
using json = nlohmann::json;

struct RelaxParams {

    double const DEF_RELAX;
    double const MAX_RELAX;
    double const RELAX_STEP;

};

RelaxParams ReadConfig() {

    const string fileName = "config.json";

    ifstream file(fileName);

    if (!file.is_open()) {
        return { 0.2, 1.0, 0.2 };  // Значения по умолчанию
    }

    json configJson;
    file >> configJson;


    return { configJson.value("DEF_RELAX", 0.2), configJson.value("MAX_RELAX", 2.0), configJson.value("RELAX_STEP", 0.2) } ;
}



void ShowProblemVector(vector <vector <double>> problemVector) {

    cout << "Equations: " << endl;

    for (int i = 0, currentXIndex = 1; i < problemVector.size(); i++, currentXIndex++) {

        cout << "x" << currentXIndex << " = ";

        for (int j = 0, xEquationIndex = 0; j < problemVector[i].size(); j++, xEquationIndex++) {
            if (xEquationIndex > 0) {
                if (xEquationIndex == currentXIndex) xEquationIndex++;
                cout << problemVector[i][j] << "*x" << xEquationIndex << " ";
            }
            else cout << problemVector[i][j] << " ";
        }

        cout << endl;
    }
}

pair<vector<vector<double>>, double> GetProblemVectorWithError() {

    bool isProblemFromFile;

    cout << "To read problem from file ProblemMatrix.txt: 1" << endl;
    cout << "To read problem from console input: 0" << endl << endl;
    cout << "Your choice: ";

    cin >> isProblemFromFile;

    if (isProblemFromFile) return ReadProblemVectorFromFile("ProblemMatrix.txt");
    else return ReadProblemVectorFromConsole();

}

void SaveRelaxationResults(vector <vector <double>> relaxResults) {
    json j;

    for (const auto relaxIter : relaxResults)
        j["plot"].push_back({ { "relaxation", relaxIter[0] }, {"iterations", relaxIter[1]} });
    
    ofstream file("plot.json");
    file << j;
    file.close();
}



int main()
{
    const auto [DEF_RELAX, MAX_RELAX, RELAX_STEP] = ReadConfig();
    const auto [PROBLEM_VECTOR, FINAL_ERROR] = GetProblemVectorWithError();

    vector <double> x(PROBLEM_VECTOR.size());
    vector <double> xError(PROBLEM_VECTOR.size());

    ShowProblemVector(PROBLEM_VECTOR);

    vector < vector <double> > relaxResults;
    vector <double> xAnswers;
    double maxError;

    for (double relaxParam = DEF_RELAX; relaxParam <= MAX_RELAX; relaxParam += RELAX_STEP) {

        vector <double> relaxIter;
        relaxIter.push_back(relaxParam);

        x.assign(x.size(), 0); // обнуляем результаты поиска решений для новой итерации

        int counter = 0;
        double prevMaxError;
        do {
            counter++;

            for (int i = 0; i < PROBLEM_VECTOR.size(); i++) {
                
                double xValue = PROBLEM_VECTOR[i][0];

                for (int j = 1, xIndexCounter = 0; j < PROBLEM_VECTOR.size(); j++, xIndexCounter++) {
                    if (xIndexCounter == i) xIndexCounter++;
                    xValue += x[xIndexCounter] * PROBLEM_VECTOR[i][j];
                }

                xError[i] = abs(xValue - x[i]);
                xValue = relaxParam*xValue + (1-relaxParam)*x[i]; // ЗДЕСЬ БУДЕТ РЕЛАКСАЦИЯ

                x[i] = xValue;
            }

            for (int i = 0; i < xError.size(); i++)
                if (i == 0 || maxError < xError[i])   maxError = xError[i];

            if (counter == 1) prevMaxError = maxError;
            else if (prevMaxError > maxError) prevMaxError = maxError; // условие для того чтобы понять есть ли сходимость или нет
            else {
                counter = -1;
                cout << "No progress with relaxation = " << relaxParam << endl;
                maxError = FINAL_ERROR;
            }

        } while (maxError > FINAL_ERROR);

        if ( (counter != -1) && (xAnswers.empty() == true) ) xAnswers = x;

        relaxIter.push_back(counter);
        relaxResults.push_back(relaxIter);
    }


    for (vector <double> relaxIter : relaxResults) {
        cout << "Relaxation param = " << relaxIter[0] << " Total iterations: " << ((relaxIter[1] != -1) ? to_string( static_cast<int>(relaxIter[1]) ) : "No progress") << endl;
    }

    cout << "Answers: " << endl;
    for (int i = 0; i < xAnswers.size(); i++)
        cout << "x" << i + 1 << ": " << xAnswers[i] << endl;

    SaveRelaxationResults(relaxResults);
    system("python plot_graph.py");

}

export module MatrixFromFile;

import <iostream>;
import <vector>;
import <fstream>;

using namespace std;

export pair<vector<vector<double>>, double> readProblemVectorFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл!" << endl;
        return { {}, 0.0 };  // Пустой вектор и нулевая ошибка, если файл не открылся
    }

    int n;
    file >> n;

    // Временная матрица для хранения коэффициентов
    vector<vector<double>> coefficients;

    // Чтение коэффициентов матрицы
    for (int i = 0; i < n; i++) {
        vector<double> row;
        for (int j = 0; j < n; j++) {
            double value;
            file >> value;
            row.push_back(value);
        }
        coefficients.push_back(row);
    }

    // Вектор независимых коэффициентов b
    vector<double> b;
    for (int i = 0; i < n; i++) {
        double value;
        file >> value;
        b.push_back(value);
    }

    // Чтение ошибки (finalError)
    double finalError;
    file >> finalError;

    // Создаем problemVector для возврата
    vector<vector<double>> problemVector;

    // Преобразуем каждую строку в уравнение для соответствующего x_i
    for (int i = 0; i < n; i++) {
        double divider = coefficients[i][i];  // Коэффициент при x_i (это divider)

        // Создаем вектор для текущей строки x_i
        vector<double> row;

        // Первый элемент — это независимый коэффициент (b), делённый на divider
        row.push_back(b[i] / divider);

        // Остальные элементы — коэффициенты других переменных
        for (int j = 0; j < n; j++) {
            if (i != j) {
                // Коэффициенты остальных переменных: -a_j / divider
                row.push_back(-coefficients[i][j] / divider);
            }
        }

        // Добавляем в problemVector
        problemVector.push_back(row);
    }

    file.close();
    return { problemVector, finalError };  // Возвращаем вектор и ошибку как пару
}

#include "matutils.h"


QVector<QVector<long double> > MatUtils::matmul(const QVector<QVector<long double> > &A, const QVector<QVector<long double> > &B)
{
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();

    QVector<QVector<long double>> result(rowsA, QVector<long double>(colsB, 0)); // матрица с нулевыми значениями

    if (colsA != rowsB) {
        throw std::invalid_argument("Несовместимые размеры матриц");
    }
    else {
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                for (int k = 0; k < colsA; k++) {
                    result[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    return result;
}

QVector<long double> MatUtils::matvec(const QVector<QVector<long double> > &A, const QVector<long double> &B)
{
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();

    QVector<long double> result(rowsA,0); // матрица с нулевыми значениями

    if (colsA != rowsB) {
        throw std::invalid_argument("Несовместимые размеры матриц");
    }
    else {
        // Умножение матрицы на вектор
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsA; j++) {
                result[i] += A[i][j] * B[j];
            }
        }
    }

    return result;
}

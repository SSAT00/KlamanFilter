#ifndef MATUTILS_H
#define MATUTILS_H

#include <QVector>

class MatUtils
{

public:
    // Метод перемножения двух матриц из QVector
    static QVector<QVector<long double>> matmul(const QVector<QVector<long double>>& A,
                                                const QVector<QVector<long double>>& B);

    static QVector<long double> matvec(const QVector<QVector<long double>>& A,
                                       const QVector<long double>& B);
private:
  const long double PI = 3.1415926535897932384626433832795;

};

#endif // MATUTILS_H

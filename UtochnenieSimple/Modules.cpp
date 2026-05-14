#include "Modules.h"

double standardDeviation2(QVector<double> data, double& mean){
    if (data.isEmpty()){
        mean = 0.0;
        return 0.0;
    }

    double sum = 0.0;
    for (double value : data)
        sum += value;

    mean = sum / data.size();

    double sq_sum = 0.0;
    for (double value : data)
        sq_sum += (value - mean) * (value - mean);

    return std::sqrt(sq_sum / (data.size() - 1));
}

Matrix<int, Dynamic, Dynamic> GetCyclogramSat(){
    Matrix<int, Dynamic, Dynamic> cyclogram;
    cyclogram.resize(24,24);
    cyclogram.setOnes();
    return cyclogram;
}

Matrix<int, Dynamic, Dynamic> GetCyclogramStan(){
    Matrix<int, Dynamic, Dynamic> cyclogram;
    cyclogram.resize(24, 11);
    cyclogram.setOnes();
    return cyclogram;
}

Vector3ld error_to_stw(const Vector6ld& ref_state, const Vector6ld& calc_state) {
    Vector3ld r_ref = ref_state.head<3>();
    Vector3ld v_ref = ref_state.tail<3>();

    Vector3ld r_calc = calc_state.head<3>();

    Vector3ld dr = r_calc - r_ref;

    Vector3ld s_hat = r_ref.normalized();

    Vector3ld h_vec = r_ref.cross(v_ref);
    Vector3ld w_hat = h_vec.normalized();

    Vector3ld t_hat = w_hat.cross(s_hat);

    Matrix3ld T;
    T.row(0) = s_hat; // Первая строка — S
    T.row(1) = t_hat; // Вторая строка — T
    T.row(2) = w_hat; // Третья строка — W

    Vector3ld pos_err_stw = T * dr;

    return pos_err_stw;
}

long double getConditionNumberSVD(const Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic>& A) {
    // JacobiSVD — самый точный метод для long double
    Eigen::JacobiSVD<Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic>> svd(A);

    // Сингулярные числа в Eigen всегда отсортированы по убыванию
    long double max_s = svd.singularValues()(0);
    long double min_s = svd.singularValues()(svd.singularValues().size() - 1);

    return max_s / min_s;
}
























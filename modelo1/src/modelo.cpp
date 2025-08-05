#include "modelo.hpp"

void RegressaoLinear::treinar(const Eigen::MatrixXd& X, const Eigen::VectorXd& y) {
    pesos = (X.transpose() * X).ldlt().solve(X.transpose() * y);
}

double RegressaoLinear::prever(const Eigen::VectorXd& x) {
    return pesos.dot(x);
}

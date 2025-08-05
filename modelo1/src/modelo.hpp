#pragma once
#include <Eigen/Dense>

class RegressaoLinear {
public:
    void treinar(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
    double prever(const Eigen::VectorXd& x);
private:
    Eigen::VectorXd pesos;
};

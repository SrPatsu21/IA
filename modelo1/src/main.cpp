// main.cpp
#include <iostream>
#include "modelo.hpp"

int main() {
    Eigen::MatrixXd X(4, 2);
    X << 1, 1,
         1, 2,
         1, 3,
         1, 4; // com bias

    Eigen::VectorXd y(4);
    y << 2, 3, 4, 5;

    RegressaoLinear modelo;
    modelo.treinar(X, y);

    Eigen::VectorXd novaEntrada(2);
    novaEntrada << 1, 5;

    std::cout << "Previsão: " << modelo.prever(novaEntrada) << std::endl;
    return 0;
}
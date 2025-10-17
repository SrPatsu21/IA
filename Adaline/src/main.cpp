#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <cmath>
#include "../external/gnuplot-iostream.h"

// Constants for training
const int TRAINING_TIMES = 20; // Maximum number of epochs
const double INIT_BIAS = 0.0; // Initial bias value
const double LEARNING_RATE = 0.1; // Learning rate (alpha)
const int INPUT_SIZE = 2; // Number of input features
const int REPEAT_TIME = 10;

class Perceptron
{
private:
    std::vector<double> weights; // Vector of weights
    double learning_rate; // Learning rate
    double bias; // Bias term

    // Calculate net input: sum of weighted inputs + bias
    double netInput(const std::vector<double> &inputs) const {
        double sum = bias;
        for (size_t i = 0; i < inputs.size(); i++)
            sum += weights[i] * inputs[i];
        return sum;
    }

public:
    // Constructor: initialize weights randomly and set learning rate & bias
    Perceptron(int input_size, double learning_rate){
        this->learning_rate = learning_rate;
        this->bias = INIT_BIAS;
        this->weights.resize(input_size);

        // Random initialization between -0.5 and 0.5
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.5, 0.5);
        for (double &w : this->weights) {
            w = dis(gen);
        }
    }

    // Predict output: use linear combination and apply threshold (0.5)
    double predict(const std::vector<double> &inputs) const {
        double output = netInput(inputs);
        return (output >= 0.5) ? 1.0 : 0.0;  // Threshold for binary classification
    }

    // Training function using gradient descent (Delta Rule)
    std::vector<double> train(const std::vector<std::vector<double>> &X, const std::vector<double> &Y, int epochs) {
        std::vector<double> mse_list;

        for (int e = 0; e < epochs; e++) {
            double sum_squared_error = 0.0;

            for (size_t i = 0; i < X.size(); i++) {
                 double output = netInput(X[i]); // Compute linear output
                double error = Y[i] - output; // Compute error
                sum_squared_error += error * error; // Accumulate squared error

                // Update weights using Delta Rule
                for (size_t j = 0; j < weights.size(); j++) {
                    weights[j] += learning_rate * error * X[i][j];
                }
                bias += learning_rate * error; // Update bias
            }

            double mse = sum_squared_error / X.size();
            mse_list.push_back(mse);
            std::cout << "Epoch " << e + 1 << " | MSE=" << mse << " | error=" << sum_squared_error << std::endl;

            if (mse < 0.1) break;
        }

        // Print MSE values for plotting
        std::cout << "MSE per epoch: ";
        for (double v : mse_list) std::cout << v << " ";
        std::cout << std::endl;
        return mse_list;
    }
};

void generateGraf(std::string gate, int time, std::vector<double> mse_list){
    // grafs with Gnuplot
    std::vector<std::pair<int, double>> points;
    for (size_t i = 0; i < mse_list.size(); ++i)
        points.emplace_back(static_cast<int>(i + 1), mse_list[i]);

    try {
        Gnuplot gp;
        gp << "set title 'ADALINE " << gate << " Training " << time <<" - MSE vs Epoch'\n";
        gp << "set xlabel 'Epoch'\n";
        gp << "set ylabel 'MSE'\n";
        gp << "set grid\n";
        gp << "plot '-' with linespoints lw 2 title 'MSE'\n";
        gp.send1d(points);
    } catch (const std::exception &e) {
        std::cerr << "Erro ao gerar gráfico com Gnuplot: " << e.what() << std::endl;
    }
}

int main() {
    // Input patterns for AND gate
    std::vector<std::vector<double>> X = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    // Output labels
    std::vector<double> Yand = {0, 0, 0, 1};
    std::vector<double> Yxor = {0, 1, 1, 0};
    std::vector<double> Yor = {0, 1, 1, 1};

    // and trainnig
    for (size_t i = 0; i < REPEAT_TIME; i++)
    {
        Perceptron adaline(INPUT_SIZE, LEARNING_RATE);
        std::cout << "=== Starting ADALINE training " << i+1 << " ===" << std::endl;
        generateGraf("and", i+1, adaline.train(X, Yand, TRAINING_TIMES));
        std::cout << "\n=== ADALINE predictions ===" << std::endl;
        for (size_t i = 0; i < X.size(); i++) {
            std::cout << X[i][0] << " AND " << X[i][1] << " = " << adaline.predict(X[i]) << std::endl;
        }
    }

    // or trainnig
    for (size_t i = 0; i < REPEAT_TIME; i++)
    {
        Perceptron adaline(INPUT_SIZE, LEARNING_RATE);
        std::cout << "=== Starting ADALINE training " << i+1 << " ===" << std::endl;
        generateGraf("or", i+1, adaline.train(X, Yor, TRAINING_TIMES));
        std::cout << "\n=== ADALINE predictions ===" << std::endl;
        for (size_t i = 0; i < X.size(); i++) {
            std::cout << X[i][0] << " OR " << X[i][1] << " = " << adaline.predict(X[i]) << std::endl;
        }
    }

    // xor trainnig
    for (size_t i = 0; i < REPEAT_TIME; i++)
    {
        Perceptron adaline(INPUT_SIZE, LEARNING_RATE);
        std::cout << "=== Starting ADALINE training " << i+1 << " ===" << std::endl;
        generateGraf("xor", i+1, adaline.train(X, Yxor, TRAINING_TIMES));
        std::cout << "\n=== ADALINE predictions ===" << std::endl;
        for (size_t i = 0; i < X.size(); i++) {
            std::cout << X[i][0] << " XOR " << X[i][1] << " = " << adaline.predict(X[i]) << std::endl;
        }
    }

    return 0;
}
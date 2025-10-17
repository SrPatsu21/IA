#include <vector>
#include <iostream>
#include <cmath>
#include <random>

// Constants for training
const int TRAINING_TIMES = 10;      // Maximum number of epochs
const double INIT_BIAS = 0.0;       // Initial bias value
const double LEARNING_RATE = 0.1;   // Learning rate (alpha)
const int INPUT_SIZE = 2;           // Number of input features

// Utility function to convert a vector of numbers to a string
template <typename T>
std::string vectorStringify(const std::vector<T>& v) {
    std::string newString;
    for (auto i : v) {
        newString += std::to_string(i) + " ";
    }
    return newString;
}

// ADALINE class definition
class Adaline
{
private:
    std::vector<double> weights;   // Vector of weights
    double learning_rate;          // Learning rate
    double bias;                   // Bias term

    // Calculate net input: sum of weighted inputs + bias
    double netInput(const std::vector<double> &inputs) const {
        double sum = bias;
        for (size_t i = 0; i < inputs.size(); i++)
            sum += weights[i] * inputs[i];
        return sum;
    }

public:
    // Constructor: initialize weights randomly and set learning rate & bias
    Adaline(int input_size, double learning_rate = 0.1){
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
    void train(const std::vector<std::vector<double>> &X, const std::vector<double> &Y, int epochs) {
        for (int e = 0; e < epochs; e++) {
            double sum_squared_error = 0.0;

            for (size_t i = 0; i < X.size(); i++) {
                double output = netInput(X[i]);         // Compute linear output
                double error = Y[i] - output;           // Compute error
                sum_squared_error += error * error;     // Accumulate squared error

                // Update weights using Delta Rule
                for (size_t j = 0; j < weights.size(); j++) {
                    weights[j] += learning_rate * error * X[i][j];
                }
                bias += learning_rate * error;          // Update bias
            }

            // Print progress
            std::cout << "Epoch " << e + 1 << " | SSE=" << sum_squared_error << " | Weights=" << vectorStringify(weights) << " | Bias=" << bias << std::endl;

            // Stop early if error is very small
            if (sum_squared_error < 0.001)
                break;
        }
    }
};

int main() {
    // Input patterns for AND gate (change Y for XOR example)
    std::vector<std::vector<double>> X = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    std::vector<double> Y = {0, 1, 1, 0}; // Output labels

    Adaline adaline(INPUT_SIZE, LEARNING_RATE);

    std::cout << "=== Starting ADALINE training ===" << std::endl;
    adaline.train(X, Y, TRAINING_TIMES);

    std::cout << "\n=== ADALINE predictions ===" << std::endl;
    for (size_t i = 0; i < X.size(); i++) {
        std::cout << X[i][0] << " AND " << X[i][1] << " = " << adaline.predict(X[i]) << std::endl;
    }

    return 0;
}
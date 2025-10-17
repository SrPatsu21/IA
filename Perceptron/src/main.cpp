#include <vector>
#include <iostream>
#include <random>
#include "../external/gnuplot-iostream.h"

const int TRAINING_TIMES = 20;
const double INIT_BIAS = 0.0;
const double LEARNING_RATE = 0.1;
const int INPUT_SIZE = 2;
const int REPEAT_TIME = 10;

class Perceptron {
private:
    std::vector<double> weights;
    double learning_rate;
    double bias;

    int activation(double x)
    {
        return (x >= 0) ? 1 : 0;
    }

public:
    Perceptron(int input_size, double learning_rate = 0.1) {
        this->learning_rate = learning_rate;
        this->bias = INIT_BIAS;
        this->weights.resize(input_size);

        // Initialize weights randomly
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.5, 0.5);
        for (double &w : this->weights) {
            w = dis(gen);
        }
    };
    ~Perceptron(){

    };

    int predict(const std::vector<double> &inputs) {
        double sum = this->bias;
        for (size_t i = 0; i < inputs.size(); i++) {
            sum += this->weights[i] * inputs[i];
        }
        return activation(sum);
    }

    std::vector<double> train(const std::vector<std::vector<double>> &X, const std::vector<int> &Y, int epochs) {
        std::vector<double> error_list;

        for (int e = 0; e < epochs; e++) {
            int errors = 0;
            for (size_t i = 0; i < X.size(); i++) {
                int output = predict(X[i]);
                // real result - network result
                int error = Y[i] - output;
                errors += std::abs(error);

                // Update weights if error occurs
                for (size_t j = 0; j < this->weights.size(); j++) {
                    this->weights[j] += this->learning_rate * error * X[i][j];
                }
                this->bias += this->learning_rate * error;
            }

            error_list.push_back(static_cast<double>(errors));
            std::cout << "Epoch " << e+1 << " | Errors=" << errors << std::endl;
            if (errors == 0) break;
        }

        return error_list;
    }
};

void generateGraf(std::string gate, int time, std::vector<double> error_list){
    std::vector<std::pair<int, double>> points;
    for (size_t i = 0; i < error_list.size(); ++i)
        points.emplace_back(static_cast<int>(i + 1), error_list[i]);

    try {
        Gnuplot gp;
        gp << "set title 'Perceptron " << gate << " Training " << time <<" - Errors vs Epoch'\n";
        gp << "set xlabel 'Epoch'\n";
        gp << "set ylabel 'Errors'\n";
        gp << "set grid\n";
        gp << "plot '-' with linespoints lw 2 title 'Errors'\n";
        gp.send1d(points);
    } catch (const std::exception &e) {
        std::cerr << "Error generating plot: " << e.what() << std::endl;
    }
}

int main() {
    std::vector<std::vector<double>> X = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    std::vector<int> Yand = {0, 0, 0, 1};
    std::vector<int> Yor = {0, 1, 1, 1};
    std::vector<int> Yxor = {0, 1, 1, 0};

    // AND training
    for (int i = 0; i < REPEAT_TIME; i++) {
        Perceptron perceptron(INPUT_SIZE, LEARNING_RATE);
        auto errors = perceptron.train(X, Yand, TRAINING_TIMES);
        generateGraf("AND", i+1, errors);

        std::cout << "\nPredictions AND gate:" << std::endl;
        for (size_t j = 0; j < X.size(); j++)
            std::cout << X[j][0] << " AND " << X[j][1] << " = " << perceptron.predict(X[j]) << std::endl;
    }

    // OR training
    for (int i = 0; i < REPEAT_TIME; i++) {
        Perceptron perceptron(INPUT_SIZE, LEARNING_RATE);
        auto errors = perceptron.train(X, Yor, TRAINING_TIMES);
        generateGraf("OR", i+1, errors);

        std::cout << "\nPredictions OR gate:" << std::endl;
        for (size_t j = 0; j < X.size(); j++)
            std::cout << X[j][0] << " OR " << X[j][1] << " = " << perceptron.predict(X[j]) << std::endl;
    }

    // XOR training
    for (int i = 0; i < REPEAT_TIME; i++) {
        Perceptron perceptron(INPUT_SIZE, LEARNING_RATE);
        auto errors = perceptron.train(X, Yxor, TRAINING_TIMES);
        generateGraf("XOR", i+1, errors);

        std::cout << "\nPredictions XOR gate:" << std::endl;
        for (size_t j = 0; j < X.size(); j++)
            std::cout << X[j][0] << " XOR " << X[j][1] << " = " << perceptron.predict(X[j]) << std::endl;
    }

    return 0;
}
#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include "../external/gnuplot-iostream.h"

const int REPEAT_TIME = 1;

const int TRAINING_TIMES = 2000;
const double INIT_BIAS = 0.5;
const double LEARNING_RATE = 0.5;

const int INPUT_SIZE = 2;
const int HIDDEN_SIZE = 3;
const int OUTPUT_SIZE = 1;

class MLPerceptron {
private:
    std::vector<std::vector<double>> weights_input_hidden;
    std::vector<std::vector<double>> weights_hidden_output;
    std::vector<double> bias_hidden;
    std::vector<double> bias_output;
    std::mt19937 gen;

    int activation(double x)
    {
        return (x >= 0) ? 1 : 0;
    }

    // Squashes neuron output to 0-1
    double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    // Guides weight updates during learning
    double sigmoid_derivative(double x) {
        return x * (1.0 - x);
    }

public:
    MLPerceptron(int input_size, int hidden_size, int output_size) {
        std::random_device rd;
        gen = std::mt19937(rd());
        std::uniform_real_distribution<> dis(-0.5, 0.5);

        weights_input_hidden.resize(hidden_size, std::vector<double>(input_size));
        weights_hidden_output.resize(output_size, std::vector<double>(hidden_size));

        bias_hidden.resize(hidden_size);
        bias_output.resize(output_size);

        for (auto &row : weights_input_hidden)
            for (auto &w : row)
                w = dis(gen);

        for (auto &row : weights_hidden_output)
            for (auto &w : row)
                w = dis(gen);

        for (auto &b : bias_hidden)
            b = dis(gen);

        for (auto &b : bias_output)
            b = dis(gen);
    }

    std::vector<double> forward(const std::vector<double> &inputs, std::vector<double> &hidden_outputs) {
        // Compute the hidden layer activations
        hidden_outputs.resize(weights_input_hidden.size());
        for (size_t i = 0; i < weights_input_hidden.size(); ++i) {
            double sum = bias_hidden[i];
            for (size_t j = 0; j < inputs.size(); ++j)
                sum += weights_input_hidden[i][j] * inputs[j];
            hidden_outputs[i] = sigmoid(sum);
        }

        // Compute the output layer activations
        std::vector<double> outputs(weights_hidden_output.size());
        for (size_t i = 0; i < weights_hidden_output.size(); ++i) {
            double sum = bias_output[i];
            for (size_t j = 0; j < hidden_outputs.size(); ++j)
                sum += weights_hidden_output[i][j] * hidden_outputs[j];
            outputs[i] = sigmoid(sum);
        }

        return outputs;
    }

    std::vector<double> train(const std::vector<std::vector<double>> &X, const std::vector<std::vector<double>> &Y, int epochs) {
        std::vector<double> error_list;

        for (int epoch = 0; epoch < epochs; epoch++) {
            double total_error = 0.0;

            for (size_t i = 0; i < X.size(); i++) {
                // For each training input:
                // Compute hidden layer activations.
                std::vector<double> hidden_outputs;
                // Compute output predictions (between 0 and 1).
                std::vector<double> outputs = forward(X[i], hidden_outputs);

                // Compute output layer errors
                std::vector<double> output_errors(outputs.size());
                for (size_t j = 0; j < outputs.size(); ++j) {
                    output_errors[j] = (Y[i][j] - outputs[j]) * sigmoid_derivative(outputs[j]);
                    total_error += std::abs(Y[i][j] - outputs[j]);
                }

                // Compute hidden layer errors
                std::vector<double> hidden_errors(hidden_outputs.size());
                for (size_t j = 0; j < hidden_outputs.size(); ++j) {
                    double error = 0.0;
                    for (size_t k = 0; k < outputs.size(); ++k)
                        error += output_errors[k] * weights_hidden_output[k][j];
                    hidden_errors[j] = error * sigmoid_derivative(hidden_outputs[j]);
                }

                // Update weights hidden → output
                for (size_t j = 0; j < weights_hidden_output.size(); ++j) {
                    for (size_t k = 0; k < weights_hidden_output[j].size(); ++k)
                        weights_hidden_output[j][k] += LEARNING_RATE * output_errors[j] * hidden_outputs[k];
                    bias_output[j] += LEARNING_RATE * output_errors[j];
                }

                // Update weights input → hidden
                for (size_t j = 0; j < weights_input_hidden.size(); ++j) {
                    for (size_t k = 0; k < weights_input_hidden[j].size(); ++k)
                        weights_input_hidden[j][k] += LEARNING_RATE * hidden_errors[j] * X[i][k];
                    bias_hidden[j] += LEARNING_RATE * hidden_errors[j];
                }
            }

            error_list.push_back(total_error);
            if (total_error < 1) {
                std::cout << "Epoch " << epoch + 1 << " | Error: " << total_error << std::endl;
                break;
            } else if (epoch%100 == 0)
            {
                std::cout << "Epoch " << epoch + 1 << " | Error: " << total_error << std::endl;
            }
        }

        return error_list;
    }


    std::vector<double> predict(const std::vector<double> &inputs) {
        std::vector<double> hidden_outputs;
        return forward(inputs, hidden_outputs);
    }
};

void generateGraf(std::string gate, int time, std::vector<double> error_list){
    std::vector<std::pair<int, double>> points;
    for (size_t i = 0; i < error_list.size(); ++i)
        points.emplace_back(static_cast<int>(i + 1), error_list[i]);

    try {
        Gnuplot gp;
        gp << "set title 'MLPerceptron " << gate << " Training " << time <<" - Errors vs Epoch'\n";
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
    std::vector<std::vector<double>> Yand = {{0}, {0}, {0}, {1}};
    std::vector<std::vector<double>> Yor  = {{0}, {1}, {1}, {1}};
    std::vector<std::vector<double>> Yxor = {{0}, {1}, {1}, {0}};

    // AND training
    for (int i = 0; i < REPEAT_TIME; i++) {
        MLPerceptron perceptron(INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
        auto errors =  perceptron.train(X, Yand, TRAINING_TIMES);
        generateGraf("AND", i + 1, errors);

        std::cout << "\nPredictions AND gate:" << std::endl;
        for (size_t j = 0; j < X.size(); j++) {
            auto pred = perceptron.predict(X[j]);
            std::cout << X[j][0] << " AND " << X[j][1] << " = " << (pred[0] > 0.5 ? 1 : 0) << std::endl;
        }
    }

    std::cout << std::endl << "--------------------------------------------" << std::endl;

    // OR training
    for (int i = 0; i < REPEAT_TIME; i++) {
        MLPerceptron perceptron(INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
        auto errors = perceptron.train(X, Yor, TRAINING_TIMES);
        generateGraf("OR", i + 1, errors);

        std::cout << "\nPredictions OR gate:" << std::endl;
        for (size_t j = 0; j < X.size(); j++) {
            auto pred = perceptron.predict(X[j]);
            std::cout << X[j][0] << " OR " << X[j][1] << " = " << (pred[0] > 0.5 ? 1 : 0) << std::endl;
        }
    }

    std::cout << std::endl << "--------------------------------------------" << std::endl;

    // XOR training
    for (int i = 0; i < REPEAT_TIME; i++) {
        MLPerceptron perceptron(INPUT_SIZE, HIDDEN_SIZE, OUTPUT_SIZE);
        auto errors = perceptron.train(X, Yxor, TRAINING_TIMES);
        generateGraf("XOR", i + 1, errors);

        std::cout << "\nPredictions XOR gate:" << std::endl;
        for (size_t j = 0; j < X.size(); j++) {
            auto pred = perceptron.predict(X[j]);
            std::cout << X[j][0] << " XOR " << X[j][1] << " = " << (pred[0] > 0.5 ? 1 : 0) << std::endl;
        }
    }

    return 0;
}
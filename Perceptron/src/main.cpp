#include <vector>
#include <iostream>
#include <random>

const int TRAINING_TIMES = 10;
const double INIT_BIAS = 0.0;
const double LEARNING_RATE = 0.1;
const int  INPUT_SIZE = 2;

template <typename T> std::string vectorStringify(std::vector<T> v){
    std::string newString;
    for (auto i : v)
    {
        newString << i;
    }
    return newString;
}

class Perceptron
{
private:
    std::vector<double> weights;
    double learning_rate;
    double bias;

    int activation(double x)
    {
        return (x >= 0) ? 1 : 0;
    }
public:
    Perceptron(int input_size, double learning_rate = 0.1){
        this->learning_rate = learning_rate;
        this->bias = INIT_BIAS;
        this->weights.resize(input_size);
        for (double &w : this->weights)
        {
            w = 0;
            // w = ((double) rand() / RAND_MAX); // * 2 - 1;
        }
    };
    ~Perceptron(){

    };

    int predict(const std::vector<double> &inputs) {
        double sum = this->bias;
        for (size_t i = 0; i < inputs.size(); i++) {
            sum += weights[i] * inputs[i];
        }
        return activation(sum);
    }

    void train(const std::vector<std::vector<double>> &X, const std::vector<int> &Y, int epochs) {
        for (int e = 0; e < epochs; e++) {
            int errors = 0;
            for (size_t i = 0; i < X.size(); i++) {
                int output = predict(X[i]);
                // real result - network result
                int error = Y[i] - output;

                if (error != 0)
                {
                    errors++;
                    // Update rule
                    for (size_t j = 0; j < this->weights.size(); j++) {
                        this->weights[j] += this->learning_rate * error * X[i][j];
                        std::cout << "updated weight [" << j << "] to:" << this->weights[j] << std::endl;
                    }
                    this->bias += this->learning_rate * error;
                    std::cout << "bias updated to:" << this->bias << std::endl;
                }
            }
            if (errors == 0)
            {
                std::cout << "ending soon with " << e+1 << " epochs" << std::endl;
                return;
            }
            std::cout << "training ended" << std::endl;
        }
    }
};


int main()
{
    std::vector<std::vector<double>> X = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    std::vector<int> Y = {0, 0, 0, 1};

    Perceptron* perceptron = new Perceptron(INPUT_SIZE, LEARNING_RATE);

    std::cout << "starting training:" << std::endl;
    perceptron->train(X, Y, TRAINING_TIMES);

    std::cout << "Perceptron on AND gate:" << std::endl;
    for (size_t i = 0; i < X.size(); i++) {
        std::cout << X[i][0] << " AND " << X[i][1] << " = " << perceptron->predict(X[i]) << std::endl;
    }

    return 0;
}
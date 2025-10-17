    #include <vector>
    #include <iostream>
    #include <cmath>
    #include <random>

    const int TRAINING_TIMES = 10;
    const double INIT_BIAS = 0.0;
    const double LEARNING_RATE = 0.1;
    const int INPUT_SIZE = 2;

    template <typename T> std::string vectorStringify(const std::vector<T>& v) {
        std::string newString;
        for (auto i : v) {
            newString += std::to_string(i) + " ";
        }
        return newString;
    }

    class Adaline
    {
    private:
        std::vector<double> weights;
        double learning_rate;
        double bias;

        double netInput(const std::vector<double> &inputs) const {
            double sum = bias;
            for (size_t i = 0; i < inputs.size(); i++)
                sum += weights[i] * inputs[i];
            return sum;
        }

    public:
        Adaline(int input_size, double learning_rate = 0.1){
            this->learning_rate = learning_rate;
            this->bias = INIT_BIAS;
            this->weights.resize(input_size);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-0.5, 0.5);
            for (double &w : this->weights) {
                w = dis(gen);
            }
        }

        double predict(const std::vector<double> &inputs) const {
            double output = netInput(inputs);
            return (output >= 0.5) ? 1.0 : 0.0;
        }

        void train(const std::vector<std::vector<double>> &X, const std::vector<double> &Y, int epochs) {
            for (int e = 0; e < epochs; e++) {
                double sum_squared_error = 0.0;

                for (size_t i = 0; i < X.size(); i++) {
                    double output = netInput(X[i]);
                    double error = Y[i] - output;
                    sum_squared_error += error * error;

                    for (size_t j = 0; j < weights.size(); j++) {
                        weights[j] += learning_rate * error * X[i][j];
                    }
                    bias += learning_rate * error;
                }

                std::cout << "Epoch " << e + 1 << " | SSE=" << sum_squared_error << " | Weights=" << vectorStringify(weights) << " | Bias=" << bias << std::endl;

                if (sum_squared_error < 0.001)
                    break;
            }
        }
    };

    int main() {
        std::vector<std::vector<double>> X = {
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1}
        };
        std::vector<double> Y = {0, 1, 1, 0};

        Adaline adaline(INPUT_SIZE, LEARNING_RATE);

        std::cout << "=== Starting ADALINE training ===" << std::endl;
        adaline.train(X, Y, TRAINING_TIMES);

        std::cout << "\n=== ADALINE on AND gate ===" << std::endl;
        for (size_t i = 0; i < X.size(); i++) {
            std::cout << X[i][0] << " AND " << X[i][1] << " = " << adaline.predict(X[i]) << std::endl;
        }

        return 0;
    }

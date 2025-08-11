#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

int main() {
    // Eigen: define 2x2 matriz and 2x1 vetor
    Eigen::Matrix2f mat;
    mat <<  1, 2, 3, 4;

    Eigen::Vector2f vec(5, 6);

    // Multiplication matriz x vetor
    Eigen::Vector2f result = mat * vec;

    std::cout << "Multiplication result:\n" << result << std::endl;

    // OpenCV: create black img
    cv::Mat image = cv::Mat::zeros(300, 600, CV_8UC3);

    // write result on img
    std::string text = "result: [" + std::to_string(result[0]) + ", " + std::to_string(result[1]) + "]";
    cv::putText(image, text, cv::Point(30, 150), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

    // show img
    cv::imshow("OpenCV + Eigen", image);
    cv::waitKey(0);

    return 0;
}
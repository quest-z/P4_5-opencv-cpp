#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat img = cv::imread("test.png");       // 读图片
    if (img.empty()) {
        std::cout << "图片没读到，检查文件名" << std::endl;
        return 1;
    }
    std::cout << "图片尺寸: " << img.cols << "x" << img.rows << std::endl;
    return 0;
}
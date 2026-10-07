#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // ===== 读图（对应 cap.read() 的离线版，逐帧循环暂时不需要）=====
    cv::Mat frame = cv::imread("test.jpg");
    if (frame.empty()) {
        std::cout << "图片没读到" << std::endl;
        return 1;
    }

    // ===== BGR → HSV（对应 cv2.cvtColor）=====
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);   // 记住：OpenCV 永远是 BGR，C++ 也是

    // ===== 阈值分割（对应 cv2.inRange，用的是你 P4 校准过的值，S/V 已放松）=====
    cv::Scalar lower(86, 92, 40);
    cv::Scalar upper(106, 255, 255);               // S 上限放到 255，V 下限放到 92
    cv::Mat mask;
    cv::inRange(hsv, lower, upper, mask);

    // ===== 找轮廓（对应 cv2.findContours）=====
    std::vector<std::vector<cv::Point>> contours;  // 轮廓是"点集列表"，C++ 里类型要写全
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // ===== 最大轮廓（对应 max(contours, key=cv2.contourArea)）=====
    // Python 的 max(key=...) 是语法糖，C++ 里没有糖，手写循环——原理一样
    if (!contours.empty()) {                       // 对应 if contours:
        size_t biggest = 0;
        double maxArea = 0.0;
        for (size_t i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours[i]);
            if (area > maxArea) {
                maxArea = area;
                biggest = i;
            }
        }

        // ===== 面积过滤 + 画框（对应 contourArea>500 和 cv2.rectangle）=====
        if (maxArea > 500) {                       // 分层防御第二层，原封不动搬过来
            cv::Rect box = cv::boundingRect(contours[biggest]);
            cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);   // 绿框，BGR 顺序
            int cx = box.x + box.width / 2;
            int cy = box.y + box.height / 2;
            std::cout << "中心点: (" << cx << ", " << cy << ")" << std::endl;
        }
    }

    // ===== 保存结果（对应 cv2.imwrite；WSL 里弹出窗口需要额外条件，先写文件最稳）=====
    cv::imwrite("mask.jpg", mask);   // 把二值图也存下来，回 Windows 看
    std::cout << "轮廓数量: " << contours.size() << std::endl;
    for (size_t i = 0; i < contours.size(); i++) {
        std::cout << "轮廓" << i << " 面积: " << cv::contourArea(contours[i]) << std::endl;
    }
    cv::imwrite("result.jpg", frame);
    std::cout << "结果已保存 result.jpg，回 Windows 用看图软件打开" << std::endl;
    return 0;
}
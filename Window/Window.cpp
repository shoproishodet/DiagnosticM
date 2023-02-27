#include "Window.h"

const int Window::HEIGHT;
const int Window::WIDTH;

Window::Window(const std::string &owner) : ownerName(owner) {
    window = cv::Mat(cv::Size(WIDTH,HEIGHT),CV_8UC1, cv::Scalar(255,255,255));
    cv::namedWindow(ownerName);
}

void Window::Show() {
    cv::imshow(ownerName, window);
}

void Window::Clear() {
    cv::Mat(cv::Size(WIDTH,HEIGHT),CV_8UC1, cv::Scalar(255,255,255)).copyTo(window);
    Show();
}

cv::Mat& Window::GetRef() {
    return window;
}

std::pair<int, int> Window::GetSize() {
    return {HEIGHT, WIDTH};
}

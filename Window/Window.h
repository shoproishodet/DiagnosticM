#ifndef DIAGNOSTICM_WINDOW_H
#define DIAGNOSTICM_WINDOW_H

#include "IWindow.h"

#include <opencv2/opencv.hpp>

class Window final : public IWindow {
private:
    static const int HEIGHT = 800;
    static const int WIDTH = 600;

    cv::Mat window;
    std::string ownerName;
public:
    Window(const std::string &owner);
    void Show() final;
    std::pair<int, int> GetSize() final;
    cv::Mat& GetRef() final;
    void Clear();
};


#endif //DIAGNOSTICM_WINDOW_H

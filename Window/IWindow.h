#ifndef DIAGNOSTICM_IWINDOW_H
#define DIAGNOSTICM_IWINDOW_H

#include <string>
#include <opencv2/core/mat.hpp>

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void Show() = 0;
    virtual std::pair<int, int> GetSize() = 0;
    virtual cv::Mat& GetRef() = 0;
};


#endif

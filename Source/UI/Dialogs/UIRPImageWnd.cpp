#include "UI/Dialogs/UIRPImageWnd.h"

template __declspec(noinline) std::string::basic_string(const std::string&);

// OFFSET: 0x0086E310
// FUNCTION: fn_11068
UIRPImageWnd::UIRPImageWnd(std::string imageName, int imageCount)
    : UIWindow(1), m_imageName(imageName), m_imageCount(imageCount) {
}

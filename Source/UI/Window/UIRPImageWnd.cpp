#include "UI/Window/UIRPImageWnd.h"

// OFFSET: 0x0086E310
// FUNCTION: fn_11068
UIRPImageWnd::UIRPImageWnd(std::string imageName, int imageCount)
    : UIWindow(1), m_imageName(std::move(imageName)), m_imageCount(imageCount) {
}

#include "UI/Window/UIRPImageWnd.h"

// OFFSET: 0x0086E310
// FUNCTION: fn_11068
UIRPImageWnd::UIRPImageWnd(UIWindowContext& context, std::string imageName, int imageCount)
    : UIWindow(context, 1), m_imageName(imageName), m_imageCount(imageCount) {
}

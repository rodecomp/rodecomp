#include "UI/Window/UIRPImageWnd2.h"

// OFFSET: 0x0094EAF0
// FUNCTION: fn_12912
void UIRPImageWnd2::Paint() {
  if (this->m_drawImage != 1) {
    return;
  }

  this->SetSolid(this->m_context->primitives, this->geometry.width, this->geometry.height, 0);
  for (int i = 0; i < this->m_imageCount; ++i) {
    this->DrawNamed(this->m_context->primitives, 0, 0, this->m_imageName, 0xFFFFFFFF, 0, 0);
  }
}

// OFFSET: 0x00951470
// FUNCTION: fn_12951
void UIRPImageWnd2::SetImage(std::string imageName, int imageCount) {
  this->m_imageName = imageName;
  this->m_imageCount = imageCount;
  this->m_drawImage = 1;
}

// OFFSET: 0x00946F20
// FUNCTION: fn_12856
UIRPImageWnd2::UIRPImageWnd2(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 1), m_drawImage(0) {
}

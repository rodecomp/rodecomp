#include "UI/Chat/UIWhisperWnd.h"

// OFFSET: 0x008C9620
// FUNCTION: fn_11984
UIWhisperWnd::UIWhisperWnd(UIWindowContext& windows) : UIFrameWnd(windows, 0) {
  this->unknown_0100 = 0;
  this->m_code = std::string();
}

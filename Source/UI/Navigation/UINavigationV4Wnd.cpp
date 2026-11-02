#include "UI/Navigation/UINavigationV4Wnd.h"

// OFFSET: 0x005C2860
// FUNCTION: fn_03536
UINavigationRuideWnd::UINavigationRuideWnd(UIWindowContext& windows, UIListBoxContext& lists)
    : UIFrameWnd(windows, 0), m_listContext(&lists) {
}

// OFFSET: 0x005C28C0
// FUNCTION: fn_03537
UINavigationV4Wnd::UINavigationV4Wnd(UIWindowContext& windows) : UIFrameWnd(windows, 0) {
  this->unknown_0118 = 0;
  this->unknown_0100 = nullptr;
  this->unknown_011C = 2000;
}

// OFFSET: 0x005C2960
// FUNCTION: fn_03538
UINavigationroadiconWnd::UINavigationroadiconWnd(UIWindowContext& windows)
    : UIFrameWnd(windows, 0) {
}

#include "UI/Buttons/UIItemMoveInfo.h"

// OFFSET: 0x0086D4D0
// FUNCTION: fn_11046
UIItemMoveInfo::UIItemMoveInfo(UIWindowContext& windows, UIButtonContext& buttons, WindowManagerWindows& windowList, WindowVisibility& visibility)
    : UITextButton(windows, buttons), m_windows(&windowList), m_visibility(&visibility), m_item() {
}

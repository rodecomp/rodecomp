#include "UI/Buttons/UIItemMoveInfo.h"

// OFFSET: 0x0086D4D0
// FUNCTION: fn_11046
UIItemMoveInfo::UIItemMoveInfo(UIWindowContext& windows, WindowManagerWindows& windowList, WindowVisibility& visibility)
    : UITextButton(windows), m_windows(&windowList), m_visibility(&visibility), m_item() {
}

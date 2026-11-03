#include "UI/UIBasicCtrl.h"

// OFFSET: 0x0086C410
// FUNCTION: fn_11026
UIBasicItemCtrl::UIBasicItemCtrl(WindowManagerWindows& windowList)
    : UIWindow(0), m_itemType(-1), m_itemMask(-1), m_identified(0), m_tooltip(""), m_item(0), m_windows(&windowList) {
}

// OFFSET: 0x0086C4C0
// FUNCTION: fn_11027
UIBasicSkillCtrl::UIBasicSkillCtrl()
    : UIWindow(0), m_skillId(-1), m_level(0) {
}

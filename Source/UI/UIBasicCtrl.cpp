#include "UI/UIBasicCtrl.h"

// OFFSET: 0x0086C410
// FUNCTION: fn_11026
UIBasicItemCtrl::UIBasicItemCtrl(UIWindowContext& windows, WindowManagerWindows& windowList)
    : UIWindow(windows, 0), m_itemType(-1), m_itemMask(-1), m_identified(0), m_tooltip(""), m_item(0), m_windows(&windowList) {
}

// OFFSET: 0x0086C4C0
// FUNCTION: fn_11027
UIBasicSkillCtrl::UIBasicSkillCtrl(UIWindowContext& windows, ClientSkills& skills)
    : UIWindow(windows, 0), m_skillId(-1), m_skills(&skills), m_level(0) {
}

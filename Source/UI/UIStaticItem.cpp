#include "UI/UIStaticItem.h"

// OFFSET: 0x0086E970
// FUNCTION: fn_11079
UIStaticItem::UIStaticItem(UIWindowContext& windows, ItemDescriptions& descriptions, UIButtonContext& controls, WindowManagerWindows& windowList)
    : UIWindow(windows, 0), m_item(), m_descriptions(&descriptions), m_controls(&controls), m_windows(&windowList) {
}

// OFFSET: 0x00871A00
// FUNCTION: fn_11109
UIStaticItem::~UIStaticItem() {
}

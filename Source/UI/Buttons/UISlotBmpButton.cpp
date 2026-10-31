#include "UI/Buttons/UISlotBmpButton.h"

// OFFSET: 0x0086E8D0
// FUNCTION: fn_11078
UISlotBmpButton::UISlotBmpButton(UIWindowContext& windows, UIButtonContext& buttons, int index, bool)
    : UIBitmapButton(windows, buttons), m_index(index), unknown_0124(false), unknown_0128(0), m_slot(-1), icon(nullptr), unknown_0134(0), unknown_0136(5),
      m_inRange(false) {
}

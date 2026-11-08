#include "UI/Buttons/UISlotBmpButton.h"

// OFFSET: 0x0086E8D0
// FUNCTION: fn_11078
UISlotBmpButton::UISlotBmpButton(int index, bool)
    : UIBitmapButton(), m_index(index), m_selected(false), m_displayMode(0), m_slot(-1), icon(nullptr), m_animationTick(0), m_frameInterval(5),
      m_inRange(false) {
}

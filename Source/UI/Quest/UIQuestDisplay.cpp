#include "UI/Quest/UIQuestDisplay.h"

// OFFSET: 0x009515D0
// FUNCTION: fn_12953
void UIQuestDisplay::Slot11() {
  if (this->m_visibleSetting) {
    *this->m_visibleSetting = this->geometry.visible;
  }
}

#include "UI/Window/UINewSkillListWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

// OFFSET: 0x009B3E70
// FUNCTION: fn_13826
void UINewSkillListWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

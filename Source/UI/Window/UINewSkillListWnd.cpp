#include "UI/Window/UINewSkillListWnd.h"
#include "Client/Session.h"
#include "UI/Lists/UITabControl.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>

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

// OFFSET: 0x009B2C60
// FUNCTION: fn_13821
void UINewSkillListWnd::Slot35(int delta) {
  UIWindow* window = g_windowMgr.LookupWindow(46);
  if (window && window->geometry.visible) {
    static_cast<UINewSkillListWnd*>(window)->m_scrollBar->Slot35(delta);
    return;
  }

  if (this->m_scrollable) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, 7, step, 0, 0, 0);
  }
}

// OFFSET: 0x009B0370
// FUNCTION: fn_13805
std::string UINewSkillListWnd::ValueText(int id) {
  for (const NewSkillListValue& entry : this->m_values) {
    if (entry.id == id) {
      if (entry.value > 0) {
        char text[12];
        _itoa(entry.value, text, 10);
        return text;
      }

      break;
    }
  }

  return std::string();
}

// OFFSET: 0x009B0680
// FUNCTION: fn_13807
CSkillInfo UINewSkillListWnd::VisibleSkill(int row) {
  int index = 0;
  for (const CSkillInfo& skill : this->SkillList(this->m_tab, false)) {
    if (row + this->m_firstRow == index) {
      return skill;
    }
    ++index;
  }

  return CSkillInfo(0);
}

// OFFSET: 0x009B07C0
// FUNCTION: fn_13809
std::list<CSkillInfo>& UINewSkillListWnd::SkillList(int tab, bool live) {
  if (tab >= 0 && static_cast<int>(this->m_tabs->labels.size()) - 1 == tab) {
    return g_session.m_skillBundle.extra;
  }

  if (live) {
    return g_session.m_skillBundle.Group(tab);
  }

  return this->m_lists[tab];
}

// OFFSET: 0x009B0EE0
// FUNCTION: fn_13812
bool UINewSkillListWnd::IsExtraTab(int tab) {
  return tab >= 0 && static_cast<int>(this->m_tabs->labels.size()) - 1 == tab;
}

#include "UI/Window/UINewSkillListWnd.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/UserData/CharacterConfigSave.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };
}

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

// OFFSET: 0x009B0460
// FUNCTION: fn_13806
CSkillInfo UINewSkillListWnd::SkillAt(int index, bool live) {
  CSkillInfo result(0);
  int sequentialIndex = 0;
  const int gate = *reinterpret_cast<const int*>(reinterpret_cast<const char*>(&g_session) + 0xB4);
  for (const CSkillInfo& skill : this->SkillList(this->m_tab, live)) {
    int key = skill.unknown_0024;
    if (gate != 0) {
      key = sequentialIndex;
    }
    if (key == index) {
      return skill;
    }
    if (gate != 0) {
      ++sequentialIndex;
    }
  }
  return result;
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

// OFFSET: 0x009B0750

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

// OFFSET: 0x009B0820

// OFFSET: 0x009B0E40

// OFFSET: 0x009B0EE0
// FUNCTION: fn_13812
bool UINewSkillListWnd::IsExtraTab(int tab) {
  return tab >= 0 && static_cast<int>(this->m_tabs->labels.size()) - 1 == tab;
}

// OFFSET: 0x009B0FA0

// OFFSET: 0x009B1050

// OFFSET: 0x009B2160

// OFFSET: 0x009B2590

// OFFSET: 0x009B26D0

// OFFSET: 0x009B2810

// OFFSET: 0x009B28F0

// OFFSET: 0x009B29E0

// OFFSET: 0x009B2CD0

// OFFSET: 0x009B3040

// OFFSET: 0x009B31C0

// OFFSET: 0x009B3580

#include "UI/Window/UIHotKeyWnd.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIBalloonText.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowOrdering.h"
#include "Utility/Text/FormatString.h"

#include <cstring>
#include <string>

// OFFSET: 0x00932F70
// FUNCTION: fn_12721
void UIHotKeyWnd::Slot32(int, int) {
}

// OFFSET: 0x00933B30
// FUNCTION: fn_12734
void UIHotKeyWnd::UpdatePageButtons() {
  this->m_prevButton->Enable(this->m_page > 1);
  this->m_nextButton->Enable(this->m_page < this->m_pageCount);
  this->Invalidate();
}

// OFFSET: 0x00911300
// FUNCTION: fn_12513
UIHotKeyWnd::UIHotKeyWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_selectedRow(-1), m_rowsPerPage(18), unknown_00BC(17), unknown_00C0(0), unknown_00C4(20), unknown_00C8(3), unknown_00CC(3),
      unknown_00D0(20), unknown_00D4(80), unknown_00D8(135), m_balloon(nullptr), m_tab(0), m_page(1), m_pageCount(0), unknown_0114(0) {
}

// OFFSET: 0x00916760
// FUNCTION: fn_12585
void UIHotKeyWnd::SetHotkey(int tab, int index, HotkeyInfo info) {
  this->hotkeys[tab][index] = info;
}

// OFFSET: 0x00934360
// FUNCTION: fn_12740
void UIHotKeyWnd::SaveHotkeys() {
  for (int tab = 0; tab < 4; ++tab) {
    for (const auto& entry : this->hotkeys[tab]) {
      g_luaScript->Call("ChangeUserHotKey", "ddsdd", tab + 1, entry.first, entry.second.description.c_str(), entry.second.key1, entry.second.key2);
      if (tab == 1) {
        g_windowMgr.sub_AAB0B0();
      }
    }
  }

  std::string path = "SaveData\\UserKeys";
  path = path + ".lua";
  if (!path.empty()) {
    g_luaScript->Call("SaveUserHotKeys2", "s", path.c_str());
    char message[500];
    std::memset(message, 0, sizeof(message));
    StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), "%s", LookupMessage(3572));
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 1, 0, 0);
  }
}


// OFFSET: 0x0091BD80
// FUNCTION: fn_12609
int UIHotKeyWnd::RowFromPoint(int x, int y) {
  if (x < this->unknown_00C8 || x > this->geometry.width - this->unknown_00C8) {
    return -1;
  }

  if (y < this->unknown_00C4 + this->unknown_00BC + this->unknown_00CC) {
    return -1;
  }

  if (y > (this->geometry.height - this->unknown_00C0) - this->unknown_00CC) {
    return -1;
  }

  int row = (((y - this->unknown_00BC) - this->unknown_00C4) - this->unknown_00CC) / this->unknown_00D0;
  if (x > this->geometry.width / 2) {
    row += this->m_rowsPerPage;
  }

  int count = 0;
  if (g_session.m_hotkeyScript->Call("GetOriginalHotKeyListSize", "d>d", this->m_tab + 1, &count) && this->m_tab == 1) {
    int hidden = 0;
    for (int index = 30; index < count; ++index) {
      switch (index) {
      case 30:
      case 33:
      case 34:
      case 36:
      case 43:
      case 45:
      case 53:
      case 58:
      case 62:
      case 64:
      case 65:
      case 66:
      case 67:
        ++hidden;
        break;
      default:
        break;
      }
    }
    count -= hidden;
  }

  if (row < count) {
    return row;
  }
  return -1;
}

// OFFSET: 0x00930B80
// FUNCTION: fn_12693
void UIHotKeyWnd::Slot25(int x, int y) {
  if (y < this->unknown_00BC) {
    UIFrameWnd::Slot25(x, y);
  }

  const int previous = this->m_selectedRow;
  this->m_selectedRow = this->RowFromPoint(x, y);

  int count = 0;
  if (g_session.m_hotkeyScript->Call("GetOriginalHotKeyListSize", "d>d", this->m_tab + 1, &count) && this->m_tab == 1) {
    int hidden = 0;
    for (int index = 30; index < count; ++index) {
      switch (index) {
      case 30:
      case 33:
      case 34:
      case 36:
      case 43:
      case 45:
      case 53:
      case 58:
      case 62:
      case 64:
      case 65:
      case 66:
      case 67:
        ++hidden;
        break;
      default:
        break;
      }
    }
    count -= hidden;
  }

  if (this->m_selectedRow + (this->m_page * 9 - 9) * 4 < count) {
    if (this->m_selectedRow < 0) {
      this->m_balloon->Show(0);
    } else {
      int screenX = 0;
      int screenY = 0;
      this->ToScreen(screenX, screenY);
      const char* text = LookupMessage(0x5CE);
      this->m_balloon->SetText(text, 0x3C);
      this->m_balloon->m_textColor = 0xFFFFFF;
      this->m_balloon->m_outlineColor = 0;
      this->m_balloon->Move(
        (this->geometry.width - this->m_balloon->geometry.width) / 2 + screenX,
        (this->m_selectedRow % this->m_rowsPerPage - 1) * this->unknown_00D0 + this->unknown_00CC + this->unknown_00C4 + screenY + this->unknown_00BC
      );
      this->m_balloon->Show(1);
      reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this->m_balloon);
    }

    if (previous == this->m_selectedRow) {
      return;
    }
  }

  this->Invalidate();
}

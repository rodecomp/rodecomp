#include "UI/Window/UIHotKeyWnd.h"

#include "Contents/MessageTable.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"
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

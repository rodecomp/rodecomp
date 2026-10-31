#include "UI/Window/UIHotKeyWnd.h"
#include "UI/Chat/ChatSuggestions.h"
#include "Client/Session.h"
#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeTooltip.h"
#include "Contents/Hotkeys/UserHotkeyMgr.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/MessageTable.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/UIBalloonText.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UISerachStoreInfoWnd.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"
#include "Utility/Text/FormatString.h"

#include <cstring>
#include <string>

namespace {
  bool HiddenHotkeyIndex(int index) {
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
      return true;
    default:
      return false;
    }
  }

  int CountHotkeys(LuaScript& script, int tab) {
    int count = 0;
    if (script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count)) {
      if (tab == 1) {
        int hidden = 0;
        for (int index = 30; index < count; ++index) {
          hidden += HiddenHotkeyIndex(index);
        }
        return count - hidden;
      }
    }
    return count;
  }

  int MapHotkeyIndex(LuaScript& script, int tab, int index) {
    if (tab == 1 && index >= 30) {
      for (int candidate = 30; candidate <= index; ++candidate) {
        index += HiddenHotkeyIndex(candidate);
      }
    }
    int count = 0;
    script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count);
    return index >= count ? -1 : index;
  }

  void SetWindowText(UIWindow* window, const char* text) {
    if (!window) {
      return;
    }
    using SetTextFn = void(__thiscall*)(UIWindow*, const char*);
    void** vtable = *reinterpret_cast<void***>(window);
    reinterpret_cast<SetTextFn>(vtable[53])(window, text);
  }
}

// OFFSET: 0x0091FFD0
// FUNCTION: fn_12639
void UIHotKeyWnd::Slot15(int, int) {
  UIButtonContext& controls = *reinterpret_cast<UIButtonContext*>(nullptr);
  reinterpret_cast<AutomaticSkills*>(g_session.unknown_7FB4)->DisableAll();
  this->AddBaseButton(controls);
  this->AddCloseButton(controls, "", 0);

  this->m_balloon = new UITransBalloonText(*this->m_context, *reinterpret_cast<BalloonTextContext*>(nullptr));
  this->m_balloon->SetSize(this->geometry.width, 20);
  this->m_balloon->SetText(LookupMessage(1486), 60);
  this->m_balloon->Show(0);
  g_windowMgr.AddRendered(this->m_balloon);

  this->unknown_0100 = new UITabControl(*this->m_context, controls);
  this->unknown_0100->Configure(1, this->unknown_00C4 - 3, 20, 1, 1, 0, 0, 0, 0, 0, 0, 0);
  this->unknown_0100->Move(this->unknown_00C8, this->unknown_00CC + this->unknown_00BC);
  this->AddChild(this->unknown_0100);

  const char* tabs[4] = {LookupMessage(1491), LookupMessage(3595), LookupMessage(1492), LookupMessage(1493)};
  for (int index = 0; index < 4; ++index) {
    this->unknown_0100->AddTab(tabs[index], "");
  }

  {
    std::string path = "유저인터페이스\\";
    const char* names[4] = {"btn_ok", "btn_cancel", "btn_reset", "btn_close"};
    const int ids[4] = {184, 185, 363, 201};
    int x = 0;
    for (int index = 0; index < 4; ++index) {
      UIBitmapButton* button = new UIBitmapButton();
      button->SetBitmap((path + names[index] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((path + names[index] + "_a.bmp").c_str(), 1, 0);
      button->SetBitmap((path + names[index] + "_b.bmp").c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());

      const int id = ids[index];
      button->SetId(id);
      this->AddChild(button);
      if (id == 184) {
        x = this->geometry.width - 139;
      } else if (id == 185) {
        x = this->geometry.width - 92;
      } else if (id == 363) {
        x = 5;
      } else if (id == 201) {
        x = this->geometry.width - 46;
      }

      button->Move(x, this->geometry.height - 25);
    }
  }

  UICheckBox* checkBox = new UICheckBox();
  this->unknown_0108 = checkBox;
  checkBox->SetSize(150, 12);
  checkBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(1775));
  checkBox->Move(80, this->geometry.height - 22);
  checkBox->SetChecked(g_windowCloseByte);
  this->AddChild(this->unknown_0108);

  this->m_prevButton = new UIBitmapButton();
  UIBitmapButton* prevButton = static_cast<UIBitmapButton*>(this->m_prevButton);
  prevButton->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 0, 0);
  prevButton->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 1, 0);
  prevButton->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 2, 0);
  prevButton->SetSize(prevButton->BitmapWidth(), prevButton->BitmapHeight());
  prevButton->Move(this->geometry.width - 80, 25);
  prevButton->Enable(0);
  prevButton->SetTooltip(LookupMessage(1053));
  prevButton->SetId(316);
  this->AddChild(this->m_prevButton);

  this->m_nextButton = new UIBitmapButton();
  UIBitmapButton* nextButton = static_cast<UIBitmapButton*>(this->m_nextButton);
  nextButton->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 0, 0);
  nextButton->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 1, 0);
  nextButton->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 2, 0);
  nextButton->SetSize(nextButton->BitmapWidth(), nextButton->BitmapHeight());
  nextButton->Move(this->geometry.width - 20, 25);
  nextButton->Enable(0);
  nextButton->SetTooltip(LookupMessage(1054));
  nextButton->SetId(317);
  this->AddChild(this->m_nextButton);
}

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

// OFFSET: 0x0091C720
// FUNCTION: fn_12617
UIHotKeyWnd::HotkeyEntry UIHotKeyWnd::GetHotkey(int tab, int index, bool resolve) {
  HotkeyEntry entry;
  entry.index = resolve ? MapHotkeyIndex(*g_luaScript, tab, index) : index;

  if (entry.index != -1) {
    const auto it = this->hotkeys[tab].find(entry.index);
    if (it != this->hotkeys[tab].end() && !it->second.description.empty()) {
      entry.info = it->second;
    } else {
      entry.info = GetHotkeyInfo(*g_luaScript, tab, entry.index);
    }
  }

  return entry;
}

// OFFSET: 0x009317E0
// FUNCTION: fn_12708
void UIHotKeyWnd::ShowHotkeyTooltip(int x, int y) {
  const int index = this->RowFromPoint(x, y);
  if (index < 0) {
    return;
  }

  const int column = index / this->m_rowsPerPage;
  const int row = index % this->m_rowsPerPage;
  int screenX = this->geometry.width / 2 * column + 0x14;
  int screenY = (row - 1) * this->unknown_00D0 + this->unknown_00CC + this->unknown_00C4 + this->unknown_00BC;
  this->ToScreen(screenX, screenY);

  const HotkeyEntry entry = this->GetHotkey(this->m_tab, index + (this->m_page * 9 - 9) * 4, true);
  if (!entry.info.text.empty()) {
    reinterpret_cast<ModeTooltip*>(g_charViewMode)->Show(entry.info.text.c_str(), screenX, screenY, 0xFFFFFFFF, 0, 1);
  }
}

// OFFSET: 0x00931C80
// FUNCTION: fn_12710
void UISerachStoreInfoWnd::Slot28(int x, int y) {
  ITEM_INFO item = this->ItemAt(x, y);
  if (item.itemId.size() == 0) {
    return;
  }

  if ((x > 160 && x < 330) || (x > 5 && x < 50) || static_cast<unsigned int>(x - 51) <= 108) {
    int screenX = (x - 40) / 32 * 32 + 40;
    int screenY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(screenX, screenY);

    std::string text;
    if (static_cast<unsigned int>(x - 51) <= 108) {
      const auto entry = g_session.unknown_5BD4.begin() + ((this->unknown_00DC - 1) * 10 + this->unknown_00E0);
      if (entry != g_session.unknown_5BD4.end()) {
        text.assign(reinterpret_cast<const char*>(&entry->unknown_0000[8]));
        const std::size_t end = text.find('\r');
        if (end != std::string::npos) {
          text = text.substr(0, end);
        }

        this->m_context->cursor.Set(2, this->m_context->cursorTick);
      }
    } else {
      text = item.DisplayName(0);
    }

    if (text.size() != 0) {
      reinterpret_cast<ModeTooltip*>(g_charViewMode)->Show(text.c_str(), screenX, screenY, g_session.GetItemNameColor(item), 0, 1);
    }
  }

  this->Invalidate();
}







// OFFSET: 0x0092BEC0
// FUNCTION: fn_12663
void UIHotKeyWnd::Paint() {
  if (this->geometry.height == this->unknown_00BC) {
    return;
  }

  FontCache& fonts = this->m_context->fonts;
  const TextLocale& locale = this->m_context->locale;

  this->unknown_00BC = this->DrawTitleBar(fonts, locale, true, LookupMessage(1494), 0);
  this->unknown_00C0 = this->DrawButtonBar(2, 0, 1);
  this->DrawPanelBackground(this->unknown_00BC, 0, "");

  const int count = CountHotkeys(*g_luaScript, this->m_tab);
  int visibleRow = 0;

  for (int row = 0; row < count; ++row) {
    if (this->unknown_0114 && row < 36 * this->m_page - 36) {
      continue;
    }
    if (row >= 36 * this->m_page) {
      continue;
    }

    const HotkeyEntry entry = this->GetHotkey(this->m_tab, row, true);
    if (entry.info.description.empty()) {
      continue;
    }

    std::string text = entry.info.text;
    if (text.empty()) {
      text = entry.info.key1 ? LookupMessage(1700) : LookupMessage(1518);
    }

    const int halfWidth = this->geometry.width / 2;
    const int column = visibleRow / this->m_rowsPerPage;
    const int rowY = this->unknown_00BC + this->unknown_00C4 + this->unknown_00CC +
                     (visibleRow % this->m_rowsPerPage) * this->unknown_00D0;

    const int textWidth = this->TextWidth(fonts, locale, text.c_str(), 0, 0, 12, 0, 0);
    const int textX = (this->unknown_00D4 - textWidth) / 2;

    const int backgroundX = column ? this->geometry.width - this->unknown_00D4 - this->unknown_00C8
                                   : halfWidth - this->unknown_00D4;
    const int keyX = column ? halfWidth : this->unknown_00C8;

    std::uint32_t color = 0xDFFFFE;
    switch (this->m_tab) {
    case 0:
      color = 0xFFEFD6;
      break;
    case 3:
      color = 0xEFD6FF;
      break;
    default:
      break;
    }

    this->FillRegion(backgroundX, rowY, this->unknown_00D4, this->unknown_00D0 - 1, color);

    if (visibleRow == this->m_selectedRow) {
      this->FillRegion(keyX + 1, rowY, halfWidth - this->unknown_00C8 - 1, 19, 0xE1E1FF);
    }

    this->FillRegion(keyX, rowY - 1 + this->unknown_00D0, this->unknown_00D4 + this->unknown_00D8, 1, 0xC0C0C0);
    this->FillRegion(backgroundX, rowY, 1, this->unknown_00D0, 0xC0C0C0);

    const std::string fitted = this->FitDisplayText(130, entry.info.description.c_str(), 0, 12, 0);
    const int fittedWidth = this->TextWidth(fonts, locale, fitted.c_str(), 0, 0, 12, 0, 0);
    const int descriptionX = keyX + (halfWidth - this->unknown_00C8 - this->unknown_00D4 - fittedWidth) / 2;

    this->DrawTextAt(fonts, locale, descriptionX, rowY + 5, fitted.c_str(), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(fonts, locale, textX + backgroundX, rowY + 5, text.c_str(), 0, 0, 12, 0, 0, 0);

    ++visibleRow;
  }

  if (this->unknown_0114) {
    const std::string pages = FormatOwnedString("%d / %d", this->m_page, this->m_pageCount);
    const int pagesWidth = this->TextWidth(fonts, locale, pages.c_str(), 0, 0, 12, 0, 0);
    const int pageX =
      (this->m_prevButton->geometry.width - pagesWidth + 60) / 2 + this->m_prevButton->geometry.x;

    const std::string current = FormatOwnedString("%d", this->m_page);
    DrawOutlinedText(*this, pageX, 25, current.c_str(), 0, 0x7F3CFF, 0xFFFFFF, 0, 11, 1, 0);

    const int currentWidth = this->TextWidth(fonts, locale, current.c_str(), 0, 0, 12, 0, 0);
    const std::string total = FormatOwnedString(" / %d", this->m_pageCount);
    DrawOutlinedText(*this, pageX + currentWidth, 25, total.c_str(), 0, 0x666666, 0xFFFFFF, 0, 11, 1, 0);
  }

  const int bottomY = this->unknown_00BC + this->unknown_00C4 + this->unknown_00CC;
  const int innerWidth = this->geometry.width - 2 * this->unknown_00C8;
  const int innerHeight =
    this->geometry.height - 2 * this->unknown_00CC - this->unknown_00C4 - this->unknown_00BC - this->unknown_00C0;

  DrawRectangleBorder(*this, this->unknown_00C8, bottomY, innerWidth, innerHeight, 0xC0C0C0);
  this->FillRegion(this->geometry.width / 2, bottomY, 1, innerHeight, 0xC0C0C0);
  this->FillRegion(this->unknown_00C8, bottomY - 3, 1, 3, 0xC0C0C0);
  this->FillRegion(this->geometry.width - this->unknown_00C8 - 1, bottomY - 3, 1, 3, 0xC0C0C0);
  this->FillRegion(
    this->unknown_0100->geometry.width + this->unknown_00C8,
    bottomY - 3,
    innerWidth - this->unknown_0100->geometry.width,
    1,
    0xC0C0C0
  );
}

namespace {
  __declspec(noinline) int CountAllHotkeys(LuaScript& script, int tab) {
    int count = 0;
    if (script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count)) {
      if (tab == 1) {
        int hidden = 0;
        for (int index = 30; index < count; ++index) {
          hidden += HiddenHotkeyIndex(index);
        }
        return count - hidden;
      }
    }
    return count;
  }

  __declspec(noinline) int ResolveHotkeyIndex(LuaScript& script, int tab, int index) {
    if (tab == 1 && index >= 30) {
      for (int candidate = 30; candidate <= index; ++candidate) {
        index += HiddenHotkeyIndex(candidate);
      }
    }
    int count = 0;
    script.Call("GetOriginalHotKeyListSize", "d>d", tab + 1, &count);
    return index >= count ? -1 : index;
  }
}

// OFFSET: 0x0091CA10
// FUNCTION: fn_12620
void UIHotKeyWnd::ReloadHotkeys() {
  this->m_selectedRow = -1;
  for (auto& map : this->hotkeys) {
    map.clear();
  }

  for (int position = 0; position < 4; ++position) {
    const int tab = g_userHotkeyMgr->TabAt(position);
    const int count = CountAllHotkeys(reinterpret_cast<LuaScript&>(g_session), tab);
    for (int row = 0; row < count; ++row) {
      const int index = ResolveHotkeyIndex(reinterpret_cast<LuaScript&>(g_session), tab, row);
      if (index == -1) {
        continue;
      }

      std::string text;
      std::string description;
      int key1 = 0;
      int key2 = 0;
      const char* textValue = nullptr;
      const char* descriptionValue = nullptr;
      if (!g_luaScript->Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &textValue, &descriptionValue)) {
        continue;
      }

      description = descriptionValue;
      if (description.empty()) {
        continue;
      }

      if (!g_luaScript->Call("GetOriginalHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &textValue, &descriptionValue)) {
        continue;
      }

      text = textValue;
      description = descriptionValue;
      if (description.empty()) {
        continue;
      }

      HotkeyInfo& info = this->hotkeys[tab][index];
      info.key1 = key1;
      info.key2 = key2;
      info.text = text;
      info.description = description;
    }
  }

  this->Invalidate();
}

// OFFSET: 0x00916220
// FUNCTION: fn_12583
bool UIHotKeyWnd::AssignSelectedHotkey(int mode) {
  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);

  const int balloonY =
    (this->m_selectedRow % this->m_rowsPerPage - 1) * this->unknown_00D0 + this->unknown_00CC + this->unknown_00C4 + this->unknown_00BC;
  this->m_balloon->SetText(LookupMessage(0x5CE), 0x3C);
  this->m_balloon->m_textColor = 0xFFFFFF;
  this->m_balloon->m_outlineColor = 0;
  this->m_balloon->Move((this->geometry.width - this->m_balloon->geometry.width) / 2 + screenX, screenY + balloonY);
  this->Invalidate();

  if (this->m_selectedRow < 0) {
    return false;
  }

  int rejection = 0;
  bool checkConflict = false;
  if (this->m_tab == 1 && ResolveHotkeyIndex(reinterpret_cast<LuaScript&>(g_session), 1, this->m_selectedRow) == 31) {
    const int key = g_windowMgr.keyCode;
    const int modifier = g_windowMgr.modifierKey;
    if (key == 0x12 || modifier == 0x12 || key == 0x11 || modifier == 0x11 || key == 0x10 || modifier == 0x10) {
      rejection = 0x5D0;
    } else if (key == 0x20 || key == 0x90) {
      if (modifier != 0) {
        checkConflict = true;
      } else {
        rejection = 0x5CF;
      }
    } else if (mode == 4) {
      if (key == 0x14) {
        rejection = 0x5D0;
      } else if (modifier != 0 || g_windowMgr.IsSharedWindowId(key)) {
        checkConflict = true;
      } else {
        rejection = 0x5D0;
      }
    }
  } else if (mode == 4) {
    checkConflict = true;
  }

  if (rejection == 0) {
    if (checkConflict) {
      mode = this->CheckHotkeyConflict();
    }

    if (mode == 1) {
      return false;
    }

    if (mode == 2) {
      rejection = 0x5CF;
    } else if (mode == 3) {
      rejection = 0x5D0;
    }
  }

  if (rejection == 0) {
    const HotkeyEntry entry = this->GetHotkey(this->m_tab, this->m_selectedRow + (this->m_page * 9 - 9) * 4, true);
    HotkeyInfo info = entry.info;
    if (g_windowMgr.keyCode == 0x1B) {
      info.key1 = 0;
      info.key2 = 0;
      info.text = "";
    } else {
      info.key1 = g_windowMgr.keyCode;
      info.key2 = g_windowMgr.modifierKey;
      const char* text = nullptr;
      if (g_luaScript->Call("GetKeyDes", "dd>s", g_windowMgr.keyCode, g_windowMgr.modifierKey, &text) && text) {
        info.text = text;
      }
    }

    this->SetHotkey(this->m_tab, entry.index, info);
    this->Invalidate();
    return true;
  }

  this->m_balloon->SetText(LookupMessage(rejection), 0x3C);
  this->m_balloon->m_textColor = 0xFFFF;
  this->m_balloon->m_outlineColor = 0;
  this->m_balloon->Move((this->geometry.width - this->m_balloon->geometry.width) / 2 + screenX, screenY + balloonY);
  this->Invalidate();
  return false;
}

// OFFSET: 0x00916930
// FUNCTION: fn_12586
int UIHotKeyWnd::CheckHotkeyConflict() {
  char message[100];
  std::memset(message, 0, sizeof(message));

  for (int tab = 0; tab < 4; ++tab) {
    if ((this->m_tab == 0 && tab == 3) || (this->m_tab == 3 && tab == 0)) {
      continue;
    }

    const int count = CountAllHotkeys(reinterpret_cast<LuaScript&>(g_session), tab);
    for (int index = 0; index < count; ++index) {
      const HotkeyEntry entry = this->GetHotkey(tab, index, true);
      HotkeyInfo info = entry.info;
      if (info.key1 != g_windowMgr.keyCode || info.key2 != g_windowMgr.modifierKey) {
        continue;
      }

      if (index == this->m_selectedRow && tab == this->m_tab) {
        return 1;
      }

      StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), LookupMessage(0x5D1), info.description.c_str());
      if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(message, 2, true, false, 0, "Message", 280, 120, 0) != 187) {
        return 1;
      }

      info.key1 = 0;
      info.key2 = 0;
      info.text = "";
      this->SetHotkey(tab, entry.index, info);

      if (tab == 0 && this->m_tab != 0) {
        const int shortcutCount = CountAllHotkeys(reinterpret_cast<LuaScript&>(g_session), 3);
        for (int shortcut = 0; shortcut < shortcutCount; ++shortcut) {
          const HotkeyEntry other = this->GetHotkey(3, shortcut, true);
          HotkeyInfo otherInfo = other.info;
          if (otherInfo.key1 == g_windowMgr.keyCode && otherInfo.key2 == g_windowMgr.modifierKey) {
            otherInfo.key1 = 0;
            otherInfo.key2 = 0;
            otherInfo.text = "";
            this->SetHotkey(3, other.index, otherInfo);
          }
        }
      }

      return 0;
    }
  }

  return 0;
}

// OFFSET: 0x00935270
// FUNCTION: fn_12745
int UIHotKeyWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == 6) {
    switch (argument1) {
    case 184:
      ++g_userHotkeyMgr->m_changes;
      this->SaveHotkeys();
      g_windowMgr.CloseWindow(156);
      return 0;
    case 185:
      this->m_selectedRow = -1;
      for (auto& map : this->hotkeys) {
        map.clear();
      }
      this->Invalidate();
      return 0;
    case 201:
      g_windowMgr.CloseWindow(156);
      return 0;
    case 213:
      if (sender != this->unknown_0108) {
        return 0;
      }

      g_windowCloseByte = argument2 != 0;
      if (g_chatSuggestions) {
        if (argument2 != 0) {
          reinterpret_cast<UIWindow*>(g_chatSuggestions)->Invalidate();
          return 0;
        }

        reinterpret_cast<ChatInputFocus*>(g_chatSuggestions)->Toggle();
      }
      return 0;
    case 316:
      if (this->m_page > 1) {
        --this->m_page;
      }
      this->UpdatePageButtons();
      return 0;
    case 317:
      if (this->m_page < this->m_pageCount) {
        ++this->m_page;
      }
      this->UpdatePageButtons();
      return 0;
    case 363:
      this->ReloadHotkeys();
      return 0;
    default:
      return 0;
    }
  }

  if (message == 22) {
    if (sender == g_windowMgr.LookupWindow(303)) {
      this->unknown_0100->SelectTab(static_cast<int>(argument1));
      return 0;
    }

    const int tab = g_userHotkeyMgr->TabAt(static_cast<int>(argument1));
    if (this->m_tab != tab || tab != 1) {
      this->m_prevButton->Enable(0);
      this->m_nextButton->Enable(0);
    }

    if (this->m_tab == tab) {
      return 0;
    }

    this->m_selectedRow = -1;
    this->m_tab = tab;
    this->unknown_0114 = 0;
    this->m_page = 1;

    const int count = CountAllHotkeys(reinterpret_cast<LuaScript&>(g_session), tab);
    if (count > 36) {
      this->unknown_0114 = 1;
      this->m_pageCount = count / 36;
      if (count % 36 != 0) {
        ++this->m_pageCount;
      }
      this->m_nextButton->Enable(1);
    }

    this->Invalidate();
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

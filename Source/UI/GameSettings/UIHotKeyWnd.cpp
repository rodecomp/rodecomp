#include "Contents/Hotkeys/InterfaceHotkey.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/GameSettings/UIHotKeyWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "Client/Session/Session.h"
#include "Contents/Hotkeys/UserHotkeyMgr.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/Tables/MessageTable.h"
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
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Shop/UISerachStoreInfoWnd.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <cstring>
#include <string>

namespace {
  bool HiddenHotkeyIndex(int index) {
    switch (index) {
    case HK_BATTLEFIELDWND:
    case HK_PARTY_RECRUIT:
    case HK_PARTY_BOOKING_LIST:
    case HK_EXMACRO_SETUPWND:
    case HK_BUTTONS_RELOCATE:
    case HK_MENUICON_HIDE:
    case HK_TWITTERWND:
    case HK_HUNTINGQUEST_INFO_ONOFF:
    case HK_HIDDEN_62:
    case HK_HIDDEN_64:
    case HK_HIDDEN_65:
    case HK_HIDDEN_66:
    case HK_HIDDEN_67:
      return true;
    default:
      return false;
    }
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
void UIHotKeyWnd::OnCreate(int, int) {
  g_session.m_automaticSkills.DisableAll();
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->m_balloon = new UITransBalloonText();
  this->m_balloon->SetSize(this->geometry.width, 20);
  this->m_balloon->SetText(LookupMessage(MSI_HOTKEYWND_NOTICE1), 60);
  this->m_balloon->Show(0);
  g_windowMgr.AddRendered(this->m_balloon);

  this->m_tabs = new UITabControl();
  this->m_tabs->Configure(1, this->m_tabHeight - 3, 20, 1, 1, 0, 0, 0, 0, 0, 0, 0);
  this->m_tabs->Move(this->m_horizontalMargin, this->m_verticalMargin + this->m_titleHeight);
  this->AddChild(this->m_tabs);

  const char* tabs[4] = {LookupMessage(MSI_HOTKEYWND_TAB1), LookupMessage(MSI_HOTKEYGUIDEWND_UP), LookupMessage(MSI_HOTKEYWND_TAB2), LookupMessage(MSI_HOTKEYWND_TAB3)};
  for (int index = 0; index < 4; ++index) {
    this->m_tabs->AddTab(tabs[index], "");
  }

  {
    std::string path = "유저인터페이스\\";
    const char* names[4] = {"btn_ok", "btn_cancel", "btn_reset", "btn_close"};
    const int ids[4] = {ID_OK, ID_CANCEL, ID_RESET, ID_CLOSE};
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
      if (id == ID_OK) {
        x = this->geometry.width - 139;
      } else if (id == ID_CANCEL) {
        x = this->geometry.width - 92;
      } else if (id == ID_RESET) {
        x = 5;
      } else if (id == ID_CLOSE) {
        x = this->geometry.width - 46;
      }

      button->Move(x, this->geometry.height - 25);
    }
  }

  this->m_optionCheckBox = new UICheckBox();
  this->m_optionCheckBox->SetSize(150, 12);
  static_cast<UICheckBox*>(this->m_optionCheckBox)->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_CHATMODE_ONOFF));
  this->m_optionCheckBox->Move(80, this->geometry.height - 22);
  static_cast<UICheckBox*>(this->m_optionCheckBox)->SetChecked(g_windowMgr.chatModeChanged);
  this->AddChild(this->m_optionCheckBox);

  this->m_prevButton = new UIBitmapButton();
  static_cast<UIBitmapButton*>(this->m_prevButton)->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 0, 0);
  static_cast<UIBitmapButton*>(this->m_prevButton)->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 1, 0);
  static_cast<UIBitmapButton*>(this->m_prevButton)->SetBitmap("유저인터페이스\\basic_interface\\arw_left.bmp", 2, 0);
  this->m_prevButton->SetSize(static_cast<UIBitmapButton*>(this->m_prevButton)->BitmapWidth(), static_cast<UIBitmapButton*>(this->m_prevButton)->BitmapHeight());
  this->m_prevButton->Move(this->geometry.width - 80, 25);
  this->m_prevButton->Enable(0);
  static_cast<UIBitmapButton*>(this->m_prevButton)->SetTooltip(LookupMessage(MSI_PREV_PAGE));
  this->m_prevButton->SetId(316);
  this->AddChild(this->m_prevButton);

  this->m_nextButton = new UIBitmapButton();
  static_cast<UIBitmapButton*>(this->m_nextButton)->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 0, 0);
  static_cast<UIBitmapButton*>(this->m_nextButton)->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 1, 0);
  static_cast<UIBitmapButton*>(this->m_nextButton)->SetBitmap("유저인터페이스\\basic_interface\\arw_right.bmp", 2, 0);
  this->m_nextButton->SetSize(static_cast<UIBitmapButton*>(this->m_nextButton)->BitmapWidth(), static_cast<UIBitmapButton*>(this->m_nextButton)->BitmapHeight());
  this->m_nextButton->Move(this->geometry.width - 20, 25);
  this->m_nextButton->Enable(0);
  static_cast<UIBitmapButton*>(this->m_nextButton)->SetTooltip(LookupMessage(MSI_NEXT_PAGE));
  this->m_nextButton->SetId(317);
  this->AddChild(this->m_nextButton);
}

// OFFSET: 0x00932F70
// FUNCTION: fn_12721
void UIHotKeyWnd::OnRBtnDown(int, int) {
}

// OFFSET: 0x00933B30
// FUNCTION: fn_12734
void UIHotKeyWnd::UpdatePageButtons() {
  if (this->m_page > 1) {
    this->m_prevButton->Enable(1);
  } else {
    this->m_prevButton->Enable(0);
  }
  if (this->m_page < this->m_pageCount) {
    this->m_nextButton->Enable(1);
  } else {
    this->m_nextButton->Enable(0);
  }
  this->Invalidate();
}

// OFFSET: 0x00911300
// FUNCTION: fn_12513
UIHotKeyWnd::UIHotKeyWnd()
    : UIFrameWnd(0), m_selectedRow(-1), m_rowsPerPage(18), m_titleHeight(17), m_buttonBarHeight(0), m_tabHeight(20), m_horizontalMargin(3), m_verticalMargin(3),
      m_rowHeight(20), m_keyColumnWidth(80), m_descriptionColumnWidth(135), m_balloon(nullptr), m_tab(0), m_page(1), m_pageCount(0), m_paged(0) {
}

// OFFSET: 0x00916760
// FUNCTION: fn_12585
void UIHotKeyWnd::SetHotkey(int tab, int index, HotkeyInfo info) {
  this->hotkeys[tab][index] = info;
}

// OFFSET: 0x00934360
// FUNCTION: fn_12740
void UIHotKeyWnd::SaveHotkeys() {
  unsigned int tab = 0;
  std::map<int, HotkeyInfo>* tables = this->hotkeys;
  for (; tab < 4; ++tab) {
    for (const auto& entry : tables[tab]) {
      g_session.m_hotkeyScript->Call("ChangeUserHotKey", "ddsdd", tab + 1, entry.first, entry.second.description.c_str(), entry.second.key1, entry.second.key2);
      if (tab == 1) {
        g_windowMgr.RefreshGameSettingHotkeys();
      }
    }
  }

  std::string path = "SaveData\\UserKeys";
  path = path + ".lua";
  if (!path.empty()) {
    g_session.m_hotkeyScript->Call("SaveUserHotKeys2", "s", path.c_str());
    char message[500];
    std::memset(message, 0, sizeof(message));
    StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), "%s", LookupMessage(MSI_TRANSLATE_TITLE));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 0xFFFF, 1, 0, 0);
  }
}


// OFFSET: 0x0091BD80
// FUNCTION: fn_12609
int UIHotKeyWnd::RowFromPoint(int x, int y) {
  if (x < this->m_horizontalMargin || x > this->geometry.width - this->m_horizontalMargin) {
    return -1;
  }

  if (y < this->m_tabHeight + this->m_titleHeight + this->m_verticalMargin) {
    return -1;
  }

  if (y > (this->geometry.height - this->m_buttonBarHeight) - this->m_verticalMargin) {
    return -1;
  }

  int row = (((y - this->m_titleHeight) - this->m_tabHeight) - this->m_verticalMargin) / this->m_rowHeight;
  if (x > this->geometry.width / 2) {
    row += this->m_rowsPerPage;
  }

  const int count = g_session.HotkeyCount(this->m_tab);

  if (row < count) {
    return row;
  }
  return -1;
}

// OFFSET: 0x00930B80
// FUNCTION: fn_12693
void UIHotKeyWnd::OnLBtnDown(int x, int y) {
  if (y < this->m_titleHeight) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->m_selectedRow;
  this->m_selectedRow = this->RowFromPoint(x, y);

  const int count = g_session.HotkeyCount(this->m_tab);

  if (this->m_selectedRow + (this->m_page * 9 - 9) * 4 < count) {
    if (this->m_selectedRow > -1) {
      int screenX = 0;
      int screenY = 0;
      this->ToScreen(screenX, screenY);
      const char* text = LookupMessage(MSI_HOTKEYWND_NOTICE1);
      this->m_balloon->SetText(text, 0x3C);
      {
        UIBalloonText* balloon = this->m_balloon;
        balloon->m_textColor = 0xFFFFFF;
        balloon->m_outlineColor = 0;
      }
      this->m_balloon->Move(
        (this->geometry.width - this->m_balloon->geometry.width) / 2 + screenX,
        (this->m_selectedRow % this->m_rowsPerPage - 1) * this->m_rowHeight + this->m_verticalMargin + this->m_tabHeight + screenY + this->m_titleHeight
      );
      this->m_balloon->Show(1);
      g_windowMgr.Promote(this->m_balloon);
    } else {
      this->m_balloon->Show(0);
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
  HotkeyInfo info;
  if (resolve) {
    index = ResolveHotkeyIndex(*g_session.m_hotkeyScript, tab, index);
  }

  if (index != -1) {
    const auto it = this->hotkeys[tab].find(index);
    if (it != this->hotkeys[tab].end()) {
      info = it->second;
    }
    if (it == this->hotkeys[tab].end() || info.description.empty()) {
      info = GetHotkeyInfo(*g_session.m_hotkeyScript, tab, index);
    }
  }

  return HotkeyEntry{index, info};
}

// OFFSET: 0x009317E0
// FUNCTION: fn_12708
void UIHotKeyWnd::OnMouseHover(int x, int y) {
  const int index = this->RowFromPoint(x, y);
  if (index <= -1) {
    return;
  }

  const int column = index / this->m_rowsPerPage;
  const int row = index % this->m_rowsPerPage;
  int screenX = 0;
  int screenY = 0;
  const int x0 = this->geometry.width / 2 * column + 0x14;
  const int y0 = (row - 1) * this->m_rowHeight + this->m_verticalMargin + this->m_tabHeight + this->m_titleHeight;
  this->ToScreen(screenX, screenY);

  const HotkeyEntry entry = this->GetHotkey(this->m_tab, index + (this->m_page * 9 - 9) * 4, true);
  if (entry.info.description.size() > 0) {
    g_modeMgr.gameMode->ShowTooltip(entry.info.description.c_str(), x0 + screenX, y0 + screenY, 0xFFFFFFFF, 0, 1);
  }
}

// OFFSET: 0x00931C80
// FUNCTION: fn_12710
void UISerachStoreInfoWnd::OnMouseHover(int x, int y) {
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
      const auto entry = g_session.storeSearchResults.begin() + ((this->m_page - 1) * 10 + this->m_hoverRow);
      if (entry != g_session.storeSearchResults.end()) {
        text.assign(entry->storeName);
        const std::size_t end = text.find('\r');
        if (end != std::string::npos) {
          text = text.substr(0, end);
        }

        g_modeMgr.gameMode->SetCursorAction(2);
      }
    } else {
      text = item.DisplayName(0);
    }

    if (text.size() != 0) {
      g_modeMgr.gameMode->ShowTooltip(text.c_str(), screenX, screenY, g_session.GetItemNameColor(item), 0, 1);
    }
  }

  this->Invalidate();
}







// OFFSET: 0x0092BEC0
// FUNCTION: fn_12663
void UIHotKeyWnd::Paint() {
  if (this->geometry.height == this->m_titleHeight) {
    return;
  }


  this->m_titleHeight = this->DrawTitleBar(true, LookupMessage(MSI_HOTKEYWND_TITLE), 0);
  this->m_buttonBarHeight = this->DrawButtonBar(2, 0, 1);
  this->DrawPanelBackground(this->m_titleHeight, 0, "");

  const int count = HotkeyCount(*g_session.m_hotkeyScript, this->m_tab);
  int visibleRow = 0;

  for (int row = 0; row < count; ++row) {
    if (this->m_paged && row < 36 * this->m_page - 36) {
      continue;
    }
    if (row >= 36 * this->m_page) {
      continue;
    }

    HotkeyInfo info = this->GetHotkey(this->m_tab, row, true).info;
    if (!info.description.empty()) {
      if (info.text.empty()) {
        info.text = info.key1 ? LookupMessage(MSI_HOTKEY_UNKOWN) : LookupMessage(MSI_HOTKEY_NOTHING);
      }

      const int halfWidth = this->geometry.width / 2;
      const int column = visibleRow / this->m_rowsPerPage;
      const int rowY = this->m_titleHeight + this->m_tabHeight + this->m_verticalMargin +
                       (visibleRow % this->m_rowsPerPage) * this->m_rowHeight;

      const int textWidth = this->TextWidth(info.text.c_str(), 0, 0, 12, 0, 0);
      const int textX = (this->m_keyColumnWidth - textWidth) / 2;

      const int backgroundX = column ? this->geometry.width - this->m_keyColumnWidth - this->m_horizontalMargin
                                     : halfWidth - this->m_keyColumnWidth;
      const int keyX = column ? halfWidth : this->m_horizontalMargin;

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

      this->FillRegion(backgroundX, rowY, this->m_keyColumnWidth, this->m_rowHeight - 1, color);

      if (visibleRow == this->m_selectedRow) {
        this->FillRegion(keyX + 1, rowY, halfWidth - this->m_horizontalMargin - 1, 19, 0xE1E1FF);
      }

      this->FillRegion(keyX, rowY - 1 + this->m_rowHeight, this->m_keyColumnWidth + this->m_descriptionColumnWidth, 1, 0xC0C0C0);
      this->FillRegion(backgroundX, rowY, 1, this->m_rowHeight, 0xC0C0C0);

      const std::string fitted = this->FitDisplayText(130, info.description.c_str(), 0, 12, 0);
      const int fittedWidth = this->TextWidth(fitted.c_str(), 0, 0, 12, 0, 0);
      const int descriptionX = keyX + (halfWidth - this->m_horizontalMargin - this->m_keyColumnWidth - fittedWidth) / 2;

      this->DrawTextAt(descriptionX, rowY + 5, fitted.c_str(), 0, 0, 12, 0, 0, 0);
      this->DrawTextAt(textX + backgroundX, rowY + 5, info.text.c_str(), 0, 0, 12, 0, 0, 0);

      ++visibleRow;
    }
  }

  if (this->m_paged) {
    std::string text = FormatOwnedString("%d / %d", this->m_page, this->m_pageCount);
    const int pagesWidth = this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);
    const int pageX =
      (this->m_prevButton->geometry.width - pagesWidth + 60) / 2 + this->m_prevButton->geometry.x;

    text = static_cast<const std::string&>(FormatOwnedString("%d", this->m_page));
    this->DrawOutlinedText(pageX, 25, text.c_str(), 0, 0x7F3CFF, 0xFFFFFF, 0, 11, 1, 0);

    const int currentWidth = this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);
    text = static_cast<const std::string&>(FormatOwnedString(" / %d", this->m_pageCount));
    this->DrawOutlinedText(pageX + currentWidth, 25, text.c_str(), 0, 0x666666, 0xFFFFFF, 0, 11, 1, 0);
  }

  const int bottomY = this->m_titleHeight + this->m_tabHeight + this->m_verticalMargin;
  const int innerWidth = this->geometry.width - 2 * this->m_horizontalMargin;
  const int innerHeight =
    this->geometry.height - 2 * this->m_verticalMargin - this->m_tabHeight - this->m_titleHeight - this->m_buttonBarHeight;

  this->DrawRectOutline(this->m_horizontalMargin, bottomY, innerWidth, innerHeight, 0xC0C0C0);
  this->FillRegion(this->geometry.width / 2, bottomY, 1, innerHeight, 0xC0C0C0);
  this->FillRegion(this->m_horizontalMargin, bottomY - 3, 1, 3, 0xC0C0C0);
  this->FillRegion(this->geometry.width - this->m_horizontalMargin - 1, bottomY - 3, 1, 3, 0xC0C0C0);
  this->FillRegion(
    this->m_tabs->geometry.width + this->m_horizontalMargin,
    bottomY - 3,
    innerWidth - this->m_tabs->geometry.width,
    1,
    0xC0C0C0
  );
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
    const int count = HotkeyCount(*g_session.m_hotkeyScript, tab);
    for (int row = 0; row < count; ++row) {
      const int index = ResolveHotkeyIndex(*g_session.m_hotkeyScript, tab, row);
      if (index == -1) {
        continue;
      }

      std::string text;
      std::string description;
      int key1 = 0;
      int key2 = 0;
      const char* textValue = nullptr;
      const char* descriptionValue = nullptr;
      if (!g_session.m_hotkeyScript->Call("GetUserHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &textValue, &descriptionValue)) {
        continue;
      }

      description = descriptionValue;
      if (description.empty()) {
        continue;
      }

      if (!g_session.m_hotkeyScript->Call("GetOriginalHotKeyInfo", "dd>ddss", tab + 1, index, &key1, &key2, &textValue, &descriptionValue)) {
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
    (this->m_selectedRow % this->m_rowsPerPage - 1) * this->m_rowHeight + this->m_verticalMargin + this->m_tabHeight + this->m_titleHeight;
  this->m_balloon->SetText(LookupMessage(MSI_HOTKEYWND_NOTICE1), 0x3C);
  {
    UIBalloonText* balloon = this->m_balloon;
    balloon->m_textColor = 0xFFFFFF;
    balloon->m_outlineColor = 0;
  }
  this->m_balloon->Move((this->geometry.width - this->m_balloon->geometry.width) / 2 + screenX, screenY + balloonY);
  this->Invalidate();

  if (this->m_selectedRow < 0) {
    return false;
  }

  int rejection = 0;
  bool checkConflict = false;
  if (this->m_tab == 1 && ResolveHotkeyIndex(*g_session.m_hotkeyScript, 1, this->m_selectedRow) == 31) {
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

    switch (mode) {
      case 1:
        return false;
      case 2:
        rejection = 0x5CF;
        break;
      case 3:
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
      if (g_session.m_hotkeyScript->Call("GetKeyDes", "dd>s", g_windowMgr.keyCode, g_windowMgr.modifierKey, &text) && text) {
        info.text = text;
      }
    }

    this->SetHotkey(this->m_tab, entry.index, info);
    this->Invalidate();
    return true;
  }

  this->m_balloon->SetText(LookupMessage(rejection), 0x3C);
  {
    UIBalloonText* balloon = this->m_balloon;
    balloon->m_textColor = 0xFFFF;
    balloon->m_outlineColor = 0;
  }
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

    const int count = g_session.HotkeyCount(tab);
    for (int index = 0; index < count; ++index) {
      const HotkeyEntry entry = this->GetHotkey(tab, index, true);
      HotkeyInfo info = entry.info;
      if (info.key1 != g_windowMgr.keyCode || info.key2 != g_windowMgr.modifierKey) {
        continue;
      }

      if (index == this->m_selectedRow && tab == this->m_tab) {
        return 1;
      }

      StringFormat(message, sizeof(message), static_cast<std::size_t>(-1), LookupMessage(MSI_HOTKEYWND_NOTICE4), info.description.c_str());
      if (g_windowMgr.Confirm(message, 2, true, false, 0, "Message", 280, 120, 0) != ID_YES) {
        return 1;
      }

      info.key1 = 0;
      info.key2 = 0;
      info.text = "";
      this->SetHotkey(tab, entry.index, info);

      if (tab == 0 && this->m_tab != 0) {
        const int shortcutCount = g_session.HotkeyCount(3);
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
  switch (message) {
  case UM_TAB: {
    if (g_windowMgr.LookupWindow(WID_HOTKEYGUIDEWND) == sender) {
      this->m_tabs->SelectTab(static_cast<int>(argument1));
      break;
    }

    const int tab = g_userHotkeyMgr->TabAt(static_cast<int>(argument1));
    if (this->m_tab != tab || tab != 1) {
      this->m_prevButton->Enable(0);
      this->m_nextButton->Enable(0);
    }

    if (this->m_tab != tab) {
      this->m_selectedRow = -1;
      this->m_tab = tab;
      this->m_paged = 0;
      this->m_page = 1;

      const int count = g_session.HotkeyCount(tab);
      if (count > 36) {
        this->m_paged = 1;
        this->m_pageCount = count / 36;
        if (count % 36 != 0) {
          ++this->m_pageCount;
        }
        this->m_nextButton->Enable(1);
      }

      this->Invalidate();
    }
    break;
  }
  case UM_COMMAND:
    switch (argument1) {
    case ID_CANCEL:
      this->m_selectedRow = -1;
      for (int tab = 0; tab < 4; ++tab) {
        this->hotkeys[tab].clear();
      }
      this->Invalidate();
      break;
    case ID_OK:
      ++g_userHotkeyMgr->m_changes;
      this->SaveHotkeys();
      g_windowMgr.CloseWindow(WID_HOTKEYWND);
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_HOTKEYWND);
      break;
    case ID_RESET:
      this->ReloadHotkeys();
      break;
    case ID_CHECK:
      if (sender == this->m_optionCheckBox) {
        g_windowMgr.chatModeChanged = argument2 != 0;
        if (ChatWindow()) {
          if (argument2 != 0) {
            ChatWindow()->Invalidate();
          } else {
            ChatWindow()->ToggleInput();
          }
        }
      }
      break;
    case ID_PREV_PAGE:
      if (this->m_page > 1) {
        --this->m_page;
      }
      this->UpdatePageButtons();
      break;
    case ID_NEXT_PAGE:
      if (this->m_page < this->m_pageCount) {
        ++this->m_page;
      }
      this->UpdatePageButtons();
      break;
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

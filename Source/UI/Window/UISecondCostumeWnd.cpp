#include "UI/Window/UISecondCostumeWnd.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIListBoxNavi.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Session.h"
#include "Script/LuaScript.h"
// OFFSET: 0x00949BA0
// FUNCTION: fn_12885
void UISecondCostumeWnd::LoadLists() {
  const int job = g_session.GetDisplayJob();
  const int sex = g_session.SyncUnknown0E98();

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_luaScript->Call("_get_DressHeadPaletteList", "dd>sd", sex, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.unknown_0018 = value;
    this->m_entries[0].push_back(entry);
  }

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_luaScript->Call("_get_Headstylelist", "dd>sd", sex, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.unknown_0018 = value;
    this->m_entries[1].push_back(entry);
  }

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_luaScript->Call("_get_DressBodyPaletteList", "dd>sd", sex, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.unknown_0018 = value;
    this->m_entries[2].push_back(entry);
  }

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_luaScript->Call("_get_dresslist", "ddd>sd", sex, job, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.unknown_0018 = value;
    this->m_entries[3].push_back(entry);
  }

  this->SelectSavedEntries();
}

// OFFSET: 0x0094A2D0
// FUNCTION: fn_12886
void UISecondCostumeWnd::SelectSavedEntries() {
  unsigned int index;
  for (index = 0; index < this->m_entries[0].size(); ++index) {
    if (this->m_entries[0][index].unknown_0018 == g_session.m_headPalette) {
      break;
    }
  }
  this->m_selected[0] = index < this->m_entries[0].size() ? index : 0;

  for (index = 0; index < this->m_entries[1].size(); ++index) {
    if (this->m_entries[1][index].unknown_0018 == g_session.m_head) {
      break;
    }
  }
  this->m_selected[1] = index < this->m_entries[1].size() ? index : 0;

  for (index = 0; index < this->m_entries[2].size(); ++index) {
    if (this->m_entries[2][index].unknown_0018 == g_session.m_bodyStyle) {
      break;
    }
  }
  this->m_selected[2] = index < this->m_entries[2].size() ? index : 0;

  for (index = 0; index < this->m_entries[3].size(); ++index) {
    if (this->m_entries[3][index].unknown_0018 == g_session.m_bodyCostume) {
      break;
    }
  }
  this->m_selected[3] = index < this->m_entries[3].size() ? index : 0;
}

// OFFSET: 0x0094C5B0
// FUNCTION: fn_12898
void UISecondCostumeWnd::Slot15(int width, int height) {
  this->LoadLists();
  this->AddCloseButton(*this->m_controls, "Close", 0);

  this->m_list = new UIListBoxNavi(*this->m_context, *this->m_listContext);
  this->m_list->SetSize(width - 212, 160);
  this->m_list->backgroundRed = 210;
  this->m_list->backgroundGreen = 210;
  this->m_list->backgroundBlue = 210;
  this->m_list->SetId(336);
  this->m_list->Move(4, 40);
  this->AddChild(this->m_list);
  this->RefreshList(this->m_tab);

  this->m_typeCombo = new UIComboBoxDefWnd(*this->m_context, *this->m_comboContext);
  this->m_typeCombo->dropdownHeight = 67;
  this->m_typeCombo->SetSize(width - 212, 18);
  this->m_typeCombo->Move(4, 20);
  this->m_typeCombo->borderRed = 100;
  this->m_typeCombo->borderGreen = 100;
  this->m_typeCombo->borderBlue = 100;
  this->m_typeCombo->red = 232;
  this->m_typeCombo->green = 232;
  this->m_typeCombo->blue = 232;
  this->AddChild(this->m_typeCombo);
  this->m_typeCombo->AddOption(LookupMessage(2763), 0);
  this->m_typeCombo->AddOption(LookupMessage(2764), 0);
  this->m_typeCombo->AddOption(LookupMessage(2765), 0);
  this->m_typeCombo->AddOption(LookupMessage(2766), 0);
  this->m_typeCombo->Select(this->m_tab);

  auto* previous = new UIBitmapButton(*this->m_context, *this->m_controls);
  previous->SetBitmap("유저인터페이스\\scroll1left.bmp", 0, 0);
  previous->SetBitmap("유저인터페이스\\scroll1left.bmp", 1, 0);
  previous->SetBitmap("유저인터페이스\\scroll1left.bmp", 2, 0);
  previous->SetSize(previous->BitmapWidth(), previous->BitmapHeight());
  previous->Move(width - 202, 110);
  previous->SetId(227);
  this->AddChild(previous);

  auto* next = new UIBitmapButton(*this->m_context, *this->m_controls);
  next->SetBitmap("유저인터페이스\\scroll1right.bmp", 0, 0);
  next->SetBitmap("유저인터페이스\\scroll1right.bmp", 1, 0);
  next->SetBitmap("유저인터페이스\\scroll1right.bmp", 2, 0);
  next->SetSize(next->BitmapWidth(), next->BitmapHeight());
  next->Move(width - 19, 110);
  next->SetId(226);
  this->AddChild(next);
}

// OFFSET: 0x0094EC60
// FUNCTION: fn_12913
void UISecondCostumeWnd::Paint() {
  int titleHeight = this->sub_8D2CE0(true, "Dress Room", 0);
  this->DrawPanelBackground(17, 0, "");
  int x = this->geometry.width - 204;
  const BitmapImage* bitmap = reinterpret_cast<const BitmapImage*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\dressroomback.bmp")));
  if (bitmap != nullptr) {
    this->DrawBitmap(x, titleHeight + 3, bitmap, false);
  }
  this->sub_948D20(x + 100, this->geometry.height - 35);
}

// OFFSET: 0x00951060
// FUNCTION: fn_12948
int UISecondCostumeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 201:
      this->m_frameContext->windows.CloseWindow(259);
      break;

    case 226:
      if (this->m_page < 1) {
        this->m_page = 7;
      } else {
        --this->m_page;
      }

      this->Invalidate();
      return 0;

    case 227:
      this->m_page = this->m_page < 7 ? this->m_page + 1 : 0;
      this->Invalidate();
      return 0;

    case 336:
      if (this->m_list->selected != -1) {
        this->m_selected[this->m_tab] = this->m_list->selected;
        this->Invalidate();
        return 0;
      }
      break;
    }

    return 0;

  case 39:
    this->m_tab = static_cast<int>(a);
    this->RefreshList(static_cast<int>(a));
    return 0;

  default:
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x009514F0
// FUNCTION: fn_12952
void UISecondCostumeWnd::RefreshList(int tab) {
  this->m_list->Clear();

  for (unsigned i = 0; i < this->m_entries[tab].size(); ++i) {
    this->m_list->AddLine(this->m_entries[tab][i].name.c_str());
  }

  this->m_list->selected = this->m_selected[tab];
  this->m_list->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x0094A410
// FUNCTION: fn_12888
bool UISecondCostumeWnd::Slot03() {
  return true;
}

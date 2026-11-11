#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Items/UISecondCostumeWnd.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "UI/Login/FuncDrawWNDChar.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Navigation/UIListBoxNavi.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Session/Session.h"
#include "Script/LuaScript.h"
#include "Resource/MSGSTRINGID.h"
// INLINED
static int SelectedEntryValue(const std::vector<SecondCostumeEntry>& entries, const int& selected) {
  if (entries.empty()) {
    return 0;
  }
  if (entries.size() <= static_cast<unsigned>(selected)) {
    return 0;
  }
  return entries[selected].value;
}

// INLINED
static int FindEntryIndex(const std::vector<SecondCostumeEntry>& entries, const int& value) {
  for (unsigned int index = 0; index < entries.size(); ++index) {
    if (entries[index].value == value) {
      return index;
    }
  }
  return 0;
}

// OFFSET: 0x00948D20
// FUNCTION: fn_12882
void UISecondCostumeWnd::DrawCharacterPreview(int x, int y) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const unsigned char flags = mode->scene->player->isRiding;
  const std::uint32_t time = this->m_previewStartTime;
  const int action = this->m_page;

  const int headPalette = SelectedEntryValue(this->m_entries[0], this->m_selected[0]);

  const int bodyStyle = SelectedEntryValue(this->m_entries[2], this->m_selected[2]);

  const int robe = g_session.m_robe;
  const unsigned short midAccessory = *reinterpret_cast<const unsigned short*>(&g_session.accessoryAppearance[8]);
  const unsigned short topAccessory = *reinterpret_cast<const unsigned short*>(&g_session.accessoryAppearance[4]);
  const unsigned short bottomAccessory = *reinterpret_cast<const unsigned short*>(&g_session.accessoryAppearance[0]);

  const unsigned short head = static_cast<unsigned short>(SelectedEntryValue(this->m_entries[1], this->m_selected[1]));

  const int body = SelectedEntryValue(this->m_entries[3], this->m_selected[3]);

  FuncDrawWNDChar(
    this, x, y, static_cast<unsigned char>(g_session.GetCurrentSex()), static_cast<short>(g_session.GetDisplayJob()), g_session.GetDisplayJob(),
    static_cast<short>(body), static_cast<short>(head), bottomAccessory, topAccessory, midAccessory, robe, flags, 0, bodyStyle, headPalette, action, 0,
    static_cast<int>(time)
  ).Draw(0);
}

// OFFSET: 0x00949BA0
// FUNCTION: fn_12885
void UISecondCostumeWnd::LoadLists() {
  const int job = g_session.GetDisplayJob();
  const int sex = g_session.GetCurrentSex();

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_session.m_hotkeyScript->Call("_get_DressHeadPaletteList", "dd>sd", sex, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.value = value;
    this->m_entries[0].push_back(entry);
  }

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_session.m_hotkeyScript->Call("_get_Headstylelist", "dd>sd", sex, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.value = value;
    this->m_entries[1].push_back(entry);
  }

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_session.m_hotkeyScript->Call("_get_DressBodyPaletteList", "dd>sd", sex, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.value = value;
    this->m_entries[2].push_back(entry);
  }

  for (int index = 1;; ++index) {
    const char* name = nullptr;
    int value = 0;
    g_session.m_hotkeyScript->Call("_get_dresslist", "ddd>sd", sex, job, index, &name, &value);
    if (name == nullptr) {
      break;
    }

    SecondCostumeEntry entry;
    entry.name = name;
    entry.value = value;
    this->m_entries[3].push_back(entry);
  }

  this->SelectSavedEntries();
}

// OFFSET: 0x0094A2D0
// FUNCTION: fn_12886
void UISecondCostumeWnd::SelectSavedEntries() {
  this->m_selected[0] = FindEntryIndex(this->m_entries[0], g_session.m_headPalette);
  this->m_selected[1] = FindEntryIndex(this->m_entries[1], g_session.m_head);
  this->m_selected[2] = FindEntryIndex(this->m_entries[2], g_session.m_bodyStyle);
  this->m_selected[3] = FindEntryIndex(this->m_entries[3], g_session.m_bodyCostume);
}

// OFFSET: 0x0094C5B0
// FUNCTION: fn_12898
void UISecondCostumeWnd::OnCreate(int width, int height) {
  this->LoadLists();
  this->AddCloseButton("Close", 0);

  this->m_list = new UIListBoxNavi();
  this->m_list->SetSize(width - 212, 160);
  this->m_list->backgroundRed = 210;
  this->m_list->backgroundGreen = 210;
  this->m_list->backgroundBlue = 210;
  this->m_list->SetId(336);
  this->m_list->Move(4, 40);
  this->AddChild(this->m_list);
  this->RefreshList(this->m_tab);

  this->m_typeCombo = new UIComboBoxDefWnd();
  this->m_typeCombo->dropdownHeight = 67;
  this->m_typeCombo->SetSize(width - 212, 18);
  this->m_typeCombo->Move(4, 20);
  this->m_typeCombo->SetBorderColor(100, 100, 100);
  this->m_typeCombo->SetColor(232, 232, 232);
  this->AddChild(this->m_typeCombo);
  this->m_typeCombo->AddOption(LookupMessage(MSI_DRESSROOM_BODYSTYLE), 0);
  this->m_typeCombo->AddOption(LookupMessage(MSI_MINIMAP_MSG_16), 0);
  this->m_typeCombo->AddOption(LookupMessage(MSI_BANK_MAX_MONEY), 0);
  this->m_typeCombo->AddOption(LookupMessage(MSI_BANK_MIN_MONEY), 0);
  this->m_typeCombo->Select(this->m_tab);

  auto* previous = new UIBitmapButton();
  previous->SetBitmap("유저인터페이스\\scroll1left.bmp", 0, 0);
  previous->SetBitmap("유저인터페이스\\scroll1left.bmp", 1, 0);
  previous->SetBitmap("유저인터페이스\\scroll1left.bmp", 2, 0);
  previous->SetSize(previous->BitmapWidth(), previous->BitmapHeight());
  previous->Move(width - 202, 110);
  previous->SetId(227);
  this->AddChild(previous);

  auto* next = new UIBitmapButton();
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
  int titleHeight = this->DrawTitleBar(true, "Dress Room", 0);
  this->DrawPanelBackground(17, 0, "");
  int x = this->geometry.width - 204;
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\dressroomback.bmp")));
  if (bitmap != nullptr) {
    this->DrawBitmap(x, titleHeight + 3, bitmap, false);
  }
  this->DrawCharacterPreview(x + 100, this->geometry.height - 35);
}

// OFFSET: 0x00951060
// FUNCTION: fn_12948
int UISecondCostumeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_LEFT:
      this->m_page = this->m_page < 7 ? this->m_page + 1 : 0;
      this->Invalidate();
      return 0;

    case ID_RIGHT:
      if (this->m_page <= 0) {
        this->m_page = 7;
      } else {
        --this->m_page;
      }

      this->Invalidate();
      return 0;

    case ID_SELECT_ITEM:
      if (this->m_list->selected != -1) {
        this->m_selected[this->m_tab] = this->m_list->selected;
        this->Invalidate();
        return 0;
      }
      break;

    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_SECONDCOSTUMEWND);
      break;
    }

    return 0;

  case UM_COMBOBOX_SELECTED:
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
  UIListBox* list = this->m_list;
  list->selected = this->m_selected[tab];
  list->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x0094A410
// FUNCTION: fn_12888
bool UISecondCostumeWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x00946FB0
// FUNCTION: fn_12857
UISecondCostumeWnd::UISecondCostumeWnd()
    : UIFrameWnd(0) {
  this->m_typeCombo = nullptr;
  this->m_list = nullptr;
  this->m_tab = 0;
  this->m_selected[0] = 0;
  this->m_selected[1] = 0;
  this->m_selected[2] = 0;
  this->m_selected[3] = 0;
  this->m_previewStartTime = timeGetTime();
  this->m_page = 0;
}

#include "Client/Session.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Shortcuts/ShortcutActivation.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Window/UIWindowMgr.inl"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Chat/UISubChatMiniWnd.h"
#include "UI/Dialogs/UIMemorialDunWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowCallBtnWnd.h"
#include "UI/Window/UIRoMapWnd.h"
#include "UI/Window/UINavigationWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "Contents/NavigationManager.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Client/Application/Launcher.h"
#include "Client/Replay.h"
#include "Contents/NavigationManager.h"
#include "UI/Window/UINavigationWnd.h"
#include "Utility/Text/FormatString.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/PixelTextWrapping.h"
#include "Network/TextEncoding.h"
#include "File/File.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIResizer.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/CashEmotion/UICashEmotionMiniWnd.h"

#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <map>
#include <new>
#include <string>
#include <vector>

namespace {
#pragma pack(push, 1)
struct MemorialDungeonCommandPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int command;
};
#pragma pack(pop)
}

// GLOBAL: gl_24660
std::uint8_t g_chatInputFlag;



// GLOBAL: gl_24888
static bool g_subChatMiniFirstCollapsed;

// GLOBAL: gl_24889
static bool g_subChatMiniSecondCollapsed;

// OFFSET: 0x009122B0
// FUNCTION: fn_12528
UIShortCutWnd::UIShortCutWnd()
    : UIFrameWnd(0), m_decrease(nullptr), m_increase(nullptr) {
}

// OFFSET: 0x00915D90
// FUNCTION: fn_12577
UIShortCutWnd::~UIShortCutWnd() = default;


// OFFSET: 0x0091E800
// FUNCTION: fn_12629
bool UIShortCutWnd::Slot03() {
  return true;
}

// OFFSET: 0x0091EA40
// FUNCTION: fn_12632
void UIShortCutWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y;
  g_windowMgr.ConstrainSnap(this, x, y, 0);
  this->geometry.x = (this->geometry.x + x);
  const int top = (this->geometry.y + y);
  this->geometry.y = top >= 0 ? top : 0;
  g_windowMgr.StoreSnapRectangle(this);
}

// OFFSET: 0x00940FE0
// FUNCTION: fn_12787
void UIShortCutWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

// OFFSET: 0x0093D220
// FUNCTION: fn_12766
void UIShortCutWnd::Show(int visible) {
  this->geometry.visible = visible;

  if (visible) {
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    this->Invalidate();
    g_windowMgr.StoreSnapRectangle(this);
  } else {
    g_windowMgr.RemoveSnapRectangle(this);
  }
}

// OFFSET: 0x00932F30
// FUNCTION: fn_12720
void UIShortCutWnd::Slot19() {
  const int x = (g_mouse.x - this->geometry.x);
  const int y = (g_mouse.y - this->geometry.y);
  const int hovered = this->m_grid.HitTest(x, y, g_session.ItemSlotActive(), g_session.m_hotkeyPage);
  if (this->m_grid.hovered != hovered) {
    this->m_grid.hovered = hovered;
  }
  this->Invalidate();
}

void UIShortCutWnd::Slot26(int x, int y) {
  ActivateShortcut(
    *this, this->m_grid, x, y, g_session.ItemSlotActive(), g_session.m_hotkeyPage, this->m_slots, g_session.m_automaticSkills
  );
}

// OFFSET: 0x009316F0
// FUNCTION: fn_12705
void UIShortCutWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
  this->Slot11();
}

// INLINED
inline void UIShortCutWnd::ClickControl(int id) {
  switch (id) {
  case 201:
    g_windowMgr.ShowShortcutWindow(0);
    return;

  case 216:
    if (this->geometry.visible == 0) {
      g_windowMgr.ShowShortcutWindow(1);
      return;
    }

    this->m_grid.rows = this->m_grid.rows + 1;

    if (this->m_grid.rows > 4) {
      this->m_grid.rows = 1;
      g_windowMgr.ShowShortcutWindow(0);
    }

    this->SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 217:
    if (this->m_grid.rows > 1) {
      --this->m_grid.rows;
    }

    this->SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 549: {
    const bool checked = g_session.GetOption(10) == 0;
    g_session.SetOption(10, checked ? 1 : 0, 0);
    this->m_bank->geometry.unknown_0030 = checked ? 1 : 0;
    this->m_bank->Invalidate();

    for (ShortcutCooldownRecord& record : g_session.m_requestTimers) {
      record.main.clear();
    }

    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    return;
  }

  case 588:
  case 589: {
    g_session.ItemSlotActive() = id == 588;
    const bool active = g_session.ItemSlotActive();

    this->m_showItems->Enable(active ? 0 : 1);
    this->m_showItems->SetDisabled(active ? 1 : 0);
    this->m_showItems->Invalidate();

    this->m_hideItems->Enable(active ? 1 : 0);
    this->m_hideItems->SetDisabled(active ? 0 : 1);
    this->m_hideItems->Invalidate();

    this->SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;
  }

  default:
    return;
  }
}

// INLINED
inline void UIShortCutWnd::PlaceRowButtons() {
  this->m_decrease->Move(this->geometry.width - this->m_decrease->geometry.width - 2, this->m_grid.rows * 33 - 28);
  this->m_increase->Move(this->geometry.width - this->m_increase->geometry.width - 2, this->m_grid.rows * 33 - 13);
  this->m_decrease->Enable(this->m_grid.rows == 1 ? 0 : 1);
  this->m_increase->Enable(1);
  this->m_increase->SetDisabled(this->m_grid.rows == 4 ? 1 : 0);
  this->m_increase->Invalidate();
}

// INLINED
inline void UIShortCutWnd::ResizeRows() {
  const int height = (g_session.ItemSlotActive() ? 43 : 10) + this->m_grid.rows * 33;
  if (height == this->geometry.height) {
    return;
  }

  this->Resize(this->geometry.width, height);

  this->m_decrease->Move(this->geometry.width - this->m_decrease->geometry.width - 2, this->m_grid.rows * 33 - 28);
  this->m_increase->Move(this->geometry.width - this->m_increase->geometry.width - 2, this->m_grid.rows * 33 - 13);
  this->m_showItems->Move(247, height - 10);
  this->m_hideItems->Move(247, height - 10);
  this->m_decrease->Enable(this->m_grid.rows == 1 ? 0 : 1);
  this->m_increase->Enable(1);
  this->m_increase->SetDisabled(this->m_grid.rows == 4 ? 1 : 0);
  this->m_increase->Invalidate();
  this->Slot11();
}

// INLINED
inline void UIShortCutWnd::RestoreLayout(NormalItemSlotWindow* value) {
  this->m_saved = value;
  if (!value->IsValid()) {
    value->Reset();
  }

  const int height = value->height;
  this->Resize(this->geometry.width, height);
  const int extent = g_session.ItemSlotActive() ? height - 43 : height;
  this->m_showItems->Move(247, height - 10);
  this->m_hideItems->Move(247, height - 10);
  this->m_showItems->Enable(g_session.ItemSlotActive() ? 0 : 1);
  this->m_showItems->SetDisabled(g_session.ItemSlotActive() ? 1 : 0);
  this->m_showItems->Invalidate();
  this->m_hideItems->Enable(g_session.ItemSlotActive() ? 1 : 0);
  this->m_hideItems->SetDisabled(g_session.ItemSlotActive() ? 0 : 1);
  this->m_hideItems->Invalidate();
  this->m_grid.rows = extent / 33;
  this->PlaceRowButtons();
  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
  this->Move(value->x, value->y);
}

// INLINED
inline void UseShortcutSlot(const NormalItemSlot& slot) {
  if (g_session.GetOption(5) > 0 || slot.itemId == 0) {
    return;
  }

  if (slot.unknown_0000 != 0) {
    CSkillInfo skill = g_session.LookupSkill(slot.itemId);
    if (skill.MatchesShortcut(&slot)) {
      int level = slot.unknown_0005;
      if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
        level = skill.level;
      }

      static_cast<CMode*>(g_charViewMode)->SendMsg(113, reinterpret_cast<int>(&skill), level, 0, 0);
    }

    return;
  }

  ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
  if (item.unknown_0000[1] == 0) {
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();

  const auto use = [&]() { mode->SendMsg(27, item.unknown_0000[1], 0, 0, 0); };

  switch (item.unknown_0000[0]) {
  case 0:
  case 1:
  case 2:
    if (item.GetItemId() != 12802 || g_windowMgr.Confirm(LookupMessage(2373), 2, false, false, 0, "Message", 280, 120, 0) != 188) {
      use();
    }

    break;

  case 10:
  case 16:
  case 17:
  case 19:
    if (item.unknown_0000[3] == 0x8000) {
      mode->SendMsg(87, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;

  case 18:
    if (*reinterpret_cast<const int*>(&g_session.unknown_16F4[4]) >= 0) {
      const std::string name = item.DisplayName(0);
      char text[4096];

      _snprintf_s(text, sizeof(text), _TRUNCATE, LookupMessage(1226), name.c_str());

      if (g_windowMgr.Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) == 187) {
        use();
      }
    }

    break;

  default:
    if (item.unknown_0000[2] != 0 && item.unknown_0000[3] == 0 && item.identified != 0) {
      mode->SendMsg(19, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;
  }
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
int UIShortCutWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    this->ClickControl(static_cast<int>(a));
    return 0;

  case 14:
    this->ResizeRows();
    return 0;

  case 23: {
    std::list<ShortcutCooldownRecord>& cooldowns = g_session.m_requestTimers;
    for (int index = 0; index < 36; ++index) {
      const int source = g_session.RotateShortcut(index, g_session.unknown_06FC);
      if (g_session.unknown_06FC == 0 || g_session.unknown_06FC == 1) {
        this->m_slots[index] = &(g_session.unknown_06FC == 0 ? g_session.m_shortcutSlots : g_session.m_secondaryShortcutSlots)[source];
      }

      NormalItemSlot& slot = *this->m_slots[index];
      if (slot.itemId == 0 || slot.unknown_0000 == 0) {
        continue;
      }

      CSkillInfo skill = g_session.m_skillBundle.Lookup(slot.itemId);
      if (skill.available == 0) {
        const auto record = std::find_if(cooldowns.begin(), cooldowns.end(), [&](const ShortcutCooldownRecord& value) {
          return value.id == slot.itemId;
        });
        if (record != cooldowns.end()) {
          const auto value = std::find_if(record->main.begin(), record->main.end(), [&](const CooldownState& state) { return state.slot == index; });
          if (value != record->main.end()) {
            record->main.erase(value);
          }
        }

        skill = g_session.LookupHomunSkill(slot.itemId);
        if (skill.available == 0) {
          skill = g_session.LookupMercSkill(slot.itemId);
        }

        if (skill.available == 0) {
          skill = g_session.LookupAdditionalSkill(slot.itemId);
        }
      }

      if (skill.available != 0 && skill.level != 0 && g_session.UseLevel(static_cast<int>(skill.id)) == 0) {
        slot.unknown_0005 = static_cast<std::int16_t>(skill.level);
      }
    }

    for (int index = 0; index < 9; ++index) {
      this->m_itemSlots[index] = &g_session.m_normalItemSlots[index];
    }
    this->Invalidate();
    return 0;
  }

  case 34:
    this->RestoreLayout(reinterpret_cast<NormalItemSlotWindow*>(a));
    return 0;

  case 41: {
    const int index = 9 * static_cast<int>(b) + static_cast<int>(a);
    UseShortcutSlot(*this->m_slots[index]);
    return 0;
  }

  case 143:
    if (RotateCooldownRows(g_session.m_requestTimers, static_cast<int>(a), static_cast<int>(b), g_session.unknown_06FC, g_session.unknown_06A8)) {
      this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }

    return 0;

  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x0091C9B0
// FUNCTION: fn_12618
bool UINewChatWnd::Slot42(void* output) {
  std::memcpy(output, &this->m_inputX, 28);
  return true;
}

// OFFSET: 0x0091E9C0
// FUNCTION: fn_12631
void UINewChatWnd::Move(int x, int y) {
  this->UIFrameWnd::Move(x, y);
  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  this->m_inputX = screenX + 3;
  this->m_inputY = this->m_inputOffsetY + screenY;

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_03F0)) {
    window->SendMsg(this, 6, 197, this->geometry.width + x, this->geometry.height + y, 0);
  }
}

// OFFSET: 0x0093D090
// FUNCTION: fn_12765
void UINewChatWnd::Show(int visible) {
  this->geometry.visible = visible;

  if (visible != 0) {
    this->Invalidate();
    g_windowMgr.StoreSnapRectangle(this);
  } else g_windowMgr.RemoveSnapRectangle(this);

  for (UIChatHisBox* tab : this->m_tabs) {
    tab->Show(this->geometry.visible);
  }

  for (int i = 0; i < g_windowMgr.retainedWindows.size(); ++i) {
    g_windowMgr.retainedWindows[i]->Show(this->geometry.visible);
  }
}

// OFFSET: 0x00933D20
// FUNCTION: fn_12737
void UINewChatWnd::Resize(int width, int height) {
  this->UIFrameWnd::Resize(width, height);
  this->m_listWidth = this->geometry.width - 6;
  this->m_listHeight = this->geometry.height - this->m_inputOffsetY;
}

// OFFSET: 0x00933E40
// FUNCTION: fn_12739
void UINewChatWnd::SaveChatWndInfo() {
  CFile file;
  file.Open("SaveData\\ChatWndInfo_U.lua", 0x1001, true, false);

  char text[500];
  sprintf(text, "CanDragWnd = %d\r\n\r\n", g_chatInputFlag == 1);
  file.Write(text, std::strlen(text));

  int index = 1;
  for (std::map<int, SubChatWndInfo>::iterator it = g_session.m_noSubChatWnds.begin(); it != g_session.m_noSubChatWnds.end(); ++it, ++index) {
    const SubChatWndInfo& info = it->second;
    sprintf(text, "ChatSubWnd_%d = {\r\n", index);
    file.Write(text, std::strlen(text));
    sprintf(text, "\tXPos = 0,\r\n\tYPos = 0,\r\n");
    file.Write(text, std::strlen(text));
    sprintf(text, "\tWidth = 0,\r\n\tHeight = 0,\r\n");
    file.Write(text, std::strlen(text));
    sprintf(text, "\tTabState = \"On\",\r\n\tTabName = [[%s]],\r\n", info.name.c_str());
    file.Write(text, std::strlen(text));
    for (int option = 1; option <= 25; ++option) {
      sprintf(text, "\toption%d = %d,\r\n", option, info.options[option - 1]);
      file.Write(text, std::strlen(text));
    }
    file.Write("}\r\n\r\n", 5);
  }

  index = static_cast<int>(g_session.m_noSubChatWnds.size()) + 1;
  for (std::map<int, SubChatWndInfo>::iterator it = g_session.m_subChatWnds.begin(); it != g_session.m_subChatWnds.end(); ++it, ++index) {
    const SubChatWndInfo& info = it->second;
    sprintf(text, "ChatSubWnd_%d = {\r\n", index);
    file.Write(text, std::strlen(text));
    sprintf(text, "\tXPos = %d,\r\n\tYPos = %d,\r\n", info.x, info.y);
    file.Write(text, std::strlen(text));
    sprintf(text, "\tWidth = %d,\r\n\tHeight = %d,\r\n", info.width, info.height);
    file.Write(text, std::strlen(text));
    sprintf(text, "\tTabState = \"Off\",\r\n\tTabName = \"%s\",\r\n", info.name.c_str());
    file.Write(text, std::strlen(text));
    for (int option = 1; option <= 25; ++option) {
      sprintf(text, "\toption%d = %d,\r\n", option, info.options[option - 1]);
      file.Write(text, std::strlen(text));
    }
    file.Write("}\r\n\r\n", 5);
  }

  file.Close();
}

// OFFSET: 0x009328F0
// FUNCTION: fn_12712
void UINewChatWnd::Slot27(int x, int y) {
  if (g_windowMgr.Capture() == this && g_chatInputFlag == 1) {
    this->UIFrameWnd::Slot27(x, y);
  }
}

// OFFSET: 0x00930D40
// FUNCTION: fn_12695
void UINewChatWnd::Slot25(int x, int y) {
  if (static_cast<unsigned int>(y - 1) <= 19) {
    if (static_cast<int>(this->geometry.unknown_0030) <= 0) {
      return;
    }

    if (y < 14 && x > this->m_lockButtonX && x < this->m_lockButtonX + 9) {
      g_session.m_canDragWnd = g_session.m_canDragWnd == 0;
      this->ApplyHeaderLayout();
      for (int i = 0; i < g_windowMgr.retainedWindows.size(); ++i) {
        static_cast<UISubChatHisWnd*>(g_windowMgr.retainedWindows[i])->ApplyButtonLayout();
        g_windowMgr.retainedWindows[i]->Invalidate();
      }

      RECT rectangle;
      rectangle.left = this->m_lockButtonX;
      rectangle.top = 4;
      rectangle.right = this->m_lockButtonX + 15;
      rectangle.bottom = 13;
      this->FillSurface(0xFFFF00FF, &rectangle);
      this->Invalidate();
    }

    if (g_windowMgr.chatModeChanged && y < 14 && x > this->m_chatModeX && x < this->m_chatModeX + 14) {
      g_windowMgr.inputSuggestionsEnabled = g_windowMgr.inputSuggestionsEnabled == 0;
      if (ChatWindow()) {
        ChatWindow()->ApplyInputLayout();
      }
      this->Invalidate();
    }
  } else {
    g_windowMgr.SetFocus(this->m_inputEdit);
    this->UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x0092D420
// FUNCTION: fn_12669
void UINewChatWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->FillRegion(4, this->geometry.height - this->m_inputAreaHeight, this->geometry.width - 8, 1, 0x8E8E8E);

  if (static_cast<unsigned char>(g_windowMgr.unknown_03F4) != 0 && this->m_inputAreaHeight + this->m_inputOffsetY < this->geometry.height) {
    const int tabsWidth = static_cast<int>(this->m_tabControl->labels.size()) * this->m_tabWidth;
    this->FillRegion(3, this->m_inputOffsetY, 1, this->geometry.height - this->m_inputOffsetY, 0xFFFFFF);
    this->FillRegion(tabsWidth + 3, this->m_inputOffsetY, this->geometry.width - tabsWidth - 10, 1, 0xFFFFFF);
    this->FillRegion(this->geometry.width - 6, this->m_inputOffsetY, 1, 1, 0x8E8E8E);
    this->FillRegion(this->geometry.width - 5, this->m_inputOffsetY + 1, 1, 1, 0x8E8E8E);
    this->FillRegion(this->geometry.width - 4, this->m_inputOffsetY + 2, 1, this->geometry.height - this->m_inputOffsetY - 2, 0x8E8E8E);
  }

  if (g_windowMgr.inputSuggestionsEnabled) {
    auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\dialog_bg.bmp")));
    this->DrawBitmap(0, this->geometry.height - background->image.height, &background->image, false);
  }

  if (static_cast<int>(this->geometry.unknown_0030) > 0) {
    if (g_windowMgr.chatModeChanged) {
      auto* chatMode = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
        Resolve(g_windowMgr.inputSuggestionsEnabled ? "유저인터페이스\\basic_interface\\chatmode_on.bmp" : "유저인터페이스\\basic_interface\\chatmode_off.bmp")
      ));
      if (chatMode) {
        this->m_chatModeX = this->m_chatModeButton->geometry.x - chatMode->image.width - 2;
        this->DrawBitmap(this->m_chatModeX, 4, &chatMode->image, true);
      }
    }

    auto* lock = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(g_chatInputFlag ? "유저인터페이스\\basic_interface\\unlock_dragwnd.bmp" : "유저인터페이스\\basic_interface\\lock_dragwnd.bmp")
    ));
    if (lock) {
      this->DrawBitmap(this->m_lockButtonX, 4, &lock->image, true);
    }
  }
}

// OFFSET: 0x009313E0
// FUNCTION: fn_12703
void UINewChatWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
}

// OFFSET: 0x009313F0
// FUNCTION: fn_12704
void UIRoMapWnd::Slot31(int x, int y) {
  this->unknown_01FC.clear();

  unsigned int mapIndex = 0;
  if (CWorldViewMap* map = g_worldViewMgr.CurrentWorld()->MapAt(mapIndex, x, y)) {
    this->unknown_0208 = mapIndex;
    for (CWorldViewField* field : map->fields) {
      this->unknown_01FC.push_back(field->unknown_002C);
    }
  }

  if (this->unknown_01FC.empty()) {
    if (this->unknown_00C0) {
      std::string name = this->unknown_00C0->name;
      if (name.size() > 4) {
        name.resize(name.size() - 4);
      }

      g_navigationMinimized = false;
      g_navigationManager.sub_B995D0(name, 0, false);
      if (g_windowMgr.window_4F174) {
        static_cast<UINavigationV4Wnd*>(g_windowMgr.window_4F174)->SelectRoute(0);
        static_cast<UINavigationV4Wnd*>(g_windowMgr.window_4F174)->SelectRouteNode(0);
      }
      this->UpdateNavigationButtons();
    }
    return;
  }

  if (g_windowMgr.unknown_0214 != 0) {
    g_windowMgr.CloseWindow(26);
  }

  if (!this->unknown_01FC.empty()) {
    UIWindow* menu = g_windowMgr.EnsureWindow(26);
    int width = 0;
    for (const std::string& name : this->unknown_01FC) {
      const int textWidth = this->TextWidth(name.c_str(), 0, 0, 14, 0, 0);
      if (width < textWidth) {
        width = textWidth;
      }
      menu->SendMsg(nullptr, 31, reinterpret_cast<std::intptr_t>(name.c_str()), 0, 0, 0);
    }

    const int height = static_cast<int>((std::min)(this->unknown_01FC.size() * 16, 160u));
    menu->SendMsg(nullptr, 40, 0, 0, 0, 0);
    menu->Resize(width, height);
    menu->SendMsg(nullptr, 83, 140, 0, 0, 0);

    const int menuY = this->geometry.height - (y + 1) - height >= 0 ? y + 1 : this->geometry.height - height;
    const int menuX = this->geometry.width - (x + 2) - width >= 0 ? x + 2 : this->geometry.width - width;
    menu->Move(menuX, menuY);
  }
}

// OFFSET: 0x0091E960
// FUNCTION: fn_12630
void UIMemorialDunWnd::Move(int x, int y) {
  const int left = x <= g_renderer->viewport.width - this->geometry.width ? x : 0;
  const int top = y <= g_renderer->viewport.height - this->geometry.height ? y : 310;
  this->UIFrameWnd::Move(left, top);

  if (g_session.unknown_5940 != left) {
    g_session.unknown_5940 = left;
  }
  if (g_session.unknown_5944 != top) {
    g_session.unknown_5944 = top;
  }
}

// OFFSET: 0x00921260
// FUNCTION: fn_12641
void UIMemorialDunWnd::Slot15(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("Alt+B", 0);

  {
    const std::string name = "유저인터페이스\\btn_ok";
    this->m_actionButton = new UIBitmapButton();
    this->m_actionButton->SetBitmap((name + ".bmp").c_str(), 0, 0);
    this->m_actionButton->SetBitmap((name + "_a.bmp").c_str(), 1, 0);
    this->m_actionButton->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    this->m_actionButton->SetSize(this->m_actionButton->BitmapWidth(), this->m_actionButton->BitmapHeight());
    this->m_actionButton->Move(-100, -100);
    this->m_actionButton->SetId(184);
    this->AddChild(this->m_actionButton);
  }

  this->m_partyButton = new (std::nothrow) UIBasicButton();
  this->m_partyButton->Configure(125, LookupMessage(3270));
  this->m_partyButton->Move(-100, -100);
  this->m_partyButton->SetId(211);
  this->AddChild(this->m_partyButton);
  this->ArrangeButtons();
}

// OFFSET: 0x0091CF00
// FUNCTION: fn_12621
void UIMemorialDunWnd::ResetDungeonInfo() {
  g_session.unknown_5950 = "";
  g_session.unknown_5968 = 0;
  g_session.unknown_596C = 0;
  g_session.unknown_5970 = 0;
  g_session.unknown_5948 = 0;
}

// OFFSET: 0x0092C6B0
// FUNCTION: fn_12665
void UIMemorialDunWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(0x538), 0);
  if (this->geometry.height == 17) {
    return;
  }

  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 19, 0xFFFFFF);
  this->FillRegion(1, this->geometry.height - 2, this->geometry.width - 2, 1, 0xFFFFFF);
  this->FillRegion(2, this->geometry.height - 1, this->geometry.width - 4, 1, 0xFFFFFF);

  std::uint32_t color;
  char text[500];
  char timeText[128];
  const unsigned short codePage = static_cast<unsigned short>(g_textCodePage);

  if (g_session.unknown_5948 != 3) {
    char nameColor[12];
    char endColor[12];
    char name[80];
    char title[100];
    sprintf(nameColor, "^%02X%02X%02X", 0x41, 0x69, 0xE1);
    sprintf(endColor, "^%02X%02X%02X", 0, 0, 0);
    sprintf(name, "%s[%s]%s", nameColor, g_session.unknown_5950.c_str(), endColor);
    sprintf(title, LookupMessage(g_session.unknown_5948 + 0x539), name);
    const int width = this->ColoredTextWidth(title, 0, 0, 12, 0);
    DrawColoredText(*this, (this->geometry.width - width) / 2, 30, title, 0, color, 0, 12, false);
  }

  if (g_session.unknown_5948 == 0) {
    sprintf(text, LookupMessage(0x53E), g_session.unknown_5968);
    const int width = this->ColoredTextWidth(text, 0, 0, 12, 0);
    DrawColoredText(*this, (this->geometry.width - width) / 2, (this->geometry.height - 17) / 2, text, 0, color, 0, 12, false);
  } else if (g_session.unknown_5948 == 1) {
    if (g_session.unknown_5970) {
      strftime(timeText, 128, LookupMessage(0x4EC), _localtime32(reinterpret_cast<const __time32_t*>(&g_session.unknown_5970)));
      sprintf(text, LookupMessage(0x53F), timeText);
      std::vector<std::string> lines;
      this->WrapPixelText(text, lines, this->geometry.width - 20);
      if (lines.size() > 1) {
        int y = static_cast<int>((this->geometry.height - lines.size() * 17 - 72) / 2) + 40;
        for (unsigned int index = 0; index < lines.size(); ++index) {
          DrawColoredText(*this, 10, y, lines[index].c_str(), 0, color, 0, 12, false);
          y += 17;
        }
      } else {
        const int width = this->ColoredTextWidth(text, 0, 0, 12, 0);
        DrawColoredText(*this, (this->geometry.width - width) / 2, 55, lines[0].c_str(), 0, color, 0, 12, false);
      }
    }
  } else if (g_session.unknown_5948 == 2) {
    if (g_session.unknown_596C) {
      this->DrawTextAt(10, 55, LookupMessage(0x540), 0, 0, 12, 0, 0, 0);
      strftime(timeText, 128, LookupMessage(0x4EC), _localtime32(reinterpret_cast<const __time32_t*>(&g_session.unknown_596C)));
      sprintf(text, "^ff0000%s^ffffff", timeText);
      const int width = this->ColoredTextWidth(text, 0, 0, 12, 0);
      DrawColoredText(*this, (this->geometry.width - width) / 2, 75, text, 0, color, 0, 12, false);
    }
  } else if (g_session.unknown_5948 == 3) {
    std::vector<std::string> lines;
    this->WrapPixelText(LookupMessage(0x53C), lines, this->geometry.width - 20);
    int y = 55;
    for (unsigned int index = 0; index < lines.size(); ++index) {
      DrawColoredText(*this, 5, y, lines[index].c_str(), 0, color, 0, 12, false);
      y += 15;
    }

    this->WrapPixelText(LookupMessage(0x53D), lines, this->geometry.width - 20);
    y = 70;
    for (unsigned int index = 0; index < lines.size(); ++index) {
      DrawColoredText(*this, 10, y, lines[index].c_str(), 0, color, 0, 12, false);
      y += 15;
    }
  }

  this->FillRegion(2, 0, this->geometry.width - 4, 1, 0x8E8E8E);
  this->FillRegion(1, 1, 1, 1, 0x8E8E8E);
  this->FillRegion(0, 2, 1, this->geometry.height - 4, 0x8E8E8E);
  this->FillRegion(1, this->geometry.height - 2, 1, 1, 0x8E8E8E);
  this->FillRegion(this->geometry.width - 2, 1, 1, 1, 0x8E8E8E);
  this->FillRegion(this->geometry.width - 1, 2, 1, this->geometry.height - 4, 0x8E8E8E);
  this->FillRegion(2, this->geometry.height - 1, this->geometry.width - 4, 1, 0x8E8E8E);
  this->FillRegion(this->geometry.width - 2, this->geometry.height - 2, 1, 1, 0x8E8E8E);
}

// OFFSET: 0x0092AFE0
// FUNCTION: fn_12656
void UISubChatMiniWnd::Slot15(int, int) {
  this->Resize(this->m_rowHeight * this->unknown_00B4, this->m_offsetY);
}

// OFFSET: 0x0093CA50
// FUNCTION: fn_12762
int UISubChatMiniWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  switch (a) {
  case 197:
    this->Move(b, c - this->m_offsetY);
    break;
  case 202:
    this->Slot25(this->m_rowHeight * b + 1, 0);
    break;
  }
  return 0;
}

// OFFSET: 0x0093CAB0
// FUNCTION: fn_12763
int UIWindowCallBtnWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6) {
    g_windowMgr.EnsureWindow(a);
  }
  return 0;
}

// OFFSET: 0x009305E0
// FUNCTION: fn_12684
void UIWindowCallBtnWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00930550
// FUNCTION: fn_12683
void UISubChatMiniWnd::Paint() {
  const char* firstName = g_subChatMiniFirstCollapsed ? "유저인터페이스\\basic_interface\\st_a.tga" : "유저인터페이스\\basic_interface\\st_b.tga";
  const auto* first = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(firstName)));
  const char* secondName = g_subChatMiniSecondCollapsed ? "유저인터페이스\\basic_interface\\bt_a.tga" : "유저인터페이스\\basic_interface\\bt_b.tga";
  const auto* second = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(secondName)));
  this->DrawBitmap(0, 0, first ? &first->image : nullptr, false);
  this->DrawBitmap(this->m_rowHeight, 0, second ? &second->image : nullptr, false);
}

// OFFSET: 0x00931330
// FUNCTION: fn_12701
void UISubChatMiniWnd::Slot25(int x, int) {
  if (x > 0 && x <= this->m_rowHeight) {
    g_subChatMiniFirstCollapsed = !g_subChatMiniFirstCollapsed;
  } else {
    g_subChatMiniSecondCollapsed = !g_subChatMiniSecondCollapsed;
  }
  this->SendMsg(this, 6, 271, 0, 0, 0);
  this->Invalidate();
}

// OFFSET: 0x00940EB0
// FUNCTION: fn_12782
void UINewChatWnd::Slot11() {
  g_windowMgr.unknown_004C = this->geometry.unknown_0030;
  g_windowMgr.unknown_0038 = this->geometry.height;
  g_windowMgr.unknown_0030 = this->geometry.x;
  g_windowMgr.unknown_0034 = this->geometry.y;
}

// OFFSET: 0x00941190
// FUNCTION: fn_12790
void UINewChatWnd::SaveChatLogs() {
  char path[2048];
  sprintf(path, "%s\\Chat", g_applicationDirectory);
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  std::string name;
  int index = 0;
  for (std::vector<UIChatHisBox*>::iterator tab = this->m_tabs.begin(); tab != this->m_tabs.end(); ++tab, ++index) {
    const std::map<int, SubChatWndInfo>::iterator info = g_session.m_noSubChatWnds.find(index);
    if (info != g_session.m_noSubChatWnds.end()) {
      name = info->second.name;
    }

    if (!name.empty()) {
      sprintf(path, "%s\\Chat\\Chat_%s.txt", g_applicationDirectory, name.c_str());
      int number = 0;
      while (_access(path, 0) == 0) {
        ++number;
        sprintf(path, "%s\\Chat\\Chat_%s_%03d.txt", g_applicationDirectory, name.c_str(), number);
      }

      if ((*tab)->SaveChatLog(path)) {
        std::string message(path);
        message += " is Saved.";
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF00, 0, 0, 0);
      }
    }
  }

  for (std::map<int, UIWindow*>::iterator it = g_windowMgr.retainedWindows.begin(); it != g_windowMgr.retainedWindows.end(); ++it) {
    UISubChatHisWnd* window = static_cast<UISubChatHisWnd*>(it->second);
    name = window->Name();
    sprintf(path, "%s\\Chat\\Chat_%s.txt", g_applicationDirectory, name.c_str());
    int number = 0;
    while (_access(path, 0) == 0) {
      ++number;
      sprintf(path, "%s\\Chat\\Chat_%s_%03d.txt", g_applicationDirectory, name.c_str(), number);
    }

    if (window->ChatBox()->SaveChatLog(path)) {
      std::string message(path);
      message += " is Saved.";
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0xFF00, 0, 0, 0);
    }
  }
}

// OFFSET: 0x009166A0
// FUNCTION: fn_12584
void UINewChatWnd::RefreshTabs(bool arrange) {
  if (arrange && this->geometry.unknown_0030 != 0) {
    int index = 0;
    for (std::vector<UIChatHisBox*>::iterator tab = this->m_tabs.begin(); tab != this->m_tabs.end(); ++tab, ++index) {
      if (this->m_currentTab == index) {
        (*tab)->Move(5, this->m_inputOffsetY + 2);
        (*tab)->Invalidate();
      } else {
        (*tab)->Move(-100 - this->geometry.width, -100);
      }
    }
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(132)) {
    window->SendMsg(this, 6, 193, this->m_currentTab, 1, 2);
  }

  this->m_tabControl->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x00933A80
// FUNCTION: fn_12733
void UIMemorialDunWnd::ArrangeButtons() {
  if (this->m_partyButton) {
    this->m_partyButton->Move(-100, -100);
  }

  if (g_session.unknown_5948 == 0 || g_session.unknown_5948 == 1 || g_session.unknown_5948 == 2) {
    this->m_actionButton->Move(-100, -100);
    this->m_message0 = 600;
    if (g_session.unknown_5464 && this->m_partyButton && g_session.unknown_5948 == 1) {
      this->m_partyButton->Move(41, 115);
    }
  } else if (g_session.unknown_5948 == 3) {
    this->m_actionButton->Move((this->geometry.width - this->m_actionButton->geometry.width) / 2, this->geometry.height - 30);
    this->m_message0 = 184;
  }

  this->Invalidate();
}

// OFFSET: 0x00935850
// FUNCTION: fn_12747
int UIMemorialDunWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 0) {
    this->SendMsg(nullptr, 6, this->m_message0, 0, 0, 0);
    return 0;
  }

  if (message == 6) {
    switch (argument1) {
    case 184:
      g_windowMgr.CloseWindow(137);
      return 0;
    case 185: {
      MemorialDungeonCommandPacket packet;
      packet.id = 0x2CF;
      if (g_session.unknown_5948 == 0) {
        packet.command = 0;
      } else if (g_session.unknown_5948 == 1) {
        packet.command = 1;
      }
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
      return 0;
    }
    case 201:
      this->Show(0);
      if (g_session.unknown_5948 == 2) {
        g_session.unknown_593C = 0;
      }
      return 0;
    case 211:
      if (g_session.unknown_5464 != 0) {
        g_modeMgr.GetGameMode()->SendMsg(0x132, 0, 0, 0, 0);
      }
      return 0;
    }
    return 0;
  }

  if (message == 0x3C) {
    this->ArrangeButtons();
    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0093CB60
// FUNCTION: fn_12764
bool UINavigationWnd::LoadRoute(int index) {
  if (this->m_history.empty() || static_cast<unsigned int>(index) > this->m_history.size() || index < 0) {
    return false;
  }

  std::string name = this->m_history[index].c_str();
  if (name.size() == 0) {
    return false;
  }

  std::string path = FormatOwnedString("%s\\Navigationdata\\%s", g_applicationDirectory, name.c_str());
  FILE* file = fopen(path.c_str(), "rb");
  if (!file) {
    return false;
  }

  g_navigationManager.ClearStates();
  char delimiters[] = "\t ,\t\n";
  char line[1024];
  std::memset(line, 0, sizeof(line));
  std::vector<std::string> tokens;
  while (!feof(file)) {
    fgets(line, 1024, file);
    for (char* token = strtok(line, delimiters); token; token = strtok(nullptr, delimiters)) {
      tokens.emplace_back(std::string(token));
    }
    g_navigationManager.AddState(tokens);
    tokens.erase(tokens.begin(), tokens.end());
  }
  fclose(file);

  if (g_navigationManager.StateCount() > 0) {
    this->RecordData(0x38F, g_navigationManager.WriteStates());
  }

  this->FillRouteList();
  this->SendMsg(this, 6, 190, 0, 0, 0);
  return true;
}

// OFFSET: 0x00911490
// FUNCTION: fn_12515
UIMemorialDunWnd::UIMemorialDunWnd()
    : UIFrameWnd(0), m_partyButton(nullptr) {
}

// OFFSET: 0x00912460
// FUNCTION: fn_12530
UISubChatMiniWnd::UISubChatMiniWnd()
    : UIFrameWnd(0), unknown_00B4(2), m_rowHeight(32), m_offsetY(32) {
}

// OFFSET: 0x00911700
// FUNCTION: fn_12518
UINewChatWnd::UINewChatWnd() : UIFrameWnd(0) {
  this->m_chatEdit = nullptr;
  this->unknown_00C0 = nullptr;
  this->m_inputEdit = nullptr;
  this->unknown_00B8 = nullptr;
  this->unknown_0110 = 10;
  *reinterpret_cast<unsigned char*>(&g_windowMgr.unknown_03F4) = 1;
  this->geometry.unknown_0030 = 0;
  this->unknown_00F0 = 0;
  this->m_currentTab = 0;
  this->unknown_0118 = 0;
  this->m_inputOffsetY = 16;
  this->m_tabWidth = 71;
  this->m_inputAreaHeight = 24;
  switch (g_serviceType) {
  case 10:
    this->m_tabWidth = 71;
    break;
  case 18:
    this->m_tabWidth = 90;
    break;
  case 1:
  case 7:
  case 11:
    this->m_tabWidth = 110;
    break;
  }
  this->m_suggestions = nullptr;
}

// OFFSET: 0x00933D60
// FUNCTION: fn_12738
void UISubChatHisWnd::Resize(int width, int height) {
  this->UIFrameWnd::Resize(width, height);
  this->m_chatBox->Resize(this->geometry.width - 8, this->geometry.height - this->unknown_00F0 - this->m_inputHeight - 4, true);
  this->m_resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->m_miniButton->Move(this->m_inputWidth + 26, 5);
  this->m_state.unknown_0008 = this->geometry.width;
  this->m_state.unknown_000C = this->geometry.height - this->m_inputHeight;
  g_session.m_subChatWnds[this->m_index].width = this->geometry.width;
  g_session.m_subChatWnds[this->m_index].height = this->geometry.height;
}

#include "UI/Edit/UIEditCtrl.h"

#include <mbstring.h>

namespace Concurrency {
namespace details {
class _ReentrantPPLLock {
public:
  _ReentrantPPLLock();
};
}
}

struct MsvcString {
  void sub_4EE2E0(const char* value);
  void sub_696770(const MsvcString* source);
  void sub_914AD0(const void* source);
};

struct TreeCursor {
  void* node;
  void sub_915170();
  void sub_915110();
};

struct ChatIndexMap {
  void* head;
  int count;
  void sub_9435A0(void** out, int* key);
  void sub_90FDD0(void** out, void* node);
  void sub_9101F0(ChatIndexMap* source);
  void sub_9432C0();
  MsvcString* sub_914D30(int* key);
  void sub_913720();
};

struct RetainedMap {
  void* head;
  int count;
  void sub_910090(RetainedMap* source);
  void sub_943240();
  UIWindow** sub_914B30(int* key);
  void sub_90F180(RetainedMap* self);
};

// OFFSET: 0x0093C210
// FUNCTION: fn_12761
int UISubChatHisWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 0:
    if (sender == this->m_edit && this->m_edit->geometry.x != -100) {
      const char* text = this->m_edit->Text();
      MsvcString local;
      local.sub_4EE2E0(text);
      const int previousWidth = this->m_inputWidth;
      if (text != nullptr && text[0] != '\0') {
        const int measured = this->TextWidth(text, 0, 0, 12, 0, 0);
        if (measured + 10 < this->geometry.width - 0x32) {
          reinterpret_cast<MsvcString*>(&this->m_name)->sub_696770(&local);
          reinterpret_cast<ChatIndexMap*>(&g_session.m_subChatWnds)->sub_914D30(&this->m_index)->sub_696770(reinterpret_cast<MsvcString*>(&this->m_name));
          const int renamed = this->TextWidth(text, 0, 0, 12, 0, 0);
          this->m_inputWidth = renamed + 10;
          if (this->m_inputWidth < 0x19) {
            this->m_inputWidth = 0x19;
          }
        }
      }
      const int measured = this->TextWidth(text, 0, 0, 12, 0, 0);
      if (this->geometry.width - 0x32 <= measured + 10) {
        g_windowMgr.Confirm(LookupMessage(0x629), 0, true, false, 0, "Message", 0x118, 0x78, 0);
      }
      this->m_edit->Move(-100, -100);
      this->m_edit->SetText("");
      this->Invalidate();
      this->ApplyButtonLayout();
      g_windowMgr.SetFocus(ChatWindow());
      RECT rectangle;
      rectangle.left = 0;
      rectangle.top = 0;
      int span = this->m_inputWidth;
      if (span < previousWidth) {
        span = previousWidth;
      }
      rectangle.right = span + 0x32;
      rectangle.bottom = this->m_inputHeight + 10;
      this->FillSurface(0xFFFF00FF, &rectangle);
      if (UIWindow* window = g_windowMgr.LookupWindow(0x84)) {
        window->SendMsg(this, 6, 0xC1, this->m_index, 0, 2);
      }
      new (static_cast<void*>(&local)) Concurrency::details::_ReentrantPPLLock();
      return 0;
    }
    if (ChatWindow() != nullptr) {
      ChatWindow()->SendMsg(this, 0, 0, 0, 0, 0);
      return 0;
    }
    return 0;
  case 3:
    if (this->m_chatBox->verticalOverflow != 0) {
      this->m_chatBox->SendMsg(nullptr, 9, 0, 0, 0, 0);
      return 0;
    }
    return 0;
  case 4:
    if (this->m_chatBox->verticalOverflow != 0) {
      this->m_chatBox->SendMsg(nullptr, 10, 0, 0, 0, 0);
      return 0;
    }
    return 0;
  case 6:
    switch (argument1) {
    case 0xC1:
      if (UIWindow* window = g_windowMgr.EnsureWindow(0x84)) {
        window->SendMsg(this, 6, 0xC1, this->m_index, 0, 0);
        return 0;
      }
      return 0;
    case 0xCA: {
      ChatIndexMap* names = reinterpret_cast<ChatIndexMap*>(&g_session.m_subChatWnds);
      void* node = nullptr;
      names->sub_9435A0(&node, &this->m_index);
      if (node == names->head) {
        return 0;
      }
      const int key = *reinterpret_cast<int*>(reinterpret_cast<char*>(node) + 0x10);
      void* erased = nullptr;
      names->sub_90FDD0(&erased, node);
      if (key != names->count) {
        ChatIndexMap saved;
        saved.sub_9101F0(names);
        names->sub_9432C0();
        TreeCursor cursor;
        cursor.node = *reinterpret_cast<void**>(saved.head);
        int index = 0;
        while (cursor.node != saved.head) {
          void* value = reinterpret_cast<char*>(cursor.node) + 0x14;
          names->sub_914D30(&index)->sub_914AD0(value);
          cursor.sub_915170();
          ++index;
        }
        saved.sub_913720();
      }
      g_windowMgr.ReleaseRetainedWindow(this->m_index);
      if (UIWindow* window = g_windowMgr.LookupWindow(0x84)) {
        window->SendMsg(this, 6, 0xC1, this->m_index, 0, 1);
      }
      RetainedMap* retained = reinterpret_cast<RetainedMap*>(&g_windowMgr.retainedWindows);
      if (this->m_index != retained->count) {
        RetainedMap saved;
        saved.sub_910090(retained);
        retained->sub_943240();
        TreeCursor cursor;
        cursor.node = *reinterpret_cast<void**>(saved.head);
        int index = 0;
        while (cursor.node != saved.head) {
          UIWindow* window = *reinterpret_cast<UIWindow**>(reinterpret_cast<char*>(cursor.node) + 0x14);
          static_cast<UISubChatHisWnd*>(window)->m_index = index;
          *retained->sub_914B30(&index) = window;
          cursor.sub_915110();
          ++index;
        }
        saved.sub_90F180(&saved);
      }
      return 0;
    }
    case 0x15B:
      g_windowMgr.SelectWhisperName(reinterpret_cast<const char*>(argument2));
      return 0;
    case 0x165:
      if (ChatWindow() != nullptr) {
        ChatWindow()->SendMsg(this, 0x76, 0, 0, 0, 0);
        return 0;
      }
      return 0;
    default:
      return 0;
    }
  case 0xE:
    if (g_chatInputFlag != 0 && argument1 == 7) {
      int width = static_cast<int>(argument2);
      int height = static_cast<int>(argument3);
      if (width > 0x200) {
        width = 0x200;
      }
      if (height > 0x180) {
        height = 0x180;
      }
      if (width < 0x118) {
        width = 0x118;
      }
      if (height < 0x4A) {
        height = 0x4A;
      }
      width = ((width - 0x118) & ~0x1F) + 0x118;
      height = ((height - 0x4A) & ~0x1F) + 0x4A;
      if (width != this->geometry.width || height != this->geometry.height) {
        this->Resize(width, height);
        this->m_resizer->Move(this->geometry.width - 0xF, this->geometry.height - 0xF);
        this->ApplyButtonLayout();
        return 0;
      }
    }
    return 0;
  case 0x22: {
    this->m_saved = reinterpret_cast<SubChatHisWndInfo*>(argument1);
    if (this->m_saved->IsValid() == 0) {
      this->m_saved->Reset(this->geometry.id);
    }
    this->Move(this->m_saved->x, this->m_saved->y);
    this->Resize(this->m_saved->width, this->m_saved->height);
    return 0;
  }
  case 0x25: {
    char detail[25];
    std::memset(detail, 0, sizeof(detail));
    if (argument3 != 0) {
      std::strcpy(detail, reinterpret_cast<const char*>(argument3));
    }
    const char* text = reinterpret_cast<const char*>(argument1);
    this->m_chatBox->AddLinkRecord(text, static_cast<int>(argument2), detail);
    if (_mbsstr(reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>("<ITEML>")) != nullptr) {
      this->m_chatBox->AddText(text, static_cast<int>(argument2), detail);
    } else {
      this->m_chatBox->AddTextWrapped(text, static_cast<int>(argument2), detail);
    }
    return 0;
  }
  case 0x74:
    if (sender == this->m_chatBox && this->m_inputHeight < this->geometry.height) {
      RECT rectangle;
      rectangle.left = this->m_chatBox->geometry.x;
      rectangle.top = this->m_chatBox->geometry.y;
      rectangle.right = this->geometry.width - 3;
      rectangle.bottom = this->geometry.height - this->unknown_00F0;
      this->FillSurface(0xFFFF00FF, &rectangle);
      return 0;
    }
    return 0;
  default:
    break;
  }
  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}


// OFFSET: 0x009230A0
// FUNCTION: fn_12644
void UINewChatWnd::Slot15(int width, int height) {
  if (height > this->m_inputAreaHeight) {
    int rows = (height - (static_cast<int>(this->geometry.unknown_0030) + this->m_inputOffsetY + 74)) / 42 + 1;
    if (rows > 4) {
      rows = 4;
    }
    this->geometry.unknown_0030 = rows;
  } else {
    this->geometry.unknown_0030 = 0;
  }

  this->m_tabWidth = 0;
  this->m_lockButtonX = width - 20;

  this->m_tabControl = new UITabControl();
  this->m_tabControl->Configure(1, this->m_inputOffsetY, 10, 1, 1, 1, 0, 0, 0xFFFFFF, 0x8E8E8E, 0xFFFFFF, 0xFFFFFF);
  this->m_tabControl->Move(3, 0);
  this->AddChild(this->m_tabControl);
  this->m_tabControl->unknown_007C = 0;

  {
    std::string name = "유저인터페이스\\basic_interface\\stickoff";
    UIBitmapButton* button = new UIBitmapButton();
    this->unknown_0128 = button;
    button->SetBitmap((name + "_a.bmp").c_str(), 0, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 1, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->m_tabWidth * 2 + 4, 4);
    button->SetId(0x166);
    this->AddChild(button);
  }

  {
    std::string name = "유저인터페이스\\basic_interface\\battle_option";
    name.append("2", 1);
    UIBitmapButton* button = new UIBitmapButton();
    this->m_chatModeButton = button;
    button->SetBitmap((name + "_a.bmp").c_str(), 0, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 1, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->m_tabWidth * 2 + 19, 5);
    button->SetId(0xC1);
    this->AddChild(button);
  }

  {
    std::string name = "유저인터페이스\\basic_interface\\battle_option";
    UIBitmapButton* button = new UIBitmapButton();
    this->m_headerButtons[0] = button;
    button->SetBitmap((name + "_a.bmp").c_str(), 0, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 1, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(this->m_tabWidth * 2 + 34, 5);
    button->SetId(0x176);
    this->AddChild(button);
  }

  {
    std::string name = "유저인터페이스\\basic_interface\\wnd_mini";
    UIBitmapButton* button = new UIBitmapButton();
    this->m_headerButtons[1] = button;
    button->SetBitmap((name + "_a.bmp").c_str(), 0, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 1, 0);
    button->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->SetId(0xCA);
    this->AddChild(button);
  }

  for (std::map<int, SubChatWndInfo>::iterator it = g_session.m_noSubChatWnds.begin(); it != g_session.m_noSubChatWnds.end(); ++it) {
    this->m_tabControl->AddTab(it->second.name.c_str(), "");
    UIChatHisBox* chatBox = new UIChatHisBox();
    chatBox->Configure(0, 0, this->geometry.width - 8, 70, 0);
    chatBox->Move(-100 - this->geometry.width, -100);
    chatBox->SetId(0x1ED);
    this->AddChild(chatBox);
    this->m_tabs.push_back(chatBox);
  }

  if (g_session.m_noSubChatWnds.size() != 0) {
    this->m_tabWidth = static_cast<unsigned>(this->m_tabControl->geometry.width) / g_session.m_noSubChatWnds.size();
  }

  int subIndex = 0;
  for (std::map<int, SubChatWndInfo>::iterator it = g_session.m_subChatWnds.begin(); it != g_session.m_subChatWnds.end(); ++it, ++subIndex) {
    g_windowMgr.sub_A97580(subIndex, it->second.x, it->second.y, it->second.width, it->second.height);
  }

  this->m_chatEdit = new UIChatEditCtrl();
  this->m_chatEdit->SetSize(460, 16);
  this->m_chatEdit->Move(110, this->geometry.height - this->m_inputAreaHeight + 4);
  this->m_chatEdit->maximum = 70;
  this->m_chatEdit->Background(240, 240, 240);
  this->AddChild(this->m_chatEdit);
  this->m_inputEdit = this->m_chatEdit;
  g_windowMgr.SetFocus(this->m_chatEdit);
  this->m_chatEdit->SetId(0x1ED);

  UIWhisperEditCtrl* whisperEdit = new UIWhisperEditCtrl();
  this->unknown_00C0 = whisperEdit;
  whisperEdit->SetSize(90, 16);
  whisperEdit->Move(6, this->geometry.height - this->m_inputAreaHeight + 4);
  whisperEdit->maximum = 24;
  whisperEdit->Background(240, 240, 240);
  this->AddChild(whisperEdit);
  this->m_message0 = 0xB8;

  UIResizer* resizer = new UIResizer();
  this->unknown_00B8 = resizer;
  resizer->SetBitmapName("유저인터페이스\\basic_interface\\dialog_mid.bmp");
  resizer->SetSize(resizer->geometry.width, resizer->geometry.height);
  resizer->SetDirection(0);
  resizer->Move(2, this->geometry.height - resizer->geometry.height);
  this->AddChild(resizer);

  std::string basePath = "유저인터페이스\\basic_interface\\";
  const int base = this->geometry.height - this->m_inputAreaHeight;
  const char* const buttonNames[3][4] = {
    {"dialog_btn0", "dialog_btn1", "dialog_btn2", nullptr},
    {"sys_base_off", "sys_base_on", "sys_base_on", nullptr},
    {"sys_base_off", "sys_base_on", "sys_base_on", nullptr},
  };
  const int buttonIds[3] = {0xE1, 0xD4, 0xEE};
  const int buttonPositions[3][2] = {{0x61, base + 3}, {0x24A, base + 7}, {0x23C, base + 7}};
  const char* const tooltips[3] = {LookupMessage(0xC73), LookupMessage(0xC75), LookupMessage(0xC74)};
  for (int index = 0; index < 3; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
    this->m_inputButtons[index] = button;
    button->SetBitmap((basePath + buttonNames[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((basePath + buttonNames[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((basePath + buttonNames[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(buttonPositions[index][0], buttonPositions[index][1]);
    button->SetId(buttonIds[index]);
    button->SetTooltip(tooltips[index]);
    this->AddChild(button);
  }

  UIResizer* splitter = new UIResizer();
  splitter->SetSize(this->geometry.width - 26, 6);
  splitter->SetDirection(2);
  splitter->Move(3, this->m_inputOffsetY);
  splitter->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(splitter);

  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  this->m_inputX = screenX + 3;
  this->m_listColor = 0x66000000;
  this->m_inputY = screenY + this->m_inputOffsetY;
  std::memset(this->unknown_00DC, 0, sizeof(this->unknown_00DC));
  this->m_listWidth = this->geometry.width - 6;
  std::memset(this->unknown_00D4, 0, sizeof(this->unknown_00D4));
  this->m_listHeight = this->geometry.height - this->m_inputOffsetY - this->m_inputAreaHeight;

  if (!g_windowMgr.chatModeChanged) {
    g_windowMgr.inputSuggestionsEnabled = 1;
    this->ApplyInputLayout();
    g_windowMgr.SetFocus(this->m_inputEdit);
  }

  if (this->m_chatEdit) {
    this->m_suggestions = new (std::nothrow) UICashEmotionMiniWnd();
    if (this->m_suggestions) {
      this->m_suggestions->SetSize(192, 44);
      this->m_suggestions->Move(this->m_chatEdit->geometry.x, this->m_chatEdit->geometry.y - this->m_suggestions->geometry.height);
      this->AddChild(this->m_suggestions);
      if (this->m_suggestions) {
        this->m_suggestions->Show(0);
      }
    }
  }
}

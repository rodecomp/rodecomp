#include "Client/Session.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Shortcuts/ShortcutActivation.h"
#include "UI/Shortcuts/ShortcutBindings.h"
#include "UI/Shortcuts/ShortcutControlEvents.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Shortcuts/ShortcutDetails.h"
#include "UI/Shortcuts/ShortcutDrop.h"
#include "UI/Shortcuts/ShortcutLayout.h"
#include "UI/Shortcuts/ShortcutPainting.h"
#include "UI/Shortcuts/ShortcutPress.h"
#include "UI/Shortcuts/ShortcutTooltip.h"
#include "UI/Shortcuts/ShortcutUse.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Chat/UISubChatMiniWnd.h"
#include "UI/Dialogs/UIMemorialDunWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
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
#include "Client/ReplayPacketRecorder.h"
#include "Contents/NavigationManager.h"
#include "UI/Window/UINavigationWnd.h"
#include "Utility/Text/FormatString.h"
#include "File/File.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <map>
#include <new>
#include <string>

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

// GLOBAL: gl_24737
int g_memorialDunWndX;

// GLOBAL: gl_24738
int g_memorialDunWndY;

// GLOBAL: gl_24888
static bool g_subChatMiniFirstCollapsed;

// GLOBAL: gl_24889
static bool g_subChatMiniSecondCollapsed;

// OFFSET: 0x009122B0
// FUNCTION: fn_12528
UIShortCutWnd::UIShortCutWnd(UIWindowContext& windows, UIFrameContext& frames, ShortcutWindowContext& shortcuts)
    : UIFrameWnd(windows, frames, 0), m_shortcuts(&shortcuts) {
  this->m_controls.decrease = nullptr;
  this->m_controls.increase = nullptr;
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
  this->m_frameContext->snapping.Constrain(this, x, y, 0);
  this->geometry.x = (this->geometry.x + x);
  const int top = (this->geometry.y + y);
  this->geometry.y = top < 0 ? 0 : top;
  this->m_frameContext->snapping.Store(this);
}

// OFFSET: 0x00917640
void UIShortCutWnd::Slot10(int x, int y, std::intptr_t value) {
  this->m_shortcuts->drop.Drop(*this, this->m_grid, x, y, *reinterpret_cast<const ModeDragState*>(value));
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

  if (visible == 0) {
    if (this->m_frameContext->snapping.enabled) {
      this->m_frameContext->snapping.rectangles.erase(this);
    }

    return;
  }

  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
  this->Invalidate();
  this->m_frameContext->snapping.Store(this);
}

// OFFSET: 0x0092A190
void UIShortCutWnd::Slot15(int, int) {
  this->m_controls =
    ShortcutControls::Create(*this, *this->m_context, this->m_shortcuts->buttons, g_session.GetOption(10), this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x00932F30
// FUNCTION: fn_12720
void UIShortCutWnd::Slot19() {
  const int x = (this->m_shortcuts->cursorX - this->geometry.x);
  const int y = (this->m_shortcuts->cursorY - this->geometry.y);
  this->m_grid.hovered = this->m_grid.HitTest(x, y, this->m_shortcuts->itemSlotActive, this->m_shortcuts->currentRow);
  this->Invalidate();
}

// OFFSET: 0x0092F8E0
void UIShortCutWnd::Paint() {
  this->m_shortcuts->painting.Paint(*this, this->m_grid, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x009310E0
void UIShortCutWnd::Slot25(int x, int y) {
  this->m_shortcuts->press.Press(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x00930900
void UIShortCutWnd::Slot26(int x, int y) {
  ActivateShortcut(
    *this, this->m_grid, x, y, this->m_shortcuts->itemSlotActive, this->m_shortcuts->currentRow, this->m_slots, this->m_shortcuts->automatic
  );
}

// OFFSET: 0x00932050
void UIShortCutWnd::Slot28(int x, int y) {
  this->m_shortcuts->tooltip.Hover(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x009316F0
// FUNCTION: fn_12705
void UIShortCutWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
  this->Slot11();
}

// OFFSET: 0x009332D0
void UIShortCutWnd::Slot32(int x, int y) {
  this->m_shortcuts->details.Open(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// INLINED
inline void ShortcutControlEvents::Click(int id) {
  switch (id) {
  case 201:
    this->visibility.Show(0);
    return;

  case 216:
    if (this->window.geometry.visible == 0) {
      this->visibility.Show(1);
      return;
    }

    this->grid.rows = this->grid.rows + 1;

    if (this->grid.rows > 4) {
      this->grid.rows = 1;
      this->visibility.Show(0);
    }

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 217:
    if (this->grid.rows > 1) {
      --this->grid.rows;
    }

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 549: {
    const bool checked = g_session.GetOption(10) == 0;
    g_session.SetOption(10, checked ? 1 : 0, 0);
    this->controls.bank->geometry.unknown_0030 = checked ? 1 : 0;
    this->controls.bank->Invalidate();

    for (ShortcutCooldownRecord& record : this->cooldowns.records) {
      record.main.clear();
    }

    this->window.SendMsg(nullptr, 23, 0, 0, 0, 0);
    return;
  }

  case 588:
  case 589: {
    this->itemSlotActive = id == 588;
    const bool active = this->itemSlotActive;

    this->controls.showItems->Enable(active ? 0 : 1);
    this->controls.showItems->SetDisabled(active ? 1 : 0);
    this->controls.showItems->Invalidate();

    this->controls.hideItems->Enable(active ? 1 : 0);
    this->controls.hideItems->SetDisabled(active ? 0 : 1);
    this->controls.hideItems->Invalidate();

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;
  }

  default:
    return;
  }
}

// INLINED
inline void ShortcutLayout::PlaceRowButtons() {
  this->decrease.Move(this->window.geometry.width - this->decrease.geometry.width - 2, this->grid.rows * 33 - 28);
  this->increase.Move(this->window.geometry.width - this->increase.geometry.width - 2, this->grid.rows * 33 - 13);
  this->decrease.Enable(this->grid.rows == 1 ? 0 : 1);
  this->increase.Enable(1);
  this->increase.SetDisabled(this->grid.rows == 4 ? 1 : 0);
  this->increase.Invalidate();
}

// INLINED
inline void ShortcutLayout::ResizeRows() {
  const int height = (this->itemSlotActive ? 43 : 10) + this->grid.rows * 33;
  if (height == this->window.geometry.height) {
    return;
  }

  this->window.Resize(this->window.geometry.width, height);

  this->decrease.Move(this->window.geometry.width - this->decrease.geometry.width - 2, this->grid.rows * 33 - 28);
  this->increase.Move(this->window.geometry.width - this->increase.geometry.width - 2, this->grid.rows * 33 - 13);
  this->showItems.Move(247, height - 10);
  this->hideItems.Move(247, height - 10);
  this->decrease.Enable(this->grid.rows == 1 ? 0 : 1);
  this->increase.Enable(1);
  this->increase.SetDisabled(this->grid.rows == 4 ? 1 : 0);
  this->increase.Invalidate();
  this->window.Slot11();
}

// INLINED
inline void ShortcutLayout::Restore(NormalItemSlotWindow* value) {
  this->saved = value;
  if (!value->IsValid(this->screenWidth, this->screenHeight, this->itemSlotActive)) {
    value->Reset();
  }

  const int height = value->height;
  this->window.Resize(this->window.geometry.width, height);
  const int extent = this->itemSlotActive ? height - 43 : height;
  this->showItems.Move(247, height - 10);
  this->hideItems.Move(247, height - 10);
  this->showItems.Enable(this->itemSlotActive ? 0 : 1);
  this->showItems.SetDisabled(this->itemSlotActive ? 1 : 0);
  this->showItems.Invalidate();
  this->hideItems.Enable(this->itemSlotActive ? 1 : 0);
  this->hideItems.SetDisabled(this->itemSlotActive ? 0 : 1);
  this->hideItems.Invalidate();
  this->grid.rows = extent / 33;
  this->PlaceRowButtons();
  this->window.SendMsg(nullptr, 23, 0, 0, 0, 0);
  this->window.Move(value->x, value->y);
}

// OFFSET: 0x00940FE0
inline void ShortcutLayout::Persist() {
  if (!this->saved) {
    return;
  }

  this->saved->x = this->window.geometry.x;
  this->saved->y = this->window.geometry.y;
  this->saved->width = this->window.geometry.width;
  this->saved->height = this->window.geometry.height;
}

// INLINED
inline void ShortcutUse::Use(const NormalItemSlot& slot) {
  if (g_session.GetOption(5) > 0 || slot.itemId == 0) {
    return;
  }

  if (slot.unknown_0000 != 0) {
    CSkillInfo skill = this->skills.Lookup(slot.itemId);
    if (skill.MatchesShortcut(&slot)) {
      int level = slot.unknown_0005;
      if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
        level = skill.level;
      }

      this->mode->SendModeMessage(113, reinterpret_cast<std::intptr_t>(&skill), level, 0, 0);
    }

    return;
  }

  ITEM_INFO item = LookupClientItem(this->inventory, static_cast<int>(slot.itemId), 1);
  if (item.unknown_0000[1] == 0) {
    return;
  }

  ModeEvents* mode = this->mode;

  const auto use = [&]() { mode->SendModeMessage(27, item.unknown_0000[1], 0, 0, 0); };

  switch (item.unknown_0000[0]) {
  case 0:
  case 1:
  case 2:
    if (item.GetItemId() != 12802 || this->dialogs.Confirm(this->naming.Message(2373), 2, 0, 0, "Message", 280, 120) != 188) {
      use();
    }

    break;

  case 10:
  case 16:
  case 17:
  case 19:
    if (item.unknown_0000[3] == 0x8000) {
      mode->SendModeMessage(87, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;

  case 18:
    if (this->itemConfirmationState >= 0) {
      const std::string name = this->naming.DisplayName(item, 0);
      char text[4096];

      _snprintf_s(text, sizeof(text), _TRUNCATE, this->naming.Message(1226), name.c_str());

      if (this->dialogs.Confirm(text, 2, 0, 0, "Message", 280, 120) == 187) {
        use();
      }
    }

    break;

  default:
    if (item.unknown_0000[2] != 0 && item.unknown_0000[3] == 0 && item.identified != 0) {
      mode->SendModeMessage(19, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;
  }
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
int UIShortCutWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  ShortcutWindowContext& owner = *this->m_shortcuts;

  switch (message) {
  case 6: {
    ShortcutControlEvents events{*this, this->m_grid, this->m_controls, owner.visibility, owner.bindings.cooldowns, owner.itemSlotActive};
    events.Click(static_cast<int>(a));
    return 0;
  }

  case 14: {
    ShortcutLayout layout{
      *this,
      this->m_grid,
      this->m_saved,
      *this->m_controls.decrease,
      *this->m_controls.increase,
      *this->m_controls.showItems,
      *this->m_controls.hideItems,
      owner.itemSlotActive,
      this->m_frameContext->snapping.screenWidth,
      this->m_frameContext->snapping.screenHeight
    };
    layout.ResizeRows();
    return 0;
  }

  case 23:
    owner.bindings.Refresh(this->m_slots, this->m_itemSlots);
    this->Invalidate();
    return 0;

  case 34: {
    ShortcutLayout layout{
      *this,
      this->m_grid,
      this->m_saved,
      *this->m_controls.decrease,
      *this->m_controls.increase,
      *this->m_controls.showItems,
      *this->m_controls.hideItems,
      owner.itemSlotActive,
      this->m_frameContext->snapping.screenWidth,
      this->m_frameContext->snapping.screenHeight
    };
    layout.Restore(reinterpret_cast<NormalItemSlotWindow*>(a));
    return 0;
  }

  case 41: {
    const int index = 9 * static_cast<int>(b) + static_cast<int>(a);
    owner.use.Use(*this->m_slots[index]);
    return 0;
  }

  case 143:
    if (owner.bindings.cooldowns.RotateRows(static_cast<int>(a), static_cast<int>(b), owner.rotation0, owner.rotation1, owner.clock)) {
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
    window->SendMsg(this, 6, 197, this->geometry.x + x, this->geometry.y + y, 0);
  }
}

// OFFSET: 0x0093D090
// FUNCTION: fn_12765
void UINewChatWnd::Show(int visible) {
  this->geometry.visible = visible;

  if (visible != 0) {
    this->Invalidate();
    this->m_frameContext->snapping.Store(this);
  } else if (this->m_frameContext->snapping.enabled) {
    this->m_frameContext->snapping.rectangles.erase(this);
  }

  for (UIWindow* tab : this->m_tabs) {
    tab->Show(this->geometry.visible);
  }

  for (unsigned int i = 0; i < g_windowMgr.retainedWindows.size(); ++i) {
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
  if (static_cast<unsigned int>(y - 1) > 19) {
    InputFocus{g_windowMgr.hoverWindow}.Set(this->m_inputEdit);
    this->UIFrameWnd::Slot25(x, y);
    return;
  }

  if (static_cast<int>(this->geometry.unknown_0030) <= 0) {
    return;
  }

  if (y < 14 && x > this->m_lockButtonX && x < this->m_lockButtonX + 9) {
    g_chatInputFlag = g_chatInputFlag == 0;
    reinterpret_cast<ChatHeaderLayout*>(this)->Apply();
    for (unsigned int i = 0; i < g_windowMgr.retainedWindows.size(); ++i) {
      reinterpret_cast<ChatButtonLayout*>(g_windowMgr.retainedWindows[i])->Apply();
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

  if (g_windowMgr.unknown_0026 && y < 14 && x > this->m_chatModeX && x < this->m_chatModeX + 14) {
    g_windowMgr.unknown_0024 = g_windowMgr.unknown_0024 == 0;
    if (g_chatSuggestions) {
      reinterpret_cast<ChatInputLayout*>(g_chatSuggestions)->Apply();
    }
    this->Invalidate();
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

  if (g_windowMgr.unknown_0024) {
    auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\dialog_bg.bmp")));
    this->DrawBitmap(0, this->geometry.height - background->image.height, &background->image, false);
  }

  if (static_cast<int>(this->geometry.unknown_0030) > 0) {
    if (g_windowMgr.unknown_0026) {
      auto* chatMode = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
        Resolve(g_windowMgr.unknown_0024 ? "유저인터페이스\\basic_interface\\chatmode_on.bmp" : "유저인터페이스\\basic_interface\\chatmode_off.bmp")
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
      reinterpret_cast<bool(__thiscall*)(CNavigationManager*, std::string, int, bool)>(0x00B995D0)(&g_navigationManager, name, 0, false);
      if (g_navigationWindow) {
        static_cast<UINavigationV4Wnd*>(g_navigationWindow)->SelectRoute(0);
        static_cast<UINavigationV4Wnd*>(g_navigationWindow)->SelectRouteNode(0);
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
    const TextLocale locale = {};
    int width = 0;
    for (const std::string& name : this->unknown_01FC) {
      const int textWidth = this->TextWidth(g_fontCache, locale, name.c_str(), 0, 0, 14, 0, 0);
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

  if (g_memorialDunWndX != left) {
    g_memorialDunWndX = left;
  }
  if (g_memorialDunWndY != top) {
    g_memorialDunWndY = top;
  }
}

// OFFSET: 0x00921260
// FUNCTION: fn_12641
void UIMemorialDunWnd::Slot15(int, int) {
  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "Alt+B", 0);

  {
    const std::string name = "유저인터페이스\\btn_ok";
    this->m_actionButton = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_actionButton->SetBitmap((name + ".bmp").c_str(), 0, 0);
    this->m_actionButton->SetBitmap((name + "_a.bmp").c_str(), 1, 0);
    this->m_actionButton->SetBitmap((name + "_b.bmp").c_str(), 2, 0);
    this->m_actionButton->SetSize(this->m_actionButton->BitmapWidth(), this->m_actionButton->BitmapHeight());
    this->m_actionButton->Move(-100, -100);
    this->m_actionButton->SetId(184);
    this->AddChild(this->m_actionButton);
  }

  this->m_partyButton = new (std::nothrow) UIBasicButton(*this->m_context, *this->m_controls);
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

  if (a == 197) {
    this->Move(b, c - this->m_offsetY);
  } else if (a == 202) {
    this->Slot25(this->m_rowHeight * b + 1, 0);
  }
  return 0;
}

// OFFSET: 0x0093CAB0
// FUNCTION: fn_12763
int UIWindowCallBtnWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6) {
    this->m_frameContext->windows.EnsureWindow(a);
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
  for (std::vector<UIWindow*>::iterator tab = this->m_tabs.begin(); tab != this->m_tabs.end(); ++tab, ++index) {
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

      if (static_cast<UIChatHisBox*>(*tab)->SaveChatLog(path)) {
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
    for (std::vector<UIWindow*>::iterator tab = this->m_tabs.begin(); tab != this->m_tabs.end(); ++tab, ++index) {
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
    reinterpret_cast<WindowReplayRecorder*>(this)->RecordData(0x38F, g_navigationManager.WriteStates());
  }

  this->FillRouteList();
  this->SendMsg(this, 6, 190, 0, 0, 0);
  return true;
}

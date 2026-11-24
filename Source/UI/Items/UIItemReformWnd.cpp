#include "UI/Window/WindowId.h"
#include "Contents/Items/ItemId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Items/UIItemReformWnd.h"
#include "UI/Window/UIEnums.h"

#include "Client/Input/InputCharset.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/ItemReformMgr.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/CreateLineInfo.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <new>
#include <string>
#include <vector>

#pragma pack(push, 1)
struct PACKET_CZ_ITEM_REFORM {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ int itemId;
  /* 0006 */ std::int16_t index;
};
#pragma pack(pop)

// OFFSET: 0x005FC010
// FUNCTION: fn_04095
UIItemReformWnd::UIItemReformWnd()
    : UIFrameWnd(0), reformButton(nullptr), list(nullptr), baseIcon(nullptr), resultIcon(nullptr),
      materialIcons(), detailButton(nullptr), detailMsg(nullptr), reformItem(), selectedItem(nullptr), resultItem(), materials(),
      information(), warning() {
}

// OFFSET: 0x005FC160
// FUNCTION: fn_04096
UIItemReformWnd_List::UIItemReformWnd_List()
    : UIFrameWnd(0), itemHeight(32), selectedIndex(0xFFFFFFFF), hoverIndex(0), scrollPosition(0),
      scrollBar(nullptr), items() {
}

// OFFSET: 0x005FC7C0
// FUNCTION: fn_04105
void UIItemReformWnd_DetailMsg::SetText(const std::string& text) {
  this->Resize(257, 127);
  unsigned int height = this->geometry.height;
  CCreateLineInfo info(this, text.c_str(), this->geometry.width - 20, 4, LineTextStyle{0, 12, false, false}, false, 0, false, 0xFFFFFF, 0);
  std::vector<std::vector<UIWindow*>> lines = info.m_lines;
  for (auto lineIt = lines.begin(); lineIt != lines.end(); ++lineIt) {
    std::vector<UIWindow*> line = *lineIt;
    for (auto windowIt = line.begin(); windowIt != line.end(); ++windowIt) {
      UIWindow* window = *windowIt;
      if (window) {
        window->Move(window->geometry.x + 12, window->geometry.y + 12);
        this->AddChild(*windowIt);
        height = std::max<unsigned int>(window->geometry.y + 12, height);
      }
    }
  }
  if (height > static_cast<unsigned int>(this->geometry.height)) {
    this->Resize(this->geometry.width, height);
  }
}

// OFFSET: 0x005FCD10
// FUNCTION: fn_04107
void UIItemReformWnd_List::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x005FCD20
// FUNCTION: fn_04108
void UIItemReformWnd::OnCreate(int width, int height) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  UIBasicButton* cancel = new UIBasicButton();
  cancel->Configure(60, LookupMessage(MSI_CANCEL));
  cancel->Move(300, 236);
  cancel->SetId(ID_CLOSE);
  this->AddChild(cancel);

  this->reformButton = new UIBasicButton();
  this->reformButton->Configure(60, LookupMessage(MSI_ITEM_REFORM_OK));
  this->reformButton->Move(365, 236);
  this->reformButton->SetId(ID_OK);
  {
    UIButton* button = this->reformButton;
    button->SetDisabled(1);
    button->Invalidate();
  }
  this->AddChild(this->reformButton);

  this->list = new UIItemReformWnd_List();
  this->list->SetSize(222, 192);
  this->list->Move(215, 21);
  this->AddChild(this->list);

  this->baseIcon = new UIItemReformWnd_Item();
  this->baseIcon->SetSize(24, 24);
  this->baseIcon->Move(144, 50);
  this->AddChild(this->baseIcon);

  this->resultIcon = new UIItemReformWnd_Item();
  this->resultIcon->SetSize(24, 24);
  this->resultIcon->Move(54, 50);
  this->AddChild(this->resultIcon);

  for (unsigned int index = 0; index < CItemReformMgr::Instance()->m_maxMaterials; ++index) {
    UIItemReformWnd_Item* icon = new UIItemReformWnd_Item();
    icon->SetSize(24, 24);
    icon->Move(index % 3 * 69 + 30, index / 3 * 50 + 140);
    this->AddChild(icon);
    this->materialIcons.push_back(icon);
  }

  this->detailButton = new UIItemReformWnd_DetailBtn();
  this->detailButton->SetSize(149, 14);
  this->detailButton->Move(36, 124);
  this->detailButton->Enable(0);
  this->AddChild(this->detailButton);
}

// OFFSET: 0x005FD1B0
// FUNCTION: fn_04109
void UIItemReformWnd_List::OnCreate(int width, int height) {
  this->scrollBar = new UIScrollBar();
  this->scrollBar->Configure(width - 13, 0, 1, height);
  this->scrollBar->Show(0);
  this->AddChild(this->scrollBar);
  this->visibleCount = height / this->itemHeight;
  this->UpdateScrollBar();
}

// INLINED
static void DrawMaterialCount(UIItemReformWnd* window, int x, int y, unsigned int half, int owned, int required) {
  const std::uint32_t color = owned < required ? 0x321EC8 : 0x3C3C3C;
  const int left = x - half;
  window->DrawTextAt(left, y, "/", 0, 0, 12, 0, 0, 0);
  window->DrawRightAlignedText(left, y, FormatOwnedString("%d", owned).c_str(), 0, 0, 12, color, 0);
  window->DrawTextAt(x + half, y, FormatOwnedString("%d", required).c_str(), 0, 0, 12, color, 0, 0);
}

// OFFSET: 0x005FD270
// FUNCTION: fn_04110
void UIItemReformWnd::Paint() {
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ItemReform\\bg_reform.bmp"))), true);
  if (std::atoi(this->reformItem.itemId.c_str()) > 0) {
    this->DrawTitleBar(false, this->reformItem.DisplayName(0).c_str(), 0);
  }
  if (std::atoi(this->resultItem.itemId.c_str()) != 0) {
    this->DrawCenteredText(6, 104, this->FitDisplayText(200, this->resultItem.DisplayName(0).c_str(), 0, 14, 1).c_str(), 209, 0, 14, 0, 1);
  }
  for (unsigned int index = 0; index < this->materials.size(); ++index) {
    const unsigned int half = this->TextWidth("/", 0, 0, 12, 0, 0) * 0.5;
    const int x = index % 3 * 69 + 42;
    const int y = index / 3 * 50 + 167;
    const int owned = g_session.GetInventoryItemCount(std::atoi(this->materials[index].itemId.c_str()));
    const int required = this->materials[index].quantity;
    DrawMaterialCount(this, x, y, half, owned, required);
  }
  if (this->warning.size() != 0) {
    this->DrawTextAt(8, 240, this->FitDisplayText(280, this->warning.c_str(), 0, 12, 0).c_str(), 0, 0, 12, 0xA0AFF, 0, 0);
  }
}

// OFFSET: 0x005FD600
// FUNCTION: fn_04111
void UIItemReformWnd_DetailBtn::Paint() {
  this->ClearSurface(0xFFFF00FF);
  std::string text = LookupMessage(MSI_ITEM_REFORM_CONFIRM_DETAILS);
  const unsigned int textWidth = this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);
  this->DrawCenteredText(0, 0, text.c_str(), this->geometry.width, 0, 12, 0, 0);
  this->FillRegion((this->geometry.width - textWidth) * 0.5, 12, textWidth, 1, 0);
}

// OFFSET: 0x005FD770
// FUNCTION: fn_04112
void UIItemReformWnd_DetailMsg::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawFilledRoundedOutline(0, 0, this->geometry.width, this->geometry.height, 0xFFFFFF);
  this->DrawRoundedOutline(2, 2, this->geometry.width - 4, this->geometry.height - 4, 0xC8C8C8);
  this->FillRegion(8, 8, this->geometry.width - 16, this->geometry.height - 16, 0xF7F7F7);
}

// OFFSET: 0x005FD7E0
// FUNCTION: fn_04113
void UIItemReformWnd_Item::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->item) {
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\item\\%s.bmp", this->item->ResourcePath()).c_str()))), true);
  }
}

// INLINED
static void DrawReformRow(UIItemReformWnd_List* list, unsigned int row) {
  const unsigned int index = list->scrollPosition + row;
  const char* image;
  if (index == list->selectedIndex) {
    image = "유저인터페이스\\ItemReform\\btn_reform_item_press.bmp";
  } else {
    image = index == list->hoverIndex ? "유저인터페이스\\ItemReform\\btn_reform_item_over.bmp" : "유저인터페이스\\ItemReform\\btn_reform_item.bmp";
  }
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(image)));
  list->DrawBitmap(0, list->itemHeight * row, bitmap, true);
  const ITEM_INFO& item = list->items[list->scrollPosition + row];
  list->DrawItemIcon(15, list->itemHeight * row + 4, item, 1);
  list->DrawTextAt(42, list->itemHeight * row + 10, list->FitDisplayText(160, item.DisplayName(0).c_str(), 0, 12, 0).c_str(), 0, 0,
    12, 0, 0, 0
  );
}

// OFFSET: 0x005FD8A0
// FUNCTION: fn_04114
void UIItemReformWnd_List::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  for (unsigned int row = 0; row < this->visibleCount; ++row) {
    const unsigned int index = this->scrollPosition + row;
    if (index < this->items.size()) {
      DrawReformRow(this, row);
    }
  }
}

// OFFSET: 0x005FDA60
// FUNCTION: fn_04117
void UIItemReformWnd_List::OnLBtnUp(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count && this->selectedIndex != index) {
    this->selectedIndex = index;
    if (this->parent) {
      this->parent->SendMsg(nullptr, UM_COMMAND, ID_SELECT_ITEM, reinterpret_cast<std::intptr_t>(&this->items[index]), 0, 0);
    }
    this->Invalidate();
  }
}

// OFFSET: 0x005FDB00
// FUNCTION: fn_04118
void UIItemReformWnd_DetailBtn::OnMouseEnter() {
  if (this->parent) {
    this->parent->SendMsg(this, UM_SHOW_ITEM_REFORM_DETAIL, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDB20
// FUNCTION: fn_04119
void UIItemReformWnd_Item::OnMouseHover(int x, int y) {
  if (this->item) {
    int screenX = x;
    int screenY = y;
    this->ToScreen(screenX, screenY);
    CMode* mode = g_modeMgr.currentMode;
    const int tipY = screenY - 5;
    const int tipX = screenX + 5;
    mode->ShowTooltip(this->item->DisplayName(0).c_str(), tipX, tipY, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x005FDBE0
// FUNCTION: fn_04120
void UIItemReformWnd_List::OnMouseHover(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count && this->hoverIndex != index) {
    this->hoverIndex = index;
    this->Invalidate();
  }
}

// OFFSET: 0x005FDC50
// FUNCTION: fn_04121
void UIItemReformWnd_DetailBtn::OnMouseLeave() {
  if (this->parent) {
    this->parent->SendMsg(this, UM_HIDE_ITEM_REFORM_DETAIL, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDC80
// FUNCTION: fn_04123
void UIItemReformWnd::OnProcess() {
  if (this->detailMsg) {
    g_windowMgr.Promote(this->detailMsg);
    this->detailMsg->Move(g_mouse.x + 5, g_mouse.y + 5);
  }
}

// OFFSET: 0x005FDCC0
// FUNCTION: fn_04124
void UIItemReformWnd_Item::OnRBtnDown(int x, int y) {
  if (this->item) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
      window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(this->item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x005FDD10
// FUNCTION: fn_04126
void UIItemReformWnd_List::OnRBtnUp(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)) {
      window->SendMsg(this, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->items[index]), 0, 0, 0);
    }
  }
}

// OFFSET: 0x005FDDA0
// FUNCTION: fn_04127
void UIItemReformWnd_List::OnWheel(int delta) {
  if (delta < 0) {
    this->SendMsg(this->scrollBar, UM_SCROLLVERTPAGEDOWN, 0, 0, 0, 0);
  } else {
    this->SendMsg(this->scrollBar, UM_SCROLLVERTPAGEUP, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDDE0
// FUNCTION: fn_04128
void UIItemReformWnd_List::UpdateScrollBar() {
  const unsigned int count = this->items.size();
  if (count > this->visibleCount) {
    this->scrollBar->Show(1);
    this->scrollBar->SetRange(count - this->visibleCount, this->visibleCount, count);
    this->scrollPosition = std::max(0, this->scrollPosition);
    this->scrollPosition = std::min<unsigned int>(count - this->visibleCount, this->scrollPosition);
    this->scrollBar->SetPosition(this->scrollPosition);
  } else {
    this->scrollPosition = 0;
    this->scrollBar->Show(0);
  }

  this->Invalidate();
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x005FDEA0
// FUNCTION: fn_04129
void UIItemReformWnd::SelectItem(ITEM_INFO* item) {
  UIItemReformWnd_Item* baseIcon = this->baseIcon;
  baseIcon->item = nullptr;
  baseIcon->Invalidate();
  UIItemReformWnd_Item* resultIcon = this->resultIcon;
  resultIcon->item = nullptr;
  resultIcon->Invalidate();
  for (auto iconIt = this->materialIcons.begin(); iconIt != this->materialIcons.end(); ++iconIt) {
    UIItemReformWnd_Item* icon = *iconIt;
    if (icon) {
      icon->item = nullptr;
      icon->Invalidate();
    }
  }
  this->warning.clear();
  this->materials.clear();
  this->selectedItem = item;
  if (!item) {
    return;
  }

  ItemReformInfo* info = CItemReformMgr::Instance()->FindReform(std::atoi(this->reformItem.itemId.c_str()), std::atoi(this->selectedItem->itemId.c_str()));
  if (!info) {
    return;
  }

  this->resultItem.SetItemId(info->resultItem);
  this->resultItem.identity[0] = 5;
  this->resultItem.identified = 1;
  this->resultItem.refiningLevel = this->selectedItem->refiningLevel + static_cast<int>(info->changeRefineValue);
  SecureZeroMemory(this->resultItem.options, sizeof(this->resultItem.options));
  if (info->randomOptionCode.size() == 0) {
    this->resultItem.optionCount = this->selectedItem->optionCount;
    memcpy_s(this->resultItem.options, sizeof(this->resultItem.options), this->selectedItem->options, this->selectedItem->optionCount * 5);
  } else if (info->randomOptionCode != "G0") {
    this->resultItem.optionCount = 1;
    this->resultItem.options[0].id = g_itemOptionPlaceholderId;
  }
  if (info->preserveSocketItem == 1) {
    for (int i = 0; i < 4; ++i) {
      this->resultItem.cards[i] = this->selectedItem->cards[i];
    }
  }
  if (info->preserveGrade == 1) {
    this->resultItem.grade = this->selectedItem->grade;
  }

  for (std::map<int, std::uint64_t>::iterator it = info->materials.begin(), end = info->materials.end(); it != end; ++it) {
    const int id = it->first;
    const int quantity = static_cast<int>(it->second);
    ITEM_INFO material;
    material.SetItemId(id);
    material.quantity = quantity;
    material.identified = 1;
    this->materials.push_back(material);
  }

  {
    UIButton* button = this->reformButton;
    button->SetDisabled(1);
    button->Invalidate();
  }
  this->detailButton->Enable(0);
  bool insufficient = false;
  for (std::vector<ITEM_INFO>::iterator it = this->materials.begin(); it != this->materials.end(); ++it) {
    if (g_session.GetInventoryItemCount(std::atoi(it->itemId.c_str())) < it->quantity) {
      insufficient = true;
      break;
    }
  }
  if (insufficient) {
    this->warning = LookupMessage(MSI_ITEM_REFORM_WARNNING_NOT_ENOUGH_MATERIAL);
  } else {
    {
      UIButton* button = this->reformButton;
      button->SetDisabled(0);
      button->Invalidate();
    }
  }

  this->information = info->informationString;
  if (this->information.size() != 0) {
    this->detailButton->Enable(1);
    this->warning = LookupMessage(MSI_ITEM_REFORM_PLZ_CHECK_DETAILS);
  }

  baseIcon = this->baseIcon;
  baseIcon->item = this->selectedItem;
  baseIcon->Invalidate();
  resultIcon = this->resultIcon;
  resultIcon->item = &this->resultItem;
  resultIcon->Invalidate();
  for (unsigned int index = 0; index < this->materials.size(); ++index) {
    if (index >= this->materialIcons.size()) {
      break;
    }
    UIItemReformWnd_Item* icon = this->materialIcons[index];
    icon->item = &this->materials[index];
    icon->Invalidate();
  }
  this->Invalidate();
}

// OFFSET: 0x005FE480
// FUNCTION: fn_04130
int UIItemReformWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_REFRESHITEM:
    this->reformItem.SetItemId(argument1);
    if (this->list->SendMsg(this, UM_REFRESHITEM, argument1, 0, 0, 0) == -1) {
      this->warning = LookupMessage(MSI_ITEM_REFORM_NO_EXIST_TARGET_ITEM);
    }
    this->Invalidate();
    break;
  case UM_COMMAND:
    switch (argument1) {
    case ID_OK:
      if (this->selectedItem && this->reformItem.GetItemId() != ITEMID_NONE) {
        PACKET_CZ_ITEM_REFORM packet;
        packet.packetType = ID_CZ_ITEM_REFORM;
        packet.itemId = this->reformItem.GetItemId();
        packet.index = static_cast<std::int16_t>(this->selectedItem->identity[1]);
        GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetType), &packet);
      }
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_ITEMREFORMWND);
      break;
    case ID_SELECT_ITEM:
      this->SelectItem(reinterpret_cast<ITEM_INFO*>(argument2));
      break;
    }
    break;
  case UM_SHOW_ITEM_REFORM_DETAIL:
    if (sender == this->detailButton && !this->detailMsg) {
      this->detailMsg = new UIItemReformWnd_DetailMsg();
      this->detailMsg->SetText(this->information);
      g_windowMgr.AddRendered(this->detailMsg);
    }
    break;
  case UM_HIDE_ITEM_REFORM_DETAIL:
    if (sender == this->detailButton && this->detailMsg) {
      g_windowMgr.Schedule(this->detailMsg);
      this->detailMsg = nullptr;
    }
    break;
  }
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x005FE770
// FUNCTION: fn_04131
int UIItemReformWnd_List::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const unsigned int previous = this->scrollPosition;
    if (message == UM_SCROLLVERTPAGEUP) {
      argument1 = -1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      argument1 = 1;
    }
    this->scrollPosition = previous + argument1;
    if (previous != this->scrollPosition) {
      this->UpdateScrollBar();
    }
    return this->scrollPosition - previous;
  }
  case UM_REFRESHITEM: {
    this->items.clear();
    const int reformItemId = static_cast<int>(argument1);
    std::vector<int> baseItems = CItemReformMgr::Instance()->GetReformBaseItems(reformItemId);
    for (std::vector<int>::iterator base = baseItems.begin(); base != baseItems.end(); ++base) {
      ItemReformInfo* info = CItemReformMgr::Instance()->FindReform(reformItemId, *base);
      if (!info) {
        continue;
      }
      std::vector<int> indices = g_session.FindInventoryIndicesByItemId(*base, 1);
      for (std::vector<int>::iterator index = indices.begin(); index != indices.end(); ++index) {
        if (g_session.HasSwapEquipmentItem(*index) != 0) {
          continue;
        }
        ITEM_INFO item = g_session.LookupClientItemByIdentity(*index);
        const std::uint64_t refine = item.refiningLevel;
        if (info->needRefineMin > refine || info->needRefineMax < refine) {
          continue;
        }
        const std::uint64_t options = item.optionCount;
        if (info->needOptionNumMin > options) {
          continue;
        }
        if (info->isEmptySocket == 1) {
          unsigned int slot = std::min(item.SlotCount(), 4);
          for (; slot < 4; ++slot) {
            if (static_cast<int>(slot) < 4 && static_cast<int>(item.cards[slot]) > 0) {
              break;
            }
          }
          if (slot < 4) {
            continue;
          }
        }
        this->items.push_back(item);
      }
    }
    this->UpdateScrollBar();
    this->Invalidate();
    if (this->items.empty()) {
      return -1;
    }
    break;
  }
  }
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x005FECD0
// FUNCTION: fn_04137
CItemReformMgr* CItemReformMgr::Instance() {
  if (!g_itemReformMgr) {
    g_itemReformMgr = new (std::nothrow) CItemReformMgr;
  }
  return g_itemReformMgr;
}

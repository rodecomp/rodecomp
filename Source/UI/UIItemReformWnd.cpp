#include "UI/UIItemReformWnd.h"

#include "Client/Input/InputCharset.h"
#include "Client/Input/Mouse.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/ItemReformMgr.h"
#include "Contents/Items/SelectPackageItemMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/CreateLineInfo.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"
#include "Utility/Text/FormatString.h"

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

// OFFSET: 0x005FC7C0
// FUNCTION: fn_04105
void UIItemReformWnd_DetailMsg::SetText(const std::string& text) {
  this->Resize(257, 127);
  unsigned int height = this->geometry.height;
  CCreateLineInfo info(this, text.c_str(), this->geometry.width - 20, 4, 0, 12, LineFontFlags{}, false, 0, false, 0xFFFFFF, 0);
  std::vector<std::vector<UIWindow*>> lines = info.m_lines;
  for (std::vector<UIWindow*> line : lines) {
    for (UIWindow* window : line) {
      if (window) {
        window->Move(window->geometry.x + 12, window->geometry.y + 12);
        this->AddChild(window);
        height = std::max<unsigned int>(height, window->geometry.y + 12);
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
void UIItemReformWnd::Slot15(int width, int height) {
  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "", 0);

  UIBasicButton* cancel = new UIBasicButton(*this->m_context, *this->m_controls);
  cancel->Configure(60, LookupMessage(218));
  cancel->Move(300, 236);
  cancel->SetId(201);
  this->AddChild(cancel);

  this->reformButton = new UIBasicButton(*this->m_context, *this->m_controls);
  this->reformButton->Configure(60, LookupMessage(3847));
  this->reformButton->Move(365, 236);
  this->reformButton->SetId(184);
  this->reformButton->SetDisabled(1);
  this->reformButton->Invalidate();
  this->AddChild(this->reformButton);

  this->list = new UIItemReformWnd_List(*this->m_context, *this->m_frameContext);
  this->list->SetSize(222, 192);
  this->list->Move(215, 21);
  this->AddChild(this->list);

  this->baseIcon = new UIItemReformWnd_Item(*this->m_context);
  this->baseIcon->SetSize(24, 24);
  this->baseIcon->Move(144, 50);
  this->AddChild(this->baseIcon);

  this->resultIcon = new UIItemReformWnd_Item(*this->m_context);
  this->resultIcon->SetSize(24, 24);
  this->resultIcon->Move(54, 50);
  this->AddChild(this->resultIcon);

  for (unsigned int index = 0; index < CItemReformMgr::Instance()->m_maxMaterials; ++index) {
    UIItemReformWnd_Item* icon = new UIItemReformWnd_Item(*this->m_context);
    icon->SetSize(24, 24);
    icon->Move(index % 3 * 69 + 30, index / 3 * 50 + 140);
    this->AddChild(icon);
    this->materialIcons.push_back(icon);
  }

  this->detailButton = new UIItemReformWnd_DetailBtn(*this->m_context, *this->m_controls);
  this->detailButton->SetSize(149, 14);
  this->detailButton->Move(36, 124);
  this->detailButton->Enable(0);
  this->AddChild(this->detailButton);
}

// OFFSET: 0x005FD1B0
// FUNCTION: fn_04109
void UIItemReformWnd_List::Slot15(int width, int height) {
  this->scrollBar = new UIScrollBar(*this->m_context);
  this->scrollBar->Configure(width - 13, 0, 1, height);
  this->scrollBar->Show(0);
  this->AddChild(this->scrollBar);
  this->visibleCount = height / this->itemHeight;
  this->UpdateScrollBar();
}

// OFFSET: 0x005FD270
// FUNCTION: fn_04110
void UIItemReformWnd::Paint() {
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ItemReform\\bg_reform.bmp")))->image, true);
  UIWindowContext& context = *this->m_context;
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  if (std::atoi(this->reformItem.itemId.c_str()) > 0) {
    this->DrawTitleBar(context.fonts, context.locale, false, naming.DisplayName(this->reformItem, 0).c_str(), 0);
  }
  if (std::atoi(this->resultItem.itemId.c_str()) != 0) {
    this->DrawCenteredText(6, 104, this->FitDisplayText(200, naming.DisplayName(this->resultItem, 0).c_str(), 0, 14, 1).c_str(), 209, 0, 14, 0, 1);
  }
  for (unsigned int index = 0; index < this->materials.size(); ++index) {
    const unsigned int half = this->TextWidth(context.fonts, context.locale, "/", 0, 0, 12, 0, 0) * 0.5;
    const int x = index % 3 * 69 + 42;
    const int y = index / 3 * 50 + 167;
    const int owned = g_session.GetInventoryItemCount(std::atoi(this->materials[index].itemId.c_str()));
    const int required = this->materials[index].quantity;
    const std::uint32_t color = owned < required ? 0x321EC8 : 0x3C3C3C;
    this->DrawTextAt(context.fonts, context.locale, x - half, y, "/", 0, 0, 12, 0, 0, 0);
    this->DrawRightAlignedText(x - half, y, FormatOwnedString("%d", owned).c_str(), 0, 0, 12, color, 0);
    this->DrawTextAt(context.fonts, context.locale, x + half, y, FormatOwnedString("%d", required).c_str(), 0, 0, 12, color, 0, 0);
  }
  if (this->warning.size() != 0) {
    this->DrawTextAt(context.fonts, context.locale, 8, 240, this->FitDisplayText(280, this->warning.c_str(), 0, 12, 0).c_str(), 0, 0, 12, 0xA0AFF, 0, 0);
  }
}

// OFFSET: 0x005FD600
// FUNCTION: fn_04111
void UIItemReformWnd_DetailBtn::Paint() {
  this->ClearSurface(0xFFFF00FF);
  std::string text = LookupMessage(3854);
  UIButtonContext& controls = *this->m_buttonContext;
  const unsigned int textWidth = this->TextWidth(controls.fonts, controls.locale, text.c_str(), 0, 0, 12, 0, 0);
  this->DrawCenteredText(0, 0, text.c_str(), this->geometry.width, 0, 12, 0, 0);
  this->FillRegion((this->geometry.width - textWidth) * 0.5, 12, textWidth, 1, 0);
}

// OFFSET: 0x005FD770
// FUNCTION: fn_04112
void UIItemReformWnd_DetailMsg::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFF);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, this->geometry.height - 4, 0xC8C8C8);
  this->FillRegion(8, 8, this->geometry.width - 16, this->geometry.height - 16, 0xF7F7F7);
}

// OFFSET: 0x005FD7E0
// FUNCTION: fn_04113
void UIItemReformWnd_Item::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->item) {
    ResourceMgr& resources = ResourceMgr::Instance();
    const std::string name = FormatOwnedString("유저인터페이스\\item\\%s.bmp", ItemResourcePath(*this->item));
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(resources.Load(Resolve(name.c_str())));
    this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, true);
  }
}

// OFFSET: 0x005FD8A0
// FUNCTION: fn_04114
void UIItemReformWnd_List::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  UIWindowContext& context = *this->m_context;
  for (unsigned int row = 0; row < this->visibleCount; ++row) {
    const unsigned int index = this->scrollPosition + row;
    if (index < this->items.size()) {
      const char* image;
      if (index == this->selectedIndex) {
        image = "유저인터페이스\\ItemReform\\btn_reform_item_press.bmp";
      } else {
        image = index == this->hoverIndex ? "유저인터페이스\\ItemReform\\btn_reform_item_over.bmp" : "유저인터페이스\\ItemReform\\btn_reform_item.bmp";
      }
      this->DrawBitmap(0, this->itemHeight * row, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(image)))->image, true);
      const ITEM_INFO& item = this->items[this->scrollPosition + row];
      ItemDescriptions descriptions{g_itemDescriptions};
      DrawItemIcon(*this, ResourceMgr::Instance(), descriptions, 15, this->itemHeight * row + 4, item, 1);
      bool languagePrefix = false;
      WORD language = g_serverLanguage->language;
      ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
      this->DrawTextAt(
        context.fonts, context.locale, 42, this->itemHeight * row + 10, this->FitDisplayText(160, naming.DisplayName(item, 0).c_str(), 0, 12, 0).c_str(), 0, 0,
        12, 0, 0, 0
      );
    }
  }
}

// OFFSET: 0x005FDA60
// FUNCTION: fn_04117
void UIItemReformWnd_List::Slot31(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count && this->selectedIndex != index) {
    this->selectedIndex = index;
    if (this->parent) {
      this->parent->SendMsg(nullptr, 6, 336, reinterpret_cast<std::intptr_t>(&this->items[index]), 0, 0);
    }
    this->Invalidate();
  }
}

// OFFSET: 0x005FDB00
// FUNCTION: fn_04118
void UIItemReformWnd_DetailBtn::Slot47() {
  if (this->parent) {
    this->parent->SendMsg(this, 167, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDB20
// FUNCTION: fn_04119
void UIItemReformWnd_Item::Slot28(int x, int y) {
  if (this->item) {
    int screenX = x;
    int screenY = y;
    this->ToScreen(screenX, screenY);
    const int tipX = screenX + 5;
    const int tipY = screenY - 5;
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(naming.DisplayName(*this->item, 0).c_str(), tipX, tipY, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x005FDBE0
// FUNCTION: fn_04120
void UIItemReformWnd_List::Slot28(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count && this->hoverIndex != index) {
    this->hoverIndex = index;
    this->Invalidate();
  }
}

// OFFSET: 0x005FDC50
// FUNCTION: fn_04121
void UIItemReformWnd_DetailBtn::Slot48() {
  if (this->parent) {
    this->parent->SendMsg(this, 168, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDC80
// FUNCTION: fn_04123
void UIItemReformWnd::Slot19() {
  if (this->detailMsg) {
    reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this->detailMsg);
    this->detailMsg->Move(g_mouse.x + 5, g_mouse.y + 5);
  }
}

// OFFSET: 0x005FDCC0
// FUNCTION: fn_04124
void UIItemReformWnd_Item::Slot32(int x, int y) {
  if (this->item) {
    if (UIWindow* window = g_windowMgr.EnsureWindow(12)) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(this->item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x005FDD10
// FUNCTION: fn_04126
void UIItemReformWnd_List::Slot33(int x, int y) {
  const unsigned int count = this->items.size();
  const unsigned int index = std::max(0u, std::min(y / this->itemHeight + this->scrollPosition, count - 1));
  if (index < count) {
    if (UIWindow* window = this->m_frameContext->windows.EnsureWindow(12)) {
      window->SendMsg(this, 24, reinterpret_cast<std::intptr_t>(&this->items[index]), 0, 0, 0);
    }
  }
}

// OFFSET: 0x005FDDA0
// FUNCTION: fn_04127
void UIItemReformWnd_List::Slot35(int delta) {
  if (delta < 0) {
    this->SendMsg(this->scrollBar, 10, 0, 0, 0, 0);
  } else {
    this->SendMsg(this->scrollBar, 9, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDDE0
// FUNCTION: fn_04128
void UIItemReformWnd_List::UpdateScrollBar() {
  const unsigned int count = this->items.size();
  if (count > this->visibleCount) {
    this->scrollBar->Show(1);
    this->scrollBar->SetRange(count - this->visibleCount, this->visibleCount, count);
    this->scrollPosition = std::min<unsigned int>(std::max<int>(this->scrollPosition, 0), count - this->visibleCount);
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
  this->baseIcon->item = nullptr;
  this->baseIcon->Invalidate();
  this->resultIcon->item = nullptr;
  this->resultIcon->Invalidate();
  for (std::vector<UIItemReformWnd_Item*>::iterator it = this->materialIcons.begin(); it != this->materialIcons.end(); ++it) {
    if (*it) {
      (*it)->item = nullptr;
      (*it)->Invalidate();
    }
  }
  this->warning.clear();
  this->materials.clear();
  this->selectedItem = item;
  if (!item) {
    return;
  }

  if (!g_itemReformMgr) {
    g_itemReformMgr = new (std::nothrow) CItemReformMgr;
  }
  ItemReformInfo* info = g_itemReformMgr->FindReform(std::atoi(this->reformItem.itemId.c_str()), std::atoi(this->selectedItem->itemId.c_str()));
  if (!info) {
    return;
  }

  this->resultItem.SetItemId(info->resultItem);
  this->resultItem.unknown_0000[0] = 5;
  this->resultItem.identified = 1;
  this->resultItem.unknown_0060 = this->selectedItem->unknown_0060 + static_cast<int>(info->changeRefineValue);
  for (int index = 0; index < 25; ++index) {
    this->resultItem.unknown_009C[index] = 0;
  }
  if (info->randomOptionCode.size() == 0) {
    this->resultItem.unknown_0098 = this->selectedItem->unknown_0098;
    memcpy_s(this->resultItem.unknown_009C, sizeof(this->resultItem.unknown_009C), this->selectedItem->unknown_009C, this->selectedItem->unknown_0098 * 5);
  } else if (info->randomOptionCode.size() != 2 || info->randomOptionCode[0] != 'G' || info->randomOptionCode[1] != '0') {
    this->resultItem.unknown_0098 = 1;
    *reinterpret_cast<short*>(this->resultItem.unknown_009C) = g_itemOptionPlaceholderId;
  }
  if (info->preserveSocketItem == 1) {
    std::memcpy(this->resultItem.unknown_001C, this->selectedItem->unknown_001C, sizeof(this->resultItem.unknown_001C));
  }
  if (info->preserveGrade == 1) {
    this->resultItem.unknown_0088 = this->selectedItem->unknown_0088;
  }

  for (std::map<int, ItemReformMaterial>::iterator it = info->materials.begin(); it != info->materials.end(); ++it) {
    const int id = it->first;
    const int quantity = it->second.unknown_0004;
    ITEM_INFO material;
    material.SetItemId(id);
    material.quantity = quantity;
    material.identified = 1;
    this->materials.push_back(material);
  }

  this->reformButton->SetDisabled(1);
  this->reformButton->Invalidate();
  this->detailButton->Enable(0);
  bool insufficient = false;
  for (std::vector<ITEM_INFO>::iterator it = this->materials.begin(); it != this->materials.end(); ++it) {
    if (g_session.GetInventoryItemCount(std::atoi(it->itemId.c_str())) < it->quantity) {
      insufficient = true;
      break;
    }
  }
  if (insufficient) {
    this->warning = LookupMessage(3848);
  } else {
    this->reformButton->SetDisabled(0);
    this->reformButton->Invalidate();
  }

  this->information = info->informationString;
  if (this->information.size() != 0) {
    this->detailButton->Enable(1);
    this->warning = LookupMessage(3855);
  }

  this->baseIcon->item = this->selectedItem;
  this->baseIcon->Invalidate();
  this->resultIcon->item = &this->resultItem;
  this->resultIcon->Invalidate();
  for (unsigned int index = 0; index < this->materials.size(); ++index) {
    if (index >= this->materialIcons.size()) {
      break;
    }
    this->materialIcons[index]->item = &this->materials[index];
    this->materialIcons[index]->Invalidate();
  }
  this->Invalidate();
}

// OFFSET: 0x005FE480
// FUNCTION: fn_04130
int UIItemReformWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 6:
    if (argument1 == 184) {
      if (this->selectedItem && this->reformItem.GetItemId() != 0) {
        PACKET_CZ_ITEM_REFORM packet;
        packet.packetType = ID_CZ_ITEM_REFORM;
        packet.itemId = this->reformItem.GetItemId();
        packet.index = static_cast<std::int16_t>(this->selectedItem->unknown_0000[1]);
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
      }
    } else if (argument1 == 201) {
      g_windowMgr.CloseWindow(348);
    } else if (argument1 == 336) {
      this->SelectItem(reinterpret_cast<ITEM_INFO*>(argument2));
    }
    break;
  case 23:
    this->reformItem.SetItemId(argument1);
    if (this->list->SendMsg(this, 23, argument1, 0, 0, 0) == -1) {
      this->warning = LookupMessage(3856);
    }
    this->Invalidate();
    break;
  case 167:
    if (sender == this->detailButton && !this->detailMsg) {
      this->detailMsg = new UIItemReformWnd_DetailMsg(*this->m_context);
      this->detailMsg->SetText(this->information);
      g_windowMgr.AddRendered(this->detailMsg);
    }
    break;
  case 168:
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
  case 7:
  case 9:
  case 10: {
    const unsigned int previous = this->scrollPosition;
    if (message == 9) {
      argument1 = -1;
    } else if (message == 10) {
      argument1 = 1;
    }
    this->scrollPosition = previous + argument1;
    if (previous != this->scrollPosition) {
      this->UpdateScrollBar();
    }
    return this->scrollPosition - previous;
  }
  case 23: {
    this->items.clear();
    std::vector<int> baseItems = CItemReformMgr::Instance()->GetReformBaseItems(argument1);
    for (std::vector<int>::iterator base = baseItems.begin(); base != baseItems.end(); ++base) {
      ItemReformInfo* info = CItemReformMgr::Instance()->FindReform(argument1, *base);
      if (!info) {
        continue;
      }
      std::vector<int> indices = g_session.FindInventoryIndicesByItemId(*base, 1);
      for (std::vector<int>::iterator index = indices.begin(); index != indices.end(); ++index) {
        if (g_session.HasItem61B8(*index) != 0) {
          continue;
        }
        ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *index);
        const std::uint64_t refine = item.unknown_0060;
        if (refine < static_cast<std::uint64_t>(info->needRefineMin) || refine > static_cast<std::uint64_t>(info->needRefineMax)) {
          continue;
        }
        const std::uint64_t options = item.unknown_0098;
        if (options < static_cast<std::uint64_t>(info->needOptionNumMin)) {
          continue;
        }
        bool emptySocket = true;
        if (info->isEmptySocket == 1) {
          for (unsigned int slot = std::min(ItemSlotsCount(item), 4); slot < 4; ++slot) {
            if (static_cast<int>(slot) < 4 && reinterpret_cast<const int*>(item.unknown_001C)[slot] > 0) {
              emptySocket = false;
              break;
            }
          }
        }
        if (emptySocket) {
          this->items.push_back(item);
        }
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

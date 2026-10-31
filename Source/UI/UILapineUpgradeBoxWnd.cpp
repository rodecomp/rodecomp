#include "UI/UILapineUpgradeBoxWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/UILapineScrollBar.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <windows.h>
#include <timeapi.h>

namespace {
#pragma pack(push, 1)
  struct LapineUpgradeClosePacket {
    /* 0000 */ std::int16_t id;
  };
#pragma pack(pop)
  static_assert(sizeof(LapineUpgradeClosePacket) == 2);
}

// INLINED
UILapineUpgradeBoxWnd::~UILapineUpgradeBoxWnd() {
  this->item = ITEM_INFO();

  LapineUpgradeClosePacket packet;
  packet.id = 0x0AB5;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x006035D0
// FUNCTION: fn_04193
void UILapineUpgradeBoxWndNoop() {}

// OFFSET: 0x00603B60
// FUNCTION: fn_04200
void UILapineUpgradeBoxWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\bg_lapineUpgrade.bmp")));
  this->DrawBitmap(0, 0, &background->image, true);

  if (this->title.size() != 0) {
    this->DrawTitleBar(controls.fonts, controls.locale, false, this->title.c_str(), 0);
  }

  this->DrawCenteredText(35, 47, LookupMessage(2896), 156, 0, 11, 0xCD7A51, 0);
  this->DrawCenteredText(35, 63, this->description.c_str(), 156, 0, 11, 0x844A42, 0);

  if (this->item.unknown_0000[1] != 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    DrawItemIcon(*this, controls.resources, descriptions, 101, 165, this->item, 0);
  }

  if (timeGetTime() < this->noticeTick + this->noticeDuration) {
    CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\icon_notify.bmp")));
    this->DrawBitmap(6, 229, &icon->image, true);
    this->DrawTextAt(controls.fonts, controls.locale, 25, 230, this->notice.c_str(), 0, 0, 12, 0xFF0000, 0, 0);
  }
}

// OFFSET: 0x006035E0
// FUNCTION: fn_04194
void UILapineUpgradeBoxWnd::Slot10(int x, int y, std::intptr_t value) {
  ModeEvents* mode = this->m_context->mode;
  if (this->item.unknown_0000[1] > 0) {
    mode->SendModeMessage(18, 0, 0, 0, 0);
    return;
  }
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 27) {
    this->SendMsg(this, 31, drag->id, 0, 0, 0);
  }
  mode->SendModeMessage(18, 0, 0, 0, 0);
}

// OFFSET: 0x00603650
// FUNCTION: fn_04195
void UILapineUpgradeBox_IngredientInfoWnd::Slot10(int x, int y, std::intptr_t value) {
  if (this->parent) {
    this->parent->Slot10(x + this->geometry.x, y + this->geometry.y, value);
  }
}

// OFFSET: 0x006036D0
// FUNCTION: fn_04197
void UILapineUpgradeBox_IngredientInfoWnd::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x00603DB0
// FUNCTION: fn_04203
void UILapineUpgradeBoxWnd::Slot26(int x, int y) {
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.unknown_0000[1] != 0) {
    this->SendMsg(this, 104, this->item.unknown_0000[1], 0, 0, 0);
  }
}

// OFFSET: 0x00603DF0
// FUNCTION: fn_04204
void UILapineUpgradeBox_IngredientInfoWnd::Slot26(int x, int y) {
  if (this->item.unknown_0000[1] > 0) {
    this->parent->SendMsg(this, 104, this->item.unknown_0000[1], 0, 0, 0);
  }
}

// OFFSET: 0x00603E20
// FUNCTION: fn_04205
void UILapineUpgradeBoxWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.unknown_0000[1] != 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 26, this->item, 1, -1);
  }
}

// OFFSET: 0x00603E80
// FUNCTION: fn_04206
void UILapineUpgradeBox_IngredientInfoWnd::Slot25(int x, int y) {
  if (this->item.unknown_0000[1] != 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 27, this->item, this->quantity, -1);
  }
}

// OFFSET: 0x00603EC0
// FUNCTION: fn_04207
void UILapineUpgradeBox_IngredientInfoWnd::Slot31(int x, int y) {
  this->selected = 1;
  this->Invalidate();
  if (this->parent) {
    this->parent->SendMsg(this, 6, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00604180
// FUNCTION: fn_04213
void UILapineUpgradeBox_IngredientInfoWnd::Slot35(int argument) {
  if (this->parent) {
    this->parent->Slot35(argument);
  }
}

// OFFSET: 0x006040E0
// FUNCTION: fn_04211
void UILapineUpgradeBoxWnd::Slot32(int x, int y) {
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.unknown_0000[1] != 0) {
    UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
    window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x00604140
// FUNCTION: fn_04212
void UILapineUpgradeBox_IngredientInfoWnd::Slot32(int x, int y) {
  if (this->item.unknown_0000[1] != 0) {
    UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
    window->SendMsg(this, 24, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x00605BB0
// FUNCTION: fn_04225
void UILapineUpgradeBoxWnd::SetWaiting(unsigned char value) {
  this->waiting = value;
  this->upgradeButton->SetDisabled(value);
  this->upgradeButton->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x006049B0

// OFFSET: 0x00603680
// FUNCTION: fn_04196
void UILapineUpgradeBox_IngredientListWnd::Slot10(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 26) {
    this->SendMsg(this, 31, drag->id, 0, 0, 0);
  }
  mode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x00604680
// FUNCTION: fn_04216
int UILapineUpgradeBox_IngredientListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c,
                                                  std::intptr_t d) {
  switch (message) {
  case 6:
    for (UILapineUpgradeBox_IngredientInfoWnd* entry : this->items) {
      if (sender != entry) {
        entry->selected = 0;
        entry->Invalidate();
      }
    }
    this->Invalidate();
    break;

  case 0x17:
    for (const ITEM_INFO& material : this->materials) {
      const int itemId = std::atoi(material.itemId.c_str());
      for (int identity : g_session.FindInventoryIndicesByItemId(itemId, 1)) {
        this->AddIngredient(static_cast<std::uint32_t>(identity));
      }
    }

    for (UILapineUpgradeBox_IngredientInfoWnd* entry : this->items) {
      const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, entry->item.unknown_0000[1]);
      if (owned.unknown_0000[1] == 0) {
        entry->Show(0);
      }
    }
    this->SendMsg(this, 0x57, 0, 0, 0, 0);
    break;

  case 0x1F:
    this->parent->SendMsg(this, 0x68, a, 0, 0, 0);
    break;

  case 0x57: {
    std::size_t index = 0;
    int y = 7;
    while (index < this->items.size()) {
      UILapineUpgradeBox_IngredientInfoWnd* entry = this->items[index];
      if (entry->geometry.visible == 0) {
        entry->Invalidate();
        this->items.erase(this->items.begin() + static_cast<std::ptrdiff_t>(index));
      } else {
        entry->Move(0, y);
        this->childOrigins[entry] = std::pair<int, int>(0, y);
        entry->Invalidate();
        ++index;
        y += 0x1E;
      }
    }
    using SortChildren = void(__thiscall*)(UILapineUpgradeBox_IngredientListWnd*);
    reinterpret_cast<SortChildren>((*reinterpret_cast<void***>(this))[53])(this);
    this->Invalidate();
    break;
  }

  case 0x68:
    this->parent->SendMsg(this, 0x1F, a, 0, 0, 0);
    break;
  }

  return UIScrollWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00603DA0
// FUNCTION: fn_04202
void UILapineUpgradeBox_IngredientListWnd::Paint() {
  this->ClearSurface(0xFFF7F7);
}

// OFFSET: 0x006041A0
// FUNCTION: fn_04214
void UILapineUpgradeBox_IngredientListWnd::Slot35(int argument) {
  if (this->wheelScroll == 1) {
    if (argument < 0) {
      this->SendMsg(this->scrollBar, 7, 15, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, 7, -15, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00605980
// FUNCTION: fn_04223
UILapineUpgradeBox_IngredientInfoWnd* UILapineUpgradeBox_IngredientListWnd::FindSelected() {
  for (UILapineUpgradeBox_IngredientInfoWnd* entry : this->items) {
    if (entry->selected == 1) {
      return entry;
    }
  }

  return nullptr;
}

// INLINED
inline UILapineUpgradeBox_IngredientInfoWnd* UILapineUpgradeBoxWnd::FindSelected() {
  return this->ingredientList->FindSelected();
}

// OFFSET: 0x00603FE0
// FUNCTION: fn_04210
void UILapineUpgradeBoxWnd::Slot19() {
  UILapineUpgradeBox_IngredientInfoWnd* selected = this->FindSelected();
  if (!selected) {
    if (this->selectButton->Disabled() == 0) {
      this->selectButton->SetDisabled(1);
      this->selectButton->Invalidate();
      this->Invalidate();
    }
  } else if (this->selectButton->Disabled() == 1 && this->item.unknown_0000[1] == 0) {
    this->selectButton->SetDisabled(0);
    this->selectButton->Invalidate();
    this->Invalidate();
  }

  if (this->waiting == 0) {
    if (this->item.unknown_0000[1] != 0) {
      if (this->upgradeButton->Disabled() == 1) {
        this->upgradeButton->SetDisabled(0);
        this->upgradeButton->Invalidate();
        this->Invalidate();
      }
    } else if (this->upgradeButton->Disabled() == 0) {
      this->upgradeButton->SetDisabled(1);
      this->upgradeButton->Invalidate();
      this->Invalidate();
    }
  }

  if (this->noticeTick + this->noticeDuration < timeGetTime()) {
    this->notice = "";
  }
}

// OFFSET: 0x00603F70
// FUNCTION: fn_04209
void UILapineUpgradeBox_IngredientInfoWnd::Slot28(int x, int y) {
  this->Slot27(x, y);

  if (!this->tooltip.empty()) {
    int tipX = 0;
    int tipY = -20;
    this->ToScreen(tipX, tipY);
    this->m_controls->services.Tooltip(this->tooltip.c_str(), tipX, tipY, -1, false, true);
  }
}

// OFFSET: 0x00603EF0
// FUNCTION: fn_04208
void UILapineUpgradeBoxWnd::Slot28(int x, int y) {
  this->Slot27(x, y);

  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && !this->item.unknown_0044.empty()) {
    int tipX = 101;
    int tipY = 145;
    this->ToScreen(tipX, tipY);
    this->m_controls->services.Tooltip(this->item.unknown_0044.c_str(), tipX, tipY, -1, false, true);
  }
}

// OFFSET: 0x00604FA0
// FUNCTION: fn_04219
void UILapineUpgradeBoxWnd::AddMaterial(int id) {
  std::vector<ITEM_INFO>& materials = this->ingredientList->materials;
  const auto found = std::find_if(materials.begin(), materials.end(), [id](ITEM_INFO material) { return id == std::atoi(material.itemId.c_str()); });

  if (found == materials.end()) {
    ITEM_INFO material;
    material.SetItemId(id);
    materials.push_back(material);
  }
}

// OFFSET: 0x006052F0

// OFFSET: 0x00605750
// FUNCTION: fn_04222
bool UILapineUpgradeBox_IngredientListWnd::CanUseIngredient(std::uint32_t identity) {
  auto entryIt = std::find_if(this->items.begin(), this->items.end(), [identity](UILapineUpgradeBox_IngredientInfoWnd* entry) {
    return entry->item.unknown_0000[1] == identity;
  });
  if (entryIt == this->items.end()) {
    return false;
  }

  UILapineUpgradeBox_IngredientInfoWnd* entry = *entryIt;
  const ITEM_INFO& item = entry->item;
  const int itemId = std::atoi(item.itemId.c_str());
  const auto material = std::find_if(this->materials.begin(), this->materials.end(), [itemId](const ITEM_INFO& required) {
    return std::atoi(required.itemId.c_str()) == itemId;
  });
  if (material == this->materials.end()) {
    return false;
  }

  if (this->minRefine > item.unknown_0060 || item.unknown_0060 > this->maxRefine) {
    return false;
  }

  if (this->minRefine != 0 && this->minRefine < item.unknown_0060) {
    this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(LookupMessage(0xE3D)), 0, 0, 0);
  }

  const std::uint32_t itemType = item.unknown_0000[0];
  const bool equipment = itemType == 4 || itemType == 5 || itemType == 8 || itemType == 9 || itemType == 11 ||
                         itemType == 12 || itemType == 13 || itemType == 14 || itemType == 15 || itemType == 21;
  if (equipment && !item.IsApprenticeItem() && !item.HasSpecialCard()) {
    for (std::size_t offset = 0; offset < sizeof(item.unknown_001C); offset += sizeof(std::uint32_t)) {
      std::uint32_t card = 0;
      std::memcpy(&card, item.unknown_001C + offset, sizeof(card));
      if (card != 0) {
        this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(LookupMessage(0xE3E)), 0, 0, 0);
        break;
      }
    }
  }

  if (this->minOptionCount > item.unknown_0098) {
    return false;
  }

  entry->Show(0);
  this->SendMsg(this, 0x57, 0, 0, 0, 0);
  return true;
}

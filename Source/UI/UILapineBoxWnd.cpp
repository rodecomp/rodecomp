#include "UI/UILapineScrollBar.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/UIIngredientInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <utility>
#include <vector>
#include <windows.h>
#include <timeapi.h>


namespace {
struct RandomCombineItemPacket {
  std::int16_t id;
  std::int16_t length;
  std::int32_t ingredientId;
};
}

// OFFSET: 0x005FEFF0
// FUNCTION: fn_04140
UILapineBoxWnd::UILapineBoxWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0), cancelButton(nullptr), makeButton(nullptr),
      moveButton(nullptr), ingredientList(nullptr), unknown_00C4(0), noticeTick(0), noticeDuration(3000), waiting(0) {
}

// OFFSET: 0x006006D0
// FUNCTION: fn_04160
void UIIngredientInfoWnd::Click(int, int) {
  if (this->insufficient == 1) {
    static_cast<UILapineBox_IngredientListWnd*>(this->parent)->CanUseIngredient(this->item.unknown_0000[1]);
    return;
  }

  const int id = this->item.unknown_0000[1];
  if (id == 0) {
    return;
  }

  UILapineBox_IngredientListWnd* list = static_cast<UILapineBox_IngredientListWnd*>(this->parent);
  UILapineBoxWnd* box = static_cast<UILapineBoxWnd*>(list->parent);
  if (box->items.size() < static_cast<std::size_t>(box->maxItems)) {
    if (list->SendMsg(list, 0x68, id, 0, 0, 0) >= 0) {
      box->AddIngredient(id, this->quantity);
    }
  }
}

// OFFSET: 0x00600830
// FUNCTION: fn_04162
void UIIngredientInfoWnd::Slot25(int x, int y) {
  if (this->insufficient != 1 && this->item.unknown_0000[1] > 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 27, this->item, this->quantity, -1);
  }
}

// OFFSET: 0x00600940
// FUNCTION: fn_04164
void UIIngredientInfoWnd::Slot31(int, int) {
  if (this->insufficient != 1) {
    this->selected = 1;
    this->Invalidate();

    if (this->parent) {
      this->parent->SendMsg(this, 6, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00600980
// FUNCTION: fn_04165
void UIIngredientInfoWnd::Slot28(int x, int y) {
  this->Slot27(x, y);

  if (this->tooltip.size() != 0) {
    int tipX = 0;
    int tipY = -20;
    this->ToScreen(tipX, tipY);
    reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(this->tooltip.c_str(), tipX, tipY, -1, 0, 1);
  }
}

// OFFSET: 0x00600770
// FUNCTION: fn_04161
void UILapineBoxWnd::DropMaterial(int x, int y) {
  alignas(ITEM_INFO) unsigned char storage[sizeof(ITEM_INFO)];
  ITEM_INFO& item = *reinterpret_cast<ITEM_INFO*>(storage);
  this->ReadMaterial(&item, x, y);

  if (item.unknown_0000[1] != 0) {
    if (this->ingredientList->AddIngredient(item.unknown_0000[1]) == 1) {
      this->SendMsg(this, 0x68, item.unknown_0000[1], 0, 0, 0);
    }
  }

  item.~ITEM_INFO();
}

// OFFSET: 0x00600870
// FUNCTION: fn_04163
void UILapineBoxWnd::DragIngredient(int x, int y) {
  if (y < 0x11) {
    UIFrameWnd::Slot25(x, y);
  }

  alignas(ITEM_INFO) unsigned char storage[sizeof(ITEM_INFO)];
  ITEM_INFO& item = *reinterpret_cast<ITEM_INFO*>(storage);
  this->ReadMaterial(&item, x, y);

  if (item.unknown_0000[1] != 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(*reinterpret_cast<WindowManagerWindows*>(&g_windowMgr), descriptions, 0x1A, item, item.quantity, -1);
  }

  item.~ITEM_INFO();
}

// OFFSET: 0x006009F0
// FUNCTION: fn_04166
void UILapineBoxWnd::Slot28(int x, int y) {
  this->Slot27(x, y);

  alignas(ITEM_INFO) unsigned char storage[sizeof(ITEM_INFO)];
  ITEM_INFO& item = *reinterpret_cast<ITEM_INFO*>(storage);
  this->ReadMaterial(&item, x, y);

  if (item.unknown_0000[1] != 0) {
    int tipX = 0;
    int tipY = -20;
    const int count = static_cast<int>(this->items.size());
    for (int index = 0; index < count; ++index) {
      if (this->items[index].unknown_0000[1] == item.unknown_0000[1]) {
        int position[2] = {0, 0};
        this->MaterialPosition(position, index);
        tipX = position[0];
        tipY = position[1] - 0x14;
        break;
      }
    }

    this->ToScreen(tipX, tipY);
    reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(item.unknown_0044.c_str(), tipX, tipY, -1, 0, 1);
  }

  item.~ITEM_INFO();
}

// OFFSET: 0x00600B60
// FUNCTION: fn_04167
void UILapineBoxWnd::Slot19() {
  if (this->waiting == 0) {
    const int count = static_cast<int>(this->items.size());
    if (this->maxItems == count) {
      if (this->makeButton->Disabled() == 1) {
        this->makeButton->SetDisabled(0);
        this->makeButton->Invalidate();
        this->Invalidate();
      }
    } else if (this->makeButton->Disabled() == 0) {
      this->makeButton->SetDisabled(1);
      this->makeButton->Invalidate();
      this->Invalidate();
    }
  }

  if (this->noticeTick + this->noticeDuration < timeGetTime()) {
    this->notice = "";
  }
}

// OFFSET: 0x00600C00
// FUNCTION: fn_04168
void UIIngredientInfoWnd::Slot32(int, int) {
  if (this->item.unknown_0000[1] != 0) {
    UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
    window->SendMsg(this, 0x18, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x006024F0
// FUNCTION: fn_04181
__declspec(noinline) void UILapineBoxWnd::MaterialPosition(int* out, int index) {
  if (index >= 0) {
    const unsigned int count = this->items.size();
    if (static_cast<unsigned int>(index) <= count) {
      int left = count > 5 ? 0x19 : 0x21;
      int top = count > 5 ? 0x98 : 0xA2;
      if (index >= 5) {
        left = 0x2A;
        top = 0xB7;
      }
      out[1] = top;
      out[0] = left + static_cast<unsigned int>(index) % 5 * 0x22;
      return;
    }
  }

  out[0] = 0;
  out[1] = 0;
}

// OFFSET: 0x00602410
// FUNCTION: fn_04180
__declspec(noinline) void UILapineBoxWnd::ReadMaterial(ITEM_INFO* out, int x, int y) {
  const unsigned int count = this->items.size();
  for (unsigned int index = 0; index < count; ++index) {
    int position[2];
    this->MaterialPosition(position, index);
    if (x >= position[0] && x <= position[0] + 0x18 && y >= position[1] && y <= position[1] + 0x18) {
      new (out) ITEM_INFO(this->items[index]);
      return;
    }
  }
  new (out) ITEM_INFO(0);
}

// OFFSET: 0x00602620
// FUNCTION: fn_04182
bool UILapineBox_IngredientListWnd::CanUseIngredient(std::uint32_t identity) {
  std::vector<UIIngredientInfoWnd*>::iterator entryIt;
  for (entryIt = this->items.begin(); entryIt != this->items.end(); ++entryIt) {
    if ((*entryIt)->item.unknown_0000[1] == identity) {
      break;
    }
  }
  if (entryIt == this->items.end()) {
    return false;
  }

  const ITEM_INFO& item = (*entryIt)->item;
  std::vector<ITEM_INFO>::iterator material;
  const std::vector<ITEM_INFO>::iterator end = this->materials.end();
  for (material = this->materials.begin(); material != end; ++material) {
    ITEM_INFO copy(*material);
    if (std::atoi(item.itemId.c_str()) == std::atoi(copy.itemId.c_str())) {
      break;
    }
  }
  if (material == this->materials.end()) {
    return false;
  }

  const int required = material->quantity;
  if (required > item.quantity) {
    this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xB52), required).c_str()), 0, 0, 0);
    return false;
  }
  if (this->minRefine > item.unknown_0060) {
    this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(LookupMessage(0xB53)), 0, 0, 0);
    return false;
  }
  if (this->maxRefine < item.unknown_0060) {
    this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(LookupMessage(0xE3C)), 0, 0, 0);
    return false;
  }
  if (this->minRefine != 0 && this->minRefine < item.unknown_0060) {
    this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(LookupMessage(0xE3D)), 0, 0, 0);
  }

  const std::uint32_t itemType = item.unknown_0000[0];
  const bool equipment = itemType == 4 || itemType == 5 || itemType == 8 || itemType == 9 || itemType == 11 || itemType == 12 ||
                         itemType == 13 || itemType == 14 || itemType == 15 || itemType == 21;
  if (equipment && !item.IsApprenticeItem() && !item.HasSpecialCard()) {
    const int* cards = reinterpret_cast<const int*>(item.unknown_001C);
    for (int index = 0; index < ItemSlotsCount(item); ++index) {
      if (index < 4 && cards[index] > 0) {
        this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(LookupMessage(0xE3E)), 0, 0, 0);
        return true;
      }
    }
  }
  return true;
}

// OFFSET: 0x00602AC0
// FUNCTION: fn_04184
void UILapineBoxWnd::SetWaiting(unsigned char waiting) {
  UIBitmapButton* button = this->makeButton;
  this->waiting = waiting;
  button->SetDisabled(waiting);
  button->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x00600C40
// FUNCTION: fn_04169
void UILapineBoxWnd::NotifyWindowAt(int x, int y) {
  alignas(ITEM_INFO) unsigned char storage[sizeof(ITEM_INFO)];
  ITEM_INFO& item = *reinterpret_cast<ITEM_INFO*>(storage);
  this->ReadMaterial(&item, x, y);

  if (static_cast<unsigned>(item.unknown_0000[1]) > 0) {
    UIWindow* window = g_windowMgr.EnsureWindow(12);
    window->SendMsg(nullptr, 0x18, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }

  item.~ITEM_INFO();
}

// OFFSET: 0x00600D00
// FUNCTION: fn_04170
void UIIngredientInfoWnd::Slot35(int argument) {
  if (this->parent) {
    this->parent->Slot35(argument);
  }
}
// OFFSET: 0x00600D20
// FUNCTION: fn_04171
void UILapineBox_IngredientListWnd::Slot35(int argument) {
  if (this->wheelScroll == 1) {
    if (argument < 0) {
      this->SendMsg(this->scrollBar, 7, 15, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, 7, -15, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00600D60
// FUNCTION: fn_04172
int UILapineBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    if (message == 0x68) {
      std::vector<ITEM_INFO>& items = this->items;
      const std::vector<ITEM_INFO>::iterator end = items.end();
      std::vector<ITEM_INFO>::iterator found;
      for (found = items.begin(); found != end; ++found) {
        ITEM_INFO copy(*found);
        if (a == copy.unknown_0000[1]) {
          break;
        }
      }

      if (found != end) {
        std::vector<ITEM_INFO>::iterator it = found + 1;
        for (; it != end; ++it) {
          *(it - 1) = *it;
        }
        items.pop_back();

        if (items.size() < static_cast<size_t>(this->maxItems)) {
          this->moveButton->SetDisabled(0);
          this->moveButton->Invalidate();
        }
        this->Invalidate();
      }
    } else if (message == 0x6F) {
      this->noticeTick = timeGetTime();
      const char* text = reinterpret_cast<const char*>(a);
      this->notice.assign(text, std::strlen(text));
      this->Invalidate();
    } else {
      return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
    }
  } else if (a == 0xC9) {
    g_windowMgr.CloseWindow(0x122);
  } else if (a == 0x1F7) {
    const int count = static_cast<int>(this->items.size());
    const int length = count * 4 + 8;

    RandomCombineItemPacket* packet = new (std::nothrow) RandomCombineItemPacket();
    packet->id = ID_CZ_REQ_RANDOM_COMBINE_ITEM;
    packet->length = static_cast<std::int16_t>(length);
    packet->ingredientId = this->unknown_00C4;

    unsigned char* payload = static_cast<unsigned char*>(std::malloc(length));
    std::memcpy(payload, packet, 8);
    for (int index = 0; index < count; ++index) {
      const ITEM_INFO& material = this->items[index];
      const std::uint32_t value = (static_cast<std::uint16_t>(material.quantity) << 16) | static_cast<std::uint16_t>(material.unknown_0000[1]);
      std::memcpy(payload + 8 + index * 4, &value, sizeof(value));
    }

    GetRagConnection().Send(length, payload);

    this->waiting = 1;
    this->makeButton->SetDisabled(1);
    this->makeButton->Invalidate();
    this->Invalidate();

    delete packet;
    std::free(payload);
  } else if (a == 0x1F9) {
    for (UIIngredientInfoWnd* entry : this->ingredientList->items) {
      if (entry->selected == 1) {
        if (entry != nullptr && entry->insufficient != 1) {
          if (this->ingredientList->SendMsg(this, 0x68, entry->item.unknown_0000[1], 0, 0, 0) >= 0) {
            this->AddIngredient(entry->item.unknown_0000[1], entry->quantity);
          }
        }
        break;
      }
    }
  }

  return 0;
}
// OFFSET: 0x006011E0
// FUNCTION: fn_04173
int UILapineBox_IngredientListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c,
                                           std::intptr_t d) {
  switch (message) {
  case 6:
    for (std::vector<UIIngredientInfoWnd*>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
      UIIngredientInfoWnd* entry = *it;
      if (sender != entry) {
        entry->selected = 0;
        entry->Invalidate();
      }
    }
    this->Invalidate();
    break;

  case 0x17:
    for (std::vector<ITEM_INFO>::iterator material = this->materials.begin(); material != this->materials.end(); ++material) {
      std::vector<int> ids = g_session.FindInventoryIndicesByItemId(std::atoi(material->itemId.c_str()), 1);
      for (std::vector<int>::iterator id = ids.begin(); id != ids.end(); ++id) {
        this->AddIngredient(*id);
      }
    }

    for (std::vector<UIIngredientInfoWnd*>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
      ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, (*it)->item.unknown_0000[1]);
      if (owned.unknown_0000[1] == 0) {
        (*it)->Show(0);
      }
    }
    this->SendMsg(this, 0x57, 0, 0, 0, 0);
    break;

  case 0x57: {
    int y = 7;
    for (std::vector<UIIngredientInfoWnd*>::iterator it = this->items.begin(); it != this->items.end();) {
      UIIngredientInfoWnd* entry = *it;
      if (entry->geometry.visible == 0) {
        entry->Invalidate();
        it = this->items.erase(it);
      } else {
        this->MoveScrollChild(entry, 0, y);
        (*it)->Invalidate();
        ++it;
        y += 0x1E;
      }
    }
    this->Slot53();
    this->Invalidate();
    break;
  }

  case 0x68: {
    std::vector<UIIngredientInfoWnd*>::iterator it;
    for (it = this->items.begin(); it != this->items.end(); ++it) {
      if (static_cast<std::uint32_t>(a) == (*it)->item.unknown_0000[1]) {
        break;
      }
    }
    if (it == this->items.end()) {
      break;
    }

    UIIngredientInfoWnd* selected = *it;
    std::vector<ITEM_INFO>::iterator found = this->materials.begin();
    const std::vector<ITEM_INFO>::iterator end = this->materials.end();
    for (; found != end; ++found) {
      ITEM_INFO copy(*found);
      if (std::atoi(selected->item.itemId.c_str()) == std::atoi(copy.itemId.c_str())) {
        break;
      }
    }

    if (found == this->materials.end()) {
      break;
    }

    const int need = found->quantity;
    if (selected->item.quantity < need) {
      this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(0xB52), need).c_str()), 0, 0, 0);
      return -1;
    }

    if (selected->item.unknown_0060 < found->unknown_0060) {
      this->parent->SendMsg(this, 0x6F, reinterpret_cast<std::intptr_t>(LookupMessage(0xB53)), 0, 0, 0);
      return -1;
    }

    selected->item.quantity -= need;
    if (selected->item.quantity <= 0) {
      (*it)->Show(0);
    } else {
      (*it)->insufficient = 1;
      (*it)->Invalidate();
    }
    this->SendMsg(this, 0x57, 0, 0, 0, 0);
    break;
  }

  default:
    break;
  }

  return UIScrollWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x006016C0
// FUNCTION: fn_04174
void UILapineBoxWnd::SetIngredient(int id) {
  this->unknown_00C4 = id;

  {
    ITEM_INFO item;
    item.SetItemId(id);

    char name[0x104];
    std::memset(name, 0, sizeof(name));
    int capacity = 0x104;
    ItemDescriptions descriptions{g_itemDescriptions};
    descriptions.Name(item, name, capacity, 0);
    this->title.assign(name, std::strlen(name));
  }

  g_luaScript->CallTable("tblLapineDdukddakBox", "set_resource_item_list", "d", id);

  int minRefine = 0;
  g_luaScript->CallTable("tblLapineDdukddakBox", "get_min_need_refine", "d>d", id, &minRefine);
  this->ingredientList->minRefine = minRefine;

  int maxRefine = 0;
  g_luaScript->CallTable("tblLapineDdukddakBox", "get_max_need_refine", "d>d", id, &maxRefine);
  this->ingredientList->maxRefine = maxRefine;

  this->ingredientList->SendMsg(this, 0x17, 0, 0, 0, 0);

  g_luaScript->CallTable("tblLapineDdukddakBox", "get_need_item_count", "d>d", id, &this->maxItems);

  const char* source = nullptr;
  g_luaScript->CallTable("tblLapineDdukddakBox", "get_need_source_string", "d>s", id, &source);
  this->description.assign(source, std::strlen(source));
}

// OFFSET: 0x00601BC0
// FUNCTION: fn_04176
void UILapineBoxWnd::AddMaterial(int id, int quantity) {
  std::vector<ITEM_INFO>& materials = this->ingredientList->materials;
  std::vector<ITEM_INFO>::iterator it;
  const std::vector<ITEM_INFO>::iterator end = materials.end();
  for (it = materials.begin(); it != end; ++it) {
    ITEM_INFO copy(*it);
    if (std::atoi(copy.itemId.c_str()) == id) {
      break;
    }
  }

  if (it == end) {
    ITEM_INFO material;
    material.SetItemId(id);
    material.quantity = quantity;
    materials.push_back(material);
  }
}

// OFFSET: 0x006028C0
// FUNCTION: fn_04183
void UIIngredientInfoWnd::sub_6028C0(std::uint32_t id) {
  this->item = LookupClientItemByIdentity(g_session.m_inventoryItems, id);

  std::vector<char*> breaks;
  char name[260];
  std::memset(name, 0, sizeof(name));
  char* highlight = nullptr;
  std::size_t remaining = sizeof(name);
  char* cursor = name;
  int highlightColor;
  this->BuildItemName(&this->item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);

  if (this->item.identified == 1) {
    const int count = this->item.unknown_0098;
    if (count != 0) {
      sprintf_s(name, sizeof(name), LookupMessage(2600), name, count);
    }
  }

  this->item.SetName(name);
  this->tooltip = name;
  this->Invalidate();
}
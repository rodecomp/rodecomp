#include "UI/UILapineScrollBar.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/UIIngredientInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <list>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>
#include <windows.h>
#include <timeapi.h>

typedef bool(__thiscall* AddMaterialSlotFn)(UILapineBoxWnd*, int, int);
typedef void(__thiscall* ReadMaterialFn)(UILapineBoxWnd*, void*, int, int);
typedef void(__thiscall* MaterialPositionFn)(UILapineBoxWnd*, void*, int);
typedef void(__thiscall* ReturnMaterialFn)(UILapineBox_IngredientListWnd*, int);
typedef bool(__thiscall* CheckMaterialFn)(UILapineBox_IngredientListWnd*, int);
typedef void(__thiscall* FillItemIdsFn)(CSession*, std::vector<int>*, int, unsigned char);
typedef void(__thiscall* LayoutChildFn)(UILapineBox_IngredientListWnd*, UIWindow*, int, int);
typedef int(__thiscall* ScrollWndSendMsgFn)(UILapineBox_IngredientListWnd*, UIWindow*, int, std::intptr_t, std::intptr_t, std::intptr_t, std::intptr_t);
typedef void(__thiscall* BeginItemOnModeFn)(CGameMode*, int, ITEM_INFO*, int, int);
typedef void(__thiscall* CloseNotifyFn)(UIWindowMgr*, int);

namespace {
static void SortChildWindows(UILapineBox_IngredientListWnd* window) {
  typedef void(__thiscall* SortFn)(UILapineBox_IngredientListWnd*);
  SortFn fn = reinterpret_cast<SortFn>((*reinterpret_cast<void***>(window))[53]);
  fn(window);
}

struct RandomCombineItemPacket {
  std::int16_t id;
  std::int16_t length;
  std::int32_t ingredientId;
};
}

// OFFSET: 0x006006D0

// OFFSET: 0x00600830
// FUNCTION: fn_04162
void UIIngredientInfoWnd::Slot25(int x, int y) {
  if (this->insufficient != 1 && this->item.unknown_0000[1] > 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 27, this->item, this->quantity, -1);
  }
}

// OFFSET: 0x00600940

// OFFSET: 0x00600980
// FUNCTION: fn_04165
void UIIngredientInfoWnd::Slot28(int x, int y) {
  this->Slot27(x, y);

  if (this->tooltip.size() != 0) {
    int tipX = 0;
    int tipY = -20;
    this->ToScreen(tipX, tipY);
    (*reinterpret_cast<ModeTooltip**>(0x013495BC))->Show(this->tooltip.c_str(), tipX, tipY, -1, 0, 1);
  }
}

// OFFSET: 0x00600770

// OFFSET: 0x00600870

// OFFSET: 0x006009F0
// FUNCTION: fn_04166
void UILapineBoxWnd::Slot28(int x, int y) {
  this->Slot27(x, y);

  alignas(ITEM_INFO) unsigned char storage[sizeof(ITEM_INFO)];
  ITEM_INFO& item = *reinterpret_cast<ITEM_INFO*>(storage);
  reinterpret_cast<ReadMaterialFn>(0x00602410)(this, &item, x, y);

  if (item.unknown_0000[1] != 0) {
    int tipX = 0;
    int tipY = -20;
    const int count = static_cast<int>(this->items.size());
    for (int index = 0; index < count; ++index) {
      if (this->items[index].unknown_0000[1] == item.unknown_0000[1]) {
        int position[2] = {0, 0};
        reinterpret_cast<MaterialPositionFn>(0x006024F0)(this, position, index);
        tipX = position[0];
        tipY = position[1] - 0x14;
        break;
      }
    }

    this->ToScreen(tipX, tipY);
    (*reinterpret_cast<ModeTooltip**>(0x013495BC))->Show(item.unknown_0044.c_str(), tipX, tipY, -1, 0, 1);
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

// OFFSET: 0x00600C40

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
    reinterpret_cast<CloseNotifyFn>(0x00A8CF20)(&g_windowMgr, 0x122);
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
            reinterpret_cast<AddMaterialSlotFn>(0x00601D20)(this, entry->item.unknown_0000[1], entry->quantity);
          }
        }
        break;
      }
    }
  }

  return 0;
}
// OFFSET: 0x006011E0

// OFFSET: 0x006016C0

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
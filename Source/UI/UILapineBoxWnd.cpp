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
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

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
typedef UIWindow*(__thiscall* BehaviorWindowFn)(UIWindowMgr*, int);
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

// OFFSET: 0x00600770

// OFFSET: 0x00600870

// OFFSET: 0x006009F0

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
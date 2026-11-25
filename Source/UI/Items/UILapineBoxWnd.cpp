#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeDragState.h"
#include "UI/Window/UIEnums.h"
#include "UI/Items/UILapineScrollBar.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Items/UIIngredientInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
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
    : UIFrameWnd(0), cancelButton(nullptr), makeButton(nullptr),
      moveButton(nullptr), ingredientList(nullptr), ingredientId(0), noticeTick(0), noticeDuration(3000), waiting(0) {
}

// OFFSET: 0x006025B0
// FUNCTION: fn_46616
bool UILapineBox_IngredientListWnd::TransferIngredient(std::uint32_t id, int quantity) {
  UILapineBoxWnd* box = static_cast<UILapineBoxWnd*>(this->parent);
  if (box->items.size() < static_cast<std::size_t>(box->maxItems)) {
    if (this->SendMsg(this, UM_DECITEM, id, 0, 0, 0) >= 0) {
      return static_cast<UILapineBoxWnd*>(this->parent)->AddIngredient(id, quantity);
    }
  }
  return false;
}

// OFFSET: 0x006006D0
// FUNCTION: fn_04160
void UIIngredientInfoWnd::OnLBtnDblClk(int, int) {
  if (this->insufficient == 1) {
    static_cast<UILapineBox_IngredientListWnd*>(this->parent)->CanUseIngredient(this->item.identity[1]);
    return;
  }

  UILapineBox_IngredientListWnd* list = static_cast<UILapineBox_IngredientListWnd*>(this->parent);
  const int id = this->item.identity[1];
  if (id == 0) {
    return;
  }

  list->TransferIngredient(id, this->quantity);
}

// OFFSET: 0x00600830
// FUNCTION: fn_04162
void UIIngredientInfoWnd::OnLBtnDown(int x, int y) {
  if (this->insufficient != 1 && this->item.identity[1] > 0) {
    g_modeMgr.GetGameMode()->BeginDragItem(DRAG_LAPINE_INGREDIENT, this->item, this->quantity, -1);
  }
}

// OFFSET: 0x00600940
// FUNCTION: fn_04164
void UIIngredientInfoWnd::OnLBtnUp(int, int) {
  if (this->insufficient != 1) {
    this->selected = 1;
    this->Invalidate();

    if (this->parent) {
      this->parent->SendMsg(this, UM_COMMAND, 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00600980
// FUNCTION: fn_04165
void UIIngredientInfoWnd::OnMouseHover(int x, int y) {
  this->OnMouseMove(x, y);

  if (this->tooltip.size() > 0) {
    int tipX = 0;
    int tipY = -20;
    this->ToScreen(tipX, tipY);
    g_modeMgr.currentMode->ShowTooltip(this->tooltip.c_str(), tipX, tipY, -1, 0, 1);
  }
}

// OFFSET: 0x00600770
// FUNCTION: fn_04161
void UILapineBoxWnd::OnLBtnDblClk(int x, int y) {
  const ITEM_INFO item = this->ReadMaterial(x, y);

  if (item.identity[1] != 0) {
    if (this->ingredientList->AddIngredient(item.identity[1]) == 1) {
      this->SendMsg(this, UM_DECITEM, item.identity[1], 0, 0, 0);
    }
  }
}

// OFFSET: 0x00600870
// FUNCTION: fn_04163
void UILapineBoxWnd::OnLBtnDown(int x, int y) {
  if (y < 0x11) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const ITEM_INFO item = this->ReadMaterial(x, y);

  if (item.identity[1] > 0) {
    g_modeMgr.GetGameMode()->BeginDragItem(DRAG_LAPINE_BOX, item, item.quantity, -1);
  }
}

// OFFSET: 0x006009F0
// FUNCTION: fn_04166
void UILapineBoxWnd::OnMouseHover(int x, int y) {
  this->OnMouseMove(x, y);

  const ITEM_INFO item = this->ReadMaterial(x, y);

  if (item.identity[1] != 0) {
    int tipX = 0;
    int tipY = -20;
    for (unsigned int index = 0; index < this->items.size(); ++index) {
      if (item.identity[1] == this->items[index].identity[1]) {
        const auto position = this->MaterialPosition(index);
        tipX = position.first;
        tipY = position.second - 0x14;
        break;
      }
    }

    this->ToScreen(tipX, tipY);
    g_modeMgr.currentMode->ShowTooltip(item.displayName.c_str(), tipX, tipY, -1, 0, 1);
  }
}

// OFFSET: 0x00600B60
// FUNCTION: fn_04167
void UILapineBoxWnd::OnProcess() {
  if (this->waiting == 0) {
    const int count = static_cast<int>(this->items.size());
    if (this->maxItems == count) {
      if (this->makeButton->Disabled() == 1) {
        auto* const button = this->makeButton;
        button->SetDisabled(0);
        button->Invalidate();
        this->Invalidate();
      }
    } else if (this->makeButton->Disabled() == 0) {
      auto* const button = this->makeButton;
      button->SetDisabled(1);
      button->Invalidate();
      this->Invalidate();
    }
  }

  if (this->noticeTick + this->noticeDuration < timeGetTime()) {
    this->notice = "";
  }
}

// OFFSET: 0x00600C00
// FUNCTION: fn_04168
void UIIngredientInfoWnd::OnRBtnDown(int, int) {
  if (this->item.identity[1] > 0) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    window->SendMsg(this, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x006024F0
// FUNCTION: fn_04181
__declspec(noinline) std::pair<int, int> UILapineBoxWnd::MaterialPosition(int index) {
  if (index >= 0) {
    const unsigned int count = this->items.size();
    if (count >= static_cast<unsigned int>(index)) {
      int columns = 5;
      int left = count > columns ? 0x19 : 0x21;
      int top = count > columns ? 0x98 : 0xA2;
      if (index >= columns) {
        left = 0x2A;
        top = 0xB7;
      }
      return {left + static_cast<int>(static_cast<unsigned int>(index) % columns) * 0x22, top};
    }
  }

  return {};
}

// OFFSET: 0x00602410
// FUNCTION: fn_04180
__declspec(noinline) ITEM_INFO UILapineBoxWnd::ReadMaterial(int x, int y) {
  const ITEM_INFO* first = &this->items[0];
  for (unsigned int index = 0, count = this->items.size(); index < count; ++index) {
    const auto position = this->MaterialPosition(index);
    if (x >= position.first && x <= position.first + 0x18 && y >= position.second && y <= position.second + 0x18) {
      return first[index];
    }
  }
  return ITEM_INFO(0);
}

// OFFSET: 0x00602620
// FUNCTION: fn_04182
bool UILapineBox_IngredientListWnd::CanUseIngredient(std::uint32_t identity) {
  const auto entryIt = std::find_if(this->items.begin(), this->items.end(), [identity](UIIngredientInfoWnd* entry) {
    return identity == entry->item.identity[1];
  });
  if (entryIt == this->items.end()) {
    return false;
  }

  const ITEM_INFO& item = (*entryIt)->item;
  const auto material = std::find_if(this->materials.begin(), this->materials.end(), [&item](ITEM_INFO candidate) {
    const int itemId = std::atoi(item.itemId.c_str());
    return itemId == std::atoi(candidate.itemId.c_str());
  });
  if (material == this->materials.end()) {
    return false;
  }

  if (material->quantity > item.quantity) {
    this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_LAPINE_FAIL_NOT_ENOUGH_ITEM_COUNT), material->quantity).c_str()), 0, 0, 0);
    return false;
  }
  if (this->minRefine > item.refiningLevel) {
    this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LAPINE_FAIL_NOT_ENOUGH_REFINING_LEVEL)), 0, 0, 0);
    return false;
  }
  if (this->maxRefine < item.refiningLevel) {
    this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LAPINE_FAIL_REFINING_LEVEL_OVERFLOW_NEEDED)), 0, 0, 0);
    return false;
  }
  if (this->minRefine != 0 && this->minRefine < item.refiningLevel) {
    this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LAPINE_WARNING_REFINING_LEVEL_HIGHER_THAN_NEEDED)), 0, 0, 0);
  }

  if (item.IsEquipment() == true && !item.IsApprenticeItem() && !item.HasSpecialCard()) {
    for (int index = 0; index < item.SlotCount(); ++index) {
      if (index < 4 && static_cast<int>(item.cards[index]) > 0) {
        this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LAPINE_WARNING_HAS_CARD_OR_ENCHANTED)), 0, 0, 0);
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
void UILapineBoxWnd::OnRBtnDown(int x, int y) {
  const ITEM_INFO item = this->ReadMaterial(x, y);

  if (static_cast<unsigned>(item.identity[1]) > 0) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00600D00
// FUNCTION: fn_04170
void UIIngredientInfoWnd::OnWheel(int argument) {
  if (this->parent) {
    this->parent->OnWheel(argument);
  }
}
// OFFSET: 0x00600D20
// FUNCTION: fn_04171
void UILapineBox_IngredientListWnd::OnWheel(int argument) {
  if (this->wheelScroll == 1) {
    if (argument < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, 15, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -15, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00600D60
// FUNCTION: fn_04172
int UILapineBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_LAPINEBOXWND);
      break;
    case ID_LAPINE_COMBINE_ITEMS: {
      RandomCombineItemPacket* packet = new (std::nothrow) RandomCombineItemPacket;
      packet->id = ID_CZ_REQ_RANDOM_COMBINE_ITEM;
      const std::size_t materialsLength = this->items.size() * 4;
      packet->length = static_cast<std::int16_t>(materialsLength + 8);
      packet->ingredientId = this->ingredientId;

      char* payload = new (std::nothrow) char[static_cast<std::int16_t>(materialsLength) + 8];
      memcpy_s(payload, packet->length, packet, 8);

      for (std::size_t index = 0; index < this->items.size(); ++index) {
        const ITEM_INFO& item = this->items[index];
        const std::uint16_t material[2] = {static_cast<std::uint16_t>(item.identity[1]), static_cast<std::uint16_t>(item.quantity)};
        memcpy_s(payload + 8 + index * 4, packet->length, material, sizeof(material));
      }

      GetRagConnection().Send(packet->length, payload);

      this->waiting = 1;
      auto* button = this->makeButton;
      button->SetDisabled(1);
      button->Invalidate();
      this->Invalidate();

      delete packet;
      if (payload) {
        delete[] payload;
      }
      break;
    }
    case ID_LAPINE_ADD_INGREDIENT: {
      const std::vector<UIIngredientInfoWnd*>& entries = this->ingredientList->items;
      for (std::vector<UIIngredientInfoWnd*>::const_iterator it = entries.begin(); it != entries.end(); ++it) {
        if ((*it)->selected == 1) {
          UIIngredientInfoWnd* entry = *it;
          if (entry != nullptr && entry->insufficient != 1 && this->ingredientList->SendMsg(this, UM_DECITEM, entry->item.identity[1], 0, 0, 0) >= 0) {
            this->AddIngredient(entry->item.identity[1], entry->quantity);
          }
          break;
        }
      }
      break;
    }
    }
    break;
  case UM_DECITEM: {
    const auto found = std::find_if(this->items.begin(), this->items.end(), [a](ITEM_INFO item) { return a == item.identity[1]; });

    if (found != this->items.end()) {
      this->items.erase(found);

      if (static_cast<std::size_t>(this->maxItems) > this->items.size()) {
        auto* button = this->moveButton;
        button->SetDisabled(0);
        button->Invalidate();
      }

      this->Invalidate();
    }
    break;
  }
  case UM_RECEIVE_MSG:
    this->noticeTick = timeGetTime();
    this->notice = reinterpret_cast<const char*>(a);
    this->Invalidate();
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}
// OFFSET: 0x006011E0
// FUNCTION: fn_04173
int UILapineBox_IngredientListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c,
                                           std::intptr_t d) {
  switch (message) {
  case UM_REFRESHITEM:
    for (std::vector<ITEM_INFO>::iterator material = this->materials.begin(); material != this->materials.end(); ++material) {
      const int itemId = std::atoi(material->itemId.c_str());
      std::vector<int> ids = g_session.FindInventoryIndicesByItemId(itemId, 1);
      for (std::vector<int>::iterator id = ids.begin(); id != ids.end(); ++id) {
        this->AddIngredient(*id);
      }
    }

    for (std::vector<UIIngredientInfoWnd*>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
      ITEM_INFO owned = g_session.LookupClientItemByIdentity((*it)->item.identity[1]);
      if (owned.identity[1] <= 0) {
        (*it)->Show(WINDOW_HIDDEN);
      }
    }
    this->SendMsg(this, UM_SETPOS, 0, 0, 0, 0);
    break;

  case UM_COMMAND:
    for (std::vector<UIIngredientInfoWnd*>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
      UIIngredientInfoWnd* entry = *it;
      if (sender != entry) {
        entry->selected = 0;
        entry->Invalidate();
      }
    }
    this->Invalidate();
    break;

  case UM_DECITEM: {
    const std::vector<UIIngredientInfoWnd*>::iterator it =
        std::find_if(this->items.begin(), this->items.end(), [a](UIIngredientInfoWnd* item) {
          return static_cast<std::uint32_t>(a) == item->item.identity[1];
        });
    if (it == this->items.end()) {
      break;
    }

    UIIngredientInfoWnd* selected = *it;
    const std::vector<ITEM_INFO>::iterator found =
        std::find_if(this->materials.begin(), this->materials.end(), [selected](ITEM_INFO material) {
          const int itemId = std::atoi(selected->item.itemId.c_str());
          return itemId == std::atoi(material.itemId.c_str());
        });

    if (found == this->materials.end()) {
      break;
    }

    const int need = found->quantity;
    if (need > selected->item.quantity) {
      this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(MSI_LAPINE_FAIL_NOT_ENOUGH_ITEM_COUNT), found->quantity).c_str()), 0, 0, 0);
      return -1;
    }

    if (found->refiningLevel > selected->item.refiningLevel) {
      this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LAPINE_FAIL_NOT_ENOUGH_REFINING_LEVEL)), 0, 0, 0);
      return -1;
    }

    selected->item.quantity -= need;
    if (selected->item.quantity <= 0) {
      (*it)->Show(WINDOW_HIDDEN);
    } else {
      (*it)->insufficient = 1;
      (*it)->Invalidate();
    }
    this->SendMsg(this, UM_SETPOS, 0, 0, 0, 0);
    break;
  }

  case UM_SETPOS: {
    int row = 0;
    for (std::vector<UIIngredientInfoWnd*>::iterator it = this->items.begin(); it != this->items.end();) {
      UIIngredientInfoWnd* entry = *it;
      if (entry->geometry.visible == 0) {
        entry->Invalidate();
        it = this->items.erase(it);
      } else {
        this->MoveScrollChild(entry, 0, row * 0x1E + 7);
        (*it)->Invalidate();
        ++row;
        ++it;
      }
    }
    this->UpdateScrollRange();
    this->Invalidate();
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
  this->ingredientId = id;

  {
    ITEM_INFO item;
    item.SetItemId(id);

    char name[0x104];
    std::memset(name, 0, sizeof(name));
    int capacity = 0x104;
    item.GetIdDisplayName(name, capacity, 0);
    this->title = name;
  }

  g_session.m_hotkeyScript->CallTable("tblLapineDdukddakBox", "set_resource_item_list", "d", id);

  int minRefine = 0;
  g_session.m_hotkeyScript->CallTable("tblLapineDdukddakBox", "get_min_need_refine", "d>d", id, &minRefine);
  this->ingredientList->minRefine = minRefine;

  int maxRefine = 0;
  g_session.m_hotkeyScript->CallTable("tblLapineDdukddakBox", "get_max_need_refine", "d>d", id, &maxRefine);
  this->ingredientList->maxRefine = maxRefine;

  this->ingredientList->SendMsg(this, UM_REFRESHITEM, 0, 0, 0, 0);

  g_session.m_hotkeyScript->CallTable("tblLapineDdukddakBox", "get_need_item_count", "d>d", id, &this->maxItems);

  const char* source = nullptr;
  g_session.m_hotkeyScript->CallTable("tblLapineDdukddakBox", "get_need_source_string", "d>s", id, &source);
  this->description = source;
}

// OFFSET: 0x00601BC0
// FUNCTION: fn_04176
void UILapineBoxWnd::AddMaterial(int id, int quantity) {
  std::vector<ITEM_INFO>& materials = this->ingredientList->materials;
  const std::vector<ITEM_INFO>::iterator it = std::find_if(materials.begin(), materials.end(), [id](ITEM_INFO material) {
    return id == std::atoi(material.itemId.c_str());
  });

  if (it != materials.end()) {
    return;
  }

  ITEM_INFO material;
  material.SetItemId(id);
  material.quantity = quantity;
  materials.push_back(material);
}

// OFFSET: 0x006028C0
// FUNCTION: fn_04183
void UIIngredientInfoWnd::SetIngredientItem(std::uint32_t id) {
  this->item = g_session.LookupClientItemByIdentity(id);

  {
    std::vector<const char*> breaks;
    char name[260];
    std::memset(name, 0, sizeof(name));
    char* highlight = nullptr;
    int remaining = sizeof(name);
    char* cursor = name;
    int highlightColor;
    this->BuildItemName(&this->item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);

    if (this->item.identified == 1 && this->item.optionCount != 0) {
      sprintf_s(name, sizeof(name), LookupMessage(MSI_EA6), name, this->item.optionCount);
    }

    this->item.SetName(name);
    this->tooltip = name;
  }

  this->Invalidate();
}

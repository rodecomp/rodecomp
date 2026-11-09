#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMessage.h"
#include "Client/Mode/CMode.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UINewSelectCharWnd.h"
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
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
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
  struct LapineUpgradeStartPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ std::int32_t ingredientId;
    /* 0006 */ std::int16_t itemId;
  };
#pragma pack(pop)
  static_assert(sizeof(LapineUpgradeClosePacket) == 2);
  static_assert(sizeof(LapineUpgradeStartPacket) == 8);
}

// OFFSET: 0x00602D30
// FUNCTION: fn_04187
UILapineUpgradeBoxWnd::UILapineUpgradeBoxWnd()
    : UIFrameWnd(0), closeButton(nullptr), upgradeButton(nullptr), selectButton(nullptr), ingredientList(nullptr), ingredientId(0),
      noticeTick(0), noticeDuration(3000), waiting(0) {
}

// OFFSET: 0x00602E60
// FUNCTION: fn_04188
UILapineUpgradeBox_IngredientInfoWnd::UILapineUpgradeBox_IngredientInfoWnd()
    : UIFrameWnd(0), selected(0), quantity(0) {
}

// INLINED
UILapineUpgradeBoxWnd::~UILapineUpgradeBoxWnd() {
  this->item = ITEM_INFO();

  LapineUpgradeClosePacket packet;
  packet.id = ID_CZ_RANDOM_UPGRADE_ITEM_UI_CLOSE;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x006035D0
// FUNCTION: fn_04193
void UILapineUpgradeBoxWndNoop() {}

// OFFSET: 0x006036E0
// FUNCTION: fn_04198
void UILapineUpgradeBoxWnd::OnCreate(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->ingredientList = new UILapineUpgradeBox_IngredientListWnd(150, 204);
  this->ingredientList->Move(241, 17);
  this->AddChild(this->ingredientList);

  this->closeButton = new UIBitmapButton();
  this->closeButton->SetBitmap("유저인터페이스\\lapine\\btn_cancel_out.bmp", 0, 0);
  this->closeButton->SetBitmap("유저인터페이스\\lapine\\btn_cancel_over.bmp", 1, 0);
  this->closeButton->SetBitmap("유저인터페이스\\lapine\\btn_cancel_press.bmp", 2, 0);
  this->closeButton->SetBitmap("유저인터페이스\\lapine\\btn_cancel_dis.bmp", 3, 0);
  this->closeButton->SetSize(this->closeButton->BitmapWidth(), this->closeButton->BitmapHeight());
  this->closeButton->Move(280, 226);
  this->closeButton->SetId(201);
  this->AddChild(this->closeButton);

  this->upgradeButton = new UIBitmapButton();
  this->upgradeButton->SetBitmap("유저인터페이스\\lapine\\btn_make_out.bmp", 0, 0);
  this->upgradeButton->SetBitmap("유저인터페이스\\lapine\\btn_make_over.bmp", 1, 0);
  this->upgradeButton->SetBitmap("유저인터페이스\\lapine\\btn_make_press.bmp", 2, 0);
  this->upgradeButton->SetBitmap("유저인터페이스\\lapine\\btn_make_dis.bmp", 3, 0);
  this->upgradeButton->SetSize(this->upgradeButton->BitmapWidth(), this->upgradeButton->BitmapHeight());
  this->upgradeButton->Move(340, 226);
  this->upgradeButton->SetId(509);
  this->AddChild(this->upgradeButton);

  this->selectButton = new UIBitmapButton();
  this->selectButton->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_out.bmp", 0, 0);
  this->selectButton->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_over.bmp", 1, 0);
  this->selectButton->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_press.bmp", 2, 0);
  this->selectButton->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_dis.bmp", 3, 0);
  this->selectButton->SetSize(this->selectButton->BitmapWidth(), this->selectButton->BitmapHeight());
  this->selectButton->Move(213, 110);
  this->selectButton->SetId(505);
  this->AddChild(this->selectButton);
}

// OFFSET: 0x00603A90
// FUNCTION: fn_04199
void UILapineUpgradeBox_IngredientListWnd::OnCreate(int width, int height) {
  this->scrollBarWidth = 9;
  if (this->scrollBar) {
    this->RemoveChild(this->scrollBar, 0);
    delete this->scrollBar;
    this->scrollBar = nullptr;
  }

  this->scrollBar = new UILapineScrollBar();
  this->scrollBar->Configure(width - this->scrollBarWidth, 0, 1, height - this->scrollBarHeight);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00603CA0
// FUNCTION: fn_04201
void UILapineUpgradeBox_IngredientInfoWnd::Paint() {
  this->ClearSurface(0xFFF7F7);
  if (this->selected == 1) {
    this->DrawBitmap(32, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\list_selected_item.bmp"))), false);
  }

  DrawItemIcon(*this, ResourceMgr::Instance(), 4, 3, this->item, 0);

  const std::string name = this->FitDisplayText(100, this->item.displayName.c_str(), 0, 12, 0);
  this->DrawShadowedText(36, 9, name.c_str(), 0, 0, 0xE0E0E0, 0, 12, 0);
}

// OFFSET: 0x00603B60
// FUNCTION: fn_04200
void UILapineUpgradeBoxWnd::Paint() {
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\bg_lapineUpgrade.bmp")));
  this->DrawBitmap(0, 0, background, true);

  if (this->title.size() != 0) {
    this->DrawTitleBar(false, this->title.c_str(), 0);
  }

  this->DrawCenteredText(35, 47, LookupMessage(MSI_LAPINE_FAIL_NOT_ENOUGH_REFINING_LEVEL), 156, 0, 11, 0xCD7A51, 0);
  this->DrawCenteredText(35, 63, this->description.c_str(), 156, 0, 11, 0x844A42, 0);

  if (this->item.identity[1] > 0) {
    DrawItemIcon(*this, ResourceMgr::Instance(), 101, 165, this->item, 0);
  }

  if (timeGetTime() < this->noticeTick + this->noticeDuration) {
    CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\icon_notify.bmp")));
    this->DrawBitmap(6, 229, icon, true);
    this->DrawTextAt(25, 230, this->notice.c_str(), 0, 0, 12, 0xFF0000, 0, 0);
  }
}

// OFFSET: 0x006035E0
// FUNCTION: fn_04194
void UILapineUpgradeBoxWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (this->item.identity[1] > 0) {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 27) {
    this->SendMsg(this, UM_ADDITEM, drag->id, 0, 0, 0);
  }
  mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x00603650
// FUNCTION: fn_04195
void UILapineUpgradeBox_IngredientInfoWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (this->parent) {
    this->parent->DragAndDrop(x + this->geometry.x, y + this->geometry.y, value);
  }
}

// OFFSET: 0x006036D0
// FUNCTION: fn_04197
void UILapineUpgradeBox_IngredientInfoWnd::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x00603DB0
// FUNCTION: fn_04203
void UILapineUpgradeBoxWnd::OnLBtnDblClk(int x, int y) {
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.identity[1] != 0) {
    this->SendMsg(this, UM_DECITEM, this->item.identity[1], 0, 0, 0);
  }
}

// OFFSET: 0x00603DF0
// FUNCTION: fn_04204
void UILapineUpgradeBox_IngredientInfoWnd::OnLBtnDblClk(int x, int y) {
  if (this->item.identity[1] > 0) {
    this->parent->SendMsg(this, UM_DECITEM, this->item.identity[1], 0, 0, 0);
  }
}

// OFFSET: 0x00603E20
// FUNCTION: fn_04205
void UILapineUpgradeBoxWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.identity[1] > 0) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(g_windowMgr, 26, this->item, 1, -1);
  }
}

// OFFSET: 0x00603E80
// FUNCTION: fn_04206
void UILapineUpgradeBox_IngredientInfoWnd::OnLBtnDown(int x, int y) {
  if (this->item.identity[1] > 0) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(g_windowMgr, 27, this->item, this->quantity, -1);
  }
}

// OFFSET: 0x00603EC0
// FUNCTION: fn_04207
void UILapineUpgradeBox_IngredientInfoWnd::OnLBtnUp(int x, int y) {
  this->selected = 1;
  this->Invalidate();
  if (this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00604180
// FUNCTION: fn_04213
void UILapineUpgradeBox_IngredientInfoWnd::OnWheel(int argument) {
  if (this->parent) {
    this->parent->OnWheel(argument);
  }
}

// OFFSET: 0x006040E0
// FUNCTION: fn_04211
void UILapineUpgradeBoxWnd::OnRBtnDown(int x, int y) {
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.identity[1] > 0) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x00604140
// FUNCTION: fn_04212
void UILapineUpgradeBox_IngredientInfoWnd::OnRBtnDown(int x, int y) {
  if (this->item.identity[1] > 0) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
    window->SendMsg(this, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x00605BB0
// FUNCTION: fn_04225
void UILapineUpgradeBoxWnd::SetWaiting(unsigned char value) {
  this->waiting = value;
  auto* const button = this->upgradeButton;
  button->SetDisabled(value);
  button->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x006049B0
// FUNCTION: fn_04217
void UILapineUpgradeBoxWnd::SetIngredient(int id) {
  this->ingredientId = id;

  ITEM_INFO item;
  item.SetItemId(id);
  char name[0x104];
  std::memset(name, 0, sizeof(name));
  int capacity = sizeof(name);
  item.GetIdDisplayName(name, capacity, 0);
  this->title.assign(name, std::strlen(name));

  int minimumRefine = 0;
  int maximumRefine = 0;
  g_session.m_hotkeyScript->CallTable("tblLapineUpgradeBox", "get_min_need_refine", "d>d", id, &minimumRefine);
  this->ingredientList->minRefine = minimumRefine;
  g_session.m_hotkeyScript->CallTable("tblLapineUpgradeBox", "get_max_need_refine", "d>d", id, &maximumRefine);
  this->ingredientList->maxRefine = maximumRefine;
  g_session.m_hotkeyScript->CallTable("tblLapineUpgradeBox", "get_min_need_option", "d>d", id, &this->minOptionCount);
  this->ingredientList->minOptionCount = this->minOptionCount;
  g_session.m_hotkeyScript->CallTable("tblLapineUpgradeBox", "set_resource_item_list", "d", id);
  this->ingredientList->SendMsg(this, UM_REFRESHITEM, 0, 0, 0, 0);

  const char* source = nullptr;
  g_session.m_hotkeyScript->CallTable("tblLapineUpgradeBox", "get_need_source_string", "d>s", id, &source);
  this->description.assign(source, std::strlen(source));
}



// OFFSET: 0x00603680
// FUNCTION: fn_04196
void UILapineUpgradeBox_IngredientListWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 26) {
    this->SendMsg(this, UM_ADDITEM, drag->id, 0, 0, 0);
  }
  mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x00604680
// FUNCTION: fn_04216
int UILapineUpgradeBox_IngredientListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c,
                                                  std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    for (auto it = this->items.begin(); it != this->items.end(); ++it) {
      UILapineUpgradeBox_IngredientInfoWnd* entry = *it;
      if (sender != entry) {
        entry->selected = 0;
        entry->Invalidate();
      }
    }
    this->Invalidate();
    break;

  case UM_REFRESHITEM:
    for (auto material = this->materials.begin(); material != this->materials.end(); ++material) {
      std::vector<int> identities = g_session.FindInventoryIndicesByItemId(std::atoi(material->itemId.c_str()), 1);
      for (auto identity = identities.begin(); identity != identities.end(); ++identity) {
        this->AddIngredient(static_cast<std::uint32_t>(*identity));
      }
    }
    for (auto it = this->items.begin(); it != this->items.end(); ++it) {
      const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, (*it)->item.identity[1]);
      if (owned.identity[1] == 0) {
        (*it)->Show(0);
      }
    }
    this->SendMsg(this, UM_SETPOS, 0, 0, 0, 0);
    break;

  case UM_ADDITEM:
    this->parent->SendMsg(this, UM_DECITEM, a, 0, 0, 0);
    break;

  case UM_SETPOS: {
    int y = 7;
    for (auto it = this->items.begin(); it != this->items.end();) {
      if ((*it)->geometry.visible == 0) {
        (*it)->Invalidate();
        it = this->items.erase(it);
      } else {
        this->MoveScrollChild(*it, 0, y);
        (*it)->Invalidate();
        ++it;
        y += 0x1E;
      }
    }
    this->UpdateScrollRange();
    this->Invalidate();
    break;
  }

  case UM_DECITEM:
    this->parent->SendMsg(this, UM_ADDITEM, a, 0, 0, 0);
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
void UILapineUpgradeBox_IngredientListWnd::OnWheel(int argument) {
  if (this->wheelScroll == 1) {
    if (argument < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, 15, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -15, 0, 0, 0);
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
void UILapineUpgradeBoxWnd::OnProcess() {
  UILapineUpgradeBox_IngredientInfoWnd* selected = this->FindSelected();
  if (!selected) {
    if (this->selectButton->Disabled() == 0) {
      this->selectButton->SetDisabled(1);
      this->selectButton->Invalidate();
      this->Invalidate();
    }
  } else if (this->selectButton->Disabled() == 1 && this->item.identity[1] <= 0) {
    this->selectButton->SetDisabled(0);
    this->selectButton->Invalidate();
    this->Invalidate();
  }

  if (this->waiting == 0) {
    if (this->item.identity[1] > 0) {
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
void UILapineUpgradeBox_IngredientInfoWnd::OnMouseHover(int x, int y) {
  this->OnMouseMove(x, y);

  if (this->tooltip.size() > 0) {
    int tipX = 0;
    int tipY = -20;
    this->ToScreen(tipX, tipY);
    g_modeMgr.gameMode->ShowTooltip(this->tooltip.c_str(), tipX, tipY, -1, false, true);
  }
}

// OFFSET: 0x00603EF0
// FUNCTION: fn_04208
void UILapineUpgradeBoxWnd::OnMouseHover(int x, int y) {
  this->OnMouseMove(x, y);

  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.identity[1] > 0) {
    int tipX = 101;
    int tipY = 145;
    this->ToScreen(tipX, tipY);
    g_modeMgr.gameMode->ShowTooltip(this->item.displayName.c_str(), tipX, tipY, -1, false, true);
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
// FUNCTION: fn_04220
bool UILapineUpgradeBox_IngredientListWnd::AddIngredient(std::uint32_t identity) {
  ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, identity);
  if (owned.identity[1] <= 0) {
    return false;
  }
  if (owned.refiningLevel < this->minRefine || owned.refiningLevel > this->maxRefine || owned.optionCount < this->minOptionCount) {
    return false;
  }
  const int itemId = std::atoi(owned.itemId.c_str());
  if ((g_session.IsPetEgg(itemId) == 1 && owned.damaged == 1) || g_session.HasSwapEquipmentItem(owned.identity[1]) == 1) {
    return false;
  }
  const auto material = std::find_if(this->materials.begin(), this->materials.end(), [itemId](ITEM_INFO value) {
    return std::atoi(value.itemId.c_str()) == itemId;
  });
  if (material == this->materials.end()) {
    return false;
  }
  for (UILapineUpgradeBox_IngredientInfoWnd* entry : this->items) {
    if (entry->item.identity[1] == identity) {
      return true;
    }
  }
  auto* owner = static_cast<UILapineUpgradeBoxWnd*>(this->parent);
  auto* entry = new UILapineUpgradeBox_IngredientInfoWnd();
  entry->SetSize(136, 30);
  entry->SetItem(identity);
  entry->quantity = 1;
  this->AddScrollChild(entry);
  entry->Invalidate();
  this->items.push_back(entry);
  this->SendMsg(this, UM_SETPOS, 0, 0, 0, 0);
  return true;
}

// OFFSET: 0x006059B0
// FUNCTION: fn_04224
__declspec(noinline) void UILapineUpgradeBox_IngredientInfoWnd::SetItem(std::uint32_t identity) {
  this->item = LookupClientItemByIdentity(g_session.m_inventoryItems, identity);

  std::vector<char*> breaks;
  char name[0x104];
  std::memset(name, 0, sizeof(name));
  char* highlight = nullptr;
  std::size_t remaining = sizeof(name);
  char* cursor = name;
  int highlightColor;
  this->BuildItemName(&this->item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);

  if (this->item.identified == 1 && this->item.optionCount != 0) {
    sprintf_s(name, sizeof(name), LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), name, this->item.optionCount);
  }

  this->item.SetName(name);
  this->tooltip.assign(name, std::strlen(name));
  this->Invalidate();
}

// OFFSET: 0x00605750
// FUNCTION: fn_04222
bool UILapineUpgradeBox_IngredientListWnd::CanUseIngredient(std::uint32_t identity) {
  auto entryIt = std::find_if(this->items.begin(), this->items.end(), [identity](UILapineUpgradeBox_IngredientInfoWnd* entry) {
    return entry->item.identity[1] == identity;
  });
  if (entryIt == this->items.end()) {
    return false;
  }

  UILapineUpgradeBox_IngredientInfoWnd* entry = *entryIt;
  const ITEM_INFO& item = entry->item;

  const auto material = std::find_if(this->materials.begin(), this->materials.end(), [&item](ITEM_INFO required) {
    return std::atoi(required.itemId.c_str()) == std::atoi(item.itemId.c_str());
  });
  if (material == this->materials.end()) {
    return false;
  }

  if (this->minRefine > item.refiningLevel) {
    return false;
  }
  if (this->maxRefine < item.refiningLevel) {
    return false;
  }

  if (this->minRefine != 0 && this->minRefine < item.refiningLevel) {
    this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ITEMSHOP_CURRENCY)), 0, 0, 0);
  }

  if (Equipment(item) && !item.IsApprenticeItem() && !item.HasSpecialCard()) {
    for (std::size_t offset = 0; offset < sizeof(item.cards); offset += sizeof(std::uint32_t)) {
      std::uint32_t card = 0;
      std::memcpy(&card, item.cards + offset, sizeof(card));
      if (card != 0) {
        this->parent->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_PURCHASEWND_TOTAL)), 0, 0, 0);
        break;
      }
    }
  }

  if (this->minOptionCount > item.optionCount) {
    return false;
  }

  entry->Show(0);
  this->SendMsg(this, UM_SETPOS, 0, 0, 0, 0);
  return true;
}

// OFFSET: 0x006041E0
// FUNCTION: fn_04215
int UILapineUpgradeBoxWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_REFRESHITEM:
    this->SetIngredient(static_cast<int>(a));
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_LAPINEUPGRADEBOXWND);
      return 0;
    case ID_LAPINE_ADD_INGREDIENT: {
      UILapineUpgradeBox_IngredientInfoWnd* selected = this->ingredientList->FindSelected();
      if (selected) {
        this->SendMsg(this, UM_ADDITEM, selected->item.identity[1], 0, 0, 0);
        return 0;
      }
      break;
    }
    case ID_LAPINE_START_UPGRADE:
      if (this->item.identity[1] != 0) {
        LapineUpgradeStartPacket packet;
        packet.id = ID_CZ_REQ_RANDOM_UPGRADE_ITEM;
        packet.ingredientId = this->ingredientId;
        packet.itemId = static_cast<std::int16_t>(this->item.identity[1]);
        GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
        this->waiting = 1;
        this->upgradeButton->SetDisabled(1);
        this->upgradeButton->Invalidate();
        this->Invalidate();
        return 0;
      }
      break;
    }
    break;
  case UM_ADDITEM:
    if (this->item.identity[1] == 0 && this->ingredientList->CanUseIngredient(static_cast<std::uint32_t>(a)) == 1) {
      ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, static_cast<std::uint32_t>(a));
      if (owned.identity[1] != 0) {
        std::vector<char*> breaks;
        char name[260];
        std::memset(name, 0, sizeof(name));
        int highlightColor = 0;
        char* cursor = name;
        std::size_t remaining = sizeof(name);
        char* highlight = nullptr;
        this->BuildItemName(&owned, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);

        if (owned.identified == 1 && owned.optionCount != 0) {
          sprintf_s(name, sizeof(name), LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), name, owned.optionCount);
        }

        owned.SetName(name);
        this->item = owned;
        this->Invalidate();
      }
      return 0;
    }
    break;
  case UM_DECITEM:
    if (this->item.identity[1] != 0 && this->item.identity[1] == a) {
      this->item = ITEM_INFO();
      this->SendMsg(this, UM_RECEIVE_MSG, reinterpret_cast<std::intptr_t>(""), 0, 0, 0);
      this->Invalidate();
      this->ingredientList->AddIngredient(static_cast<std::uint32_t>(a));
      return 0;
    }
    break;
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

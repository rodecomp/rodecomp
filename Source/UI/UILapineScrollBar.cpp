#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/UIIngredientInfoWnd.h"
#include "UI/UILapineScrollBar.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x005FF800
// FUNCTION: fn_04148
void UILapineScrollBarNoop() {}

// OFFSET: 0x005FF810
// FUNCTION: fn_04149
void UIIngredientInfoWnd::DragAndDrop(int argument1, int argument2, std::intptr_t argument3) {
  this->parent->DragAndDrop(argument1, argument2, argument3);
}

// OFFSET: 0x005FF910
// FUNCTION: fn_04152
void UIIngredientInfoWnd::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x00600270
// FUNCTION: fn_04158
void UILapineScrollBar::Paint() {
  const auto load = [](const char* name) {
    return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  };
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    CBitmapRes* top = load("유저인터페이스\\lapine\\scrollbar_bg_top.bmp");
    CBitmapRes* bottom = load("유저인터페이스\\lapine\\scrollbar_bg_btm.bmp");
    CBitmapRes* middle = load("유저인터페이스\\lapine\\scrollbar_bg_mid.bmp");
    this->DrawBitmap(0, 0, top, true);

    const int interior = this->geometry.height - bottom->image.height - top->image.height;
    const int count = interior / middle->image.height + (interior % middle->image.height != 0);
    for (int index = 0; index < count; ++index) {
      this->DrawBitmap(0, middle->image.height * index + top->image.height, middle, true);
    }
    this->DrawBitmap(0, this->geometry.height - bottom->image.height, bottom, true);

    const int travel = this->geometry.height - bottom->image.height - top->image.height;
    const int thumbLength = this->visible * travel / this->total;
    const int thumbOffset = this->position * travel / this->total;

    if (this->state == 0) {
      CBitmapRes* capTop = load("유저인터페이스\\lapine\\scrollbar_thumb_out_top.bmp");
      CBitmapRes* capBottom = load("유저인터페이스\\lapine\\scrollbar_thumb_out_btm.bmp");
      CBitmapRes* fill = load("유저인터페이스\\lapine\\scrollbar_thumb_out_mid.bmp");
      const int span = thumbLength - capBottom->image.height - capTop->image.height;
      const int pieces = span / fill->image.height + (span % fill->image.height != 0);
      for (int index = 0; index < pieces; ++index) {
        this->DrawBitmap(0, fill->image.height * index + thumbOffset + capTop->image.height + top->image.height, fill, true);
      }
      this->DrawBitmap(0, top->image.height + thumbOffset, capTop, true);
      this->DrawBitmap(0, thumbOffset - capBottom->image.height + thumbLength + top->image.height, capBottom, true);
    } else if (this->state == 3) {
      CBitmapRes* capTop = load("유저인터페이스\\lapine\\scrollbar_thumb_over_top.bmp");
      CBitmapRes* capBottom = load("유저인터페이스\\lapine\\scrollbar_thumb_over_btm.bmp");
      CBitmapRes* fill = load("유저인터페이스\\lapine\\scrollbar_thumb_over_mid.bmp");
      const int span = thumbLength - capBottom->image.height - capTop->image.height;
      const int pieces = span / fill->image.height + (span % fill->image.height != 0);
      for (int index = 0; index < pieces; ++index) {
        this->DrawBitmap(0, fill->image.height * index + thumbOffset + capTop->image.height + top->image.height, fill, true);
      }
      this->DrawBitmap(0, top->image.height + thumbOffset, capTop, true);
      this->DrawBitmap(0, thumbOffset - capBottom->image.height + thumbLength + top->image.height, capBottom, true);
    } else if (this->state == 6) {
      CBitmapRes* capTop = load("유저인터페이스\\lapine\\scrollbar_thumb_press_top.bmp");
      CBitmapRes* capBottom = load("유저인터페이스\\lapine\\scrollbar_thumb_press_btm.bmp");
      CBitmapRes* fill = load("유저인터페이스\\lapine\\scrollbar_thumb_press_mid.bmp");
      const int span = thumbLength - capBottom->image.height - capTop->image.height;
      const int pieces = span / fill->image.height + (span % fill->image.height != 0);
      for (int index = 0; index < pieces; ++index) {
        this->DrawBitmap(0, fill->image.height * index + thumbOffset + capTop->image.height + top->image.height, fill, true);
      }
      this->DrawBitmap(0, top->image.height + thumbOffset, capTop, true);
      this->DrawBitmap(0, thumbOffset - capBottom->image.height + thumbLength + top->image.height, capBottom, true);
    }
  }

  this->minimumThumb = 0;
  this->parent->Invalidate();
}

// OFFSET: 0x00600260
// FUNCTION: fn_04157
void UILapineBox_IngredientListWnd::Paint() {
  this->ClearSurface(0xFFF7F7);
}

// OFFSET: 0x005FF130
// FUNCTION: fn_45447
UILapineBox_IngredientListWnd::UILapineBox_IngredientListWnd(int width, int height)
    : UIScrollWnd(width, height, 0), minRefine(0), maxRefine(20) {
  this->Resize(width, height);
  this->OnCreate(width, height);
}

// OFFSET: 0x005FF820
// FUNCTION: fn_04150
void UILapineBoxWnd::DragAndDrop(int, int, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (this->items.size() < static_cast<unsigned>(this->maxItems) && drag->source == 27) {
    const int result = this->ingredientList->SendMsg(this, 104, drag->id, 0, 0, 0);
    if (result >= 0) {
      this->AddIngredient(drag->id, drag->quantity);
    }
  }
  mode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x005FF8B0
// FUNCTION: fn_04151
void UILapineBox_IngredientListWnd::DragAndDrop(int, int, std::intptr_t value) {
  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 26) {
    if (this->AddIngredient(drag->id) == 1) {
      this->parent->SendMsg(this, 104, drag->id, 0, 0, 0);
    }
  }
  mode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x005FF920
// FUNCTION: fn_04153
void UILapineBoxWnd::OnCreate(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  UILapineBox_IngredientListWnd* list = new UILapineBox_IngredientListWnd(150, 204);
  this->ingredientList = list;
  list->Move(241, 17);
  this->AddChild(list);

  UIBitmapButton* cancel = new UIBitmapButton();
  this->cancelButton = cancel;
  cancel->SetBitmap("유저인터페이스\\lapine\\btn_cancel_out.bmp", 0, 0);
  cancel->SetBitmap("유저인터페이스\\lapine\\btn_cancel_over.bmp", 1, 0);
  cancel->SetBitmap("유저인터페이스\\lapine\\btn_cancel_press.bmp", 2, 0);
  cancel->SetBitmap("유저인터페이스\\lapine\\btn_cancel_dis.bmp", 3, 0);
  cancel->SetSize(cancel->BitmapWidth(), cancel->BitmapHeight());
  cancel->Move(280, 226);
  cancel->SetId(201);
  this->AddChild(cancel);

  UIBitmapButton* make = new UIBitmapButton();
  this->makeButton = make;
  make->SetBitmap("유저인터페이스\\lapine\\btn_make_out.bmp", 0, 0);
  make->SetBitmap("유저인터페이스\\lapine\\btn_make_over.bmp", 1, 0);
  make->SetBitmap("유저인터페이스\\lapine\\btn_make_press.bmp", 2, 0);
  make->SetBitmap("유저인터페이스\\lapine\\btn_make_dis.bmp", 3, 0);
  make->SetSize(make->BitmapWidth(), make->BitmapHeight());
  make->Move(340, 226);
  make->SetId(503);
  this->AddChild(make);

  UIBitmapButton* move = new UIBitmapButton();
  this->moveButton = move;
  move->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_out.bmp", 0, 0);
  move->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_over.bmp", 1, 0);
  move->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_press.bmp", 2, 0);
  move->SetBitmap("유저인터페이스\\lapine\\btn_moveitem_dis.bmp", 3, 0);
  move->SetSize(move->BitmapWidth(), move->BitmapHeight());
  move->Move(213, 110);
  move->SetId(505);
  this->AddChild(move);
}

// OFFSET: 0x005FFCD0
// FUNCTION: fn_04154
void UILapineBox_IngredientListWnd::OnCreate(int width, int height) {
  this->scrollBarWidth = 9;
  if (this->scrollBar != nullptr) {
    this->RemoveChild(this->scrollBar, 0);
    if (this->scrollBar != nullptr) {
      delete this->scrollBar;
    }
    this->scrollBar = nullptr;
  }

  UILapineScrollBar* bar = new UILapineScrollBar();
  this->scrollBar = bar;
  bar->Configure(width - this->scrollBarWidth, 0, 1, height - this->scrollBarHeight);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x005FFDC0
// FUNCTION: fn_04155
void UIIngredientInfoWnd::Paint() {
  this->ClearSurface(0xFFF7F7);
  if (this->selected == 1) {
    CBitmapRes* selected = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\list_selected_item.bmp")));
    this->DrawBitmap(32, 0, selected, false);
  }

  std::string name;
  name = this->FitDisplayText(100, this->item.unknown_0044.c_str(), 0, 12, 0).c_str();

  std::uint32_t color = 0;
  if (this->insufficient == 0) {
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), 4, 3, this->item, 0, 0);
  } else {
    DrawItemWithQuantityAuxiliary(*this, 2, 4, 3, this->item, this->item.identified, 0);
    color = 0x969696;
  }

  this->DrawShadowedText(36, 9, name.c_str(), 0, color, 0xE0E0E0, 0, 12, 0);
}

// OFFSET: 0x005FFF90
// FUNCTION: fn_04156
void UILapineBoxWnd::Paint() {
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\bg_lapine.bmp")));
  this->DrawBitmap(0, 0, background, true);

  if (this->title.size() != 0) {
    this->DrawTitleBar(false, this->title.c_str(), 0);
  }

  this->DrawCenteredText(33, 47, LookupMessage(2896), 160, 0, 11, 0xCD7A51, 0);
  this->DrawCenteredText(33, 63, this->description.c_str(), 160, 0, 11, 0x844A42, 0);

  const std::string count = FormatOwnedString("%d", static_cast<int>(this->items.size()));
  this->DrawCenteredText(98, 76, count.c_str(), 14, 0, 11, 0xFFFFFF, 0);
  const std::string maximum = FormatOwnedString("%d", this->maxItems);
  this->DrawCenteredText(115, 76, maximum.c_str(), 14, 0, 11, 0xFFFFFF, 0);

  for (unsigned index = 0; index < this->items.size(); ++index) {
    int position[2];
    this->MaterialPosition(position, index);
    const int x = position[0];
    const int y = position[1];
    CBitmapRes* slot = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\shadow_item_off.bmp")));
    this->DrawBitmap(x - 4, y + 11, slot, true);
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, y, this->items[index], 0, 0);
  }

  if (timeGetTime() < this->noticeTick + this->noticeDuration) {
    CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\lapine\\icon_notify.bmp")));
    this->DrawBitmap(6, 229, icon, true);
    this->DrawTextAt(25, 230, this->notice.c_str(), 0, 0, 12, 0xFF0000, 0, 0);
  }
}

// OFFSET: 0x00601D20
// FUNCTION: fn_04177
int UILapineBoxWnd::AddIngredient(std::uint32_t id, int quantity) {
  if (this->items.size() < 10) {
    if (std::find_if(this->items.begin(), this->items.end(), [id](ITEM_INFO entry) { return id == entry.unknown_0000[1]; }) != this->items.end()) {
      return 0;
    }
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, id);
    item.quantity = quantity;
    {
      std::vector<char*> breaks;
      char name[260];
      std::memset(name, 0, sizeof(name));
      char* highlight = nullptr;
      std::size_t remaining = sizeof(name);
      char* cursor = name;
      int highlightColor;
      this->BuildItemName(&item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);
      if (item.identified == 1 && item.unknown_0098 != 0) {
        sprintf_s(name, sizeof(name), LookupMessage(2600), name, item.unknown_0098);
      }
      item.SetName(name);
    }
    this->items.push_back(item);
    if (static_cast<int>(this->items.size()) == this->maxItems) {
      this->moveButton->SetDisabled(1);
      this->moveButton->Invalidate();
    }
    this->Invalidate();
    return 1;
  }
  return 0;
}

// OFFSET: 0x00602030
// FUNCTION: fn_04178
unsigned char UILapineBox_IngredientListWnd::AddIngredient(std::uint32_t id) {
  ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, id);
  if (owned.unknown_0000[1] == 0) {
    return 0;
  }

  const int itemId = std::atoi(owned.itemId.c_str());
  if (g_session.IsPetEgg(itemId) == 1 && owned.unknown_005D == 1) {
    return 0;
  }

  if (g_session.HasItem61B8(owned.unknown_0000[1]) == 1) {
    return 0;
  }

  auto it = std::find_if(this->materials.begin(), this->materials.end(), [owned](ITEM_INFO mat) {
    return std::atoi(owned.itemId.c_str()) == std::atoi(mat.itemId.c_str());
  });

  if (it == this->materials.end()) {
    return 0;
  }

  for (UIIngredientInfoWnd* itemWnd : this->items) {
    if (itemWnd->item.unknown_0000[1] == id) {
      itemWnd->item.quantity += it->quantity;
      itemWnd->insufficient = 0;
      itemWnd->Show(1);
      itemWnd->Invalidate();
      this->Invalidate();
      return 1;
    }
  }

  UILapineBoxWnd* owner = static_cast<UILapineBoxWnd*>(this->parent);
  UIIngredientInfoWnd* info = new UIIngredientInfoWnd();
  info->SetSize(136, 30);
  info->SetIngredientItem(id);
  info->quantity = it->quantity;
  this->AddScrollChild(info);

  if (owned.quantity < it->quantity) {
    info->insufficient = 1;
  }

  if (this->minRefine > owned.unknown_0060 || this->maxRefine < owned.unknown_0060) {
    info->insufficient = 1;
  }

  info->Invalidate();
  this->items.push_back(info);
  this->SendMsg(this, 87, 0, 0, 0, 0);
  return 1;
}


// OFFSET: 0x005FF210
// FUNCTION: fn_04141
UILapineScrollBar::UILapineScrollBar() : UIScrollBar() {
  this->arrowLength = 0;
}

// OFFSET: 0x005FF370
// FUNCTION: fn_04143
UIIngredientInfoWnd::~UIIngredientInfoWnd() = default;

UILapineBoxWnd::~UILapineBoxWnd() {
  this->items.clear();
  short packetType = ID_CZ_RANDOM_COMBINE_ITEM_UI_CLOSE;
  CRagConnection& connection = GetRagConnection();
  connection.Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packetType), &packetType);
}

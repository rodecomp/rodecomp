#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "UI/UIChangeMaterialWnd.h"
#include "Client/Mode/CMode.h"

#include "Client/Input/InputCharset.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ChangeMaterialMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Utility/StackAlloc.h"
#include "Utility/Text/FormatString.h"
#include "Client/Mode/ModeMgr.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <malloc.h>
#include <string>

// INLINED
inline const MaterialListEntry* UIChangeMaterialWnd_List::GetEntry(unsigned int index) const {
  if (index >= this->m_items.size()) {
    return nullptr;
  }

  const unsigned int entryIndex = static_cast<unsigned int>(this->m_items[index]);
  if (entryIndex >= this->m_entries.size()) {
    return nullptr;
  }

  return &this->m_entries[entryIndex];
}

// OFFSET: 0x005E03B0
// FUNCTION: fn_03861
UIChangeMaterialWnd::UIChangeMaterialWnd()
    : UIFrameWnd(0), unknown_00B4(nullptr), unknown_00B8(nullptr), m_sortBox(nullptr), m_button(nullptr), m_resultItem(), m_materials(), m_resultAmount(0),
      m_resultText() {
  this->m_resultX = 168;
  this->m_resultY = 131;

  const MaterialSlotPos positions[6] = {{90, 86}, {246, 86}, {168, 222}, {168, 40}, {90, 176}, {246, 176}};
  for (int index = 0; index < 6; ++index) {
    this->m_materialPos[index] = positions[index];
  }
}

// OFFSET: 0x005E0560
// FUNCTION: fn_03862
UIChangeMaterialWnd_List::UIChangeMaterialWnd_List()
    : UIFrameWnd(0), unknown_00B4(27), unknown_00B8(16), m_selectedIndex(0), m_hoverIndex(0), m_scrollOffset(0), m_previousScrollOffset(0), m_scrollBar(nullptr), m_items(),
      m_entries() {
}

// OFFSET: 0x005E0650
// FUNCTION: fn_03863
UIChangeMaterialWnd_ScrollBar::UIChangeMaterialWnd_ScrollBar() : UIScrollBar() {
  this->thickness = 16;
}

// OFFSET: 0x005E1B30
// FUNCTION: fn_03877
void UIChangeMaterialWnd_List::CreateScrollBar(int width, int height) {
  this->m_scrollBar = new UIChangeMaterialWnd_ScrollBar();
  this->m_scrollBar->Configure(width - this->unknown_00B8, 0, 1, height);
  this->m_scrollBar->Show(0);
  this->AddChild(this->m_scrollBar);
  this->m_pageSize = height / this->unknown_00B4;
  this->sub_5E2A70();
}

// OFFSET: 0x005E1080
// FUNCTION: fn_03871
void UIChangeMaterialWnd_List::sub_5E1080(int delta) {
  int64_t newIndex = this->m_selectedIndex + delta;
  const int count = static_cast<int>(this->m_items.size());
  const int maxIndex = count - 1;

  int targetIndex = static_cast<int>(newIndex);
  if (newIndex >= static_cast<uint32_t>(maxIndex)) {
    targetIndex = maxIndex;
  }

  if (targetIndex < this->m_scrollOffset || targetIndex >= this->m_scrollOffset + this->m_pageSize) {
    int newOffset = this->m_previousScrollOffset;
    if (targetIndex < newOffset || targetIndex >= newOffset + this->m_pageSize) {
      const int candidate = targetIndex - this->m_pageSize + 1;
      const int limit = count - this->m_pageSize;
      newOffset = static_cast<unsigned int>(candidate) < static_cast<unsigned int>(limit) ? candidate : limit;
    }

    this->m_scrollOffset = newOffset;
    this->sub_5E2A70();
    this->m_previousScrollOffset = this->m_scrollOffset;
  }

  this->sub_5E3B80(targetIndex);
}

// OFFSET: 0x005E1120
// FUNCTION: fn_03872
ITEM_INFO* UIChangeMaterialWnd::GetItemAt(int x, int y) {
  if (this->m_resultX <= x && x <= this->m_resultX + 24 && this->m_resultY <= y && y <= this->m_resultY + 24) {
    return &this->m_resultItem;
  }

  for (unsigned int i = 0; i < this->m_materials.size(); ++i) {
    if (this->m_materialPos[i].x <= x && x <= this->m_materialPos[i].x + 24 &&
        this->m_materialPos[i].y <= y && y <= this->m_materialPos[i].y + 24) {
      return &this->m_materials[i];
    }
  }
  return nullptr;
}

// OFFSET: 0x005E1300
// FUNCTION: fn_03874
bool UIChangeMaterialWnd::Slot02() {
  return true;
}

// OFFSET: 0x005E1310
// FUNCTION: fn_03875
void UIChangeMaterialWnd_List::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x005E2590
// FUNCTION: fn_03883
void UIChangeMaterialWnd_List::sub_5E2590(int, int position) {
  const int rowHeight = this->unknown_00B4;
  const unsigned int last = static_cast<unsigned int>(this->m_items.size() - 1);
  unsigned int index = static_cast<unsigned int>(position / rowHeight + this->m_scrollOffset);
  index = std::min(index, last);
  this->sub_5E3B80(static_cast<int>(index));
}

// OFFSET: 0x005E25D0
// FUNCTION: fn_03884
void UIChangeMaterialWnd::Slot28(int x, int y) {
  ITEM_INFO* item = this->GetItemAt(x, y);
  if (!item) {
    return;
  }

  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);
  const int itemId = std::atoi(item->itemId.c_str());
  const std::string name = ItemDisplayName(itemId);
  CMode* const mode = g_modeMgr.gameMode;
  mode->ShowTooltip(name.c_str(), screenX + 5, screenY - 5, -1, 0, 1);
}

// OFFSET: 0x005E2840
// FUNCTION: fn_03886
void UIChangeMaterialWnd_List::Slot27(int x, int y) {
}

// OFFSET: 0x005E2850
// FUNCTION: fn_03887
void UIChangeMaterialWnd::UpdateSearchPlaceholder() {
  const std::string text = this->unknown_00B8->Text();
  if (text.compare(LookupMessage(0xec0)) != 0) {
    if (text.empty()) {
      this->unknown_00B8->SetText(LookupMessage(0xec0));
      this->unknown_00B8->TextColor(0xb1b1b1u, 0xffffffffu);
    } else {
      this->unknown_00B8->TextColor(0, 0xffffffffu);
    }
  }
}

// OFFSET: 0x005E29F0
// FUNCTION: fn_03888
void UIChangeMaterialWnd::Slot32(int x, int y) {
  ITEM_INFO* item = this->GetItemAt(x, y);
  if (!item) {
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(12);
  if (window) {
    window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(item), 0, 0, 0);
  }
}

// OFFSET: 0x005E2A30
// FUNCTION: fn_03889
void UIChangeMaterialWnd_List::Slot35(int argument) {
  if (argument < 0) {
    this->SendMsg(this->m_scrollBar, 10, 0, 0, 0, 0);
  } else {
    this->SendMsg(this->m_scrollBar, 9, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005E2A70
// FUNCTION: fn_03890
void UIChangeMaterialWnd_List::sub_5E2A70() {
  const int count = static_cast<int>(this->m_items.size());
  if (count > this->m_pageSize) {
    this->m_scrollBar->Show(1);
    this->m_scrollBar->SetRange(count - this->m_pageSize, this->m_pageSize, count);

    this->m_scrollOffset = std::max(0, this->m_scrollOffset);
    this->m_scrollOffset = std::min(count - this->m_pageSize, this->m_scrollOffset);
    this->m_scrollBar->SetPosition(this->m_scrollOffset);
  } else {
    this->m_scrollOffset = 0;
    this->m_scrollBar->Show(0);
  }

  this->Invalidate();
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x005E2B20
// FUNCTION: fn_03891
void UIChangeMaterialWnd_List::ApplyFilter(std::string text) {
  const int rowHeight = this->unknown_00B4;
  this->m_pageSize = this->geometry.height / rowHeight;
  this->m_selectedIndex = 0;
  this->m_scrollOffset = 0;
  this->m_previousScrollOffset = 0;
  this->m_scrollBar->Show(0);
  this->m_items.clear();

  std::string filter = text;
  std::transform(filter.begin(), filter.end(), filter.begin(), [](unsigned char c) { return static_cast<char>(tolower(c)); });

  int index = 0;
  for (auto entry = this->m_entries.begin(); entry != this->m_entries.end(); ++entry, ++index) {
    std::string name = ItemDisplayName(entry->results.front().id);
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return static_cast<char>(tolower(c)); });

    if (name.find(filter) != std::string::npos) {
      this->m_items.push_back(index);
    }
  }

  this->sub_5E2A70();
  this->SortItems(true);
}

// OFFSET: 0x005E30B0
// FUNCTION: fn_03892
void UIChangeMaterialWnd_List::RefreshVisibleEntries() {
  this->m_entries.clear();

  std::map<MaterialKey, ChangeMaterialRecipeList, MaterialKeyLess> library = g_changeMaterialMgr->m_library;
  for (auto it = library.begin(); it != library.end(); ++it) {
    const std::vector<ChangeMaterialRecipe>& recipes = it->second.m_recipes;
    for (auto recipe = recipes.begin(); recipe != recipes.end(); ++recipe) {
      MaterialListEntry entry = {};
      if (recipe->materials.empty() || recipe->products.empty()) {
        continue;
      }

      entry.materials.resize(recipe->materials.size());
      auto material = entry.materials.begin();
      for (auto source = recipe->materials.begin(); source != recipe->materials.end(); ++source, ++material) {
        material->id = source->itemId;
        material->quantity = source->count;
      }

      entry.results.resize(recipe->products.size());
      auto result = entry.results.begin();
      for (auto source = recipe->products.begin(); source != recipe->products.end(); ++source, ++result) {
        result->id = source->itemId;
        result->quantity = source->count;
        result->amount = source->rate;
      }

      bool available = true;
      for (auto required = entry.materials.begin(); required != entry.materials.end(); ++required) {
        const ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, required->id, 1);
        if (item.quantity < required->quantity) {
          available = false;
        }
      }

      entry.state = available;
      std::sort(entry.results.begin(), entry.results.end(), [](const MaterialResult& left, const MaterialResult& right) {
        return left.quantity < right.quantity;
      });
      this->m_entries.push_back(entry);
    }
  }

  this->ApplyFilter(std::string());
  this->sub_5E3B80(0);
}

// OFFSET: 0x005E39A0
// FUNCTION: fn_03893
void UIChangeMaterialWnd::sub_5E39A0() {
  const std::int16_t count = static_cast<std::int16_t>(this->m_materials.size());
  const int length = count * 4 + 0xC;
  auto* packet = static_cast<unsigned char*>(STACK_ALLOC(static_cast<std::size_t>(length)));

  if (!packet) {
    return;
  }

  *reinterpret_cast<std::uint16_t*>(packet) = 0x7E4;
  *reinterpret_cast<std::uint16_t*>(packet + 2) = static_cast<std::uint16_t>(length);
  *reinterpret_cast<std::int32_t*>(packet + 4) = 0;
  *reinterpret_cast<std::int32_t*>(packet + 8) = 1;

  unsigned char* entry = packet + 0xC;
  for (auto it = this->m_materials.begin(); it != this->m_materials.end(); ++it) {
    *reinterpret_cast<std::uint16_t*>(entry + 2) = static_cast<std::uint16_t>(it->quantity);
    *reinterpret_cast<std::uint16_t*>(entry) = static_cast<std::uint16_t>(it->unknown_0000[1]);
    entry += 4;
  }

  GetRagConnection().Send(static_cast<int>(*reinterpret_cast<const std::int16_t*>(packet + 2)), packet);
  UIBitmapButton* const button = this->m_button;
  button->SetDisabled(1);
  button->Invalidate();
  _freea(packet);
}

// OFFSET: 0x005E3B80
// FUNCTION: fn_03894
void UIChangeMaterialWnd_List::sub_5E3B80(int index) {
  this->m_selectedIndex = index;
  this->m_previousScrollOffset = this->m_scrollOffset;
  this->Invalidate();
  this->parent->SendMsg(nullptr, 23, reinterpret_cast<std::intptr_t>(this->GetEntry(index)), 0, 0, 0);
}

// OFFSET: 0x005E3C30
// FUNCTION: fn_03895
int UIChangeMaterialWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 0:
    if (sender == reinterpret_cast<UIWindow*>(this->unknown_00B8) && reinterpret_cast<UIWindow*>(this->unknown_00B8) == g_windowMgr.GetHoverWindow()) {
      this->unknown_00B4->SendMsg(nullptr, 6, 0x141, reinterpret_cast<std::intptr_t>(this->unknown_00B8->Text()), 0, 0);
    }
    break;
  case 6:
    switch (argument1) {
    case 0xb8:
      this->sub_5E39A0();
      break;
    case 0xc9:
      g_windowMgr.CloseWindow(0x158);
      break;
    case 0x141:
      this->unknown_00B4->SendMsg(nullptr, 6, 0x141, reinterpret_cast<std::intptr_t>(this->unknown_00B8->Text()), 0, 0);
      break;
    case 0x16c:
      this->unknown_00B4->SortItems(argument2 > 0);
      break;
    case 0x19f:
      this->unknown_00B4->SendMsg(nullptr, 0x17, 0, 0, 0, 0);
      break;
    default:
      break;
    }
    break;
  case 0x12:
    this->unknown_00B4->SendMsg(nullptr, 0x12, 0, 0, 0, 0);
    break;
  case 0x13:
    this->unknown_00B4->SendMsg(nullptr, 0x13, 0, 0, 0, 0);
    break;
  case 0x17:
    if (argument1 != 0) {
      auto* entry = reinterpret_cast<MaterialListEntry*>(argument1);
      this->m_materials.clear();
      this->m_resultItem.SetItemId(entry->results.front().id);
      this->m_resultItem.quantity = entry->results.front().quantity;

      for (auto it = entry->materials.begin(); it != entry->materials.end(); ++it) {
        ITEM_INFO info;
        info.SetItemId(it->id);
        info.quantity = it->quantity;
        const ITEM_INFO stock = LookupClientItem(g_session.m_inventoryItems, it->id, 1);
        info.unknown_0000[1] = stock.unknown_0000[1];
        info.identified = it->quantity <= stock.quantity;
        this->m_materials.push_back(info);
      }

      if (entry->results.size() < 2) {
        this->m_resultAmount = entry->results.front().amount;
        this->m_resultText = "";
      } else {
        int total = 0;
        for (const MaterialResult& result : entry->results) {
          total += result.amount;
        }
        this->m_resultAmount = total > 1000 ? total : 1000;
        const MaterialResult& first = entry->results.front();
        const MaterialResult& last = entry->results.back();
        const std::string name = ItemDisplayName(first.id);
        const std::string text = FormatOwnedString(LookupMessage(0xe8b), name.c_str(), first.quantity, last.quantity);
        this->m_resultText = text;
      }

      this->m_button->SetDisabled(entry->state != 1);
      this->m_button->Invalidate();
      this->Invalidate();
    }
    break;
  default:
    break;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x005E4170
// FUNCTION: fn_03896
int UIChangeMaterialWnd_List::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 0x17:
    this->RefreshVisibleEntries();
    break;
  case 0x12:
    this->ShiftSelectedIndex(1);
    break;
  case 0x13:
    this->sub_5E1080(1);
    break;
  case 3:
    this->ShiftSelectedIndex(this->m_pageSize);
    break;
  case 4:
    this->sub_5E1080(this->m_pageSize);
    break;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scrollOffset;
    int delta = static_cast<int>(argument1);
    if (message == 9) {
      delta = -1;
    } else if (message == 10) {
      delta = 1;
    }
    this->m_scrollOffset = previous + delta;
    if (previous != this->m_scrollOffset) {
      this->sub_5E2A70();
    }
    return this->m_scrollOffset - previous;
  }
  case 6:
    if (argument1 == 0x141 && argument2 != 0) {
      std::string filter(reinterpret_cast<const char*>(argument2));
      if (filter.compare(LookupMessage(0xec0)) == 0) {
        filter = "";
      }
      this->ApplyFilter(filter);
    }
    break;
  default:
    break;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x005E4330
// FUNCTION: fn_03897
void UIChangeMaterialWnd_List::SortItems(bool ascending) {
  std::sort(this->m_items.begin(), this->m_items.end(), MaterialNameLess(this->m_entries));

  if (!ascending) {
    std::reverse(this->m_items.begin(), this->m_items.end());
  }

  this->Invalidate();
}

// OFFSET: 0x005E44C0
// FUNCTION: fn_03898
void UIChangeMaterialWnd_List::ShiftSelectedIndex(int value) {
  const int index = static_cast<int>(std::max<int64_t>(0, this->m_selectedIndex - value));
  if (index < this->m_scrollOffset || this->m_scrollOffset + this->m_pageSize <= index) {
    int newOffset = this->m_previousScrollOffset;
    if (index < newOffset || newOffset + this->m_pageSize <= index) {
      newOffset = index > 0 ? index : 0;
    }

    this->m_scrollOffset = newOffset;
    this->sub_5E2A70();
  }

  this->sub_5E3B80(index);
}

// OFFSET: 0x005E1C00
// FUNCTION: fn_03878
void UIChangeMaterialWnd::Paint() {

  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ChangeMaterial\\bg_ChangeMaterial.bmp")));
  this->DrawBitmap(0, 0, background ? &background->image : nullptr, true);

  const std::string mixName = FormatOwnedString("유저인터페이스\\ChangeMaterial\\bg_mix_%d.bmp", static_cast<int>(this->m_materials.size()));
  CBitmapRes* mix = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(mixName.c_str())));
  this->DrawBitmap(0x50, 0x1E, mix ? &mix->image : nullptr, true);

  int slot = 0;
  for (auto it = this->m_materials.begin(); it != this->m_materials.end(); ++it, ++slot) {
    if (it->identified == 1) {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), this->m_materialPos[slot].x, this->m_materialPos[slot].y, *it, 1, 0);
    } else {
      DrawItemWithQuantityAuxiliary(*this, 1, this->m_materialPos[slot].x, this->m_materialPos[slot].y, *it, 1, 0);
    }
  }

  DrawItemWithQuantity(*this, ResourceMgr::Instance(), this->m_resultX, this->m_resultY, this->m_resultItem, 1, 0);

  const std::string amount = FormatOwnedString(LookupMessage(0xE8C), this->m_resultAmount / 10);
  const int width = this->TextWidth(amount.c_str(), 0, 0x1D, 0x17, 1, 0);
  DrawOutlinedText(*this, 0x4F + (0xC8 - width) / 2, 0x64, amount.c_str(), 0, 0x5082DC, 0x643C00, 0x1D, 0x17, 1, 0);

  if (!this->m_resultText.empty()) {
    this->DrawTextAt(0x50, 0x103, this->m_resultText.c_str(), 0, 0, 0xC, 0x1E00E6, 0, 0);
  }
}

// OFFSET: 0x005E1F20
// FUNCTION: fn_03880
void UIChangeMaterialWnd_List::Paint() {
  this->ClearSurface(0xFFFF00FF);

  for (int row = 0; row < this->m_pageSize; ++row) {
    const int index = row + this->m_scrollOffset;
    const MaterialListEntry* entry = nullptr;
    if (static_cast<unsigned int>(index) < this->m_items.size()) {
      const unsigned int entryIndex = static_cast<unsigned int>(this->m_items[index]);
      if (entryIndex < this->m_entries.size()) {
        entry = &this->m_entries[entryIndex];
      }
    }

    const int rowHeight = this->unknown_00B4;
    if (!entry) {
      CRes* disabled = ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ChangeMaterial\\btn_material_disable.bmp"));
      this->DrawBitmap(0, rowHeight * row, reinterpret_cast<const BitmapImage*>(disabled), true);
      continue;
    }

    const char* bitmap;
    if (index == this->m_selectedIndex) {
      bitmap = "유저인터페이스\\ChangeMaterial\\btn_material_press.bmp";
    } else if (index == this->m_hoverIndex) {
      bitmap = "유저인터페이스\\ChangeMaterial\\btn_material_over.bmp";
    } else {
      bitmap = "유저인터페이스\\ChangeMaterial\\btn_material_out.bmp";
    }

    CRes* background = ResourceMgr::Instance().Load(Resolve(bitmap));
    this->DrawBitmap(0, rowHeight * row, reinterpret_cast<const BitmapImage*>(background), true);

    ITEM_INFO item;
    item.SetItemId(entry->results[0].id);
    item.quantity = entry->results[0].quantity;
    item.identified = 1;

    std::string name = item.DisplayName(0);
    if (this->TextWidth(name.c_str(), 0, 0, 12, 0, 0) > 100) {
      name = this->FitDisplayText(100, name.c_str(), 0, 12, 0);
    }

    const int y = rowHeight * row;
    if (entry->state == 1) {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), 0, y, item, 1, 0);
      this->DrawTextAt(30, y + 6, name.c_str(), 0, 0, 12, 0, 0, 0);
      this->DrawRightAlignedText(163, y + 6, FormatOwnedString(LookupMessage(182), item.quantity).c_str(), 0, 0, 12, 0, 0);
    } else {
      DrawItemWithQuantityAuxiliary(*this, 1, 0, y, item, 1, 0);
      this->DrawTextAt(30, y + 6, name.c_str(), 0, 0, 12, 0xAAAAAA, 0, 0);
      this->DrawRightAlignedText(163, y + 6, FormatOwnedString(LookupMessage(182), item.quantity).c_str(), 0, 0, 12, 0xAAAAAA, 0);
    }
  }
}

// OFFSET: 0x005E2370
// FUNCTION: fn_03881
void UIChangeMaterialWnd_ScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    CBitmapRes* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ChangeMaterial\\scroll_arrow_up_br.bmp")));
    CBitmapRes* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ChangeMaterial\\scroll_arrow_down_br.bmp")));
    CBitmapRes* track = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ChangeMaterial\\scroll_bg_br.bmp")));

    this->DrawBitmap(0, 0, up ? &up->image : nullptr, true);

    for (std::int64_t index = 0; index < (this->geometry.height - up->image.height - down->image.height) / track->image.height; ++index) {
      const int y = track->image.height * static_cast<int>(index) + up->image.height;
      this->DrawBitmap(0, y, track ? &track->image : nullptr, true);
    }

    this->DrawBitmap(0, this->geometry.height - track->image.height - down->image.height, track ? &track->image : nullptr, true);
    this->DrawBitmap(0, this->geometry.height - down->image.height, down ? &down->image : nullptr, true);

    CBitmapRes* thumb = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\ChangeMaterial\\scroll_btn_br.bmp")));
    const int available = this->geometry.height - thumb->image.height - down->image.height - up->image.height;
    int diff = this->total - this->visible;
    int maxVal = 0;
    if (diff > 0) {
      maxVal = diff;
    }
    const int y = static_cast<int>(
      static_cast<double>(available) / static_cast<double>(maxVal) * static_cast<double>(this->position) + static_cast<double>(up->image.height)
    );
    this->DrawBitmap(0, y, thumb ? &thumb->image : nullptr, true);
  }

  this->minimumThumb = 0;
  this->parent->Invalidate();
}

// OFFSET: 0x005E26B0
// FUNCTION: fn_03885
void UIChangeMaterialWnd_List::Slot28(int x, int y) {
  const int rowHeight = this->unknown_00B4;
  const int index = y / rowHeight + this->m_scrollOffset;
  if (static_cast<unsigned int>(index) >= this->m_items.size()) {
    return;
  }

  this->m_hoverIndex = index;
  this->Invalidate();

  const MaterialListEntry* entry = this->GetEntry(index);
  if (!entry) {
    return;
  }

  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);

  const std::string text = FormatOwnedString(LookupMessage(0xB7), ItemDisplayName(entry->results.front().id).c_str(), entry->results.front().quantity);
  CMode* const mode = g_modeMgr.gameMode;
  mode->ShowTooltip(text.c_str(), screenX + 5, screenY - 5, -1, 0, 1);
}

// INLINED
static __forceinline void SetButtonText(UIBmpTextButton* button, std::string value) {
  button->text = value;
  button->Invalidate();
}

// INLINED
__forceinline void UIChangeMaterialWnd_CheckBox::SetLabels(std::string ascending, std::string descending) {
  this->ascendingText = ascending;
  this->descendingText = descending;
  this->Invalidate();
}

// OFFSET: 0x005E1320
// FUNCTION: fn_03876
void UIChangeMaterialWnd::Slot15(int width, int height) {
  UIBitmapButton* close = new UIBitmapButton();
  close->SetBitmap("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0, 0);
  close->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 1, 0);
  close->SetBitmap("유저인터페이스\\basic_interface\\sys_close_on.bmp", 2, 0);
  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(480, 16);
  close->SetId(201);
  this->AddChild(close);

  this->unknown_00B8 = new UIEditCtrl();
  this->unknown_00B8->SetSize(118, 13);
  this->unknown_00B8->Move(313, 262);
  this->unknown_00B8->maximum = 20;
  this->unknown_00B8->preserveInput = 1;
  this->unknown_00B8->SetText(LookupMessage(3776));
  this->unknown_00B8->TextColor(0xB1B1B1, 0xFFFFFFFFu);
  this->unknown_00B8->Background(255, 236, 207);
  this->unknown_00B8->topPadding = -1;
  this->AddChild(this->unknown_00B8);

  UIBmpTextButton* search = new UIBmpTextButton();
  search->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_search.bmp", 0, 0);
  search->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_search_over.bmp", 1, 0);
  search->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_search_press.bmp", 2, 0);
  search->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_search_disable.bmp", 3, 0);
  search->SetSize(search->BitmapWidth(), search->BitmapHeight());
  SetButtonText(search, LookupMessage(3777));
  search->Move(432, 259);
  search->SetId(321);
  this->AddChild(search);

  UIBitmapButton* refresh = new UIBitmapButton();
  refresh->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_refresh.bmp", 0, 0);
  refresh->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_refresh_over.bmp", 1, 0);
  refresh->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_refresh_press.bmp", 2, 0);
  refresh->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_refresh_disable.bmp", 3, 0);
  refresh->SetSize(refresh->BitmapWidth(), refresh->BitmapHeight());
  refresh->Move(476, 259);
  refresh->SetId(415);
  this->AddChild(refresh);

  this->m_sortBox = new UIChangeMaterialWnd_CheckBox();
  this->m_sortBox->SetSize(74, 14);
  this->m_sortBox->SetBitmaps("유저인터페이스\\ChangeMaterial\\btn_sort_up.bmp", "유저인터페이스\\ChangeMaterial\\btn_sort_down.bmp");
  this->m_sortBox->SetLabels(LookupMessage(3722), LookupMessage(3775));
  this->m_sortBox->Move(316, 24);
  this->m_sortBox->SetId(364);
  this->m_sortBox->SetChecked(1);
  this->AddChild(this->m_sortBox);

  this->m_button = new UIBitmapButton();
  this->m_button->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_change_start.bmp", 0, 0);
  this->m_button->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_change_over.bmp", 1, 0);
  this->m_button->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_change_press.bmp", 2, 0);
  this->m_button->SetBitmap("유저인터페이스\\ChangeMaterial\\btn_change_disable.bmp", 3, 0);
  this->m_button->SetSize(this->m_button->BitmapWidth(), this->m_button->BitmapHeight());
  this->m_button->Move(140, 171);
  this->m_button->SetId(184);
  this->AddChild(this->m_button);

  UIBasicButton* cancel = new UIBasicButton();
  cancel->Configure(42, LookupMessage(3045));
  cancel->Move(width - cancel->geometry.width - 55, height - cancel->geometry.height - 12);
  cancel->SetId(201);
  this->AddChild(cancel);

  this->unknown_00B4 = new UIChangeMaterialWnd_List();
  this->unknown_00B4->SetSize(186, 216);
  this->unknown_00B4->Move(310, 41);
  this->AddChild(this->unknown_00B4);
  this->unknown_00B4->SendMsg(nullptr, 23, 0, 0, 0, 0);
}

// OFFSET: 0x005E1E20
// FUNCTION: fn_03879
void UIChangeMaterialWnd_CheckBox::Paint() {
  if (this->m_checked.empty() || this->m_unchecked.empty()) {
    return;
  }

  if (this->m_disabled == 1) {
    this->ClearSurface(0xFFFF00FFu);
    return;
  }

  if (this->m_hidden == 1) {
    return;
  }

  if (this->geometry.unknown_0030 == 0) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_unchecked.c_str())));
    this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
    if (!this->ascendingText.empty()) {
      this->DrawTextAt(0xc, 1, this->ascendingText.c_str(), 0, 0, 0xc, 0, 0, 0);
    }
  } else if (this->geometry.unknown_0030 == 1) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_checked.c_str())));
    this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
    if (!this->descendingText.empty()) {
      this->DrawTextAt(0xc, 1, this->descendingText.c_str(), 0, 0, 0xc, 0, 0, 0);
    }
  }
}

// OFFSET: 0x005E0A20
// FUNCTION: fn_03866
bool MaterialNameLess::operator()(unsigned int first, unsigned int second) const {
  const std::string left = first < this->entries.size() ? ItemDisplayName(this->entries[first].results[0].id) : std::string();
  const std::string right = second < this->entries.size() ? ItemDisplayName(this->entries[second].results[0].id) : std::string();
  return left < right;
}

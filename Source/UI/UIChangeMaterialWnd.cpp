#include "UI/UIChangeMaterialWnd.h"

#include "Client/Mode/ModeTooltip.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/StackAlloc.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <malloc.h>
#include <string>

// OFFSET: 0x005E1080
// FUNCTION: fn_03871
void UIChangeMaterialWnd_List::sub_5E1080(int delta) {
  int64_t newIndex = this->m_selectedIndex + delta;
  const int count = static_cast<int>(this->m_items.size());
  const int maxIndex = count - 1;

  if (newIndex >= static_cast<uint32_t>(maxIndex)) {
    newIndex = maxIndex;
  }

  int targetIndex = static_cast<int>(newIndex);
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
    const MaterialSlotPos& pos = this->m_materialPos[i];
    if (pos.x <= x && x <= pos.x + 24 && pos.y <= y && y <= pos.y + 24) {
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
  const int rowHeight = *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0xB4);
  unsigned int index = static_cast<unsigned int>(position / rowHeight + this->m_scrollOffset);
  const unsigned int last = static_cast<unsigned int>(this->m_items.size() - 1);
  if (last < index) {
    index = last;
  }
  this->sub_5E3B80(static_cast<int>(index));
}

// OFFSET: 0x005E25D0
// FUNCTION: fn_03884
void UIChangeMaterialWnd::Slot28(int x, int y) {
  ITEM_INFO* item = this->GetItemAt(x, y);
  if (!item) {
    return;
  }

  this->ToScreen(x, y);
  const int itemId = std::atoi(item->itemId.c_str());
  const std::string name = ItemDisplayName(itemId);
  (*reinterpret_cast<ModeTooltip**>(0x013495BC))->Show(name.c_str(), x + 5, y - 5, -1, 0, 1);
}

// OFFSET: 0x005E2840
// FUNCTION: fn_03886
void UIChangeMaterialWnd_List::Slot27(int x, int y) {
}

// OFFSET: 0x005E2850
// FUNCTION: fn_03887
void UIChangeMaterialWnd::UpdateSearchPlaceholder() {
  auto* edit = *reinterpret_cast<UIEditCtrl**>(reinterpret_cast<char*>(this) + 0xB8);
  const std::string text = edit->Text();
  const char* placeholder = LookupMessage(0xec0);
  if (text != placeholder) {
    if (text.empty()) {
      edit->SetText(placeholder);
      edit->TextColor(0xb1b1b1u, 0xffffffffu);
    } else {
      edit->TextColor(0, 0xffffffffu);
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
  this->SendMsg(this->m_scrollBar, argument < 0 ? 10 : 9, 0, 0, 0, 0);
}

// OFFSET: 0x005E2A70
// FUNCTION: fn_03890
void UIChangeMaterialWnd_List::sub_5E2A70() {
  const int count = static_cast<int>(this->m_items.size());
  if (count > this->m_pageSize) {
    this->m_scrollBar->Show(1);
    this->m_scrollBar->SetRange(count - this->m_pageSize, this->m_pageSize, count);

    const int offset = this->m_scrollOffset > 0 ? this->m_scrollOffset : 0;
    this->m_scrollOffset = offset;
    const int maximum = count - this->m_pageSize;
    const int position = offset < maximum ? offset : maximum;
    this->m_scrollOffset = position;
    this->m_scrollBar->SetPosition(position);
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
void UIChangeMaterialWnd_List::ApplyFilter(const std::string& text) {
  const int rowHeight = *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0xB4);
  this->m_pageSize = this->geometry.height / rowHeight;
  this->m_selectedIndex = 0;
  this->m_scrollOffset = 0;
  this->m_previousScrollOffset = 0;
  this->m_scrollBar->Show(0);
  this->m_items.clear();

  std::string filter = text;
  for (std::size_t i = 0; i < filter.size(); ++i) {
    filter[i] = static_cast<char>(tolower(static_cast<unsigned char>(filter[i])));
  }

  for (int index = 0; index < static_cast<int>(this->m_entries.size()); ++index) {
    MaterialListItemRef* item = this->m_entries[index].item;
    std::string name = ItemDisplayName(item->id);
    for (std::size_t i = 0; i < name.size(); ++i) {
      name[i] = static_cast<char>(tolower(static_cast<unsigned char>(name[i])));
    }

    if (filter.empty() || name.find(filter) != std::string::npos) {
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
  for (const ITEM_INFO& material : this->m_materials) {
    *reinterpret_cast<std::uint16_t*>(entry) = static_cast<std::uint16_t>(material.unknown_0000[1]);
    *reinterpret_cast<std::uint16_t*>(entry + 2) = static_cast<std::uint16_t>(material.quantity);
    entry += 4;
  }

  GetRagConnection().Send(static_cast<int>(*reinterpret_cast<const std::int16_t*>(packet + 2)), packet);
  this->m_button->SetDisabled(1);
  this->m_button->Invalidate();
  _freea(packet);
}

// OFFSET: 0x005E3B80
// FUNCTION: fn_03894
void UIChangeMaterialWnd_List::sub_5E3B80(int index) {
  this->m_selectedIndex = index;
  this->m_previousScrollOffset = this->m_scrollOffset;
  this->Invalidate();

  const MaterialListEntry* entry = nullptr;
  if (static_cast<std::uint32_t>(index) < this->m_items.size()) {
    const int entryIndex = this->m_items[index];
    if (static_cast<std::uint32_t>(entryIndex) < this->m_entries.size()) {
      entry = &this->m_entries[entryIndex];
    }
  }

  this->parent->SendMsg(nullptr, 23, reinterpret_cast<std::intptr_t>(entry), 0, 0, 0);
}

// OFFSET: 0x005E4170
// FUNCTION: fn_03896
int UIChangeMaterialWnd_List::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 3:
    this->ShiftSelectedIndex(this->m_pageSize);
    break;
  case 4:
    this->sub_5E1080(this->m_pageSize);
    break;
  case 6:
    if (argument1 == 0x141 && argument2 != 0) {
      std::string filter(reinterpret_cast<const char*>(argument2));
      if (filter == LookupMessage(0xec0)) {
        filter.clear();
      }
      this->ApplyFilter(filter);
    }
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
  case 0x12:
    this->ShiftSelectedIndex(1);
    break;
  case 0x13:
    this->sub_5E1080(1);
    break;
  case 0x17:
    this->RefreshVisibleEntries();
    break;
  default:
    break;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x005E4330
// FUNCTION: fn_03897
void UIChangeMaterialWnd_List::SortItems(bool ascending) {
  std::sort(this->m_items.begin(), this->m_items.end(), [this](int left, int right) {
    const std::string leftName = ItemDisplayName(this->m_entries[left].item->id);
    const std::string rightName = ItemDisplayName(this->m_entries[right].item->id);
    return leftName < rightName;
  });

  if (!ascending) {
    std::reverse(this->m_items.begin(), this->m_items.end());
  }

  this->Invalidate();
}

// OFFSET: 0x005E44C0
// FUNCTION: fn_03898
void UIChangeMaterialWnd_List::ShiftSelectedIndex(int value) {
  int64_t target = this->m_selectedIndex - value;
  if (target < 0) {
    target = 0;
  }

  const int index = static_cast<int>(target);
  if (index < this->m_scrollOffset || index >= this->m_scrollOffset + this->m_pageSize) {
    int newOffset = this->m_previousScrollOffset;
    if (index < newOffset || index >= newOffset + this->m_pageSize) {
      newOffset = index > 0 ? index : 0;
    }

    this->m_scrollOffset = newOffset;
    this->sub_5E2A70();
  }

  this->sub_5E3B80(index);
}

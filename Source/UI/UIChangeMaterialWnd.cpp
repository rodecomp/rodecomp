#include "UI/UIChangeMaterialWnd.h"

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

// OFFSET: 0x005E2840
// FUNCTION: fn_03886
void UIChangeMaterialWnd_List::Slot27(int x, int y) {
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

// OFFSET: 0x005E3B80

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

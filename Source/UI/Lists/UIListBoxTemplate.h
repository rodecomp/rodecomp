#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXTEMPLATE_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXTEMPLATE_H

#include "UI/Lists/UIVScrollBar.h"
#include "UI/Window/UIControl.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <list>
#include <vector>

class CUIListItem : public CUIControl {
public:
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  virtual void SetSelected(bool selected);
  virtual void SetHighlighted(bool highlighted);
  virtual void SetFrozen(bool frozen);

  /* 0084 */ unsigned char unknown_0084[0x04];
  /* 0088 */ int selected;
  /* 008C */ unsigned char unknown_008C[0x04];
  /* 0090 */ unsigned char frozen;
  /* 0094 */ int index;
};

namespace AdventureGuide {
class CUISubTab : public CUIListItem {};
class CUITab : public CUIListItem {};
class CUIItemInfo : public CUIListItem {};
class CUIMonsterInfo : public CUIListItem {};
class CUIQuestInfo : public CUIListItem {};
class CUITabInfo : public CUIListItem {};
class CItemData {};
class CMonsterData {};
class CQuestInfo {};
}

namespace Guide_Intro {
struct Contents {};
}

template <class T> class CUIListBox : public CUIControl {
public:
  // OFFSET: 0x009D3FD0
  // OFFSET: 0x009D4040
  // FUNCTION: fn_14037
  // FUNCTION: fn_14038
  void Resize(int width, int height) override {
    UIWindow::Resize(width, height);
    this->scrollBar->Resize(this->scrollBar->geometry.width, height);
    int total = 0;
    for (T* item : this->items) {
      total += item->geometry.height;
    }
    this->scrollBar->SetTotal(total);
    this->scrollBar->SetVisible(this->geometry.height);
    this->scrollBar->SetPosition(this->scrollBar->position);
  }

  /* 0084 */ unsigned char unknown_0084[0x2C];
  /* 00B0 */ CUIVScrollBar* scrollBar;
  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ std::list<T*> items;
};

template <class T, class D> class CUIListBoxEx : public CUIControl {
public:
  // OFFSET: 0x009D43D0
  // OFFSET: 0x009D45D0
  // OFFSET: 0x009D47D0
  // OFFSET: 0x009D49D0
  // FUNCTION: fn_14043
  // FUNCTION: fn_14044
  // FUNCTION: fn_14045
  // FUNCTION: fn_14046
  void OnScroll(int position) {
    auto it = this->items.begin();
    if (it == this->items.end()) {
      return;
    }
    if (this->data.size() == 0) {
      for (; it != this->items.end(); ++it) {
        (*it)->index = -1;
        (*it)->Show(0);
      }
      return;
    }

    const int first = (*it)->index;
    unsigned int row = position / this->rowHeight;
    if (first != -1) {
      if (first < static_cast<int>(row)) {
        if (row - first < static_cast<unsigned int>(this->items.size())) {
          std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
        }
      } else if (first > static_cast<int>(row)) {
        if (first - row < static_cast<unsigned int>(this->items.size())) {
          std::rotate(this->items.begin(), this->items.end() - (first - row), this->items.end());
        }
      }
    }

    it = this->items.begin();
    for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
      if (it == this->items.end() || row >= this->data.size()) {
        break;
      }
      T* item = *it;
      if (item) {
        if (this->rebind || row != static_cast<unsigned int>(item->index)) {
          item->index = row;
          if (this->binder) {
            auto node = this->data.begin();
            std::advance(node, static_cast<int>(row));
            this->binder(item, *node);
          }
        }
        item->SetSelected(row == this->selected);
        item->Move(this->itemX, y);
        item->Show(1);
      }
    }

    for (; it != this->items.end(); ++it) {
      (*it)->index = -1;
      (*it)->Show(0);
    }
  }

  /* 0084 */ unsigned char unknown_0084[0x2C];
  /* 00B0 */ CUIVScrollBar* scrollBar;
  /* 00B4 */ int itemWidth;
  /* 00B8 */ int rowHeight;
  /* 00BC */ int itemX;
  /* 00C0 */ int itemY;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ std::vector<T*> items;
  /* 00D4 */ std::list<D> data;
  /* 00DC */ unsigned int selected;
  /* 00E0 */ unsigned char unknown_00E0;
  /* 00E1 */ unsigned char rebind;
  /* 00E2 */ unsigned char unknown_00E2[0x2E];
  /* 0110 */ std::function<void(T*, const D&)> binder;
};

#endif

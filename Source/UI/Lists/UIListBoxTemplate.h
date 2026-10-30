#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXTEMPLATE_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXTEMPLATE_H

#include "UI/Lists/UIVScrollBar.h"
#include "UI/Window/UIControl.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <vector>

class CUIListItem : public CUIControl {
public:
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot47() override;
  virtual void SetSelected(bool selected);
  virtual void SetHighlighted(bool highlighted);
  virtual void SetFrozen(bool frozen);

  /* 0084 */ unsigned char unknown_0084[0x04];
  /* 0088 */ int selected;
  /* 008C */ unsigned char unknown_008C[0x04];
  /* 0090 */ unsigned char frozen;
  /* 0094 */ int index;
  /* 0098 */ std::function<void(CUIListItem*)> onHover;
  /* 00C0 */ std::function<void(CUIListItem*)> onPress;
};

class CUIStaticText;

namespace AdventureGuide {
class CUIPage;

class CUISubTab : public CUIListItem {
public:
  void Slot16() override;

  /* 00E8 */ unsigned char unknown_00E8[0x04];
  /* 00EC */ UIWindow* unknown_00EC;
  /* 00F0 */ int id;
  /* 00F4 */ unsigned int unknown_00F4;
};

class CUITab : public CUIListItem {
public:
  void Slot16() override;

  void OnSelectSubTab(CUISubTab* subTab);

  /* 00E8 */ unsigned char unknown_00E8[0x08];
  /* 00F0 */ std::string unknown_00F0;
  /* 0108 */ int type;
  /* 010C */ CUIPage* page;
};

class CUIItemInfo : public CUIListItem {
public:
  void Slot16() override;

  /* 00E8 */ unsigned char unknown_00E8[0x08];
  /* 00F0 */ CUIStaticText* unknown_00F0;
  /* 00F4 */ CUIStaticText* unknown_00F4;
  /* 00F8 */ CUIStaticText* unknown_00F8;
  /* 00FC */ unsigned char unknown_00FC[0x0C];
  /* 0108 */ std::string unknown_0108;
};

class CUIMonsterInfo : public CUIListItem {
public:
  void Slot16() override;

  /* 00E8 */ unsigned char unknown_00E8[0x04];
  /* 00EC */ CUIStaticText* unknown_00EC;
  /* 00F0 */ CUIStaticText* unknown_00F0;
};

class CUIQuestInfo : public CUIListItem {
public:
  void Slot16() override;

  /* 00E8 */ unsigned char unknown_00E8[0x04];
  /* 00EC */ CUIStaticText* unknown_00EC;
  /* 00F0 */ CUIStaticText* unknown_00F0;
  /* 00F4 */ CUIStaticText* unknown_00F4;
  /* 00F8 */ std::string unknown_00F8;
};

class CUITabInfo : public CUIListItem {
public:
  void Slot16() override;
};
class CItemData {};
class CMonsterData {
public:
  /* 0000 */ std::string name;
  /* 0018 */ std::vector<std::string> values;
};
class CQuestInfo {};
}

namespace Guide_Intro {
struct Contents {
  Contents(const std::string& first, const std::string& second) : first(first), second(second) {
  }

  /* 0000 */ std::string first;
  /* 0018 */ std::string second;
};
}

template <class T> class CUIListBox : public CUIControl {
public:
  // OFFSET: 0x009D23B0
  // OFFSET: 0x009D23C0
  // FUNCTION: fn_14011
  // FUNCTION: fn_14012
  void Paint() override {
    this->ClearSurface(0xFFFF00FF);
  }

  // OFFSET: 0x009D2410
  // OFFSET: 0x009D2450
  // FUNCTION: fn_14017
  // FUNCTION: fn_14018
  void Slot35(int value) override {
    if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
      this->scrollBar->Slot35(value);
    } else if (this->parent) {
      this->parent->Slot35(value);
    }
  }

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

  // OFFSET: 0x009DD9A0
  // OFFSET: 0x009DD9F0
  // FUNCTION: fn_14345
  // FUNCTION: fn_14346
  void UpdateScrollBar() {
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
  explicit CUIListBoxEx(UIWindowContext& context);
  ~CUIListBoxEx() override;
  void Slot15(int width, int height) override;
  // OFFSET: 0x00A01E70
  // OFFSET: 0x00A01E80
  // OFFSET: 0x00A0CE60
  // OFFSET: 0x00A63890
  // FUNCTION: fn_14832
  // FUNCTION: fn_14833
  // FUNCTION: fn_15037
  // FUNCTION: fn_16437
  void Paint() override {
    this->ClearSurface(0xFFFF00FF);
  }

  // OFFSET: 0x00A01E90
  // OFFSET: 0x00A01ED0
  // OFFSET: 0x00A0CE70
  // OFFSET: 0x00A638A0
  // FUNCTION: fn_14834
  // FUNCTION: fn_14835
  // FUNCTION: fn_15038
  // FUNCTION: fn_16438
  void Slot35(int value) override {
    if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
      this->scrollBar->Slot35(value);
    } else if (this->parent) {
      this->parent->Slot35(value);
    }
  }

  // OFFSET: 0x009F9890
  // OFFSET: 0x009F98F0
  // OFFSET: 0x00A08DE0
  // FUNCTION: fn_14810
  // FUNCTION: fn_14811
  // FUNCTION: fn_15032
  void ClearItems() {
    for (T* item : this->items) {
      this->RemoveChild(item, 1);
    }
    this->items.clear();
    this->selected = static_cast<unsigned int>(-1);
  }

  void SelectItem(CUIListItem* item);

  // OFFSET: 0x00A04B30
  // FUNCTION: fn_14963
  int AddData(const D& value, bool refresh) {
    this->data.push_back(value);
    if (refresh) {
      this->UpdateScrollBar();
    }
    return this->data.size() - 1;
  }

  // OFFSET: 0x00A051E0
  // OFFSET: 0x00A662D0
  // FUNCTION: fn_14971
  // FUNCTION: fn_16614
  void ClearData() {
    this->data.clear();
    this->selected = static_cast<unsigned int>(-1);
    this->UpdateScrollBar();
  }

  // OFFSET: 0x00A15450
  // FUNCTION: fn_15196
  void SetOnSelect(std::function<void(T*, const D&)> handler) {
    this->onSelect = handler;
  }

  // OFFSET: 0x00A154C0
  // FUNCTION: fn_15197
  void SetBinder(std::function<void(T*, const D&)> handler) {
    this->binder = handler;
  }

  // OFFSET: 0x00A15AA0
  // FUNCTION: fn_15201
  void SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& disabled) {
    this->imageNames[0] = normal;
    this->imageNames[1] = hover;
    this->imageNames[2] = pressed;
    this->disabledName = disabled;
    this->Slot53(0);
    this->Invalidate();
  }

  // OFFSET: 0x009DD950
  // OFFSET: 0x00A06300
  // FUNCTION: fn_14344
  // FUNCTION: fn_15001
  void UpdateScrollBar() {
    this->scrollBar->SetTotal(this->data.size() * this->rowHeight + this->itemY);
    this->scrollBar->SetVisible(this->geometry.height);
    this->scrollBar->SetPosition(this->scrollBar->position);
    this->rebind = 1;
  }

  // OFFSET: 0x009D43D0
  // OFFSET: 0x009D45D0
  // OFFSET: 0x009D47D0
  // OFFSET: 0x009D49D0
  // OFFSET: 0x00A027C0
  // OFFSET: 0x00A029C0
  // FUNCTION: fn_14043
  // FUNCTION: fn_14044
  // FUNCTION: fn_14045
  // FUNCTION: fn_14046
  // FUNCTION: fn_14843
  // FUNCTION: fn_14844
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

  /* 0084 */ CResPtr<CBitmapRes> unknown_0084;
  /* 0088 */ CResPtr<CBitmapRes> unknown_0088;
  /* 008C */ CResPtr<CBitmapRes> unknown_008C;
  /* 0090 */ std::map<int, std::string> imageNames;
  /* 0098 */ std::string disabledName;
  /* 00B0 */ CUIVScrollBar* scrollBar;
  /* 00B4 */ int itemWidth;
  /* 00B8 */ int rowHeight;
  /* 00BC */ int itemX;
  /* 00C0 */ int itemY;
  /* 00C4 */ unsigned char unknown_00C4;
  /* 00C5 */ unsigned char unknown_00C5[0x03];
  /* 00C8 */ std::vector<T*> items;
  /* 00D4 */ std::list<D> data;
  /* 00DC */ unsigned int selected;
  /* 00E0 */ unsigned char unknown_00E0;
  /* 00E1 */ unsigned char rebind;
  /* 00E2 */ unsigned char unknown_00E2[0x06];
  /* 00E8 */ std::function<void(T*, const D&)> onSelect;
  /* 0110 */ std::function<void(T*, const D&)> binder;
};

#endif

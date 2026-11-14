#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXTEMPLATE_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXTEMPLATE_H

#include "UI/Lists/UIVScrollBar.h"
#include "UI/Window/UIControl.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <vector>

class CUIListItem : public CUIControl {
public:
  __declspec(noinline) CUIListItem();
  ~CUIListItem() override;

  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnMouseEnter() override;
  void Paint() override;
  virtual void SetSelected(bool selected);
  virtual void SetHighlighted(bool highlighted);
  virtual void SetFrozen(bool frozen);
  __declspec(noinline) void SetImageResources(CBitmapRes* normal, CBitmapRes* hover, CBitmapRes* pressed, CBitmapRes* disabled);

  /* 0080 */ std::map<int, CBitmapRes*> images;
  /* 0088 */ int selected;
  /* 008C */ CBitmapRes* disabledImage;
  /* 0090 */ unsigned char frozen;
  /* 0094 */ int index;
  /* 0098 */ std::function<void(CUIListItem*)> onHover;
  /* 00C0 */ std::function<void(CUIListItem*)> onPress;
};

class CUIStaticText;
class CUITextViewer;
class CSprRes;
class CActRes;

template <class T> class CUIListBox;

namespace AdventureGuide {
class CUIPage;

class CUISubTab : public CUIListItem {
public:
  CUISubTab();
  void OnCreate(int width, int height) override;
  void OnInit() override;

  /* 00E8 */ CUIStaticText* labelText;
  /* 00EC */ CUIStaticImage* stampImage;
  /* 00F0 */ int id;
  /* 00F4 */ unsigned int questId;
};

class CUITab : public CUIListItem {
public:
  CUITab();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void SetSelected(bool selected) override;

  void OnSelectSubTab(CUISubTab* subTab);
  CUIListBox<CUISubTab>* CreateSubTabList();

  /* 00E8 */ CUIStaticText* labelText;
  /* 00EC */ CUIListBox<CUISubTab>* subTabs;
  /* 00F0 */ std::string title;
  /* 0108 */ int type;
  /* 010C */ CUIPage* page;
  /* 0110 */ int collapsedHeight;
};

class CUIItemInfo : public CUIListItem {
public:
  CUIItemInfo();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void OnRBtnDown(int x, int y) override;

  void SetDescription(int type, const std::vector<const char*>& description);
  void SetStatusLines(bool first, bool second, bool third);

  union {
    /* 00E8 */ unsigned char headerControls[0x08];
    struct {
      /* 00E8 */ CUIStaticImage* image;
      /* 00EC */ CUIStaticText* nameText;
    };
  };
  /* 00F0 */ CUITextViewer* descriptionViewer;
  /* 00F4 */ CUITextViewer* optionsViewer;
  /* 00F8 */ CUITextViewer* sourcesViewer;
  /* 00FC */ int itemId;
  /* 0100 */ int type;
  /* 0104 */ int location;
  /* 0108 */ std::string description;
};

class CUIMonsterInfo : public CUIListItem {
public:
  CUIMonsterInfo();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void SetMonster(const std::string& title);

  /* 00E8 */ CUIStaticText* nameText;
  /* 00EC */ CUITextViewer* detailsViewer;
  /* 00F0 */ CUITextViewer* valuesViewer;
  /* 00F4 */ CSprRes* sprite;
  /* 00F8 */ CActRes* action;
};

class CUIQuestInfo : public CUIListItem {
public:
  CUIQuestInfo();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void SetTitle(int id);
  void SetTip(int id);
  void SetViewerLines(const std::vector<std::string>& lines);

  /* 00E8 */ CUIStaticImage* stampImage;
  /* 00EC */ CUITextViewer* titleViewer;
  /* 00F0 */ CUITextViewer* linesViewer;
  /* 00F4 */ CUITextViewer* statusViewer;
  /* 00F8 */ std::string title;
  /* 0110 */ int questId;
};

class CUITabInfo : public CUIListItem {
public:
  CUITabInfo();
  void OnCreate(int width, int height) override;
  void OnInit() override;

  /* 00E8 */ CUIStaticText* titleText;
  /* 00EC */ CUIStaticText* descriptionText;
};
class CItemData {
public:
  /* 0000 */ int id;
  /* 0004 */ std::uint8_t first;
  /* 0005 */ std::uint8_t second;
  /* 0006 */ std::uint8_t third;
  /* 0008 */ int categoryId;
  /* 000C */ int categoryIndex;
  /* 0010 */ std::string text;
  /* 0028 */ std::vector<std::string> values;
  /* 0034 */ int type;
};

class CMonsterData {
public:
  /* 0000 */ std::string name;
  /* 0018 */ std::vector<std::string> values;
};
class CQuestInfo {
public:
  /* 0000 */ int id;
  /* 0004 */ std::uint8_t categoryType;
  /* 0008 */ int categoryId;
  /* 000C */ int categoryIndex;
  /* 0010 */ int questId;
  /* 0014 */ std::string title;
  /* 002C */ std::string text;
  /* 0044 */ std::vector<std::string> rewards;
  /* 0050 */ int type;
};
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
  // INLINED
  explicit CUIListBox()
      : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), selected(nullptr), locked(0) {
  }
  ~CUIListBox() override;

  void OnCreate(int width, int height) override;
  // OFFSET: 0x009D23B0
  // FUNCTION: fn_14011
  // OFFSET: 0x009D23C0
  // FUNCTION: fn_14012
  void Paint() override {
    this->ClearSurface(0xFFFF00FF);
  }

  // OFFSET: 0x009D2410
  // FUNCTION: fn_14017
  // OFFSET: 0x009D2450
  // FUNCTION: fn_14018
  void OnWheel(int value) override {
    if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
      this->scrollBar->OnWheel(value);
    } else if (this->parent) {
      this->parent->OnWheel(value);
    }
  }

  // OFFSET: 0x009D3FD0
  // FUNCTION: fn_14037
  // OFFSET: 0x009D4040
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
  // FUNCTION: fn_14345
  // OFFSET: 0x009DD9F0
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

  void LoadSkin(bool resolveSkin) override;

  void SelectItem(CUIListItem* item);
  T* CreateItem();

  // INLINED
  void SetOnSelect(std::function<void(T*)> handler) {
    this->onSelect = handler;
  }

  // INLINED
  void SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& disabled) {
    this->imageNames[0] = normal;
    this->imageNames[1] = hover;
    this->imageNames[2] = pressed;
    this->disabledName = disabled;
    this->LoadSkin(0);
    this->Invalidate();
  }

  // INLINED
  void SetScrollBarButtonImages(const std::string& up, const std::string& down) {
    this->scrollBar->SetButtonImages(up, down);
  }

  // INLINED
  void SetScrollBarThumbImages(const std::string& top, const std::string& middle, const std::string& bottom) {
    this->scrollBar->SetThumbImages(top, middle, bottom);
  }

  // INLINED
  void SetScrollBarThumbImage(const std::string& name) {
    this->scrollBar->SetThumbImage(name);
  }

  // INLINED
  void SetScrollBarBackground(const std::string& name) {
    this->scrollBar->SetBackground(name);
  }

  CResPtr<CBitmapRes>& NormalImage() {
    return this->normalImage;
  }

  /* 0080 */ CResPtr<CBitmapRes> normalImage;
  /* 0084 */ CResPtr<CBitmapRes> hoverImage;
  /* 0088 */ CResPtr<CBitmapRes> pressedImage;
  /* 008C */ CResPtr<CBitmapRes> disabledImage;
  /* 0090 */ std::map<int, std::string> imageNames;
  /* 0098 */ std::string disabledName;
  /* 00B0 */ CUIVScrollBar* scrollBar;
  /* 00B4 */ int itemWidth;
  /* 00B8 */ int rowHeight;
  /* 00BC */ int itemX;
  /* 00C0 */ int itemY;
  /* 00C4 */ std::list<T*> items;
  /* 00CC */ T* selected;
  /* 00D0 */ unsigned char locked;
  /* 00D1 */ unsigned char padding_00D1[0x07];
  /* 00D8 */ std::function<void(T*)> onSelect;
};

template <class T, class D> class CUIListBoxEx : public CUIControl {
public:
  CUIListBoxEx();
  ~CUIListBoxEx() override;
  void OnCreate(int width, int height) override;
  // OFFSET: 0x00A01E70
  // FUNCTION: fn_14832
  // OFFSET: 0x00A01E80
  // FUNCTION: fn_14833
  // OFFSET: 0x00A0CE60
  // FUNCTION: fn_15037
  // OFFSET: 0x00A63890
  // FUNCTION: fn_16437
  void Paint() override {
    this->ClearSurface(0xFFFF00FF);
  }

  // OFFSET: 0x00A01E90
  // FUNCTION: fn_14834
  // OFFSET: 0x00A01ED0
  // FUNCTION: fn_14835
  // OFFSET: 0x00A0CE70
  // FUNCTION: fn_15038
  // OFFSET: 0x00A638A0
  // FUNCTION: fn_16438
  void OnWheel(int value) override {
    if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
      this->scrollBar->OnWheel(value);
    } else if (this->parent) {
      this->parent->OnWheel(value);
    }
  }

  // OFFSET: 0x009F9890
  // FUNCTION: fn_14810
  // OFFSET: 0x009F98F0
  // FUNCTION: fn_14811
  // OFFSET: 0x00A08DE0
  // FUNCTION: fn_15032
  void ClearItems() {
    for (T* item : this->items) {
      this->RemoveChild(item, 1);
    }
    this->items.clear();
    this->selected = static_cast<unsigned int>(-1);
  }
  bool RebindIndex(int index);
  void SelectItem(CUIListItem* item);
  bool SelectIndex(int index);
  void AddNewItem() {
  }
  __declspec(noinline) T* CreateItem();
  void Resize(int width, int height) override {
    UIWindow::Resize(width, height);
  }
  void LoadSkin(bool resolveSkin) override;

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
  // FUNCTION: fn_14971
  // OFFSET: 0x00A662D0
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
    this->LoadSkin(0);
    this->Invalidate();
  }

  // OFFSET: 0x009DD950
  // FUNCTION: fn_14344
  // OFFSET: 0x00A06300
  // FUNCTION: fn_15001
  // OFFSET: 0x00A4E360
  // FUNCTION: fn_16205
  void UpdateScrollBar() {
    this->scrollBar->SetTotal(this->data.size() * this->rowHeight + this->itemY);
    this->scrollBar->SetVisible(this->geometry.height);
    this->scrollBar->SetPosition(this->scrollBar->position);
    this->rebind = 1;
  }

  // OFFSET: 0x009D43D0
  // FUNCTION: fn_14043
  // OFFSET: 0x009D45D0
  // FUNCTION: fn_14044
  // OFFSET: 0x009D47D0
  // FUNCTION: fn_14045
  // OFFSET: 0x009D49D0
  // FUNCTION: fn_14046
  // OFFSET: 0x00A027C0
  // FUNCTION: fn_14843
  // OFFSET: 0x00A029C0
  // FUNCTION: fn_14844
  void OnScroll(int position) {
    if (this->items.empty()) {
      return;
    }
    if (this->data.size() == 0) {
      for (T* item : this->items) {
        item->index = -1;
        item->Show(0);
      }
      return;
    }

    const int first = this->items.front()->index;
    unsigned int row = position / this->rowHeight;
    if (first != -1) {
      if (first < static_cast<int>(row)) {
        if (row - first < static_cast<unsigned int>(this->items.size())) {
          std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
        }
      } else if (first > static_cast<int>(row)) {
        const unsigned int distance = first - row;
        if (distance < static_cast<unsigned int>(this->items.size())) {
          std::rotate(this->items.begin(), this->items.end() - distance, this->items.end());
        }
      }
    }

    auto it = this->items.begin();
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

    for (const auto end = this->items.end(); it != end; ++it) {
      T* item = *it;
      item->index = -1;
      item->Show(0);
    }
  }

  // INLINED
  void SetScrollBarButtonImages(const std::string& up, const std::string& down) {
    this->scrollBar->SetButtonImages(up, down);
  }

  // INLINED
  void SetScrollBarThumbImages(const std::string& top, const std::string& middle, const std::string& bottom) {
    this->scrollBar->SetThumbImages(top, middle, bottom);
  }

  // INLINED
  void SetScrollBarThumbImage(const std::string& name) {
    this->scrollBar->SetThumbImage(name);
  }

  // INLINED
  void SetScrollBarBackground(const std::string& name) {
    this->scrollBar->SetBackground(name);
  }

  CResPtr<CBitmapRes>& NormalImage() {
    return this->normalImage;
  }

  /* 0080 */ CResPtr<CBitmapRes> normalImage;
  /* 0084 */ CResPtr<CBitmapRes> hoverImage;
  /* 0088 */ CResPtr<CBitmapRes> pressedImage;
  /* 008C */ CResPtr<CBitmapRes> disabledImage;
  /* 0090 */ std::map<int, std::string> imageNames;
  /* 0098 */ std::string disabledName;
  /* 00B0 */ CUIVScrollBar* scrollBar;
  /* 00B4 */ int itemWidth;
  /* 00B8 */ int rowHeight;
  /* 00BC */ int itemX;
  /* 00C0 */ int itemY;
  /* 00C4 */ unsigned char layoutFlag;
  /* 00C5 */ unsigned char padding_00C5[0x03];
  /* 00C8 */ std::vector<T*> items;
  /* 00D4 */ std::list<D> data;
  /* 00DC */ unsigned int selected;
  /* 00E0 */ unsigned char selectionFlag;
  /* 00E1 */ unsigned char rebind;
  /* 00E2 */ unsigned char padding_00E2[0x06];
  /* 00E8 */ std::function<void(T*, const D&)> onSelect;
  /* 0110 */ std::function<void(T*, const D&)> binder;
};

#endif

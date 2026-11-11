#include "Client/Mode/CMode.h"
#include "Client/Session/Session.h"
#include "UI/Window/UIControl.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Lists/UIThumb.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Mode/ModeMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIPadding.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Lists/UIVScrollBar.h"
#include "Render/Text/FontCache.h"
#include "Script/LuaScript.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Client/Input/InputCharset.h"
#include "Render/Text/TextContext.h"
#include "File/File.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Utility/Text/Base64.h"
#include "Utility/Threading/NSyncObject.h"
#include "Render/Renderer.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Window/FrameSkin.h"
#include "UI/Text/CreateLineInfo.h"

#include <algorithm>
#include <cmath>
#include <new>
#include <format>
#include <windows.h>

// GLOBAL: gl_25174
const char* g_uiInfoTempPath = "savedata\\UIInfo.temp";

// GLOBAL: gl_25442
const char* g_uiInfoPath = "savedata\\UIInfo.lua";

// GLOBAL: gl_25443
const char* g_uiInfoScriptPath = "Lua Files\\OptionInfo\\UIInfo_F";

// INLINED
template <class Signature>
static __forceinline void AssignHandler(std::function<Signature>& target, const std::function<Signature>& source) {
  std::function<Signature>(source).swap(target);
}

class CUIListBoxItemSelector {
public:
  void SetHighlighted(CUIListItem* const& item);
  void SetHighlightedEntry(CUIListItem* const& item);

private:
  /* 0000 */ void* padding_0000;
  /* 0004 */ CUIListBox<CUIListItem>* listBox;
};

// OFFSET: 0x009E0A80
// FUNCTION: fn_14371
void CUIListBoxItemSelector::SetHighlighted(CUIListItem* const& item) {
  CUIListItem* const target = item;
  for (CUIListItem* entry : this->listBox->items) {
    entry->SetHighlighted(target == entry);
  }
}

// OFFSET: 0x009E0AD0
// FUNCTION: fn_14372
void CUIListBoxItemSelector::SetHighlightedEntry(CUIListItem* const& item) {
  CUIListItem* const target = item;
  for (CUIListItem* entry : this->listBox->items) {
    entry->SetHighlighted(target == entry);
  }
}

template <class T, class D> class CUIListBoxExItemSelector {
public:
  void Select(CUIListItem* const& item);

private:
  /* 0000 */ void* padding_0000;
  /* 0004 */ CUIListBoxEx<T, D>* listBox;
};

// OFFSET: 0x009D6610
// FUNCTION: fn_14131
template <>
__declspec(noinline) void CUIListBoxExItemSelector<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::Select(CUIListItem* const& item) {
  CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>* listBox = this->listBox;
  CUIListItem* selection = item;
  if (listBox->layoutFlag) {
    return;
  }
  for (AdventureGuide::CUIItemInfo* other : listBox->items) {
    if (selection != other) {
      other->SetSelected(0);
    }
  }
  selection->SetSelected(1);
  if (selection->index == static_cast<int>(listBox->selected) && listBox->selectionFlag) {
    return;
  }
  AdventureGuide::CUIItemInfo* selectedItem = dynamic_cast<AdventureGuide::CUIItemInfo*>(selection);
  if (!selectedItem) {
    return;
  }
  listBox->selected = selectedItem->index;
  if (listBox->onSelect && static_cast<int>(listBox->selected) >= 0 && listBox->selected < listBox->data.size()) {
    auto node = listBox->data.begin();
    for (int count = static_cast<int>(listBox->selected); count > 0; --count) {
      ++node;
    }
    listBox->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x009D66F0
// FUNCTION: fn_14132
template <>
__declspec(noinline) void CUIListBoxExItemSelector<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::Select(CUIListItem* const& item) {
  CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>* listBox = this->listBox;
  CUIListItem* selection = item;
  if (listBox->layoutFlag) {
    return;
  }
  for (AdventureGuide::CUIMonsterInfo* other : listBox->items) {
    if (selection != other) {
      other->SetSelected(0);
    }
  }
  selection->SetSelected(1);
  if (selection->index == static_cast<int>(listBox->selected) && listBox->selectionFlag) {
    return;
  }
  AdventureGuide::CUIMonsterInfo* selectedItem = dynamic_cast<AdventureGuide::CUIMonsterInfo*>(selection);
  if (!selectedItem) {
    return;
  }
  listBox->selected = selectedItem->index;
  if (listBox->onSelect && static_cast<int>(listBox->selected) >= 0 && listBox->selected < listBox->data.size()) {
    auto node = listBox->data.begin();
    for (int count = static_cast<int>(listBox->selected); count > 0; --count) {
      ++node;
    }
    listBox->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x009D67D0
// FUNCTION: fn_14133
template <>
__declspec(noinline) void CUIListBoxExItemSelector<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::Select(CUIListItem* const& item) {
  CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>* listBox = this->listBox;
  CUIListItem* selection = item;
  if (listBox->layoutFlag) {
    return;
  }
  for (AdventureGuide::CUIQuestInfo* other : listBox->items) {
    if (selection != other) {
      other->SetSelected(0);
    }
  }
  selection->SetSelected(1);
  if (selection->index == static_cast<int>(listBox->selected) && listBox->selectionFlag) {
    return;
  }
  AdventureGuide::CUIQuestInfo* selectedItem = dynamic_cast<AdventureGuide::CUIQuestInfo*>(selection);
  if (!selectedItem) {
    return;
  }
  listBox->selected = selectedItem->index;
  if (listBox->onSelect && static_cast<int>(listBox->selected) >= 0 && listBox->selected < listBox->data.size()) {
    auto node = listBox->data.begin();
    for (int count = static_cast<int>(listBox->selected); count > 0; --count) {
      ++node;
    }
    listBox->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x009D68B0
// FUNCTION: fn_14134
template <>
__declspec(noinline) void CUIListBoxExItemSelector<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::Select(CUIListItem* const& item) {
  CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>* listBox = this->listBox;
  CUIListItem* selection = item;
  if (listBox->layoutFlag) {
    return;
  }
  for (AdventureGuide::CUITabInfo* other : listBox->items) {
    if (selection != other) {
      other->SetSelected(0);
    }
  }
  selection->SetSelected(1);
  if (selection->index == static_cast<int>(listBox->selected) && listBox->selectionFlag) {
    return;
  }
  AdventureGuide::CUITabInfo* selectedItem = dynamic_cast<AdventureGuide::CUITabInfo*>(selection);
  if (!selectedItem) {
    return;
  }
  listBox->selected = selectedItem->index;
  if (listBox->onSelect && static_cast<int>(listBox->selected) >= 0 && listBox->selected < listBox->data.size()) {
    auto node = listBox->data.begin();
    for (int count = static_cast<int>(listBox->selected); count > 0; --count) {
      ++node;
    }
    listBox->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x009EF520
// FUNCTION: fn_14635
CBitmapRes* CUIControl::LoadBitmapResource(const std::string& name, bool resolveSkin) {
  if (name.size() == 0) {
    return nullptr;
  }

  ResourceMgr& resources = ResourceMgr::Instance();

  if (resolveSkin) {
    return static_cast<CBitmapRes*>(resources.Load(Resolve(name.c_str())));
  }

  return static_cast<CBitmapRes*>(resources.Load(name));
}

// INLINED
__forceinline CUIButton::CUIButton() : CUIControl(), state(0), disabled(0), tooltip(), clickWindow(nullptr), hoverWindow(nullptr) {
}

// OFFSET: 0x009BDF40
// FUNCTION: fn_13944
__forceinline CUIImageButton::CUIImageButton() : CUIButton(), images(), disabledImage(), transparent(0), imageNames(), disabledName() {
}

CUIControl::~CUIControl() = default;

// OFFSET: 0x009D5490
// FUNCTION: fn_14047
bool CUIControl::CanHitTest() {
  return this->hitEnabled;
}

// OFFSET: 0x009EEAD0
// FUNCTION: fn_14634
void CUIControl::DragAndDrop(int x, int y, std::intptr_t value) {
  if (this->parent) {
    this->parent->DragAndDrop(x + this->geometry.x, y + this->geometry.y, value);
  }
}

// OFFSET: 0x009EF680
// FUNCTION: fn_14639
void CUIControl::OnLBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDown(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF650
// FUNCTION: fn_14638
void CUIControl::OnLBtnDblClk(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDblClk(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF710
// FUNCTION: fn_14642
void CUIControl::OnMouseMove(int x, int y) {
  if (this->parent) {
    this->parent->OnMouseMove(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF6E0
// FUNCTION: fn_14641
void CUIControl::OnMouseHover(int x, int y) {
  if (this->parent) {
    this->parent->OnMouseHover(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF740
// FUNCTION: fn_14643
void CUIControl::ResetCursor(int x, int y) {
  if (this->parent) {
    this->parent->ResetCursor(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF6B0
// FUNCTION: fn_14640
void CUIControl::OnLBtnUp(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnUp(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF7A0
// FUNCTION: fn_14645
void CUIControl::OnRBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnRBtnDown(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF7D0
// FUNCTION: fn_14646
void CUIControl::OnRBtnUp(int x, int y) {
  if (this->parent) {
    this->parent->OnRBtnUp(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF770
// FUNCTION: fn_14644
void CUIControl::OnRBtnDblClk(int x, int y) {
  if (this->parent) {
    this->parent->OnRBtnDblClk(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF800
// FUNCTION: fn_14647
void CUIControl::OnWheel(int value) {
  if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x009EF5F0
// FUNCTION: fn_14637
bool CUIControl::IsPointInside(int x, int y) {
  RECT rect = {this->geometry.x, this->geometry.y, this->geometry.x + this->geometry.width, this->geometry.y + this->geometry.height};
  POINT point = {x, y};
  return PtInRect(&rect, point) != FALSE;
}

// OFFSET: 0x009EF5E0
// FUNCTION: fn_14636
void CUIControl::Invalidate() {
  this->Root()->InvalidateChildren();
}

// OFFSET: 0x009EF820
// FUNCTION: fn_14648
void CUIControl::ApplySkin(bool value) {
  this->LoadSkin(value);

  for (UIWindow* child : this->m_children) {
    child->ApplySkin(value);
  }
}

// OFFSET: 0x009D3FC0
// FUNCTION: fn_14036
void CUIControl::LoadSkin(bool) {
}

// OFFSET: 0x009C0220
// FUNCTION: fn_13965
CUIVScrollBar::~CUIVScrollBar() {
}

// OFFSET: 0x009F2B70
// FUNCTION: fn_14726
void CUIVScrollBar::SetTotal(int value) {
  this->total = std::max(1, value);
  this->visible = std::min(this->visible, this->total);
  this->maximum = this->total - this->visible;
  this->Invalidate();
}

// OFFSET: 0x009D3120
// FUNCTION: fn_14028
void CUIVScrollBar::SetOnChange(std::function<void(int)> handler) {
  AssignHandler(this->onChange, handler);
}

// OFFSET: 0x009F2E80
// FUNCTION: fn_14732
void CUIVScrollBar::SetVisible(int value) {
  this->visible = std::min(value, this->total);
  this->maximum = this->total - this->visible;
  this->Invalidate();
}

// OFFSET: 0x009F2530
// FUNCTION: fn_14716
void CUIVScrollBar::SetPosition(int value) {
  int zero = 0;
  const int* clamped = &this->maximum;
  if (this->maximum >= value) {
    clamped = value < 0 ? &zero : &value;
  }
  if (!this->dragging) {
    this->position = *clamped;
  }
  this->Show(this->total > this->visible);
  this->UpdateThumb();
  if (this->onChange) {
    this->onChange(this->position);
  }
  this->Invalidate();
}

// OFFSET: 0x009F23B0
// FUNCTION: fn_14709
void CUIVScrollBar::OnLBtnDown(int, int y) {
  const int top = this->thumb->geometry.y;
  if (y < top) {
    this->SetPosition(this->position - this->step);
  } else if (y > this->thumb->geometry.height + top) {
    this->SetPosition(this->position + this->step);
  }
}

// OFFSET: 0x009F0F00
// FUNCTION: fn_14684
void CUITab::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009F2400
// FUNCTION: fn_14710
void CUIVThumb::OnLBtnDown(int, int y) {
  g_windowMgr.SetCapture(this);
  this->dragOrigin = y;
}

// OFFSET: 0x009F2420
// FUNCTION: fn_14711
void CUIVThumb::OnLBtnUp(int, int y) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
  this->dragOrigin = y;
}

// OFFSET: 0x009F24B0
// FUNCTION: fn_14713
void CUIVThumb::OnMouseMove(int, int y) {
  if (this == g_windowMgr.Capture() && y != this->dragOrigin && this->onDrag) {
    this->onDrag(y - this->dragOrigin);
  }
}

// OFFSET: 0x009ED970
// FUNCTION: fn_14592
CUIThumb::~CUIThumb() = default;

// OFFSET: 0x009EE080
// FUNCTION: fn_14598
void CUIHThumb::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (this->resizable) {
    const int end = this->geometry.width - (this->bottom ? this->bottom->image.width : 0);
    int x = 0;
    if (this->top) {
      if (this->transparent) {
        this->DrawAuxiliary(1, 0, 0, &this->top->image, true, false);
      } else {
        this->DrawBitmap(0, 0, this->top, false);
      }
      x = this->top->image.width;
    }

    if (this->middle) {
      while (x < end) {
        if (this->transparent) {
          this->DrawAuxiliary(1, x, 0, &this->middle->image, true, false);
        } else {
          this->DrawBitmap(x, 0, this->middle, false);
        }
        x += this->middle->image.width;
      }
    }

    if (this->bottom) {
      const int bottomX = this->geometry.width - this->bottom->image.width;
      if (this->transparent) {
        this->DrawAuxiliary(1, bottomX, 0, &this->bottom->image, true, false);
      } else {
        this->DrawBitmap(bottomX, 0, this->bottom, false);
      }
    }
  } else if (this->middle) {
    if (this->transparent) {
      this->DrawAuxiliary(1, 0, 0, &this->middle->image, true, false);
    } else {
      this->DrawBitmap(0, 0, this->middle, false);
    }
  }
}

// OFFSET: 0x009EE1E0
// FUNCTION: fn_14600
void CUIHThumb::OnLBtnDown(int x, int) {
  g_windowMgr.SetCapture(this);
  this->dragOrigin = x;
}

// OFFSET: 0x009EE200
// FUNCTION: fn_14601
void CUIHThumb::OnLBtnUp(int x, int) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
  this->dragOrigin = x;
}

// OFFSET: 0x009EE230
// FUNCTION: fn_14602
void CUIHThumb::OnMouseMove(int x, int) {
  if (this == g_windowMgr.Capture() && x != this->dragOrigin && this->onDrag) {
    this->onDrag(x - this->dragOrigin);
  }
}

// OFFSET: 0x009F24F0
// FUNCTION: fn_14714
void CUIThumb::ResetCursor(int, int) {
  g_modeMgr.gameMode->SetCursorAction(2);
}

// OFFSET: 0x009F2450
// FUNCTION: fn_14712
void CUIThumb::OnMouseHover(int, int) {
  if (!this->tooltip.empty()) {
    int screenX = 0;
    int screenY = -20;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(this->tooltip.c_str(), screenX, screenY, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x009F2500
// FUNCTION: fn_14715
void CUIVScrollBar::OnWheel(int value) {
  this->SetPosition(value > 0 ? this->position - this->step : this->step + this->position);
}

// OFFSET: 0x009F2A90
// FUNCTION: fn_14724
void CUIThumb::SetImages(const std::string& top, const std::string& middle, const std::string& bottom) {
  this->resizable = 1;
  this->topName = top;
  this->middleName = middle;
  this->bottomName = bottom;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F2B20
// FUNCTION: fn_14725
void CUIThumb::SetImage(const std::string& name) {
  this->resizable = 0;
  this->middleName = name;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F2BC0
// FUNCTION: fn_14727
void CUIVScrollBar::Show(int visible) {
  this->geometry.visible = this->hidden ? 0 : visible;
}

// OFFSET: 0x009F25C0
// FUNCTION: fn_14717
void CUIVScrollBar::UpdateThumb() {
  CUIThumb* bar = this->thumb;
  float offset;
  if (bar->resizable) {
    const int length = static_cast<int>((static_cast<float>(this->visible) / static_cast<float>(this->total)) * static_cast<float>(this->trackLength));
    bar->Resize(this->geometry.width, length > 1 ? length : 1);
    offset = (static_cast<float>(this->trackLength) / static_cast<float>(this->total)) * static_cast<float>(this->position);
  } else {
    if (this->total - this->visible <= 0) {
      bar->Move(0, this->upButton->geometry.height);
      return;
    }
    offset = static_cast<float>(this->position * (this->trackLength - bar->geometry.height)) / static_cast<float>(this->total - this->visible);
  }
  this->thumb->Move(0, static_cast<int>(offset + static_cast<float>(this->upButton->geometry.height)));
}

// OFFSET: 0x009F2180
// FUNCTION: fn_14707
void CUIVScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (!this->background) {
    return;
  }

  if (this->transparent) {
    for (int y = 0; y < this->trackLength - this->background->image.height; y += this->background->image.height) {
      this->DrawAuxiliary(1, 0, this->upButton->geometry.height + y, &this->background->image, true, false);
    }
    this->DrawAuxiliary(
      1, 0, this->geometry.height - this->downButton->geometry.height - this->background->image.height, &this->background->image, true, false
    );
    return;
  }

  for (int y = 0; y < this->trackLength - this->background->image.height; y += this->background->image.height) {
    this->DrawBitmap(0, this->upButton->geometry.height + y, this->background, false);
  }
  this->DrawBitmap(0, this->geometry.height - this->downButton->geometry.height - this->background->image.height, this->background, false);
}

// OFFSET: 0x009F2800
// FUNCTION: fn_14719
void CUIVScrollBar::LoadSkin(bool resolveSkin) {
  this->background = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->backgroundName, resolveSkin));
}

// OFFSET: 0x009F28A0
// FUNCTION: fn_14720
void CUIVScrollBar::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->downButton->Move(0, this->geometry.height - this->downButton->geometry.height);
  this->trackLength = this->geometry.height - this->upButton->geometry.height - this->downButton->geometry.height;
}

// OFFSET: 0x009F28F0
// FUNCTION: fn_14721
void CUIVScrollBar::SetButtonImages(const std::string& up, const std::string& down) {
  CBitmapRes* image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(up));
  if (image) {
    this->upButton->Resize(image->image.width, image->image.height);
    this->upButton->Move(0, 0);
    this->upButton->SetImages(up, up, up, up);
    this->upButton->Show(1);
  }

  image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(down));
  if (image) {
    this->downButton->Resize(image->image.width, image->image.height);
    this->downButton->Move(0, this->geometry.height - image->image.height);
    this->downButton->SetImages(down, down, down, down);
    this->downButton->Show(1);
  }

  this->Invalidate();
}

// OFFSET: 0x009F2A50
// FUNCTION: fn_14723
void CUIVScrollBar::SetTransparent(bool transparent) {
  this->transparent = transparent;
  this->upButton->transparent = transparent;
  this->downButton->transparent = transparent;
  this->thumb->transparent = transparent;
}

// OFFSET: 0x009F2BE0
// FUNCTION: fn_14728
void CUIVScrollBar::SetThumbImages(const std::string& top, const std::string& middle, const std::string& bottom) {
  this->trackLength = this->geometry.height - this->downButton->geometry.height - this->upButton->geometry.height;
  const int length = static_cast<int>((static_cast<float>(this->visible) / static_cast<float>(this->total)) * static_cast<float>(this->trackLength));
  this->thumb->Resize(this->geometry.width, length > 1 ? length : 1);
  this->thumb->Move(0, this->upButton->geometry.height);
  this->thumb->Show(1);

  CUIThumb* bar = this->thumb;
  bar->resizable = 1;
  bar->topName = top;
  bar->middleName = middle;
  bar->bottomName = bottom;
  bar->LoadSkin(0);
  bar->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x009F2D00
// FUNCTION: fn_14729
void CUIVScrollBar::SetThumbImage(const std::string& name) {
  this->trackLength = this->geometry.height - this->downButton->geometry.height - this->upButton->geometry.height;

  CBitmapRes* image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(name));
  if (image) {
    this->thumb->Resize(image->image.width, image->image.height);
    this->thumb->Move(0, this->upButton->geometry.height);
    this->thumb->Show(1);

    CUIThumb* bar = this->thumb;
    bar->resizable = 0;
    bar->middleName = name;
    bar->LoadSkin(0);
    bar->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x009F2E10
// FUNCTION: fn_14730
void CUIVScrollBar::SetThumbTooltip(const std::string& text) {
  this->thumb->tooltip = text;
}

// OFFSET: 0x009F2E40
// FUNCTION: fn_14731
void CUIVScrollBar::SetBackground(const std::string& name) {
  this->backgroundName = name;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F2280
// FUNCTION: fn_14708
void CUIVThumb::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (this->resizable) {
    int y = 0;
    const int end = this->geometry.height - (this->bottom ? this->bottom->image.height : 0);
    if (this->top) {
      if (this->transparent) {
        this->DrawAuxiliary(1, 0, 0, &this->top->image, true, false);
      } else {
        this->DrawBitmap(0, 0, &this->top->image, false);
      }
      y = this->top->image.height;
    }

    if (this->middle) {
      while (y < end) {
        if (this->transparent) {
          this->DrawAuxiliary(1, 0, y, &this->middle->image, true, false);
        } else {
          this->DrawBitmap(0, y, &this->middle->image, false);
        }
        y += this->middle->image.height;
      }
    }

    if (this->bottom) {
      const int bottomY = this->geometry.height - this->bottom->image.height;
      if (this->transparent) {
        this->DrawAuxiliary(1, 0, bottomY, &this->bottom->image, true, false);
      } else {
        this->DrawBitmap(0, bottomY, &this->bottom->image, false);
      }
    }
  } else if (this->middle) {
    if (this->transparent) {
      this->DrawAuxiliary(1, 0, 0, &this->middle->image, true, false);
    } else {
      this->DrawBitmap(0, 0, &this->middle->image, false);
    }
  }
}

#pragma inline_depth(0)
// OFFSET: 0x009F1650
// FUNCTION: fn_14692
void CUITabButton::OnMouseEnter() {
  if (this->state != 1) {
    this->SetState(2);
  }
}

// OFFSET: 0x009F1670
// FUNCTION: fn_14693
void CUITabButton::OnMouseLeave() {
  if (this->state != 1) {
    this->SetState(0);
  }
}
#pragma inline_depth()

// OFFSET: 0x009EE8F0
// FUNCTION: fn_14626
void CUIButton::SetState(int value) {
  if (this->state != value) {
    this->state = value;
    if (!this->disabled) {
      this->Invalidate();
    }
  }
}

// OFFSET: 0x009D3050
// FUNCTION: fn_14027
void CUIButton::SetOnClick(std::function<void()> handler) {
  std::function<void()>& onClick = *reinterpret_cast<std::function<void()>*>(this->clickHandlerStorage);
  onClick = handler;
}

// OFFSET: 0x009EE920
// FUNCTION: fn_14627
void CUIButton::OnLBtnDblClk(int x, int y) {
  this->OnLBtnDown(x, y);
}

// OFFSET: 0x009EE930
// FUNCTION: fn_14628
void CUIButton::OnLBtnDown(int, int) {
  this->SetState(1);
  if (!this->disabled) {
    g_windowMgr.SetCapture(this);
  }
}

// OFFSET: 0x009EE970
// FUNCTION: fn_14629
void CUIButton::OnLBtnUp(int, int) {
  this->SetState(2);
  if (this == g_windowMgr.Capture()) {
    if (!this->disabled && this->clickWindow) {
      this->clickWindow->IsFrameWnd();
    }
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x009EE9D0
// FUNCTION: fn_14630
void CUIButton::OnMouseEnter() {
  this->SetState(2);
}

// OFFSET: 0x009EEA00
// FUNCTION: fn_14631
void CUIButton::OnMouseHover(int, int) {
  if (this->hoverWindow) {
    this->hoverWindow->IsFrameWnd();
  }

  if (this->tooltip.empty()) {
    return;
  }

  int screenX = 0;
  int screenY = -20;

  this->ToScreen(screenX, screenY);

  g_modeMgr.gameMode->ShowTooltip(this->tooltip.c_str(), screenX, screenY, 0xFFFFFFFFu, 0, 1);
}

// OFFSET: 0x009EEA70
// FUNCTION: fn_14632
void CUIButton::OnMouseLeave() {
  this->SetState(0);
}

// OFFSET: 0x009EEAA0
// FUNCTION: fn_14633
void CUIButton::ResetCursor(int, int) {
  if (this->disabled) {
    g_modeMgr.gameMode->SetCursorAction(0);
  } else {
    g_modeMgr.gameMode->SetCursorAction(2);
  }
}

// OFFSET: 0x009ECCD0
// FUNCTION: fn_14563
void CUITextButton::DrawVerticalLabel() {
  if (this->disabled) {
    this->DrawVerticalText(this->textX, this->textY, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->disabledTextColor, this->bold);
    return;
  }

  if (this->state == 1) {
    if (this->shadow) {
      this->DrawVerticalText(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->shadowColor, this->bold);
    }
    if (this->outline) {
      this->DrawVerticalOutlinedText(
        this->textX, this->textY, this->displayText.c_str(), 0, this->textColors[this->state], this->outlineColor, this->fontIndex, this->fontHeight, this->bold
      );
      return;
    }
    this->DrawVerticalText(this->textX, this->textY, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->textColors[this->state], this->bold);
    return;
  }

  if (this->shadow) {
    this->DrawVerticalText(this->textX, this->textY, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->shadowColor, this->bold);
  }
  if (this->outline) {
    this->DrawVerticalOutlinedText(
      this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, this->textColors[this->state], this->outlineColor, this->fontIndex, this->fontHeight,
      this->bold
    );
    return;
  }
  this->DrawVerticalText(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->textColors[this->state], this->bold);
}

// OFFSET: 0x009ECF40
// FUNCTION: fn_14564
void CUITextButton::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawLabel();
}

// OFFSET: 0x009ECF60
// FUNCTION: fn_14565
void CUITextButton::OnMouseHover(int, int) {
  if (this->tooltipText.size() > 0) {
    int screenX = 0;
    int screenY = -20;
    this->ToScreen(screenX, screenY);
    g_modeMgr.gameMode->ShowTooltip(this->tooltipText.c_str(), screenX, screenY, 0xFFFFFFFFu, 0, 1);
    return;
  }

  int screenX = -6;
  int screenY = -4;
  this->ToScreen(screenX, screenY);
  g_modeMgr.gameMode->ShowTooltip(this->fullText.c_str(), screenX, screenY, 0xFFFFFFFFu, 0, 1);
}

// OFFSET: 0x009ED360
// FUNCTION: fn_14571
void CUITextButton::SetTextColors(std::uint32_t normal, std::uint32_t hover, std::uint32_t pressed, std::uint32_t disabled) {
  this->textColors[0] = normal;
  this->textColors[1] = hover;
  this->textColors[2] = pressed;
  this->disabledTextColor = disabled;
  this->Invalidate();
}

// OFFSET: 0x009ED430
// FUNCTION: fn_14574
void CUITextButton::SetOutline(bool enabled, std::uint32_t color) {
  this->outline = enabled;
  this->outlineColor = color;
}

// OFFSET: 0x009ED450
// FUNCTION: fn_14575
void CUITextButton::SetShadow(bool enabled, std::uint32_t color) {
  this->shadow = enabled;
  this->shadowColor = color;
}

// OFFSET: 0x009E2CE0
// FUNCTION: fn_14407
void CUIBasicButton::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->LoadSkin(0);
}

// OFFSET: 0x009E1DE0
// FUNCTION: fn_14403
void CUIBasicButton::Create(int width, int height) {
  this->defaultWidth = width;
  this->SetAlign(6, 6);
  this->Resize(width, height);
  this->OnCreate(width, height);
  this->OnInit();
}

// OFFSET: 0x009E1F90
// FUNCTION: fn_14405
void CUIBasicButton::Paint() {
  this->ClearSurface(0xFFFF00FF);
  CBitmapRes* left = this->disabled ? this->disabledLeft : this->leftImages[this->state];
  CBitmapRes* middle = this->disabled ? this->disabledMiddle : this->middleImages[this->state];
  CBitmapRes* right = this->disabled ? this->disabledRight : this->rightImages[this->state];
  const int end = this->geometry.width - (right ? right->image.width : 0);
  int x = 0;

  if (left) {
    this->DrawBitmap(0, 0, left, false);
    x = left->image.width;
  }

  if (middle) {
    for (; x < end; x += middle->image.width) {
      this->DrawBitmap(x, 0, middle, false);
    }
  }

  if (right) {
    this->DrawBitmap(this->geometry.width - right->image.width, 0, right, false);
  }

  this->DrawLabel();
}

// OFFSET: 0x009E20B0
// FUNCTION: fn_14406
void CUIBasicButton::LoadSkin(bool resolveSkin) {
  this->leftImages[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_out_left.bmp", resolveSkin));
  this->leftImages[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_press_left.bmp", resolveSkin));
  this->leftImages[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_over_left.bmp", resolveSkin));
  this->disabledLeft = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_disable_left.bmp", resolveSkin));
  this->middleImages[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_out_mid.bmp", resolveSkin));
  this->middleImages[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_press_mid.bmp", resolveSkin));
  this->middleImages[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_over_mid.bmp", resolveSkin));
  this->disabledMiddle = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_disable_mid.bmp", resolveSkin));
  this->rightImages[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_out_right.bmp", resolveSkin));
  this->rightImages[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_press_right.bmp", resolveSkin));
  this->rightImages[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_over_right.bmp", resolveSkin));
  this->disabledRight = CResPtr<CBitmapRes>(this->LoadBitmapResource("유저인터페이스\\basic_interface\\btn_disable_right.bmp", resolveSkin));
}

// OFFSET: 0x009F26B0
// FUNCTION: fn_14718
void CUIThumb::LoadSkin(bool resolveSkin) {
  this->middle = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->middleName, resolveSkin));
  if (this->resizable) {
    this->top = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->topName, resolveSkin));
    this->bottom = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->bottomName, resolveSkin));
  }
}

// OFFSET: 0x009F0ED0
// FUNCTION: fn_14683
CUITabButton* CUITab::FindButton(UIWindow* page) {
  for (UIWindow* child : this->m_children) {
    CUITabButton* button = static_cast<CUITabButton*>(child);
    if (button->page == page) {
      return button;
    }
  }

  return nullptr;
}

// OFFSET: 0x009F0F10
// FUNCTION: fn_14685
void CUITab::Select(CUITabButton* button) {
  if (this->current == button) {
    return;
  }

  if (!button) {
    this->current = nullptr;
    for (UIWindow* child : this->m_children) {
      static_cast<CUITabButton*>(child)->SetSelected(false);
    }
  } else {
    if (button->disabled) {
      return;
    }
    if (this->onChanging && !this->onChanging(this->current, button)) {
      return;
    }

    this->current = button;
    for (UIWindow* child : this->m_children) {
      if (this->current != child) {
        static_cast<CUITabButton*>(child)->SetSelected(false);
      }
    }
    this->current->SetSelected(true);
  }

  if (this->onChange) {
    this->onChange(this->current);
  }
}

// OFFSET: 0x009F0FF0
// FUNCTION: fn_14686
void CUITab::SelectPage(UIWindow* page) {
  CUITabButton* button = this->FindButton(page);
  if (this->current == button) {
    return;
  }

  if (!button) {
    this->current = nullptr;
    for (UIWindow* child : this->m_children) {
      static_cast<CUITabButton*>(child)->SetSelected(false);
    }
  } else {
    if (button->disabled) {
      return;
    }
    if (this->onChanging && !this->onChanging(this->current, button)) {
      return;
    }

    this->current = button;
    for (UIWindow* child : this->m_children) {
      if (child != this->current) {
        static_cast<CUITabButton*>(child)->SetSelected(false);
      }
    }
    this->current->SetSelected(true);
  }

  if (this->onChange) {
    this->onChange(this->current);
  }
}

// OFFSET: 0x009F10F0
// FUNCTION: fn_14687
bool CUITab::SelectIndex(unsigned int index) {
  if (index >= this->m_children.size()) {
    return false;
  }

  CUITabButton* button = static_cast<CUITabButton*>(*std::next(this->m_children.begin(), index));
  if (this->current == button) {
    return true;
  }

  if (!button) {
    this->current = nullptr;
    for (UIWindow* child : this->m_children) {
      static_cast<CUITabButton*>(child)->SetSelected(false);
    }
  } else {
    if (button->disabled) {
      return true;
    }
    if (this->onChanging && !this->onChanging(this->current, button)) {
      return true;
    }

    this->current = button;
    for (UIWindow* child : this->m_children) {
      if (child != this->current) {
        static_cast<CUITabButton*>(child)->SetSelected(false);
      }
    }
    this->current->SetSelected(true);
  }

  if (this->onChange) {
    this->onChange(this->current);
  }
  return true;
}

// OFFSET: 0x009F16F0
// FUNCTION: fn_14695
void CUITabButton::SetSelected(bool selected) {
  this->SetState(selected);
  if (this->page) {
    this->page->Show(selected);
  }
  if (selected && this->clickWindow) {
    this->clickWindow->IsFrameWnd();
  }
  this->Invalidate();
}

// OFFSET: 0x009F1740
// FUNCTION: fn_14696
void CUITabButton::SetSkinColors(int normalColumn, int normalRow, int selectedColumn, int selectedRow, int borderColumn, int borderRow) {
  this->normalCell.column = normalColumn;
  this->normalCell.row = normalRow;
  this->selectedCell.column = selectedColumn;
  this->selectedCell.row = selectedRow;
  this->borderCell.column = borderColumn;
  this->borderCell.row = borderRow;
  this->useSkinColors = 1;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F1600
// FUNCTION: fn_14690
void CUITabButton::OnLBtnDown(int, int) {
  if (!this->disabled) {
    g_windowMgr.SetCapture(this);
    if (this->parent) {
      static_cast<CUITab*>(this->parent)->Select(this);
    }
  }
}

// OFFSET: 0x009F1630
// FUNCTION: fn_14691
void CUITabButton::OnLBtnUp(int, int) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x009F1690
// FUNCTION: fn_14694
void CUITabButton::LoadSkin(bool resolveSkin) {
  this->normalColor = SkinPaletteColor(ResourceMgr::Instance(), this->normalCell, resolveSkin);
  this->selectedColor = SkinPaletteColor(ResourceMgr::Instance(), this->selectedCell, resolveSkin);
  this->borderColor = SkinPaletteColor(ResourceMgr::Instance(), this->borderCell, resolveSkin);
  this->CUIImageTextButton::LoadSkin(resolveSkin);
}

// OFFSET: 0x009ED6D0
// FUNCTION: fn_14582
void CUIEditCtrl::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->PaintText();
}

// OFFSET: 0x009ED5A0
// FUNCTION: fn_14579
void CUIEditCtrl::Invalidate() {
  UIWindow* window = this;
  while (window->parent != nullptr) {
    window = window->parent;
  }

  window->InvalidateChildren();
}

// OFFSET: 0x009F0780
// FUNCTION: fn_14665
void CUIStaticImage::Paint() {
  this->ClearSurface(0xFFFF00FF);

  CBitmapRes* res = this->image;
  if (!res) {
    return;
  }

  const bool transparent = this->transparent != 0;
  if (100 != this->scale) {
    const float factor = static_cast<float>(this->scale) / 100.0f;
    const int height = static_cast<int>(static_cast<float>(res->image.height) * factor);
    const int width = static_cast<int>(static_cast<float>(res->image.width) * factor);
    if (transparent) {
      this->DrawAuxiliaryScaled(1, 0, 0, width, height, &res->image, 1, false);
    } else {
      this->DrawBitmapScaled(0, 0, width, height, &res->image, 0);
    }
  } else if (transparent) {
    this->DrawAuxiliary(1, 0, 0, &res->image, true, false);
  } else {
    this->DrawBitmap(0, 0, res, false);
  }
}

// OFFSET: 0x009F0840
// FUNCTION: fn_14666
void CUIStaticImage::OnMouseHover(int, int) {
  int x = 0;
  int y = -8;
  this->ToScreen(x, y);
  g_modeMgr.gameMode->ShowTooltip(this->tooltip.c_str(), x, y, 0xFFFFFFFFu, 0, 1);
}

// OFFSET: 0x009F08A0
// FUNCTION: fn_14667
void CUIStaticImage::LoadSkin(bool resolveSkin) {
  this->image = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageName, resolveSkin));
}

// OFFSET: 0x009F0940
// FUNCTION: fn_14668
void CUIStaticImage::SetImage(const std::string& name) {
  this->imageName = name;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F0DA0
// FUNCTION: fn_14675
void CUIStaticText::SetColor(std::uint32_t color) {
  this->color = color;
  this->Invalidate();
}

// OFFSET: 0x009F0E10
// FUNCTION: fn_14678
void CUIStaticText::SetOutline(bool enabled, std::uint32_t color) {
  this->outline = enabled;
  this->outlineColor = color;
}

// OFFSET: 0x009F0E30
// FUNCTION: fn_14679
void CUIStaticText::SetShadow(bool enabled, std::uint32_t color) {
  this->shadow = enabled;
  this->shadowColor = color;
}

// OFFSET: 0x009F1C40
// FUNCTION: fn_14701
void CUITitleBar::SetImages(const std::string& left, const std::string& middle, const std::string& right) {
  this->resizable = 1;
  this->leftName = left;
  this->middleName = middle;
  this->rightName = right;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F1BF0
// FUNCTION: fn_14700
void CUITitleBar::Resize(int width, int height) {
  const int delta = width - this->geometry.width;
  UIWindow::Resize(width, height);
  if (this->firstButton) {
    this->firstButton->Move(this->firstButton->geometry.x + delta, this->firstButton->geometry.y);
  }
  if (this->secondButton) {
    this->secondButton->Move(this->secondButton->geometry.x + delta, this->secondButton->geometry.y);
  }
}

// OFFSET: 0x009F19E0
// FUNCTION: fn_14698
void CUITitleBar::Paint() {
  if (this->resizable) {
    int x = 0;
    const int end = this->geometry.width - (this->right ? this->right->image.width : 0);
    if (this->left) {
      this->DrawBitmap(0, 0, this->left, false);
      x = this->left->image.width;
    }

    if (this->middle) {
      while (x < end) {
        this->DrawBitmap(x, 0, this->middle, false);
        x += this->middle->image.width;
      }
    }

    if (this->right) {
      this->DrawBitmap(this->geometry.width - this->right->image.width, 0, this->right, false);
    }
  } else if (this->middle) {
    this->DrawBitmap(0, 0, this->middle, false);
  }
}

// OFFSET: 0x009F1AA0
// FUNCTION: fn_14699
void CUITitleBar::LoadSkin(bool resolveSkin) {
  this->middle = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->middleName, resolveSkin));
  if (this->resizable) {
    this->left = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->leftName, resolveSkin));
    this->right = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->rightName, resolveSkin));
  }
}

// OFFSET: 0x009F17A0
// FUNCTION: fn_14697
void CUITitleBar::SetSecondButton(CUIButton* button) {
  this->secondButton = button;
  this->AddChild(button);

  const char* behavior = nullptr;
  if (g_session.m_hotkeyScript->Call("GetBeHaviorOfWindow", "d>s", this->parent->geometry.id, &behavior) && behavior) {
    const std::string text = behavior;
    static_cast<CUIButton*>(this->secondButton)->tooltip = text;
  }
}

// OFFSET: 0x009ED680
// FUNCTION: fn_14581
void CUIEditBox::Paint() {
  if (this->borderWidth > 0) {
    this->ClearSurface(this->borderColor);
    const int border = this->borderWidth;
    this->FillRegion(border, border, this->geometry.width - border * 2, this->geometry.height - border * 2, this->backgroundColor);
  } else {
    this->ClearSurface(this->backgroundColor);
  }
}

// OFFSET: 0x009ED6F0
// FUNCTION: fn_14583
void CUIEditBox::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->edit->Resize(width + this->editRight - this->editLeft, height + this->editBottom - this->editTop);
  this->edit->Move(this->editLeft, this->editTop);
}

// OFFSET: 0x009ED750
// FUNCTION: fn_14584
int CUIEditBox::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 0 && this->onSubmit) {
    this->onSubmit();
  }
  return UIWindow::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009ED790
// FUNCTION: fn_14585
void CUIEditBox::SetBackgroundColor(std::uint32_t color) {
  this->backgroundColor = color;
  this->Invalidate();
}

// OFFSET: 0x009ED7D0
// FUNCTION: fn_14586
void CUIEditBox::SetTextColor(std::uint32_t color) {
  this->edit->TextColor(GetRValue(color), GetGValue(color), GetBValue(color), 0xFFFFFFFFu);
}

// OFFSET: 0x009ED800
// FUNCTION: fn_14587
void CUIEditBox::SetBorderColor(std::uint32_t color) {
  this->borderColor = color;
  this->Invalidate();
}

// OFFSET: 0x009ED820
// FUNCTION: fn_14588
void CUIEditBox::SetBorderWidth(int width) {
  this->borderWidth = width;
  this->Invalidate();
}

// OFFSET: 0x009ED840
// FUNCTION: fn_14589
void CUIEditBox::SetMaximum(int value) {
  this->edit->maximum = value;
  this->edit->SetText(this->edit->Text());
}

// OFFSET: 0x009ED880
// FUNCTION: fn_14590
void CUIEditBox::SetEditBounds(int left, int top, int right, int bottom) {
  this->editLeft = left;
  this->editTop = top;
  this->editRight = right;
  this->editBottom = bottom;
  this->Resize(this->geometry.width, this->geometry.height);
}

// OFFSET: 0x009ED8E0
// FUNCTION: fn_14591
void CUIEditBox::SetText(const std::string& text) {
  this->edit->SetText(text.c_str());
}

// OFFSET: 0x009EF860
// FUNCTION: fn_14649
void CUIImageButton::Paint() {
  this->ClearSurface(0xFFFF00FF);

  CBitmapRes* image;
  if (this->disabled) {
    image = this->disabledImage;
  } else {
    const auto found = this->images.find(this->state);
    if (found == this->images.end()) {
      return;
    }
    image = found->second;
  }

  if (image) {
    if (this->transparent) {
      this->DrawAuxiliary(1, 0, 0, &image->image, true, false);
      return;
    }
    this->DrawBitmap(0, 0, image, false);
  }
}

// OFFSET: 0x009EF900
// FUNCTION: fn_14650
void CUIImageButton::LoadSkin(bool resolveSkin) {
  this->images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
}

// OFFSET: 0x009EFB00
// FUNCTION: fn_14651
void CUIImageButton::SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& inactive) {
  this->imageNames[0] = normal;
  this->imageNames[1] = hover;
  this->imageNames[2] = pressed;
  this->disabledName = inactive;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009E31B0
// FUNCTION: fn_14411
void CUICheckBox::Paint() {
  std::map<int, CResPtr<CBitmapRes>>::iterator found;
  if (this->checked) {
    found = this->images.find(1);
  } else if (this->disabled) {
    if (this->disabledImage) {
      this->DrawBitmap(0, 0, this->disabledImage, false);
    }
    return;
  } else {
    found = this->images.find(this->state);
  }

  if (found != this->images.end() && found->second) {
    this->DrawBitmap(0, 0, found->second, false);
  }
}

// OFFSET: 0x009E3230
// FUNCTION: fn_14412
void CUICheckBox::OnLBtnDown(int, int) {
  if (this->disabled) {
    return;
  }

  g_windowMgr.SetCapture(this);
  this->checked = !this->checked;
  this->Invalidate();
  if (this->onCheck) {
    this->onCheck(this->checked);
  }
}

// OFFSET: 0x009E3290
// FUNCTION: fn_14413
void CUICheckBox::OnLBtnUp(int, int) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x009E8430
// FUNCTION: fn_14492
CUICheckBox::CUICheckBox() : CUIImageButton(), checked(false), onCheck() {
}

// OFFSET: 0x009E89B0
// FUNCTION: fn_14495
bool CUIRadioItem::IsChecked() {
  return this->radioButton->checked;
}

// OFFSET: 0x009E7D60
// FUNCTION: fn_14486
CUIImageTextButton::CUIImageTextButton() : CUITextButton(), images(), disabledImage(), imageNames(), disabledName() {
  this->textColors[0] = 0;
  this->textColors[1] = 0;
  this->textColors[2] = 0;
}

// OFFSET: 0x009E7EB0
// FUNCTION: fn_14487
CUIImageTextButton::~CUIImageTextButton() = default;

// OFFSET: 0x009E8150
// FUNCTION: fn_14490
void CUIImageTextButton::LoadSkin(bool resolveSkin) {
  this->images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
}

// OFFSET: 0x009E8350
// FUNCTION: fn_14491
void CUIImageTextButton::SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& inactive) {
  this->imageNames[0] = normal;
  this->imageNames[1] = hover;
  this->imageNames[2] = pressed;
  this->disabledName = inactive;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F01B0
// FUNCTION: fn_14656
void CUIListItem::OnLBtnDown(int, int) {
  if (!this->frozen) {
    g_windowMgr.SetCapture(this);
    if (this->onPress) {
      this->onPress(this);
    }
  }
}

// OFFSET: 0x009F0210
// FUNCTION: fn_14658
void CUIListItem::OnMouseEnter() {
  if (this->onHover) {
    this->onHover(this);
  }
}

// OFFSET: 0x009F01F0
// FUNCTION: fn_14657
void CUIListItem::OnLBtnUp(int, int) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x009F0230
// FUNCTION: fn_14659
void CUIListItem::OnMouseMove(int, int) {
  if (this->selected != 2) {
    this->OnMouseEnter();
  }
}

// OFFSET: 0x009F0250
// FUNCTION: fn_14660
void CUIListItem::SetHighlighted(bool highlighted) {
  if (this->selected != 1) {
    const int state = highlighted * 2;
    if (this->selected != state) {
      this->selected = state;
      if (!this->frozen) {
        this->Invalidate();
      }
    }
  }
}

// OFFSET: 0x009F0290
// FUNCTION: fn_14661
void CUIListItem::SetSelected(bool selected) {
  if (this->selected != selected) {
    this->selected = selected;
    if (!this->frozen) {
      this->Invalidate();
    }
  }
}

// OFFSET: 0x009F02C0
// FUNCTION: fn_14662
void CUIListItem::SetFrozen(bool frozen) {
  this->frozen = frozen;
  this->Invalidate();
}

// OFFSET: 0x009D23D0
// FUNCTION: fn_14013
template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009D23E0
// FUNCTION: fn_14014
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009D23F0
// FUNCTION: fn_14015
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009D2400
// FUNCTION: fn_14016
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009D2490
// FUNCTION: fn_14019
template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x009D24D0
// FUNCTION: fn_14020
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x009D2510
// FUNCTION: fn_14021
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x009D2550
// FUNCTION: fn_14022
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x009D26D0
// FUNCTION: fn_14023
template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (AdventureGuide::CUIItemInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (AdventureGuide::CUIItemInfo*& slot : this->items) {
      AdventureGuide::CUIItemInfo* item = new AdventureGuide::CUIItemInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (AdventureGuide::CUIItemInfo* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        if (this->layoutFlag) {
          return;
        }
        for (AdventureGuide::CUIItemInfo* other : this->items) {
          if (listItem != other) {
            other->SetSelected(0);
          }
        }
        listItem->SetSelected(1);
        if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
          return;
        }
        AdventureGuide::CUIItemInfo* selectedItem = dynamic_cast<AdventureGuide::CUIItemInfo*>(listItem);
        if (!selectedItem) {
          return;
        }
        this->selected = selectedItem->index;
        if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
          auto node = this->data.begin();
          for (int remaining = static_cast<int>(this->selected); remaining > 0; --remaining) {
            ++node;
          }
          this->onSelect(selectedItem, *node);
        }
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x009D2930
// FUNCTION: fn_14024
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (AdventureGuide::CUIMonsterInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (AdventureGuide::CUIMonsterInfo*& slot : this->items) {
      AdventureGuide::CUIMonsterInfo* item = new AdventureGuide::CUIMonsterInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (AdventureGuide::CUIMonsterInfo* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        if (this->layoutFlag) {
          return;
        }
        for (AdventureGuide::CUIMonsterInfo* other : this->items) {
          if (listItem != other) {
            other->SetSelected(0);
          }
        }
        listItem->SetSelected(1);
        if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
          return;
        }
        AdventureGuide::CUIMonsterInfo* selectedItem = dynamic_cast<AdventureGuide::CUIMonsterInfo*>(listItem);
        if (!selectedItem) {
          return;
        }
        this->selected = selectedItem->index;
        if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
          auto node = this->data.begin();
          for (int remaining = static_cast<int>(this->selected); remaining > 0; --remaining) {
            ++node;
          }
          this->onSelect(selectedItem, *node);
        }
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x009D2B90
// FUNCTION: fn_14025
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (AdventureGuide::CUIQuestInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (AdventureGuide::CUIQuestInfo*& slot : this->items) {
      AdventureGuide::CUIQuestInfo* item = new AdventureGuide::CUIQuestInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (AdventureGuide::CUIQuestInfo* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        if (this->layoutFlag) {
          return;
        }
        for (AdventureGuide::CUIQuestInfo* other : this->items) {
          if (listItem != other) {
            other->SetSelected(0);
          }
        }
        listItem->SetSelected(1);
        if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
          return;
        }
        AdventureGuide::CUIQuestInfo* selectedItem = dynamic_cast<AdventureGuide::CUIQuestInfo*>(listItem);
        if (!selectedItem) {
          return;
        }
        this->selected = selectedItem->index;
        if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
          auto node = this->data.begin();
          for (int remaining = static_cast<int>(this->selected); remaining > 0; --remaining) {
            ++node;
          }
          this->onSelect(selectedItem, *node);
        }
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x009D2DF0
// FUNCTION: fn_14026
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (AdventureGuide::CUITabInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (AdventureGuide::CUITabInfo*& slot : this->items) {
      AdventureGuide::CUITabInfo* item = new AdventureGuide::CUITabInfo();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
      item->SetImageResources(images[0], images[1], images[2], images[3]);
      item->onHover = [this](CUIListItem* listItem) {
        for (AdventureGuide::CUITabInfo* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        if (this->layoutFlag) {
          return;
        }
        for (AdventureGuide::CUITabInfo* other : this->items) {
          if (listItem != other) {
            other->SetSelected(0);
          }
        }
        listItem->SetSelected(1);
        if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
          return;
        }
        AdventureGuide::CUITabInfo* selectedItem = dynamic_cast<AdventureGuide::CUITabInfo*>(listItem);
        if (!selectedItem) {
          return;
        }
        this->selected = selectedItem->index;
        if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
          auto node = this->data.begin();
          for (int remaining = static_cast<int>(this->selected); remaining > 0; --remaining) {
            ++node;
          }
          this->onSelect(selectedItem, *node);
        }
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x009D3360
// FUNCTION: fn_14030
template <>
void CUIListBox<AdventureGuide::CUISubTab>::LoadSkin(bool resolveSkin) {
  this->NormalImage() = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (AdventureGuide::CUISubTab* item : this->items) {
    item->SetImageResources(this->NormalImage(), this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x009D3570
// FUNCTION: fn_14031
template <>
void CUIListBox<AdventureGuide::CUITab>::LoadSkin(bool resolveSkin) {
  this->NormalImage() = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (AdventureGuide::CUITab* item : this->items) {
    item->SetImageResources(this->NormalImage(), this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x009D3780
// FUNCTION: fn_14032
template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::LoadSkin(bool resolveSkin) {
  this->NormalImage() = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (AdventureGuide::CUIItemInfo* item : this->items) {
    item->SetImageResources(this->NormalImage(), this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x009D3990
// FUNCTION: fn_14033
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::LoadSkin(bool resolveSkin) {
  this->NormalImage() = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (AdventureGuide::CUIMonsterInfo* item : this->items) {
    item->SetImageResources(this->NormalImage(), this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x009D3BA0
// FUNCTION: fn_14034
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::LoadSkin(bool resolveSkin) {
  this->NormalImage() = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (AdventureGuide::CUIQuestInfo* item : this->items) {
    item->SetImageResources(this->NormalImage(), this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x009D3DB0
// FUNCTION: fn_14035
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::LoadSkin(bool resolveSkin) {
  this->NormalImage() = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (AdventureGuide::CUITabInfo* item : this->items) {
    item->SetImageResources(this->NormalImage(), this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x009D40B0
// FUNCTION: fn_14039
template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x009D4130
// FUNCTION: fn_14040
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x009D41B0
// FUNCTION: fn_14041
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x009D4230
// FUNCTION: fn_14042
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

template void CUIListBox<AdventureGuide::CUITab>::Resize(int, int);
template void CUIListBox<AdventureGuide::CUISubTab>::Resize(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::OnScroll(int);
template void CUIListBox<AdventureGuide::CUISubTab>::Paint();
template void CUIListBox<AdventureGuide::CUITab>::Paint();
template void CUIListBox<AdventureGuide::CUISubTab>::OnWheel(int);
template void CUIListBox<AdventureGuide::CUITab>::OnWheel(int);

// OFFSET: 0x009F1D40
// FUNCTION: fn_14703
void CUIVScrollBar::PageUp() {
  this->SetPosition(this->position - this->step);
}

// OFFSET: 0x009F1D60
// FUNCTION: fn_14704
void CUIVScrollBar::PageDown() {
  this->SetPosition(this->position + this->step);
}

// OFFSET: 0x009F1D80
// FUNCTION: fn_14705
int CUIVScrollBar::PositionFromOffset(int offset) {
  float value = 0.0f;
  if (this->trackLength != 0) {
    value = static_cast<float>(this->total) * static_cast<float>(offset) / static_cast<float>(this->trackLength);
    value = static_cast<float>(std::ceil(value));
  }

  return static_cast<int>(value);
}

// OFFSET: 0x009F2A30
// FUNCTION: fn_14722
void CUIVScrollBar::SetHidden(int) {
  this->hidden = 1;
  this->Show(0);
}

// OFFSET: 0x009EDF80
// FUNCTION: fn_14597
void CUIHScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (!this->background) {
    return;
  }

  if (this->transparent) {
    for (int x = 0; x < this->trackLength - this->background->image.width; x += this->background->image.width) {
      this->DrawAuxiliary(1, this->upButton->geometry.width + x, 0, &this->background->image, true, false);
    }
    this->DrawAuxiliary(
      1, this->geometry.width - this->downButton->geometry.width - this->background->image.width, 0, &this->background->image, true, false
    );
    return;
  }

  for (int x = 0; x < this->trackLength - this->background->image.width; x += this->background->image.width) {
    this->DrawBitmap(this->upButton->geometry.width + x, 0, this->background, false);
  }
  this->DrawBitmap(this->geometry.width - this->downButton->geometry.width - this->background->image.width, 0, this->background, false);
}

// OFFSET: 0x009EE1B0
// FUNCTION: fn_14599
void CUIHScrollBar::OnLBtnDown(int x, int) {
  const int left = this->thumb->geometry.x;
  if (x < left) {
    this->PageUp();
  } else if (x > this->thumb->geometry.width + left) {
    this->PageDown();
  }
}

// OFFSET: 0x009EE270
// FUNCTION: fn_14603
void CUIHScrollBar::OnWheel(int value) {
  if (value > 0) {
    this->PageDown();
  } else {
    this->PageUp();
  }
}

// OFFSET: 0x009EE290
// FUNCTION: fn_14604
void CUIHScrollBar::UpdateThumb() {
  CUIThumb* bar = this->thumb;
  float offset;
  if (bar->resizable) {
    const int length = static_cast<int>((static_cast<float>(this->visible) / static_cast<float>(this->total)) * static_cast<float>(this->trackLength));
    bar->Resize(length > 1 ? length : 1, this->geometry.height);
    offset = (static_cast<float>(this->trackLength) / static_cast<float>(this->total)) * static_cast<float>(this->position);
  } else {
    if (this->total - this->visible <= 0) {
      bar->Move(this->upButton->geometry.width, 0);
      return;
    }
    offset = static_cast<float>((this->trackLength - bar->geometry.width) * this->position) / static_cast<float>(this->total - this->visible);
  }
  this->thumb->Move(static_cast<int>(offset + static_cast<float>(this->upButton->geometry.width)), 0);
}

// OFFSET: 0x009EE380
// FUNCTION: fn_14605
void CUIHScrollBar::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->downButton->Move(this->geometry.width - this->downButton->geometry.width, 0);
  this->trackLength = this->geometry.width - this->upButton->geometry.width - this->downButton->geometry.width;
}

// OFFSET: 0x009EE3D0
// FUNCTION: fn_14606
void CUIHScrollBar::SetButtonImages(const std::string& left, const std::string& right) {
  CBitmapRes* image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(left));
  if (image) {
    this->upButton->Resize(image->image.width, image->image.height);
    this->upButton->Move(0, 0);
    this->upButton->SetImages(left, left, left, left);
    this->upButton->Show(1);
  }

  image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(right));
  if (image) {
    this->downButton->Resize(image->image.width, image->image.height);
    this->downButton->Move(this->geometry.width - image->image.width, 0);
    this->downButton->SetImages(right, right, right, right);
    this->downButton->Show(1);
  }

  this->Invalidate();
}

// OFFSET: 0x009EE510
// FUNCTION: fn_14607
void CUIHScrollBar::SetThumbImages(const std::string& top, const std::string& middle, const std::string& bottom) {
  this->trackLength = this->geometry.width - this->downButton->geometry.width - this->upButton->geometry.width;
  const int length = static_cast<int>((static_cast<float>(this->visible) / static_cast<float>(this->total)) * static_cast<float>(this->trackLength));
  this->thumb->Resize(length > 1 ? length : 1, this->geometry.height);
  this->thumb->Move(this->upButton->geometry.width, 0);
  this->thumb->Show(1);
  this->thumb->SetImages(top, middle, bottom);
  this->Invalidate();
}

// OFFSET: 0x009EA990
// FUNCTION: fn_14536
bool CUIPadding::CanHitTest() {
  return false;
}

// OFFSET: 0x009EAE20
// FUNCTION: fn_14543
void CUIRadioButton::OnLBtnDown(int, int) {
  if (this->disabled) {
    return;
  }

  g_windowMgr.SetCapture(this);
  if (this->checked) {
    return;
  }

  if (this->parent) {
    std::list<UIWindow*> UIWindow::*const children = &CUIRadioButton::m_children;
    for (UIWindow* child : this->parent->*children) {
      CUIRadioButton* button = dynamic_cast<CUIRadioButton*>(child);
      if (button) {
        button->checked = this == button;
        button->Invalidate();
      }
    }
  }

  if (this->onSelect) {
    this->onSelect(this);
  }
}

// OFFSET: 0x009EA1B0
// FUNCTION: fn_14514
void CUIPadding::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->FillRegion(0, 0, this->left, this->geometry.height, this->color);
  this->FillRegion(0, 0, this->geometry.width, this->top, this->color);
  this->FillRegion(this->geometry.width - this->right, 0, this->right, this->geometry.height, this->color);
  this->FillRegion(0, this->geometry.height - this->bottom, this->geometry.width, this->bottom, this->color);
}

// OFFSET: 0x009EA240
// FUNCTION: fn_14515
void CUITextViewer::Paint() {
  this->ClearSurface(this->backgroundColor);
}

// OFFSET: 0x009EA250
// FUNCTION: fn_14516
void CUITextViewer::OnProcess() {
  for (UIWindow* child : this->m_children) {
    child->OnProcess();
  }
}

// OFFSET: 0x009EA280
// FUNCTION: fn_14517
void CUITextViewer::OnWheel(int value) {
  CUIVScrollBar* bar = this->scrollBar;

  if (!bar->dragging && bar->total > bar->visible) {
    bar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x009EA6A0
// FUNCTION: fn_14520
int CUITextViewer::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 98) {
    switch (a) {
    case LINE_SEGMENT_URL:
      OpenTextButtonLink(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_ITEM:
      ShowTextButtonItem(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_NAVI:
      OpenTextButtonNavigation(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_QUEST:
      RequestTextButtonService(static_cast<UITextButton*>(sender));
      break;
    case LINE_SEGMENT_TIPBOX:
      ShowTextButtonTip(static_cast<UITextButton*>(sender));
      break;
    }
  }
  return UIWindow::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009EA7A0
// FUNCTION: fn_14521
void CUITextViewer::SetTextOffset(int x, int y) {
  this->textX = x;
  this->textY = y;
}

// OFFSET: 0x009EA7E0
// FUNCTION: fn_14522
void CUITextViewer::SetBackgroundColor(std::uint32_t value) {
  this->backgroundColor = value;

  CUIPadding* padding = this->padding;
  padding->color = value;
  padding->Invalidate();
}

// OFFSET: 0x009EA810
// FUNCTION: fn_14523
void CUITextViewer::SetBold(bool value) {
  this->bold = value;
}

// OFFSET: 0x009EA820
// FUNCTION: fn_14524
void CUITextViewer::SetTextColor(std::uint32_t value) {
  this->textColor = value;
}

// OFFSET: 0x009EA830
// FUNCTION: fn_14525
void CUITextViewer::SetFontHeight(int value) {
  this->fontHeight = value;
}

// OFFSET: 0x009EA840
// FUNCTION: fn_14526
void CUITextViewer::SetFont(const std::string& name) {
  this->font = FallbackFontIndex(name);
}

// OFFSET: 0x009EA860
// FUNCTION: fn_14527
void CUITextViewer::SetLineSpacing(int value) {
  this->lineSpacing = value;
}

// OFFSET: 0x009EA870
// FUNCTION: fn_14528
void CUITextViewer::SetOutline(bool enabled, std::uint32_t value) {
  this->outline = enabled;
  this->outlineColor = value;
}

// OFFSET: 0x009EA8D0
// FUNCTION: fn_14529
void CUITextViewer::SetPadding(int left, int top, int right, int bottom) {
  this->paddingLeft = left;
  this->paddingTop = top;
  this->paddingRight = right;
  this->paddingBottom = bottom;
}

// OFFSET: 0x009EA910
// FUNCTION: fn_14531
void CUITextViewer::SetScrollStep(int value) {
  this->scrollBar->step = value;
  this->Invalidate();
}

// OFFSET: 0x009EA930
// FUNCTION: fn_14532
void CUITextViewer::SetScrollBarSize(int width, int height) {
  this->scrollBar->SetSize(width, height);
}

// OFFSET: 0x009EA900
// FUNCTION: fn_14530
void CUITextViewer::SetScrollBarButtonImages(const std::string& up, const std::string& down) {
  this->scrollBar->SetButtonImages(up, down);
}

// OFFSET: 0x009EA940
// FUNCTION: fn_14533
void CUITextViewer::SetScrollBarThumbImages(const std::string& top, const std::string& middle, const std::string& bottom) {
  this->scrollBar->SetThumbImages(top, middle, bottom);
}

// OFFSET: 0x009EA950
// FUNCTION: fn_14534
void CUITextViewer::SetScrollBarThumbImage(const std::string& name) {
  this->scrollBar->SetThumbImage(name);
}

// OFFSET: 0x009EA960
// FUNCTION: fn_14535
void CUITextViewer::SetScrollBarBackground(const std::string& name) {
  this->scrollBar->SetBackground(name);
}

// OFFSET: 0x009E9DD0
// FUNCTION: fn_14512
void CUITextViewer::Clear() {
  for (auto& child : this->childPositions) {
    this->RemoveChild(child.first, 1);
  }
  this->childPositions.clear();
  this->lines.clear();
  this->contentHeight = 0;
  this->scrollBar->SetTotal(1);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->Invalidate();
}

// OFFSET: 0x009EA2C0
// FUNCTION: fn_14518
void CUITextViewer::Relayout() {
  std::vector<std::string> lines;
  lines = std::move(this->lines);
  this->Clear();
  for (const std::string& line : lines) {
    this->AddLine(line);
  }
  this->scrollBar->SetPosition(this->scrollBar->position);
}

// OFFSET: 0x009EA400
// FUNCTION: fn_14519
void CUITextViewer::Resize(int width, int height) {
  const int position = this->scrollBar->position;
  UIWindow::Resize(width, height);
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->padding->Resize(width, height);
  std::vector<std::string> lines;
  lines = std::move(this->lines);
  this->Clear();
  for (const std::string& line : lines) {
    this->AddLine(line);
  }
  this->scrollBar->SetPosition(position);
}

// OFFSET: 0x009E9400
// FUNCTION: fn_14503
void CUIResizer::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->image) {
    this->DrawBitmap(0, 0, this->image, false);
  }
}

// OFFSET: 0x009E9430
// FUNCTION: fn_14504
void CUIResizer::OnLBtnDown(int x, int y) {
  if (!this->parent) {
    return;
  }

  g_windowMgr.SetCapture(this);
  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);

  this->startX = screenX;
  this->startY = screenY;
  this->startWidth = this->parent->geometry.width;
  this->startHeight = this->parent->geometry.height;
  this->deltaX = 0;
  this->deltaY = 0;
}

// OFFSET: 0x009E94B0
// FUNCTION: fn_14505
void CUIResizer::OnLBtnUp(int x, int y) {
  if (!this->parent || this != g_windowMgr.Capture()) {
    return;
  }

  g_windowMgr.ReleaseCapture();
  this->parent->RefreshSnap();
  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);

  this->startX = screenX;
  this->startY = screenY;
  this->startWidth = this->parent->geometry.width;
  this->startHeight = this->parent->geometry.height;
  this->deltaX = 0;
  this->deltaY = 0;
}

// OFFSET: 0x009E9540
// FUNCTION: fn_14506
void CUIResizer::OnMouseMove(int x, int y) {
  if (!this->parent || this != g_windowMgr.Capture()) {
    return;
  }

  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);

  const int rows = static_cast<int>(std::roundf(static_cast<float>(screenY - this->startY) / static_cast<float>(this->stepY)) * static_cast<float>(this->stepY));
  int width = static_cast<int>(std::roundf(static_cast<float>(screenX - this->startX) / static_cast<float>(this->stepX)) * static_cast<float>(this->stepX)) + this->startWidth;
  if (width > this->maxWidth) {
    width = this->maxWidth;
  } else if (width < this->minWidth) {
    width = this->minWidth;
  }

  int height = rows + this->startHeight;
  if (height > this->maxHeight) {
    height = this->maxHeight;
  } else if (height < this->minHeight) {
    height = this->minHeight;
  }

  width -= this->startWidth;
  height -= this->startHeight;
  if (width != this->deltaX || height != this->deltaY) {
    if (this->onResize) {
      this->onResize(width - this->deltaX, height - this->deltaY);
    }
    this->deltaX = width;
    this->deltaY = height;
  }
}

// OFFSET: 0x009E96B0
// FUNCTION: fn_14507
void CUIResizer::ResetCursor(int, int) {
  g_modeMgr.gameMode->SetCursorAction(2);
}

// OFFSET: 0x009E96C0
// FUNCTION: fn_14508
void CUIResizer::LoadSkin(bool resolveSkin) {
  this->image = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageName, resolveSkin));
  if (this->image) {
    this->Resize(this->image->image.width, this->image->image.height);
    if (this->parent) {
      this->Move(this->parent->geometry.width - this->geometry.width, this->parent->geometry.height - this->geometry.height);
    }
  }
}

// OFFSET: 0x009E9790
// FUNCTION: fn_14509
void CUIResizer::SetImage(const std::string& name) {
  this->imageName = name;
  this->LoadSkin(0);
  this->Invalidate();
}

// OFFSET: 0x009F0EA0
// FUNCTION: fn_14682
UIWindow* CUIControl::FindChildById(int id) {
  for (UIWindow* child : this->m_children) {
    if (child->geometry.id == id) {
      return child;
    }
  }

  return nullptr;
}

// OFFSET: 0x009BE300
// FUNCTION: fn_13947
CUITextViewer::CUITextViewer()
    : CUIControl(), textX(0), textY(0), font(0), fontHeight(12), bold(false), textColor(0), shadow(false), shadowColor(0xFFFFFF), outline(false),
      outlineColor(0xFFFFFF), lineSpacing(6), backgroundColor(0xFFFFFF), paddingLeft(3), paddingTop(3), paddingRight(3), paddingBottom(3), scrollBar(nullptr),
      padding(nullptr), lines(), contentHeight(0), childPositions() {
}

// OFFSET: 0x009BD4F0
// FUNCTION: fn_13921
bool UnusedTrue9BD4F0() {
  return true;
}

// OFFSET: 0x009BD500
// FUNCTION: fn_13922
bool UnusedTrue9BD500() {
  return true;
}

// OFFSET: 0x009BD510
// FUNCTION: fn_13923
bool UnusedTrue9BD510() {
  return true;
}

// OFFSET: 0x009BD520
// FUNCTION: fn_13924
bool UnusedTrue9BD520() {
  return true;
}

// OFFSET: 0x009BD530
// FUNCTION: fn_13925
bool UnusedTrue9BD530() {
  return true;
}

// OFFSET: 0x009BD540
// FUNCTION: fn_13926
bool UnusedTrue9BD540() {
  return true;
}

// OFFSET: 0x009BD550
// FUNCTION: fn_13927
bool UnusedTrue9BD550() {
  return true;
}

// OFFSET: 0x009BD560
// FUNCTION: fn_13928
bool UnusedTrue9BD560() {
  return true;
}

// OFFSET: 0x009BD570
// FUNCTION: fn_13929
bool UnusedTrue9BD570() {
  return true;
}

// OFFSET: 0x009BD580
// FUNCTION: fn_13930
bool UnusedTrue9BD580() {
  return true;
}

// OFFSET: 0x009BD590
// FUNCTION: fn_13931
bool UnusedTrue9BD590() {
  return true;
}

// OFFSET: 0x009BD5A0
// FUNCTION: fn_13932
bool UnusedTrue9BD5A0() {
  return true;
}

// OFFSET: 0x009BD5B0
// FUNCTION: fn_13933
bool UnusedTrue9BD5B0() {
  return true;
}

// OFFSET: 0x009BD5C0
// FUNCTION: fn_13934
bool UnusedTrue9BD5C0() {
  return true;
}

// OFFSET: 0x009BD7D0
// FUNCTION: fn_13939
template <>
CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x009BD990
// FUNCTION: fn_13940
template <>
CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x009BDB50
// FUNCTION: fn_13941
template <>
CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x009BDD10
// FUNCTION: fn_13942
template <>
CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x009BE5F0
// FUNCTION: fn_13949
template <>
CUIListBox<AdventureGuide::CUISubTab>::~CUIListBox() {
}

// OFFSET: 0x009BE750
// FUNCTION: fn_13950
template <>
CUIListBox<AdventureGuide::CUITab>::~CUIListBox() {
}

// OFFSET: 0x009BE8B0
// FUNCTION: fn_13951
template <>
CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::~CUIListBoxEx() {
}

// OFFSET: 0x009BEA10
// FUNCTION: fn_13952
template <>
CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::~CUIListBoxEx() {
}

// OFFSET: 0x009BEB70
// FUNCTION: fn_13953
template <>
CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::~CUIListBoxEx() {
}

// OFFSET: 0x009BECD0
// FUNCTION: fn_13954
template <>
CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::~CUIListBoxEx() {
}

// OFFSET: 0x009BFDB0
// FUNCTION: fn_13963
CUIButton::~CUIButton() {
  using Handler = std::function<void()>;
  reinterpret_cast<Handler*>(this->hoverHandlerStorage)->~Handler();
  reinterpret_cast<Handler*>(this->clickHandlerStorage)->~Handler();
}

// OFFSET: 0x009BFED0
// FUNCTION: fn_13964
CUIImageButton::~CUIImageButton() = default;

// OFFSET: 0x009E1840
// FUNCTION: fn_14396
CUITextButton::~CUITextButton() = default;

// OFFSET: 0x009E1730
// FUNCTION: fn_14395
CUIBasicButton::~CUIBasicButton() = default;

namespace AdventureGuide {

// OFFSET: 0x009D9150
// FUNCTION: fn_14276
__declspec(noinline) CUIItemInfo::CUIItemInfo()
    : CUIListItem(), headerControls(), descriptionViewer(nullptr), optionsViewer(nullptr), sourcesViewer(nullptr), itemId(-1), type(21), location(-1),
      description() {
}

// OFFSET: 0x009D92D0
// FUNCTION: fn_14278
CUIMonsterInfo::CUIMonsterInfo()
    : CUIListItem(), nameText(nullptr), detailsViewer(nullptr), valuesViewer(nullptr), sprite(nullptr), action(nullptr) {
}

// OFFSET: 0x009D9420
// FUNCTION: fn_14280
__declspec(noinline) CUIQuestInfo::CUIQuestInfo()
    : CUIListItem(), stampImage(nullptr), titleViewer(nullptr), linesViewer(nullptr), statusViewer(nullptr), title(), questId(0) {
}

// OFFSET: 0x009D94D0
// FUNCTION: fn_14281
CUISubTab::CUISubTab() : CUIListItem(), labelText(nullptr), stampImage(nullptr), id(0), questId(0) {
}

// OFFSET: 0x009D9550
// FUNCTION: fn_14282
CUITab::CUITab()
    : CUIListItem(), labelText(nullptr), subTabs(nullptr), title(), type(7), page(nullptr), collapsedHeight(0) {
}

// OFFSET: 0x009D9600
// FUNCTION: fn_14283
CUITabInfo::CUITabInfo() : CUIListItem(), titleText(nullptr), descriptionText(nullptr) {
}

}

// OFFSET: 0x009F2EC0
// FUNCTION: fn_14733
void CUIVScrollBar::ScrollBy(int value) {
  const int target = this->position - value % this->step + value;
  if (target != this->position) {
    this->SetPosition(target);
  }
}

// OFFSET: 0x009F0A70
// FUNCTION: fn_14670
void CUIStaticText::OnMouseHover(int x, int y) {
  if (this->Tooltip().empty()) {
    CUIControl::OnMouseHover(x, y);
    return;
  }

  x = -6;
  y = -4;
  this->ToScreen(x, y);
  g_modeMgr.gameMode->ShowTooltip(this->Tooltip().c_str(), x, y, 0xFFFFFFFFu, 0, 1);
}

// OFFSET: 0x009F0AE0
// FUNCTION: fn_14671
void CUIStaticText::UpdateText() {
  this->Tooltip().clear();
  if (this->fitText) {
    this->displayText = this->FitDisplayText(this->geometry.width, this->text.c_str(), this->fontIndex, this->fontHeight, this->bold);
    if (this->displayText != this->text) {
      this->Tooltip() = this->text;
    }
  } else {
    this->displayText = this->text;
  }

  const SIZE size =
    this->TextSize(this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->bold, 0);

  if (this->horizontalAlign == 0) {
    this->textX = 0;
  } else if (this->horizontalAlign == 6) {
    this->textX = static_cast<int>(std::roundf(static_cast<float>(this->geometry.width - size.cx) * 0.5f));
  } else if (this->horizontalAlign == 2) {
    this->textX = this->geometry.width - size.cx;
  }

  if (this->verticalAlign == 0) {
    this->textY = 0;
  } else if (this->verticalAlign == 6) {
    this->textY = static_cast<int>(std::roundf(static_cast<float>(this->geometry.height - size.cy) * 0.5f));
  } else if (this->verticalAlign == 8) {
    this->textY = this->geometry.height - size.cy;
  }

  this->Invalidate();
}

// OFFSET: 0x009F0D40
// FUNCTION: fn_14672
void CUIStaticText::SetAlign(int horizontal, int vertical) {
  this->horizontalAlign = horizontal;
  this->verticalAlign = vertical;
  this->UpdateText();
}

// OFFSET: 0x009F0D60
// FUNCTION: fn_14673
void CUIStaticText::SetFitText(bool enabled) {
  this->fitText = enabled;
  this->UpdateText();
}

// OFFSET: 0x009F0D80
// FUNCTION: fn_14674
void CUIStaticText::SetBold(unsigned char enabled) {
  this->bold = enabled;
  this->UpdateText();
}

// OFFSET: 0x009F0DC0
// FUNCTION: fn_14676
void CUIStaticText::SetFontHeight(int height) {
  this->fontHeight = height;
  this->UpdateText();
}

// OFFSET: 0x009F0DE0
// FUNCTION: fn_14677
void CUIStaticText::SetFont(const std::string& name) {
  this->fontIndex = FallbackFontIndex(name);
  this->UpdateText();
}

// OFFSET: 0x009F0E50
// FUNCTION: fn_14680
void CUIStaticText::SetText(std::string_view value) {
  this->text.assign(value.data(), value.size());
  this->UpdateText();
}

// OFFSET: 0x009F0E80
// FUNCTION: fn_14681
void CUIStaticText::SetTooltip(const char* value, std::size_t length) {
  this->Tooltip().assign(value, length);
}

// OFFSET: 0x009F1D20
// FUNCTION: fn_14702
void CUITitleBar::SetTitle(std::string_view title) {
  if (this->titleText) {
    this->titleText->SetText(title);
  }
}

// OFFSET: 0x009ED010
// FUNCTION: fn_14566
void CUITextButton::UpdateText() {
  if (this->fitText) {
    if (this->orientation == 1) {
      this->displayText = this->FitVerticalDisplayText(this->geometry.height, this->text.c_str(), this->fontIndex, this->fontHeight, this->bold);
    } else {
      this->displayText = this->FitDisplayText(this->geometry.width, this->text.c_str(), this->fontIndex, this->fontHeight, this->bold);
    }
    if (this->displayText != this->text) {
      this->fullText = this->text;
    }
  } else {
    this->displayText = this->text;
    this->fullText.clear();
  }

  const SIZE size = this->orientation == 1
                      ? this->VerticalTextSize(this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->bold, 0
                        )
                      : this->TextSize(this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->bold, 0
                        );

  if (this->horizontalAlign == 0) {
    this->textX = 0;
  } else if (this->horizontalAlign == 6) {
    this->textX = (this->geometry.width - size.cx) / 2;
  } else if (this->horizontalAlign == 2) {
    this->textX = this->geometry.width - size.cx;
  }

  if (this->verticalAlign == 0) {
    this->textY = 0;
  } else if (this->verticalAlign == 6) {
    this->textY = (this->geometry.height - size.cy) / 2;
  } else if (this->verticalAlign == 8) {
    this->textY = this->geometry.height - size.cy;
  }

  this->Invalidate();
}

// OFFSET: 0x009ED2E0
// FUNCTION: fn_14567
void CUITextButton::SetAlign(int horizontal, int vertical) {
  this->horizontalAlign = horizontal;
  this->verticalAlign = vertical;
  this->UpdateText();
}

// OFFSET: 0x009ED300
// FUNCTION: fn_14568
void CUITextButton::SetOrientation(int value) {
  this->orientation = value;
  this->UpdateText();
}

// OFFSET: 0x009ED320
// FUNCTION: fn_14569
void CUITextButton::SetFitText(bool enabled) {
  this->fitText = enabled;
  this->UpdateText();
}

// OFFSET: 0x009ED340
// FUNCTION: fn_14570
void CUITextButton::SetBold(unsigned char enabled) {
  this->bold = enabled;
  this->UpdateText();
}

// OFFSET: 0x009ED3E0
// FUNCTION: fn_14572
void CUITextButton::SetFontHeight(int height) {
  this->fontHeight = height;
  this->UpdateText();
}

// OFFSET: 0x009ED400
// FUNCTION: fn_14573
void CUITextButton::SetFont(const std::string& name) {
  this->fontIndex = FallbackFontIndex(name);
  this->UpdateText();
}

// OFFSET: 0x009ED470
// FUNCTION: fn_14576
__declspec(noinline) void CUITextButton::SetText(const std::string& value) {
  this->text = value;
  this->UpdateText();
}

// OFFSET: 0x009EC930
// FUNCTION: fn_14561
void CUITextButton::ResizeToText(int) {
  if (this->orientation == 1) {
    const int height = this->VerticalTextHeight(this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->bold
    );
    this->Resize(this->geometry.width, height);
    if (this->verticalAlign == 0) {
      this->textY = 0;
    } else if (this->verticalAlign == 6) {
      this->textY = (this->geometry.height - height) / 2;
    } else if (this->verticalAlign == 8) {
      this->textY = this->geometry.height - height;
    }
  } else {
    const int width = this->TextWidth(this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->bold, 0
    );
    this->Resize(width, this->geometry.height);
    if (this->horizontalAlign == 0) {
      this->textX = 0;
    } else if (this->horizontalAlign == 6) {
      this->textX = (this->geometry.width - width) / 2;
    } else if (this->horizontalAlign == 2) {
      this->textX = this->geometry.width - width;
    }
  }
}

// OFFSET: 0x009F1210
// FUNCTION: fn_14688
void CUITabButton::ResizeToText(int padding) {
  if (this->orientation == 1) {
    const int height = this->VerticalTextHeight(this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->bold
    );
    this->Resize(this->geometry.width, padding + height);
    if (this->verticalAlign == 0) {
      this->textY = 0;
    } else if (this->verticalAlign == 6) {
      this->textY = (this->geometry.height - height) / 2;
    } else if (this->verticalAlign == 8) {
      this->textY = this->geometry.height - height;
    }
  } else {
    const int width = this->TextWidth(this->displayText.c_str(), 0, this->fontIndex, this->fontHeight, this->bold, 0
    );
    this->Resize(width + 2, this->geometry.height);
    if (this->horizontalAlign == 0) {
      this->textX = 0;
    } else if (this->horizontalAlign == 6) {
      this->textX = (this->geometry.width - width) / 2;
    } else if (this->horizontalAlign == 2) {
      this->textX = this->geometry.width - width;
    }
  }
}

// OFFSET: 0x009ECA30
// FUNCTION: fn_14562
void CUITextButton::DrawLabel() {
  if (this->displayText.size() == 0) {
    return;
  }

  if (this->orientation == 1) {
    this->DrawVerticalLabel();
    return;
  }

  if (this->disabled) {
    this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight,
      this->disabledTextColor, this->bold, 0
    );
    return;
  }

  if (this->state == 1) {
    if (this->shadow) {
      this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight,
        this->shadowColor, this->bold, 0
      );
    }
    if (this->outline) {
      DrawOutlinedText(
        *this, this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, this->textColors[this->state], this->outlineColor, this->fontIndex,
        this->fontHeight, this->bold, 0
      );
      return;
    }
    this->DrawTextAt(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, this->fontIndex,
      this->fontHeight, this->textColors[this->state], this->bold, 0
    );
    return;
  }

  if (this->shadow) {
    this->DrawTextAt(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, this->fontIndex,
      this->fontHeight, this->shadowColor, this->bold, 0
    );
  }
  if (this->outline) {
    DrawOutlinedText(
      *this, this->textX, this->textY, this->displayText.c_str(), 0, this->textColors[this->state], this->outlineColor, this->fontIndex,
      this->fontHeight, this->bold, 0
    );
    return;
  }
  this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight,
    this->textColors[this->state], this->bold, 0
  );
}

// OFFSET: 0x009E1E20
// FUNCTION: fn_14404
void CUIBasicButton::DrawLabel() {
  if (this->displayText.size() == 0) {
    return;
  }

  if (this->disabled) {
    this->DrawTextAt(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, 0, 12, 0xE2E2E2, 0, 0);
    this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, 0, 12, 0x6F6F6F, 0, 0);
    return;
  }

  switch (this->state) {
  case 0:
    this->DrawTextAt(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, 0, 12, 0xD7D7D7, 0, 0);
    this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, 0, 12, 0, 0, 0);
    break;
  case 1:
    this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, 0, 12, 0, 0, 0);
    break;
  case 2:
    this->DrawTextAt(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, 0, 12, 0xC7A796, 0, 0);
    this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, 0, 12, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x009F0980
// FUNCTION: fn_14669
void CUIStaticText::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->displayText.size() == 0) {
    return;
  }

  if (this->shadow) {
    this->DrawTextAt(this->textX + 1, this->textY + 1, this->displayText.c_str(), 0, this->fontIndex,
      this->fontHeight, this->shadowColor, this->bold, 0
    );
  }
  if (this->outline) {
    DrawOutlinedText(
      *this, this->textX, this->textY + 1, this->displayText.c_str(), 0, this->color, this->outlineColor, this->fontIndex, this->fontHeight, this->bold,
      0
    );
    return;
  }
  this->DrawTextAt(this->textX, this->textY, this->displayText.c_str(), 0, this->fontIndex, this->fontHeight,
    this->color, this->bold, 0
  );
}

// OFFSET: 0x009E80C0
// FUNCTION: fn_14489
void CUIImageTextButton::Paint() {
  this->ClearSurface(0xFFFF00FF);

  CBitmapRes* image = nullptr;
  if (this->disabled) {
    image = this->disabledImage;
  } else {
    const auto found = this->images.find(this->state);
    if (found != this->images.end()) {
      image = found->second;
    }
  }

  if (image) {
    this->DrawBitmap(0, 0, image, false);
  }
  this->CUITextButton::DrawLabel();
}

// OFFSET: 0x009F1320
// FUNCTION: fn_14689
void CUITabButton::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (!this->useSkinColors) {
    this->CUIImageTextButton::Paint();
    return;
  }

  if (this->orientation == 1) {
    if (this->state != 1) {
      this->FillRegion(2, 1, this->geometry.width - 3, this->geometry.height - 2, this->normalColor);
      this->FillRegion(1, 2, 1, this->geometry.height - 4, this->borderColor);
      this->FillRegion(2, 1, 1, 1, this->borderColor);
      this->FillRegion(2, this->geometry.height - 2, 1, 1, this->borderColor);
      this->FillRegion(3, 0, this->geometry.width - 3, 1, this->borderColor);
      this->FillRegion(3, this->geometry.height - 1, this->geometry.width - 3, 1, this->borderColor);
      this->FillRegion(this->geometry.width - 1, 0, 1, this->geometry.height, this->borderColor);
    } else {
      this->FillRegion(1, 1, this->geometry.width - 1, this->geometry.height - 2, this->selectedColor);
      this->FillRegion(0, 2, 1, this->geometry.height - 4, this->borderColor);
      this->FillRegion(1, 1, 1, 1, this->borderColor);
      this->FillRegion(1, this->geometry.height - 2, 1, 1, this->borderColor);
      this->FillRegion(2, 0, this->geometry.width - 2, 1, this->borderColor);
      this->FillRegion(2, this->geometry.height - 1, this->geometry.width - 2, 1, this->borderColor);
    }
  } else if (this->state == 1) {
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 1, this->selectedColor);
    this->FillRegion(2, 0, this->geometry.width - 4, 1, this->borderColor);
    this->FillRegion(1, 1, 1, 1, this->borderColor);
    this->FillRegion(this->geometry.width - 2, 1, 1, 1, this->borderColor);
    this->FillRegion(0, 2, 1, this->geometry.height - 2, this->borderColor);
    this->FillRegion(this->geometry.width - 1, 2, 1, this->geometry.height - 2, this->borderColor);
  } else {
    this->FillRegion(1, 2, this->geometry.width - 2, this->geometry.height - 3, this->normalColor);
    this->FillRegion(2, 1, this->geometry.width - 4, 1, this->borderColor);
    this->FillRegion(1, 2, 1, 1, this->borderColor);
    this->FillRegion(this->geometry.width - 2, 2, 1, 1, this->borderColor);
    this->FillRegion(0, 3, 1, this->geometry.height - 3, this->borderColor);
    this->FillRegion(this->geometry.width - 1, 3, 1, this->geometry.height - 3, this->borderColor);
    this->FillRegion(0, this->geometry.height - 1, this->geometry.width, 1, this->borderColor);
  }
  this->CUITextButton::DrawLabel();
}

// OFFSET: 0x009EC430
// FUNCTION: fn_14559
CUITextButton::CUITextButton()
    : CUIButton(), text(), displayText(), fullText(), fitText(false), orientation(0), horizontalAlign(0), verticalAlign(0), fontIndex(0), fontHeight(12),
      bold(0), textColors(), disabledTextColor(0x808080), shadow(0), shadowColor(0xFFFFFF), outline(0), outlineColor(0xFFFFFF), textX(0), textY(0),
      tooltipText() {
  this->textColors[0] = 0;
  this->textColors[1] = 0;
  this->textColors[2] = 0;
}

// OFFSET: 0x009E1250
// FUNCTION: fn_14394
CUIBasicButton::CUIBasicButton()
    : CUITextButton(), enabledState(0), defaultWidth(0), leftImages(), disabledLeft(), middleImages(), disabledMiddle(), rightImages(),
      disabledRight() {
  this->textColors[0] = 0;
  this->textColors[1] = 0;
  this->textColors[2] = 0;
}

// OFFSET: 0x009E9F00
// FUNCTION: fn_14513
void CUITextViewer::OnCreate(int width, int height) {
  this->padding = new CUIPadding();
  this->padding->SetSize(width, height);
  this->AddChild(this->padding);

  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  const std::function<void(int)> onChange = [this](int position) {
    int y = this->paddingTop;
    const CUIVScrollBar* bar = this->scrollBar;
    if (bar->dragging || bar->total <= bar->visible) {
      const int space = this->geometry.height - this->paddingBottom - this->paddingTop;
      if (this->textY == 6) {
        y += (space - this->contentHeight) / 2;
      } else if (this->textY == 8) {
        y += space - this->contentHeight;
      }
    }

    for (const auto& child : this->childPositions) {
      UIWindow* window = child.first;
      window->Move(this->paddingLeft + child.second.first, child.second.second - position + y);
      window->Show(window->geometry.y + window->geometry.height >= 0 && window->geometry.y < this->geometry.height);
    }
  };
  AssignHandler(this->scrollBar->onChange, onChange);
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x009E2D10
// FUNCTION: fn_14408
void CUIBasicButton::SetAutoWidth(bool enabled) {
  this->enabledState = enabled;
  if (enabled) {
    const int width = this->TextWidth(this->text.c_str(), 0, 0, 12, 0, 0) + 8;
    if (width > this->geometry.width) {
      this->Resize(width, this->geometry.height);
    }
    this->SetFitText(false);
  } else {
    this->Resize(this->defaultWidth, this->geometry.height);
    this->SetFitText(true);
  }
  this->SetText(this->text);
}

// OFFSET: 0x009ED570
// FUNCTION: fn_14578
void CUIEditCtrl::DrawBlinkingCaret(const std::uint32_t& color) {
  if (this == g_windowMgr.GetHoverWindow()) {
    this->UIEditCtrl3::DrawBlinkingCaret(color);
  }
}

// OFFSET: 0x00AB10B0
// FUNCTION: fn_17331
template <>
CUIRegisterMgr* Singleton<CUIRegisterMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CUIRegisterMgr;
  }

  return s_instance;
}

// OFFSET: 0x00A829D0
// FUNCTION: fn_16994
const char* UIWindow::ClipTextToWidth(const char* text, int maxWidth, int family, int height, unsigned char bold) {
  if (!text) {
    return nullptr;
  }

  const int width = this->TextWidth(text, static_cast<unsigned>(std::strlen(text)), family, height, bold, 0);
  int ellipsisWidth;
  {
    TextContext context(*this->geometry.surface);
    context.SetFont(family, height, bold, 0);
    ellipsisWidth = context.Measure("...", 3).cx;
  }

  if (width > maxWidth) {
    const char* current = text;
    const char* end = text + std::strlen(text);
    if (current < end) {
      maxWidth -= ellipsisWidth;
      int bytes = 0;
      int total = 0;
      do {
        const int previous = bytes;
        const char* start = current;
        current = g_serverLanguage->NextChar(start, static_cast<int>(end - start));
        const int step = static_cast<int>(current - start);
        total += this->TextWidth(start, step, family, height, bold, 0);
        bytes += step;
        if (total > maxWidth) {
          bytes = previous;
          break;
        }
      } while (current < end);

      if (bytes > 0) {
        char* buffer = static_cast<char*>(std::malloc(bytes + 1));
        std::memcpy(buffer, text, bytes);
        buffer[bytes] = '\0';
        // GLOBAL: gl_24797
        static std::string clipped;
        clipped = buffer + std::string("...");
        std::free(buffer);
        return clipped.c_str();
      }
    }
  }
  return text;
}

// OFFSET: 0x009EBF40
// FUNCTION: fn_14557
bool CUIRegisterMgr::SaveTemporary() {
  std::string data;

  if (this->windowPositions.size() != 0) {
    data += "TblPos = \r\n";
    data += "{\r\n";
    for (const auto& position : this->windowPositions) {
      data += std::format("\t[ {} ] = {{ x = {}, y = {} }},\r\n", position.first, position.second.first, position.second.second);
    }
    data += "}\r\n";
  }

  if (this->windowSizes.size() != 0) {
    data += "TblSize = \r\n";
    data += "{\r\n";
    for (const auto& size : this->windowSizes) {
      data += std::format("\t[ {} ] = {{ w = {}, h = {} }},\r\n", size.first, size.second.first, size.second.second);
    }
    data += "}\r\n";
  }

  if (data.size() == 0) {
    return false;
  }

  NSyncObject sync(&this->lock);
  CFile file;
  if (file.Open(g_uiInfoTempPath, 0x1001, false, true)) {
    data = EncodeBase64(data, false);
    file.Write(data.c_str(), data.size());
    file.Close();
  }
  return true;
}

// OFFSET: 0x009D31F0
// FUNCTION: fn_14029
void CUIRegisterMgr::Register(int key, std::function<CUIFrameWnd*()> factory) {
  this->entries[key] = factory;
}

// OFFSET: 0x009EB200
// FUNCTION: fn_14549
CUIRegisterMgr::CUIRegisterMgr() {
  InitializeCriticalSection(&this->lock);
}

// OFFSET: 0x009EB770
// FUNCTION: fn_14555
CUIFrameWnd* CUIRegisterMgr::Create(int key) {
  auto entry = this->entries.find(key);
  if (entry == this->entries.end()) {
    return nullptr;
  }

  CUIFrameWnd* window = entry->second();
  window->SetId(key);
  window->Initialize();

  if (window->m_ownsBackgroundColor) {
    auto position = this->windowPositions.find(key);
    if (position != this->windowPositions.end()) {
      RECT bounds{0, 0, g_renderer->viewport.width - window->geometry.width, g_renderer->viewport.height - window->geometry.height};
      POINT savedPosition{position->second.first, position->second.second};
      if (PtInRect(&bounds, savedPosition)) {
        window->Move(position->second.first, position->second.second);
      }
    }
  }

  if (window->m_ownsBorderColor) {
    auto size = this->windowSizes.find(key);
    if (size != this->windowSizes.end()) {
      window->Grow(size->second.first - window->geometry.width, size->second.second - window->geometry.height);
    }
  }

  if (window->m_resolveSkin) {
    window->RefreshSkinColors();
  }
  return window;
}

// OFFSET: 0x009EB920
// FUNCTION: fn_14556
void CUIRegisterMgr::Load() {
  CFile file;
  if (file.Open(g_uiInfoPath, 0, false, true)) {
    std::string data;
    data.resize(file.GetSize(), '\0');
    file.Read(data.data(), file.GetSize());
    file.Close();
    data = Base64Decode(data, false);

    LuaScript script;
    script.Open();
    if (script.Execute(data.c_str(), data.size()) && script.Load(g_uiInfoScriptPath, true, false)) {
      for (const auto& entry : this->entries) {
        bool available = false;
        int x = 0;
        int y = 0;
        script.Call("GetWindowPos", "d>bdd", entry.first, &available, &x, &y);
        if (available) {
          this->windowPositions[entry.first] = std::pair<int, int>(x, y);
        }

        int width = 0;
        int height = 0;
        script.Call("GetWindowSize", "d>bdd", entry.first, &available, &width, &height);
        if (available) {
          this->windowSizes[entry.first] = std::pair<int, int>(width, height);
        }
      }
    }
    script.Close();
  }
}

// INLINED
CUIThumb::CUIThumb()
    : CUIControl(), top(), middle(), bottom(), dragOrigin(0), transparent(0), resizable(1), tooltip(), topName(), middleName(), bottomName(), onDrag() {
}

// INLINED
CUIVThumb::CUIVThumb() : CUIThumb() {
}

// INLINED
CUIHThumb::CUIHThumb() : CUIVThumb() {
}

// OFFSET: 0x009EDBE0
// FUNCTION: fn_14596
void CUIHScrollBar::OnCreate(int, int) {
  if (!this->upButton) {
    this->upButton = new CUIImageButton();
    this->upButton->Show(0);
    this->upButton->SetOnClick([this]() { this->PageUp(); });
    this->AddChild(this->upButton);
  }
  if (!this->downButton) {
    this->downButton = new CUIImageButton();
    this->downButton->Show(0);
    this->downButton->SetOnClick([this]() { this->PageDown(); });
    this->AddChild(this->downButton);
  }
  if (!this->thumb) {
    this->thumb = new CUIHThumb();
    this->thumb->Show(0);
    const std::function<void(int)> onDrag = [this](int value) { this->ScrollBy(value); };
    AssignHandler(this->thumb->onDrag, onDrag);
    this->AddChild(this->thumb);
  }
}

// OFFSET: 0x009F1DE0
// FUNCTION: fn_14706
void CUIVScrollBar::OnCreate(int, int) {
  if (!this->upButton) {
    this->upButton = new CUIImageButton();
    this->upButton->Show(0);
    this->upButton->SetOnClick([this]() { this->SetPosition(this->position - this->step); });
    this->AddChild(this->upButton);
  }
  if (!this->downButton) {
    this->downButton = new CUIImageButton();
    this->downButton->Show(0);
    this->downButton->SetOnClick([this]() { this->SetPosition(this->position + this->step); });
    this->AddChild(this->downButton);
  }
  if (!this->thumb) {
    this->thumb = new CUIVThumb();
    this->thumb->Show(0);
    const std::function<void(int)> onDrag = [this](int value) { this->ScrollBy(value); };
    AssignHandler(this->thumb->onDrag, onDrag);
    this->AddChild(this->thumb);
  }
}

// OFFSET: 0x009BE1A0
// FUNCTION: fn_13946
CUIStaticText::CUIStaticText()
    : CUIControl(), displayText(), tooltipStorage(), scrollBar(nullptr), tooltipTailStorage(), fitText(false), horizontalAlign(0),
      verticalAlign(0), fontIndex(0), fontHeight(12), bold(0), color(0), shadow(false), shadowColor(0xFFFFFF), outline(false), outlineColor(0xFFFFFF), textX(0),
      textY(0), text() {
  new (&this->Tooltip()) std::string();
}

// OFFSET: 0x009DA310
// FUNCTION: fn_14285
CUIListItem::~CUIListItem() {
}

// OFFSET: 0x009EFC30
// FUNCTION: fn_14653
CUIListItem::CUIListItem() : CUIControl(), images(), selected(0), disabledImage(nullptr), frozen(0), index(-1), onHover(), onPress() {
  this->images[0] = nullptr;
  this->images[1] = nullptr;
  this->images[2] = nullptr;
  this->m_clip = 1;
}

// OFFSET: 0x009F0110
// FUNCTION: fn_14655
void CUIListItem::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->frozen) {
    if (this->disabledImage) {
      this->DrawBitmap(0, 0, this->disabledImage, false);
    }
  } else if (!this->images[this->selected]) {
    switch (this->selected) {
    case 1:
      this->ClearSurface(0xFFDECE);
      break;
    case 2:
      this->ClearSurface(0xEF9C73);
      break;
    }
  } else {
    this->DrawBitmap(0, 0, this->images[this->selected], false);
  }
}

// OFFSET: 0x009F02E0
// FUNCTION: fn_14663
void CUIListItem::SetImageResources(CBitmapRes* normal, CBitmapRes* hover, CBitmapRes* pressed, CBitmapRes* disabled) {
  this->images.clear();
  this->images[0] = normal;
  this->images[1] = hover;
  this->images[2] = pressed;
  this->disabledImage = disabled;
  this->Invalidate();
}

// OFFSET: 0x009ED5C0
// FUNCTION: fn_14580
void CUIEditBox::OnCreate(int width, int height) {
  this->edit = new CUIEditCtrl;
  this->edit->SetSize(width, height);
  this->edit->invalidatesParent = 1;
  reinterpret_cast<unsigned char*>(&this->edit->editFlags)[1] = 1;
  this->AddChild(this->edit);
}

// OFFSET: 0x009E9990
// FUNCTION: fn_14511
void CUITextViewer::AddLine(const std::string& text) {
  const int scrollWidth = this->scrollBar->geometry.visible ? this->scrollBar->geometry.width : 0;
  const int availableWidth = std::max(0, this->geometry.width - (this->paddingLeft + this->paddingRight + scrollWidth));
  unsigned char align;
  if (this->textX == 6) {
    align = 2;
  } else {
    align = this->textX == 2 ? 1 : 0;
  }
  LineFontFlags flags{this->bold, false};
  CCreateLineInfo lineInfo(
    this, text.c_str(), availableWidth, this->lineSpacing, this->textColor, this->fontHeight, flags,
    this->outline, this->outlineColor, this->shadow, this->shadowColor, align
  );
  if (!lineInfo.m_lines.empty()) {
    this->lines.push_back(text);
    this->contentHeight += this->childPositions.empty() ? 0 : this->lineSpacing;
    for (const auto& row : lineInfo.m_lines) {
      for (UIWindow* child : row) {
        child->Move(child->geometry.x, child->geometry.y + this->contentHeight);
        this->AddChild(child);
        this->childPositions[child] = std::pair<int, int>(child->geometry.x, child->geometry.y);
      }
    }
    this->contentHeight += lineInfo.m_height;
    this->scrollBar->Move(this->geometry.width - this->scrollBar->geometry.width, 0);
    this->scrollBar->SetTotal(this->contentHeight);
    this->scrollBar->SetVisible(this->geometry.height - this->paddingTop - this->paddingBottom);
    this->scrollBar->SetPosition(this->scrollBar->position);
    const int newScrollWidth = this->scrollBar->geometry.visible ? this->scrollBar->geometry.width : 0;
    this->padding->left = this->paddingLeft;
    this->padding->top = this->paddingTop;
    this->padding->right = this->paddingRight + newScrollWidth;
    this->padding->bottom = this->paddingBottom;
    this->padding->Invalidate();
  }
}

// OFFSET: 0x009D8E30
// FUNCTION: fn_14272
CUIAdventureGuide::CUIAdventureGuide()
    : background(nullptr), titleBar(nullptr), characterInfo(nullptr), titleText(nullptr), intro(nullptr), quest(nullptr), monster(nullptr), item(nullptr),
      tabs(nullptr) {
}

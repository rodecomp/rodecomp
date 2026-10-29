#include "UI/Window/UIControl.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Lists/UIThumb.h"
#include "Client/Mode/ModeCursor.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>

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
CUIControl::CUIControl(UIWindowContext& context) : UIWindow(context, 0), hitEnabled(1) {
}

// OFFSET: 0x009C08C0
CUIControl::~CUIControl() = default;

// OFFSET: 0x009D5490
// FUNCTION: fn_14047
bool CUIControl::CanHitTest() {
  return this->hitEnabled != 0;
}

// OFFSET: 0x009EEAD0
// FUNCTION: fn_14634
void CUIControl::Slot10(int x, int y, std::intptr_t value) {
  if (this->parent) {
    this->parent->Slot10(x + this->geometry.x, y + this->geometry.y, value);
  }
}

// OFFSET: 0x009EF680
// FUNCTION: fn_14639
void CUIControl::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF650
// FUNCTION: fn_14638
void CUIControl::Slot26(int x, int y) {
  if (this->parent) {
    this->parent->Slot26(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF710
// FUNCTION: fn_14642
void CUIControl::Slot27(int x, int y) {
  if (this->parent) {
    this->parent->Slot27(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF6E0
// FUNCTION: fn_14641
void CUIControl::Slot28(int x, int y) {
  if (this->parent) {
    this->parent->Slot28(x + this->geometry.x, y + this->geometry.y);
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
void CUIControl::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF7A0
// FUNCTION: fn_14645
void CUIControl::Slot32(int x, int y) {
  if (this->parent) {
    this->parent->Slot32(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF7D0
// FUNCTION: fn_14646
void CUIControl::Slot33(int x, int y) {
  if (this->parent) {
    this->parent->Slot33(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF770
// FUNCTION: fn_14644
void CUIControl::Slot34(int x, int y) {
  if (this->parent) {
    this->parent->Slot34(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x009EF800
// FUNCTION: fn_14647
void CUIControl::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
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
void CUIControl::Slot52(int value) {
  this->Slot53(value);

  for (UIWindow* child : this->m_children) {
    child->Slot52(value);
  }
}

// OFFSET: 0x009D3FC0
// FUNCTION: fn_14036
void CUIControl::Slot53(int) {
}

// OFFSET: 0x009F2B70
// FUNCTION: fn_14726
void CUIVScrollBar::SetTotal(int value) {
  const int count = value > 1 ? value : 1;
  this->total = count;
  const int shown = this->visible <= count ? this->visible : count;
  this->visible = shown;
  this->maximum = count - shown;
  this->Invalidate();
}

// OFFSET: 0x009F2E80
// FUNCTION: fn_14732
void CUIVScrollBar::SetVisible(int value) {
  const int count = this->total;
  const int shown = count < value ? count : value;
  this->visible = shown;
  this->maximum = count - shown;
  this->Invalidate();
}

// OFFSET: 0x009F2530
// FUNCTION: fn_14716
void CUIVScrollBar::SetPosition(int value) {
  int zero = 0;
  const int* clamped = &this->maximum;
  if (value <= this->maximum) {
    clamped = value >= 0 ? &value : &zero;
  }
  if (!this->dragging) {
    this->position = *clamped;
  }
  this->Show(this->total > this->visible);
  this->Slot54();
  if (this->onChange) {
    this->onChange(this->position);
  }
  this->Invalidate();
}

// OFFSET: 0x009F23B0
// FUNCTION: fn_14709
void CUIVScrollBar::Slot25(int, int y) {
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
void CUIVThumb::Slot25(int, int y) {
  g_windowMgr.SetCapture(this);
  this->dragOrigin = y;
}

// OFFSET: 0x009F2420
// FUNCTION: fn_14711
void CUIVThumb::Slot31(int, int y) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
  this->dragOrigin = y;
}

// OFFSET: 0x009F24B0
// FUNCTION: fn_14713
void CUIVThumb::Slot27(int, int y) {
  if (this == g_windowMgr.Capture() && y != this->dragOrigin && this->onDrag) {
    this->onDrag(y - this->dragOrigin);
  }
}

// OFFSET: 0x009F24F0
// FUNCTION: fn_14714
void CUIThumb::ResetCursor(int, int) {
  this->m_context->cursor.Set(2, this->m_context->cursorTick);
}

// OFFSET: 0x009F2500
// FUNCTION: fn_14715
void CUIVScrollBar::Slot35(int value) {
  this->SetPosition(value > 0 ? this->position - this->step : this->step + this->position);
}

// OFFSET: 0x009F2BC0
// FUNCTION: fn_14727
void CUIVScrollBar::Show(int visible) {
  this->geometry.visible = this->unknown_00A9[0] ? 0 : visible;
}

// OFFSET: 0x009F25C0
// FUNCTION: fn_14717
void CUIVScrollBar::Slot54() {
  CUIControl* bar = this->thumb;
  float offset;
  if (bar->hitEnabled) {
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

// OFFSET: 0x009F01F0
// FUNCTION: fn_14657
void CUIListItem::Slot31(int, int) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x009F0230
// FUNCTION: fn_14659
void CUIListItem::Slot27(int, int) {
  if (this->selected != 2) {
    this->Slot47();
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

template void CUIListBox<AdventureGuide::CUITab>::Resize(int, int);
template void CUIListBox<AdventureGuide::CUISubTab>::Resize(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::OnScroll(int);
template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::OnScroll(int);

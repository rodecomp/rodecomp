#include "UI/Window/UIControl.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Lists/UIThumb.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Client/Mode/ModeCursor.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIPadding.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Lists/UIVScrollBar.h"
#include "Render/Text/FontCache.h"

#include <cmath>
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

// OFFSET: 0x009ED970
// FUNCTION: fn_14592
CUIThumb::~CUIThumb() = default;

// OFFSET: 0x009EE080
// FUNCTION: fn_14598
void CUIHThumb::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (!this->resizable) {
    if (this->middle) {
      if (this->transparent) {
        this->DrawAuxiliary(1, 0, 0, &this->middle->image, true, false);
      } else {
        this->DrawBitmap(0, 0, &this->middle->image, false);
      }
    }
    return;
  }

  const int end = this->geometry.width - (this->bottom ? this->bottom->image.width : 0);
  int x = 0;
  if (this->top) {
    if (this->transparent) {
      this->DrawAuxiliary(1, 0, 0, &this->top->image, true, false);
    } else {
      this->DrawBitmap(0, 0, &this->top->image, false);
    }
    x = this->top->image.width;
  }

  if (this->middle) {
    while (x < end) {
      if (this->transparent) {
        this->DrawAuxiliary(1, x, 0, &this->middle->image, true, false);
      } else {
        this->DrawBitmap(x, 0, &this->middle->image, false);
      }
      x += this->middle->image.width;
    }
  }

  if (this->bottom) {
    const int bottomX = this->geometry.width - this->bottom->image.width;
    if (this->transparent) {
      this->DrawAuxiliary(1, bottomX, 0, &this->bottom->image, true, false);
    } else {
      this->DrawBitmap(bottomX, 0, &this->bottom->image, false);
    }
  }
}

// OFFSET: 0x009EE1E0
// FUNCTION: fn_14600
void CUIHThumb::Slot25(int x, int) {
  g_windowMgr.SetCapture(this);
  this->dragOrigin = x;
}

// OFFSET: 0x009EE200
// FUNCTION: fn_14601
void CUIHThumb::Slot31(int x, int) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
  this->dragOrigin = x;
}

// OFFSET: 0x009EE230
// FUNCTION: fn_14602
void CUIHThumb::Slot27(int x, int) {
  if (this == g_windowMgr.Capture() && x != this->dragOrigin && this->onDrag) {
    this->onDrag(x - this->dragOrigin);
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
  this->geometry.visible = this->hidden ? 0 : visible;
}

// OFFSET: 0x009F25C0
// FUNCTION: fn_14717
void CUIVScrollBar::Slot54() {
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
    this->DrawBitmap(0, this->upButton->geometry.height + y, &this->background->image, false);
  }
  this->DrawBitmap(0, this->geometry.height - this->downButton->geometry.height - this->background->image.height, &this->background->image, false);
}

// OFFSET: 0x009F2800
// FUNCTION: fn_14719
void CUIVScrollBar::Slot53(int resolveSkin) {
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
  bar->Slot53(0);
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
    bar->Slot53(0);
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
  this->Slot53(0);
  this->Invalidate();
}

// OFFSET: 0x009F2280
// FUNCTION: fn_14708
void CUIVThumb::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (!this->resizable) {
    if (this->middle) {
      if (this->transparent) {
        this->DrawAuxiliary(1, 0, 0, &this->middle->image, true, false);
      } else {
        this->DrawBitmap(0, 0, &this->middle->image, false);
      }
    }
    return;
  }

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
}

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

// OFFSET: 0x009EE920
// FUNCTION: fn_14627
void CUIButton::Slot26(int x, int y) {
  this->Slot25(x, y);
}

// OFFSET: 0x009EE930
// FUNCTION: fn_14628
void CUIButton::Slot25(int, int) {
  this->SetState(1);
  if (!this->disabled) {
    g_windowMgr.SetCapture(this);
  }
}

// OFFSET: 0x009EE9D0
// FUNCTION: fn_14630
void CUIButton::Slot47() {
  this->SetState(2);
}

// OFFSET: 0x009EEA70
// FUNCTION: fn_14632
void CUIButton::Slot48() {
  this->SetState(0);
}

// OFFSET: 0x009EEAA0
// FUNCTION: fn_14633
void CUIButton::ResetCursor(int, int) {
  this->m_context->cursor.Set(this->disabled ? 0 : 2, this->m_context->cursorTick);
}

// OFFSET: 0x009F26B0

// OFFSET: 0x009F1630
// FUNCTION: fn_14691
void CUITabButton::Slot31(int, int) {
  if (this == g_windowMgr.Capture()) {
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x009F1650
// FUNCTION: fn_14692
void CUITabButton::Slot47() {
  if (this->state != 1) {
    this->SetState(2);
  }
}

// OFFSET: 0x009F1670
// FUNCTION: fn_14693
void CUITabButton::Slot48() {
  if (this->state != 1) {
    this->SetState(0);
  }
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
  this->Root()->InvalidateChildren();
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
  if (this->scale != 100) {
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
    this->DrawBitmap(0, 0, &res->image, false);
  }
}

// OFFSET: 0x009F08A0
// FUNCTION: fn_14667
void CUIStaticImage::Slot53(int resolveSkin) {
  this->image = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageName, resolveSkin));
}

// OFFSET: 0x009F0940
// FUNCTION: fn_14668
void CUIStaticImage::SetImage(const std::string& name) {
  this->imageName = name;
  this->Slot53(0);
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
  if (!this->resizable) {
    if (this->middle) {
      this->DrawBitmap(0, 0, &this->middle->image, false);
    }
    return;
  }

  const int end = this->geometry.width - (this->right ? this->right->image.width : 0);
  int x = 0;
  if (this->left) {
    this->DrawBitmap(0, 0, &this->left->image, false);
    x = this->left->image.width;
  }

  if (this->middle) {
    while (x < end) {
      this->DrawBitmap(x, 0, &this->middle->image, false);
      x += this->middle->image.width;
    }
  }

  if (this->right) {
    this->DrawBitmap(this->geometry.width - this->right->image.width, 0, &this->right->image, false);
  }
}

// OFFSET: 0x009F1AA0
// FUNCTION: fn_14699
void CUITitleBar::Slot53(int resolveSkin) {
  this->middle = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->middleName, resolveSkin));
  if (this->resizable) {
    this->left = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->leftName, resolveSkin));
    this->right = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->rightName, resolveSkin));
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

// OFFSET: 0x009EF860
// FUNCTION: fn_14649
void CUIImageButton::Paint() {
  this->ClearSurface(0xFFFF00FF);

  CBitmapRes* image;
  if (!this->disabled) {
    const auto found = this->images.find(this->state);
    if (found == this->images.end()) {
      return;
    }
    image = found->second;
  } else {
    image = this->disabledImage;
  }

  if (image) {
    if (this->transparent) {
      this->DrawAuxiliary(1, 0, 0, &image->image, true, false);
      return;
    }
    this->DrawBitmap(0, 0, &image->image, false);
  }
}

// OFFSET: 0x009EF900
// FUNCTION: fn_14650
void CUIImageButton::Slot53(int resolveSkin) {
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
  this->Slot53(0);
  this->Invalidate();
}

// OFFSET: 0x009F01B0
// FUNCTION: fn_14656
void CUIListItem::Slot25(int, int) {
  if (!this->frozen) {
    g_windowMgr.SetCapture(this);
    if (this->onPress) {
      this->onPress(this);
    }
  }
}

// OFFSET: 0x009F0210
// FUNCTION: fn_14658
void CUIListItem::Slot47() {
  if (this->onHover) {
    this->onHover(this);
  }
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
  if (this->trackLength == 0) {
    return static_cast<int>(0.0f);
  }

  return static_cast<int>(std::ceil(static_cast<float>(this->total) * static_cast<float>(offset) / static_cast<float>(this->trackLength)));
}

// OFFSET: 0x009F2A30
// FUNCTION: fn_14722
void CUIVScrollBar::SetHidden(int) {
  this->hidden = 1;
  this->Show(0);
}

// OFFSET: 0x009EA990
// FUNCTION: fn_14536
bool CUIPadding::CanHitTest() {
  return false;
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
void CUITextViewer::Slot19() {
  for (UIWindow* child : this->m_children) {
    child->Slot19();
  }
}

// OFFSET: 0x009EA280
// FUNCTION: fn_14517
void CUITextViewer::Slot35(int value) {
  CUIVScrollBar* bar = this->scrollBar;

  if (!bar->dragging && bar->total > bar->visible) {
    bar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
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
  this->padding->color = value;
  this->padding->Invalidate();
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

#include "UI/Window/UIEnums.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Client/Replay.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UICheckBox.h"

// OFFSET: 0x0084F010
// FUNCTION: fn_10656
UICheckBox::UICheckBox()
    : UIWindow(0), m_disabled(0), m_hidden(0), m_color(0), m_fontHeight(12), m_message(-1), m_notify(0) {
  this->geometry.state = 0;
  this->geometry.id = 213;
}

// OFFSET: 0x008514C0
// FUNCTION: fn_10691
UICheckBox::~UICheckBox() = default;

// OFFSET: 0x0085C8C0
// FUNCTION: fn_10803
void UICheckBox::Paint() {
  int width = 0;
  if (this->m_checked.empty() || this->m_unchecked.empty()) {
    return;
  }

  if (this->m_disabled != 0) {
    this->ClearSurface(0xFFFF00FFu);
    return;
  }

  if (this->m_hidden != 0) {
    return;
  }

  switch (this->geometry.state) {
  case 0: {
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_unchecked.c_str())));
    this->DrawBitmap(0, (this->geometry.height - bitmap->image.height) / 2, bitmap, false);
    width = bitmap->image.width;
    break;
  }
  case 1: {
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_checked.c_str())));
    this->DrawBitmap(0, (this->geometry.height - bitmap->image.height) / 2, bitmap, false);
    width = bitmap->image.width;
    break;
  }
  }

  if (!this->m_text.empty()) {
    this->DrawTextAt((width + 3), 0, this->m_text.c_str(), 0, 0, this->m_fontHeight, this->m_color, 0, 0);
  }
}

// INLINED
__forceinline void UICheckBox::Notify() {
  this->Invalidate();
  if (this->Root()) {
    this->Root()->Invalidate();
  }

  if (this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, this->geometry.state, 0, 0);
  }
}

// OFFSET: 0x008658D0
// FUNCTION: fn_10932
void UICheckBox::SetChecked(int value) {
  if (this->m_disabled != 0) {
    return;
  }

  this->geometry.state = value;
  this->Notify();
}

// OFFSET: 0x0086AF70
// FUNCTION: fn_10999
void UICheckBox::SetHidden(bool hidden) {
  this->m_hidden = hidden;
  this->Show(!hidden);
  this->Invalidate();
}

// OFFSET: 0x0085EB90
// FUNCTION: fn_10841
void UICheckBox::OnLBtnDown(int, int) {
  if (this->m_disabled != 0 || g_replayAnimationScaling) {
    return;
  }

  this->geometry.state = 1u - this->geometry.state;
  this->Notify();
  if (this->m_notify != 0) {
    this->parent->SendMsg(this, UM_TOGGLEPUSH, this->m_message, 0, 0, 0);
  }
}

// OFFSET: 0x0085F310
// FUNCTION: fn_10859
void UICheckBox::OnMouseHover(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  this->OnMouseMove(x, y);
  if (this->m_tooltip.empty()) {
    return;
  }

  int screenX = 0;
  int screenY = -20;
  this->ToScreen(screenX, screenY);

  if (this->geometry.state != 0 || this->m_uncheckedTooltip.empty()) {
    g_modeMgr.gameMode->ShowTooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
  } else {
    g_modeMgr.gameMode->ShowTooltip(this->m_uncheckedTooltip.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x0085F9C0
// FUNCTION: fn_10877
void UICheckBox::ResetCursor(int, int) {
  if (this->m_disabled == 0) {
    g_modeMgr.gameMode->SetCursorAction(2);
  }
}

// OFFSET: 0x00865450
// FUNCTION: fn_10926
void UICheckBox::SetImages(const char* checked, const char* unchecked, const char* text) {
  this->m_checked = checked;
  this->m_unchecked = unchecked;
  ResourceMgr::Instance().Load(Resolve(this->m_unchecked.c_str()));
  this->m_text = text ? text : "";

  if (!this->m_text.empty()) {
    const int width = this->TextWidth(this->m_text.c_str(), 0, 0, 12, 0, 0);
    CBitmapRes* bitmap = nullptr;
    switch (this->geometry.state) {
    case 0:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_unchecked.c_str())));
      break;
    case 1:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_checked.c_str())));
      break;
    }

    const int desired = ((bitmap->image.width + width) + 3);
    if (desired != this->geometry.width) {
      this->Resize(desired, this->geometry.height);
    }
  }
}

// OFFSET: 0x00865600
// FUNCTION: fn_10928
void UICheckBox::SetBitmaps(const char* checked, const char* unchecked) {
  this->m_checked = checked;
  this->m_unchecked = unchecked;

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_checked.c_str())));
  this->geometry.width = bitmap->image.width;
  this->geometry.height = bitmap->image.height;
  ResourceMgr::Instance().Load(Resolve(this->m_unchecked.c_str()));
}

// OFFSET: 0x00869760
// FUNCTION: fn_10968
void UICheckBox::SetTooltip(const char* text) {
  if (text) {
    this->m_tooltip = text;
  }
}

// OFFSET: 0x008653B0
// FUNCTION: fn_10925
void UICheckBox::FitToText() {
  if (this->m_text.empty()) {
    return;
  }

  const int width = this->TextWidth(this->m_text.c_str(), 0, 0, 12, 0, 0);
  CBitmapRes* bitmap = nullptr;
  switch (this->geometry.state) {
  case 0:
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_unchecked.c_str())));
    break;
  case 1:
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_checked.c_str())));
    break;
  }

  const int desired = ((bitmap->image.width + width) + 3);
  if (desired != this->geometry.width) {
    this->Resize(desired, this->geometry.height);
  }
}

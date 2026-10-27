#include "Client/Mode/ModeCursor.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/FrameFocus.h"
#include "UI/Buttons/UICheckBox.h"

// OFFSET: 0x0084F010
// FUNCTION: fn_10656
UICheckBox::UICheckBox(UIWindowContext& windowContext, UIButtonContext& controls)
    : UIWindow(windowContext, 0), m_disabled(0), m_hidden(0), m_color(0), m_fontHeight(12), m_message(-1), m_notify(0), m_controls(&controls) {
  this->geometry.unknown_0030 = 0;
  this->geometry.id = 213;
}

// OFFSET: 0x008514C0
// FUNCTION: fn_10691
UICheckBox::~UICheckBox() = default;

// OFFSET: 0x0085C8C0
// FUNCTION: fn_10803
void UICheckBox::Paint() {
  if (this->m_checked.empty() || this->m_unchecked.empty()) {
    return;
  }

  if (this->m_disabled != 0) {
    if (this->geometry.surface) {
      this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
    }

    return;
  }

  if (this->m_hidden != 0) {
    return;
  }

  int width = 0;
  if (this->geometry.unknown_0030 == 0 || this->geometry.unknown_0030 == 1) {
    const std::string& name = this->geometry.unknown_0030 == 0 ? this->m_unchecked : this->m_checked;
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(this->m_controls->resources.Load(name.c_str()));
    this->DrawBitmap(0, (this->geometry.height - bitmap->image.height) / 2, &bitmap->image, false);
    width = bitmap->image.width;
  }

  if (!this->m_text.empty()) {
    this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, (width + 3), 0, this->m_text.c_str(), 0, 0, this->m_fontHeight, this->m_color, 0, 0);
  }
}

// INLINED
void UICheckBox::Notify() {
  this->Invalidate();
  UIWindow* root = this;

  while (root->parent) {
    root = static_cast<UIWindow*>(root->parent);
  }

  root->Invalidate();
  if (this->parent) {
    static_cast<UIWindow*>(this->parent)->SendMsg(this, 6, this->geometry.id, this->geometry.unknown_0030, 0, 0);
  }
}

// OFFSET: 0x008658D0
// FUNCTION: fn_10932
void UICheckBox::SetChecked(int value) {
  if (this->m_disabled != 0) {
    return;
  }

  this->geometry.unknown_0030 = value;
  this->Notify();
}

// OFFSET: 0x0085EB90
// FUNCTION: fn_10841
void UICheckBox::Slot25(int, int) {
  if (this->m_disabled != 0 || this->m_controls->restricted) {
    return;
  }

  this->geometry.unknown_0030 = 1u - this->geometry.unknown_0030;
  this->Notify();
  if (this->m_notify != 0) {
    static_cast<UIWindow*>(this->parent)->SendMsg(this, 2, this->m_message, 0, 0, 0);
  }
}

// OFFSET: 0x0085F310
// FUNCTION: fn_10859
void UICheckBox::Slot28(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  this->Slot27(x, y);
  if (this->m_tooltip.empty()) {
    return;
  }

  int screenX = 0, screenY = -20;

  for (const UIWindow* node = this; node; node = static_cast<const UIWindow*>(node->parent)) {
    screenX = (screenX + node->geometry.x);
    screenY = (screenY + node->geometry.y);
  }

  const std::string& tooltip = this->geometry.unknown_0030 != 0 || this->m_uncheckedTooltip.empty() ? this->m_tooltip : this->m_uncheckedTooltip;
  this->m_controls->services.Tooltip(tooltip.c_str(), screenX, screenY, -1, false, true);
}

// OFFSET: 0x0085F9C0
// FUNCTION: fn_10877
void UICheckBox::ResetCursor(int, int) {
  if (this->m_disabled == 0) {
    this->m_context->cursor.Set(2, this->m_context->cursorTick);
  }
}

// OFFSET: 0x00865450
// FUNCTION: fn_10926
void UICheckBox::SetImages(const char* checked, const char* unchecked, const char* text) {
  this->m_checked = checked;
  this->m_unchecked = unchecked;
  this->m_controls->resources.Load(this->m_unchecked.c_str());
  this->m_text = text ? text : "";

  if (!this->m_text.empty()) {
    const int width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, this->m_text.c_str(), 0, 0, 12, 0, 0);
    CBitmapRes* bitmap = nullptr;
    if (this->geometry.unknown_0030 == 0) {
      bitmap = static_cast<CBitmapRes*>(this->m_controls->resources.Load(this->m_unchecked.c_str()));
    } else if (this->geometry.unknown_0030 == 1) {
      bitmap = static_cast<CBitmapRes*>(this->m_controls->resources.Load(this->m_checked.c_str()));
    }

    const int desired = ((bitmap->image.width + width) + 3);
    if (desired != this->geometry.width) {
      this->Resize(desired, this->geometry.height);
    }
  }
}

// OFFSET: 0x00869760
// FUNCTION: fn_10968
void UICheckBox::SetTooltip(const char* text) {
  if (text) {
    this->m_tooltip = text;
  }
}

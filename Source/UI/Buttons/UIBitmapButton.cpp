#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Buttons/UIBitmapButton.h"

// OFFSET: 0x0084EAB0
UIBitmapButton::UIBitmapButton(UIWindowContext& windowContext, UIButtonContext& buttonContext)
    : UIButton(windowContext, buttonContext), m_bitmapWidth(0), m_bitmapHeight(0), m_invalidateParent(0) {
}

// OFFSET: 0x008511B0
// FUNCTION: fn_10689
// OFFSET: 0x00852220
// FUNCTION: fn_10705
UIBitmapButton::~UIBitmapButton() = default;

// OFFSET: 0x008657A0
// FUNCTION: fn_10931
bool UIBitmapButton::SetBitmap(const char* name, int state, unsigned char exactSize) {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(name));
  if (!bitmap) {
    return false;
  }

  if (bitmap->image.width > this->m_bitmapWidth) {
    this->m_bitmapWidth = bitmap->image.width;
  }

  if (bitmap->image.height > this->m_bitmapHeight) {
    this->m_bitmapHeight = bitmap->image.height;
  }

  if (exactSize != 0) {
    this->m_bitmapWidth = bitmap->image.width;
    this->m_bitmapHeight = bitmap->image.height;
  }

  if (state >= 0 && state <= 3) {
    this->m_names[state] = name;
  }

  return true;
}

// OFFSET: 0x0085BA90
// FUNCTION: fn_10795
void UIBitmapButton::Paint() {
  CBitmapRes* bitmap;

  switch (this->geometry.unknown_0030) {
  case 0:
    bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(this->m_names[0].c_str()));
    break;
  case 1:
    bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(this->m_names[1].c_str()));
    break;
  case 2:
    bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(this->m_names[2].c_str()));
    break;
  }

  if (this->m_disabled != 0 && !this->m_names[3].empty()) {
    bitmap = static_cast<CBitmapRes*>(this->m_buttonContext->resources.Load(this->m_names[3].c_str()));
  }

  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  if (this->m_invalidateParent != 0 && this->parent) {
    static_cast<UIWindow*>(this->parent)->Invalidate();
  }
}

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBmpTextButton2.h"

// OFFSET: 0x0084ECB0
// FUNCTION: fn_10651
UIBmpTextButton2::UIBmpTextButton2(UIWindowContext& windows, UIButtonContext& buttons) : UIBitmapButton(windows, buttons) {
}

// OFFSET: 0x0085BCF0
// FUNCTION: fn_10798
void UIBmpTextButton2::Paint() {
  CBitmapRes* bitmap;

  if (this->m_disabled != 0) {
    this->geometry.unknown_0030 = 3;
  } else {
    switch (this->geometry.unknown_0030) {
    case 0:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[0].c_str())));
      break;
    case 1:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[1].c_str())));
      break;
    case 2:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[2].c_str())));
      break;
    }
  }

  if (this->m_disabled != 0 && !this->m_names[3].empty()) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[3].c_str())));
  }

  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  if (this->m_invalidateParent != 0 && this->parent) {
    this->parent->Invalidate();
  }

  this->DrawLabels();
}

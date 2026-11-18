#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBmpTextButton2.h"

#include <cstddef>

// OFFSET: 0x0084ECB0
// FUNCTION: fn_10651
UIBmpTextButton2::UIBmpTextButton2() : UIBitmapButton() {
}

// OFFSET: 0x0085BCF0
// FUNCTION: fn_10798
void UIBmpTextButton2::Paint() {
  CBitmapRes* bitmap;

  if (this->m_disabled != 0) {
    this->geometry.state = 3;
  } else {
    switch (this->geometry.state) {
    case 0:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_normalName.c_str())));
      break;
    case 1:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_hoverName.c_str())));
      break;
    case 2:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_pressedName.c_str())));
      break;
    }
  }

  if (this->m_disabled != 0 && this->m_disabledName.size() > 0) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_disabledName.c_str())));
  }

  this->DrawBitmap(0, 0, bitmap, false);
  if (this->m_invalidateParent != 0 && this->parent) {
    this->parent->Invalidate();
  }

  this->DrawLabels();
}

// INLINED
inline bool UIBmpTextButton2::HasLabel(int index) const {
  return index >= 0 && static_cast<std::size_t>(index) < this->labels.size();
}

// OFFSET: 0x00858DE0

// OFFSET: 0x00869390

// OFFSET: 0x0086A670
// FUNCTION: fn_45629
void UIBmpTextButton2::SetLabelText(std::string text, int index) {
  if (this->HasLabel(index)) {
    this->labels[index].text = text;
  }
  this->Invalidate();
}

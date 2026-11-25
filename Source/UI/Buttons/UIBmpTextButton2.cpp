#include "UI/Buttons/ButtonState.h"
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
    this->geometry.state = BUTTON_STATE_DISABLED;
  } else {
    switch (this->geometry.state) {
    case BUTTON_STATE_NORMAL:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_normalName.c_str())));
      break;
    case BUTTON_STATE_HOVER:
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_hoverName.c_str())));
      break;
    case BUTTON_STATE_PRESSED:
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
// FUNCTION: fn_45628
std::string UIBmpTextButton2::GetLabel(int index, int* height, std::uint32_t* color, unsigned char* bold,
                                       unsigned char* shadow, int* x, int* y) const {
  std::string text;
  if (this->HasLabel(index)) {
    text = this->labels[index].text;
    *height = this->labels[index].height;
    *color = this->labels[index].color;
    *bold = this->labels[index].bold;
    *shadow = this->labels[index].shadow;
    *x = this->labels[index].x;
    *y = this->labels[index].y;
  }
  return text;
}

// OFFSET: 0x00869390
// FUNCTION: fn_45625
void UIBmpTextButton2::SetLabel(std::string text, int index, int height, std::uint32_t color,
                                unsigned char bold, unsigned char shadow, int x, int y) {
  if (this->HasLabel(index)) {
    this->labels[index].text = text;
    this->labels[index].height = height;
    this->labels[index].color = color;
    this->labels[index].bold = bold;
    this->labels[index].shadow = shadow;
    this->labels[index].x = x;
    this->labels[index].y = y;
  } else {
    BmpTextButtonLabel label;
    label.text = text;
    label.height = height;
    label.color = color;
    label.bold = bold;
    label.shadow = shadow;
    label.x = x;
    label.y = y;
    this->labels.push_back(label);
  }
}

// OFFSET: 0x0086A670
// FUNCTION: fn_45629
void UIBmpTextButton2::SetLabelText(std::string text, int index) {
  if (this->HasLabel(index)) {
    this->labels[index].text = text;
  }
  this->Invalidate();
}

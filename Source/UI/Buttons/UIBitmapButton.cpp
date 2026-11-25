#include "UI/Buttons/ButtonState.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UITextureButton.h"
#include "UI/Buttons/UIBitmapButton2.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

// OFFSET: 0x0084EAB0
// FUNCTION: fn_10650
UIBitmapButton::UIBitmapButton()
    : UIButton() {
  this->m_bitmapWidth = 0;
  this->m_bitmapHeight = 0;
  this->m_disabledName.clear();
  this->m_invalidateParent = 0;
}

// OFFSET: 0x0086C530
// FUNCTION: fn_45791
UIBitmapButton2::UIBitmapButton2()
    : UIBitmapButton() {
}

// OFFSET: 0x00870280
// FUNCTION: fn_45792
UIBitmapButton2::~UIBitmapButton2() = default;

// OFFSET: 0x008695B0
// FUNCTION: fn_45624
bool UITextureButton::SetTexture(const char* name, int state) {
  if (g_serviceType != ServiceKorea) {
    return false;
  }

  CTexture* texture = g_texMgr.Load(name, 0, false, true, false);
  if (!texture) {
    return false;
  }

  if (texture->Width() > this->textureWidth) {
    this->textureWidth = texture->Width();
  }
  if (texture->Height() > this->textureHeight) {
    this->textureHeight = texture->Height();
  }

  switch (state) {
  case BUTTON_STATE_NORMAL: this->normalTexture.assign(name, std::strlen(name)); break;
  case BUTTON_STATE_HOVER: this->hoverTexture.assign(name, std::strlen(name)); break;
  case BUTTON_STATE_PRESSED: this->pressedTexture.assign(name, std::strlen(name)); break;
  case BUTTON_STATE_DISABLED: this->disabledTexture.assign(name, std::strlen(name)); break;
  }

  return true;
}

// OFFSET: 0x008657A0
// FUNCTION: fn_10931
bool UIBitmapButton::SetBitmap(const char* name, int state, unsigned char exactSize) {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
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
  switch (state) {
  case BUTTON_STATE_NORMAL: this->m_normalName = name; break;
  case BUTTON_STATE_HOVER: this->m_hoverName = name; break;
  case BUTTON_STATE_PRESSED: this->m_pressedName = name; break;
  case BUTTON_STATE_DISABLED: this->m_disabledName = name; break;
  }

  return true;
}

// OFFSET: 0x0085BA90
// FUNCTION: fn_10795
void UIBitmapButton::Paint() {
  CBitmapRes* bitmap;

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

  if (this->m_disabled != 0 && this->m_disabledName.size() > 0) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_disabledName.c_str())));
  }

  this->DrawBitmap(0, 0, bitmap, false);
  if (this->m_invalidateParent != 0 && this->parent) {
    this->parent->Invalidate();
  }
}


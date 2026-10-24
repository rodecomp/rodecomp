#include "Render/TextureBindingDX7.h"
#include "Render/TextureDX7.h"

// OFFSET: 0x0055F7D0
// FUNCTION: fn_02119
void TextureBindingDX7::Set(unsigned stage, CTextureDX7* texture) {
  if (stage == 0) {
    if (this->first != texture) {
      this->device->SetTexture(0, texture ? texture->NativeSurface() : nullptr);
      this->first = texture;
    }
  } else if (stage == 1) {
    if (this->second != texture) {
      this->device->SetTexture(1, texture ? texture->NativeSurface() : nullptr);
      this->second = texture;
    }
  } else {
    this->device->SetTexture(stage, texture ? texture->NativeSurface() : nullptr);
  }
}

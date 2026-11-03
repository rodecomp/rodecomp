#include "Render/DX7/RenderSystemDX7.h"
#include "Render/RendererDX7.h"
#include "Render/DX7/TextureDX7.h"

// OFFSET: 0x0055F7D0
// FUNCTION: fn_02119
void CRendererDX7::SetTexture(unsigned stage, CTextureDX7* texture) {
  if (stage == 0) {
    if (this->boundTexture != texture) {
      this->device->SetTexture(0, texture ? texture->NativeSurface() : nullptr);
      this->boundTexture = texture;
    }
  } else if (stage == 1) {
    if (this->boundLightmap != texture) {
      this->device->SetTexture(1, texture ? texture->NativeSurface() : nullptr);
      this->boundLightmap = texture;
    }
  } else {
    this->device->SetTexture(stage, texture ? texture->NativeSurface() : nullptr);
  }
}

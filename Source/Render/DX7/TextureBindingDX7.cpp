#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX7/RendererDX7.h"
#include "Render/DX7/TextureDX7.h"

// OFFSET: 0x0055F7D0
// FUNCTION: fn_02119
void CRendererDX7::SetTexture(unsigned stage, CTextureDX7* texture) {
  switch (stage) {
  case 0:
    if (this->boundTexture != texture) {
      if (texture == nullptr) {
        this->device->SetTexture(0, nullptr);
      } else {
        this->device->SetTexture(0, texture->NativeSurface());
      }
      this->boundTexture = texture;
    }
    break;
  case 1:
    if (this->boundLightmap != texture) {
      if (texture == nullptr) {
        this->device->SetTexture(1, nullptr);
      } else {
        this->device->SetTexture(1, texture->NativeSurface());
      }
      this->boundLightmap = texture;
    }
    break;
  default:
    if (texture != nullptr) {
      this->device->SetTexture(stage, texture->NativeSurface());
    } else {
      this->device->SetTexture(stage, nullptr);
    }
  }
}

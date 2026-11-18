#include "Render/DX7/RenderSystemDX7.h"
#include "Render/DX7/QueuedDrawingDX7.h"

// OFFSET: 0x0055F7D0
// FUNCTION: fn_02119
void CRendererDX7::SetTexture(unsigned stage, CTextureDX7* texture) {
  switch (stage) {
  case 0:
    this->BindTexture(texture);
    break;
  case 1:
    this->BindLightmap(texture);
    break;
  default:
    if (texture != nullptr) {
      this->device->SetTexture(stage, texture->NativeSurface());
    } else {
      this->device->SetTexture(stage, nullptr);
    }
  }
}

#ifndef RAGNAROK_SOURCE_RENDER_DX7_QUEUEDDRAWINGDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_QUEUEDDRAWINGDX7_H

#include "Render/DX7/TextureDX7.h"
#include "Render/RendererDX7.h"
#include "Render/Primitives/RenderQueues.h"

#include <cstddef>
#include <d3d.h>

// INLINED
inline void CRendererDX7::BindTexture(CTextureDX7* texture) {
  if (this->boundTexture != texture) {
    if (texture) {
      this->device->SetTexture(0, texture->NativeSurface());
    } else {
      this->device->SetTexture(0, nullptr);
    }
    this->boundTexture = texture;
  }
}

// INLINED
inline void CRendererDX7::BindLightmap(CTextureDX7* lightmap) {
  if (this->boundLightmap != lightmap) {
    if (lightmap) {
      this->device->SetTexture(1, lightmap->NativeSurface());
    } else {
      this->device->SetTexture(1, nullptr);
    }
    this->boundLightmap = lightmap;
  }
}


#endif

#ifndef RAGNAROK_SOURCE_RENDER_DX7_QUEUEDDRAWINGDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_QUEUEDDRAWINGDX7_H

#include "Render/DX7/TextureDX7.h"
#include "Render/Primitives/RenderQueues.h"

#include <cstddef>
#include <d3d.h>

struct QueuedDrawingDX7 {
  /* 0000 */ unsigned char unknown_0000[0x48];
  /* 0048 */ unsigned char twoPass;
  /* 0049 */ unsigned char unknown_0049[0x113];
  /* 015C */ RenderQueues queues;
  /* 024C */ unsigned char unknown_024C[0x1C];
  /* 0268 */ CTextureDX7* boundTexture;
  /* 026C */ CTextureDX7* boundLightmap;
  /* 0270 */ unsigned unknown_0270;
  /* 0274 */ IDirect3DDevice7* device;

  void DrawDepthSortedQueue();
  void DrawAlphaQueues();
  void DrawOpaqueQueues();
  void DrawGroundBaseQueue();
  void DrawAuxiliaryQueue();
  // INLINED
  void BindTexture(CTextureDX7* texture) {
    if (this->boundTexture != texture) {
      this->device->SetTexture(0, texture ? texture->NativeSurface() : nullptr);
      this->boundTexture = texture;
    }
  }
  // INLINED
  void BindLightmap(CTextureDX7* lightmap) {
    if (this->boundLightmap != lightmap) {
      this->device->SetTexture(1, lightmap ? lightmap->NativeSurface() : nullptr);
      this->boundLightmap = lightmap;
    }
  }
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(QueuedDrawingDX7, twoPass) == 0x0048);
static_assert(offsetof(QueuedDrawingDX7, queues) == 0x015C);
static_assert(offsetof(QueuedDrawingDX7, boundTexture) == 0x0268);
static_assert(offsetof(QueuedDrawingDX7, boundLightmap) == 0x026C);
static_assert(offsetof(QueuedDrawingDX7, device) == 0x0274);
#endif

#endif

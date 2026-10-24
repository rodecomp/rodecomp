#ifndef RAGNAROK_SOURCE_RENDER_TEXTUREBINDINGDX7_H
#define RAGNAROK_SOURCE_RENDER_TEXTUREBINDINGDX7_H

#include <cstddef>
#include <d3d.h>

class CTextureDX7;
struct IDirect3DDevice7;

// Original renderer +0x268/+0x26C and native device +0x274.
// Cache entries borrow texture objects; native SetTexture retains its own surface reference.
struct TextureBindingDX7 {
  /* 0000 */ unsigned char unknown_0000[0x268]{};
  /* 0268 */ CTextureDX7* first = nullptr;
  /* 026C */ CTextureDX7* second = nullptr;
  /* 0270 */ unsigned unknown_0270 = 0;
  /* 0274 */ IDirect3DDevice7* device = nullptr;

  void Set(unsigned stage, CTextureDX7* texture);
  void Set(IDirect3DDevice7* dev, unsigned stage, CTextureDX7* texture) {
    this->device = dev;
    this->Set(stage, texture);
  }
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(TextureBindingDX7, first) == 0x268);
static_assert(offsetof(TextureBindingDX7, second) == 0x26C);
static_assert(offsetof(TextureBindingDX7, device) == 0x274);
#endif

#endif

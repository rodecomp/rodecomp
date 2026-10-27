#ifndef RAGNAROK_SOURCE_RENDER_RENDERERBACKBUFFER_H
#define RAGNAROK_SOURCE_RENDER_RENDERERBACKBUFFER_H

#include <cstddef>
#include <windows.h>

class DeviceOwnerDX7;

struct RendererBackBuffer {
  /* 0250 */ void* pixels;
  /* 0254 */ int pitch;
  /* 0258 */ int bitsPerPixel;

  bool Lock(const DeviceOwnerDX7& owner);
  HRESULT Unlock(const DeviceOwnerDX7& owner);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RendererBackBuffer) == 12);
static_assert(offsetof(RendererBackBuffer, pixels) + 0x0250 == 0x0250);
static_assert(offsetof(RendererBackBuffer, pitch) + 0x0250 == 0x0254);
static_assert(offsetof(RendererBackBuffer, bitsPerPixel) + 0x0250 == 0x0258);
#endif

#endif

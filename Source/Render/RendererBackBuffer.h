#ifndef RAGNAROK_SOURCE_RENDER_RENDERERBACKBUFFER_H
#define RAGNAROK_SOURCE_RENDER_RENDERERBACKBUFFER_H

#include <windows.h>

class DeviceOwnerDX7;

struct RendererBackBuffer {
  void* pixels;
  int pitch;
  int bitsPerPixel;

  bool Lock(const DeviceOwnerDX7& owner);
  HRESULT Unlock(const DeviceOwnerDX7& owner);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RendererBackBuffer) == 12);
#endif

#endif

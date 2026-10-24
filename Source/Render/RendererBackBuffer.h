#ifndef RAGNAROK_SOURCE_RENDER_RENDERERBACKBUFFER_H
#define RAGNAROK_SOURCE_RENDER_RENDERERBACKBUFFER_H

#include <windows.h>

class DeviceOwnerDX7;

// Original renderer +0x250..+0x25B. Failed locking leaves every field intact;
// unlocking does not clear the last published pointer, pitch or bit depth.
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

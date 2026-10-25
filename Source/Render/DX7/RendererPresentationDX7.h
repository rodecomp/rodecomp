#ifndef RAGNAROK_SOURCE_RENDER_DX7_RENDERERPRESENTATIONDX7_H
#define RAGNAROK_SOURCE_RENDER_DX7_RENDERERPRESENTATIONDX7_H

#include <memory>
#include <windows.h>

class DeviceOwnerDX7;
struct RendererBackBuffer;
class CSurface;
class CRenderSystemBase;

struct RendererPresentationDX7 {
  unsigned& frame;
  unsigned& samples;
  unsigned& lastTick;
  unsigned& rate;
  RendererBackBuffer& backBuffer;

  void Present(DeviceOwnerDX7& owner, bool wait, const RECT& monitor);
  float FPS() const;
  void DrawFPS(CRenderSystemBase* system, int width, int height);
};

#endif

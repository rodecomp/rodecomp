#ifndef RAGNAROK_SOURCE_RENDER_RENDERERPRESENTATIONDX7_H
#define RAGNAROK_SOURCE_RENDER_RENDERERPRESENTATIONDX7_H

#include <memory>
#include <windows.h>

class DeviceOwnerDX7;
struct RendererBackBuffer;
class CSurface;
class CRenderSystemBase;

// Borrowed CRenderer frame (+0x38) and legacy renderer counters
// (+0x25C/+0x260/+0x264). The published rate is a sample count, not a ratio.
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

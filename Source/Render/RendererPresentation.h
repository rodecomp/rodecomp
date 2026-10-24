#ifndef RAGNAROK_SOURCE_RENDER_RENDERERPRESENTATION_H
#define RAGNAROK_SOURCE_RENDER_RENDERERPRESENTATION_H

#include <windows.h>

class DeviceOwnerDX9;

struct RenderDiagnostic {
  unsigned line;
  const char* file;
};

// Borrowed CRenderer +0x38 frame and DX9 +0x268..+0x273 counters. They are
// initialized by renderer construction, not reset on every presentation.
struct RendererPresentation {
  unsigned& frame;
  unsigned& lastTick;
  unsigned& frames;
  float& fps;
  RenderDiagnostic& diagnostic;

  HRESULT Present(DeviceOwnerDX9& owner, int option);
};

#endif

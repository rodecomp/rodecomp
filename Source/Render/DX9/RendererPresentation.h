#ifndef RAGNAROK_SOURCE_RENDER_DX9_RENDERERPRESENTATION_H
#define RAGNAROK_SOURCE_RENDER_DX9_RENDERERPRESENTATION_H

#include <windows.h>

class DeviceOwnerDX9;

struct RenderDiagnostic {
  unsigned line;
  const char* file;
};

struct RendererPresentation {
  unsigned& frame;
  unsigned& lastTick;
  unsigned& frames;
  float& fps;
  RenderDiagnostic& diagnostic;

  HRESULT Present(DeviceOwnerDX9& owner, int option);
};

#endif

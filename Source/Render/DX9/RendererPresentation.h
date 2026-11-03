#ifndef RAGNAROK_SOURCE_RENDER_DX9_RENDERERPRESENTATION_H
#define RAGNAROK_SOURCE_RENDER_DX9_RENDERERPRESENTATION_H

#include <windows.h>

class DeviceOwnerDX9;

struct RendererPresentation {
  unsigned& frame;
  unsigned& lastTick;
  unsigned& frames;
  float& fps;

  HRESULT Present(DeviceOwnerDX9& owner, int option);
};

#endif

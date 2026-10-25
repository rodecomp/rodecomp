#include "Render/DeviceContext/DCSurface.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/RendererBackBuffer.h"
#include "Render/DX7/RendererPresentationDX7.h"

#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include <utility>

// OFFSET: 0x0055B000
// FUNCTION: fn_02087
void RendererPresentationDX7::Present(DeviceOwnerDX7& owner, bool wait, const RECT& monitor) {
  ++this->frame;
  const unsigned elapsed = GetTickCount() - this->lastTick;

  if (static_cast<float>(elapsed) < 1000.0f) {
    ++this->samples;
  } else {
    this->lastTick = GetTickCount();
    this->rate = this->samples;
    this->samples = 0;
  }

  if (owner.RestoreLostSurfaces()) {
    return;
  }

  if (this->backBuffer.Lock(owner)) {
    this->backBuffer.Unlock(owner);
  }

  if (owner.Present(wait, monitor) == DDERR_SURFACELOST) {
    owner.MarkSurfacesLost();
  }
}

// OFFSET: 0x0055D0F0
// FUNCTION: fn_02096
float RendererPresentationDX7::FPS() const {
  return static_cast<float>(this->rate);
}

// OFFSET: 0x0055F8F0
// FUNCTION: fn_02120
void RendererPresentationDX7::DrawFPS(CRenderSystemBase* system, int width, int height) {
  const unsigned previous = this->lastTick;
  const unsigned elapsed = timeGetTime() - previous;
  const unsigned count = this->samples;
  const float measured = (static_cast<float>(count) * 1000.0f) / static_cast<float>(elapsed);

  if (elapsed > 1000 && !(count & 1)) {
    this->samples = count >> 1;
    this->lastTick = previous + (elapsed >> 1);
  }

  char text[128];
  std::sprintf(text, "%.01f fps", static_cast<double>(measured));
  CDCSurface drawing(AcquireBackBufferSurface(system));
  HDC dc = nullptr;
  drawing.GetDC(&dc);
  HGDIOBJ font = nullptr;

  if (dc) {
    font = GetCurrentObject(dc, OBJ_FONT);
    SetBkMode(dc, TRANSPARENT);
  }

  SetTextColor(dc, 0xFFFF);
  const int x = width - 40;
  const int y = height - 14;
  TextOutA(dc, x, y, text, static_cast<int>(std::strlen(text)));

  if (dc) {
    SelectObject(dc, font);
    drawing.ReleaseDC(dc);
  }
}

#include "Render/DeviceContext/DCSurface.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/RendererBackBuffer.h"
#include "Render/RendererDX7.h"
#include "Render/Text/TextContext.h"
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>
#include <utility>

// OFFSET: 0x0055B000
// FUNCTION: fn_02087
HRESULT CRendererDX7::Present(int option) {
  ++this->frame;
  const unsigned elapsed = GetTickCount() - this->time;

  if (static_cast<float>(elapsed) < 1000.0f) {
    ++this->samples;
  } else {
    this->time = GetTickCount();
    this->rate = this->samples;
    this->samples = 0;
  }

  DeviceOwnerDX7* owner = g_deviceOwnerDX7;
  if (owner && owner->RestoreLostSurfaces()) {
    return S_OK;
  }

  if (this->LockBackBuffer()) {
    this->UnlockBackBuffer();
  }

  owner = g_deviceOwnerDX7;
  if (owner && owner->Present(option != 0) == DDERR_SURFACELOST) {
    owner = g_deviceOwnerDX7;
    if (owner) {
      owner->MarkSurfacesLost();
    }
  }
  return S_OK;
}

// OFFSET: 0x0055D0F0
// FUNCTION: fn_02096
float CRendererDX7::FPS() const {
  return static_cast<float>(this->rate);
}

// OFFSET: 0x0055F8F0
// FUNCTION: fn_02120
void CRendererDX7::CleanupTextures() {
  const unsigned previous = this->time;
  const unsigned elapsed = timeGetTime() - previous;
  const unsigned count = this->samples;
  const float measured = (static_cast<float>(count) * 1000.0f) / static_cast<float>(elapsed);

  if (elapsed > 1000 && !(count & 1)) {
    this->samples = count >> 1;
    this->time = previous + (elapsed >> 1);
  }

  char text[128];
  std::sprintf(text, "%.01f fps", static_cast<double>(measured));
  const int x = this->viewport.width - 40;
  const int y = this->viewport.height - 14;
  CDCSurface drawing(GetRenderSystemHolder()->BackBufferSurface());
  TextContext context(drawing);
  context.SetFont(0, 12, 0, 0);
  SetTextColor(context.Dc(), 0xFFFF);
  TextOutA(context.Dc(), x, y, text, static_cast<int>(std::strlen(text)));
}

// OFFSET: 0x0055FAD0
// FUNCTION: fn_02121
int DrawBackBufferText(int x, int y, const char* text, COLORREF color) {
  CDCSurface surface(GetRenderSystemHolder()->BackBufferSurface());
  TextContext context(surface);
  context.SetFont(0, 12, 0, 0);
  SetTextColor(context.Dc(), color);
  TextOutA(context.Dc(), x, y, text, static_cast<int>(std::strlen(text)));
  return 0;
}

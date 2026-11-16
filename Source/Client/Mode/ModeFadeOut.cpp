#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeFade.h"
#include "Contents/Sound/SoundMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"

#include <timeapi.h>

// OFFSET: 0x00AD6960
// FUNCTION: fn_17847
std::uint32_t CMode::FadeOutMode(int showLogo) {
  g_soundMgr->FadeOut();
  g_modeFadeTick = timeGetTime();
  g_renderer->CaptureBackBuffer();
  std::uint32_t elapsed = timeGetTime() - g_modeFadeTick;
  while (elapsed < 255) {
    DrawAlphaRectangle(0, 0, g_renderer->viewport.width, g_renderer->viewport.height, elapsed << 24);
    if (showLogo != 0) {
      CBitmapRes* logo = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("rag_logo.bmp")));
      g_windowMgr.DrawBitmapResource(g_renderer->viewport.width - 192, g_renderer->viewport.height - 122, logo);
    }
    if (g_renderer->Draw()) {
      g_renderer->Present(1);
    }
    g_renderer->RestoreBackBuffer();
    elapsed = timeGetTime() - g_modeFadeTick;
  }
  return elapsed;
}

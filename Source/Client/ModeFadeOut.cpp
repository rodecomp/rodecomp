#include "Client/ModeFadeOut.h"
#include "Client/ModeFade.h"
#include "Contents/Sound/SoundMgr.h"
#include "Render/BitmapDrawing.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

#include <timeapi.h>

// OFFSET: 0x00AD6960
// FUNCTION: fn_17847
std::uint32_t FadeOutMode(CRenderer& renderer, CRenderSystemBase& system, ResourceMgr& resources,
  int showLogo, bool depthAlpha, float depthStart, float depthEnd) {
  g_soundMgr->FadeOut();
  g_modeFadeTick = timeGetTime();
  renderer.CaptureBackBuffer();
  std::uint32_t elapsed = timeGetTime() - g_modeFadeTick;
  while (elapsed < 255) {
    DrawAlphaRectangle(renderer.primitives, renderer.queues, 0, 0, renderer.viewport.width, renderer.viewport.height, elapsed << 24,
      depthAlpha, depthStart, depthEnd);
    if (showLogo != 0) {
      auto* logo = static_cast<CBitmapRes*>(resources.Load(Resolve("rag_logo.bmp")));
      DrawBitmapResource(system, renderer.viewport.width - 192, renderer.viewport.height - 122, logo);
    }
    if (renderer.Draw()) {
      renderer.Present(1);
    }
    renderer.RestoreBackBuffer();
    elapsed = timeGetTime() - g_modeFadeTick;
  }
  return elapsed;
}

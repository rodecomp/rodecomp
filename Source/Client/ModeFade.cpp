#include "Client/ModeFade.h"
#include "Client/Mode/CMode.h"
#include "Contents/Sound/SoundMgr.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"

#include <timeapi.h>

// GLOBAL: gl_24680
std::uint32_t g_modeFadeTick = 0;

// OFFSET: 0x00AD6740
// FUNCTION: fn_17842
void CMode::DrawFadeIn() {
  if (this->fadeInRequested != 0) {
    g_modeFadeTick = timeGetTime();
    g_soundMgr->FadeIn();
    this->fadeInRequested = 0;
  }

  const std::uint32_t elapsed = timeGetTime() - g_modeFadeTick;
  if (elapsed < 255) {
    DrawAlphaRectangle(0, 0, g_renderer->viewport.width, g_renderer->viewport.height, (255 - elapsed) << 24);
  }
}

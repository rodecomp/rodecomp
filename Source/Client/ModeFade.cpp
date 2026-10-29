#include "Client/ModeFade.h"
#include "Contents/Sound/SoundMgr.h"
#include "Render/Primitives/AlphaRectangle.h"

#include <timeapi.h>

// GLOBAL: gl_24680
std::uint32_t g_modeFadeTick = 0;

// OFFSET: 0x00AD6740
// FUNCTION: fn_17842
void ModeFade::Draw() {
  if (this->requested != 0) {
    g_modeFadeTick = timeGetTime();
    g_soundMgr->FadeIn();
    this->requested = 0;
  }

  const std::uint32_t elapsed = timeGetTime() - g_modeFadeTick;
  if (elapsed < 255) {
    DrawAlphaRectangle(
      this->primitives, this->queues, 0, 0, this->screenWidth, this->screenHeight, (255 - elapsed) << 24, this->depthAlpha, this->depthStart,
      this->depthEnd
    );
  }
}

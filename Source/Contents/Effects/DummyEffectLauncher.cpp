#include "Contents/Effects/DummyEffectLauncher.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4C1C0
// FUNCTION: fn_24281
bool CDummyEffectLauncher::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  return frame <= static_cast<float>(this->lifeFrames);
}

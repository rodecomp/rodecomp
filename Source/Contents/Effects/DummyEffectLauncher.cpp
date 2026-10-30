#include "Contents/Effects/DummyEffectLauncher.h"

#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4C1C0
// FUNCTION: fn_24281
bool CDummyEffectLauncher::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  return frame <= static_cast<float>(this->lifeFrames);
}

// OFFSET: 0x00E50A60
// FUNCTION: fn_24296
void CDummyEffectLauncher::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  if ((static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) != 14) {
    this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const vector3d* position = reinterpret_cast<const vector3d*>(b);
  this->render.position.x = position->x;
  this->render.position.y = position->y;
  this->render.position.z = position->z;
  this->render.visible = 0;
  this->render.unknown_008C = timeGetTime();
  if (g_effectMgr->Spawn(0, d, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0) != nullptr) {
    return;
  }

  CRagEffect* effect = this->LaunchEffect(d, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  const std::int64_t life = static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f);
  if (life == 0) {
    this->lifeFrames = 300;
    return;
  }

  this->lifeFrames = f;
  if (life == 333) {
    vector3d value{static_cast<float>(static_cast<std::int64_t>(i) << 32 | static_cast<std::uint32_t>(h)), 0.0f, 0.0f};
    if (effect != nullptr) {
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&value), 0, 0, 0, 0);
    }
  }
}

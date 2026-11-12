#include "Contents/Effects/Core/DummyEffectLauncher.h"
#include "Contents/Actors/ActorEnums.h"

#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/RagEffect.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4C1C0
// FUNCTION: fn_24281
bool CDummyEffectLauncher::OnProcess() {
  const float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.motionStartTime) / 24.0);
  const float limit = static_cast<float>(this->lifeFrames);
  return !(frame > limit);
}

// OFFSET: 0x00E50A60
// FUNCTION: fn_24296
__declspec(safebuffers) void CDummyEffectLauncher::SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) {
  if (message != AM_SETPOS) {
    this->EffectActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    return;
  }

  const vector3d* position = reinterpret_cast<const vector3d*>(static_cast<std::intptr_t>(b));
  this->render.position.x = position->x;
  this->render.position.y = position->y;
  this->render.position.z = position->z;
  this->render.visible = 0;
  this->render.motionStartTime = timeGetTime();
  if (g_effectMgr->Spawn(0, static_cast<int>(c), this->render.position, 0, -1, 0, 0) != nullptr) {
    return;
  }

  CRagEffect* effect = this->LaunchEffect(static_cast<int>(c), vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  if (d != 0) {
    this->lifeFrames = static_cast<int>(d);
    if (d == 333) {
      vector3d value;
      value.y = 0.0f;
      value.z = 0.0f;
      value.x = static_cast<float>(e);
      if (effect != nullptr) {
        effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<std::intptr_t>(&value), 0, 0, 0, 0);
      }
    }
  } else {
    this->lifeFrames = 300;
  }
}

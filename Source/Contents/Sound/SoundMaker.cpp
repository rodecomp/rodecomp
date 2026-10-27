#include "Contents/Sound/SoundMaker.h"

#include "Contents/Sound/SoundMgr.h"
#include "Resource/World/WorldResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <cmath>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00DA1E30
// FUNCTION: fn_22363
CSoundMaker::CSoundMaker(ResourceMgr& resources, JobNames& names, SoundMakerContext& context)
    : CRenderObject(resources, names), m_context(&context) {
  this->file.clear();
  this->volume = 100.0f;
  this->range = 1.0f;
  this->width = 3;
  this->height = 3;
  this->radius = SquareRoot(18.0f);
  this->lastTick = timeGetTime() - 4000u;
}

// OFFSET: 0x00DA3A60
// FUNCTION: fn_22406
bool CSoundMaker::Update() {
  if (this->width == 0) {
    return true;
  }

  const CRenderObject* player = this->m_context->localPlayer;
  if (!player) {
    return true;
  }

  const float distance = this->HorizontalDistance(player->render.position[0], player->render.position[2]);
  if (distance - (this->radius + this->range) > 130.0f) {
    return true;
  }

  const float deltaX = player->render.position[0] - this->render.position[0];
  const float deltaZ = player->render.position[2] - this->render.position[2];
  float signX = 1.0f;
  float signZ = 1.0f;
  if (deltaX < 0.0f && deltaZ >= 0.0f) {
    signX = -1.0f;
  } else if (deltaX < 0.0f && deltaZ < 0.0f) {
    signX = -1.0f;
    signZ = -1.0f;
  } else if (deltaX >= 0.0f && deltaZ < 0.0f) {
    signZ = -1.0f;
  }

  const float slope = deltaZ / std::fmax(deltaX, 0.001f);
  float offsetX;
  float offsetZ;
  if (!(static_cast<float>(this->height / this->width) >= std::fabs(slope))) {
    offsetZ = static_cast<float>(this->height) * signZ * 0.5f;
    offsetX = offsetZ / std::fmax(slope, 0.001f);
  } else {
    offsetX = static_cast<float>(this->width) * signX * 0.5f;
    offsetZ = slope * offsetX;
  }

  const float x = this->render.position[0] + offsetX;
  const float z = this->render.position[2] + offsetZ;
  if (player->HorizontalDistance(x, z) - this->range > 75.0f || this->file.empty()) {
    return true;
  }

  if (timeGetTime() > this->lastTick + this->cycle) {
    this->lastTick = timeGetTime();
    g_soundMgr->PlayWave(
      this->file.c_str(), x - player->render.position[0], 0.0f, z - player->render.position[2], TruncateToInt(this->range),
      TruncateToInt(this->range / 6.0f), this->volume, 0
    );
  }
  return true;
}

// OFFSET: 0x00DA4960
// FUNCTION: fn_22417
void CSoundMaker::Draw(int) {
}

// OFFSET: 0x00DA56E0
// FUNCTION: fn_22428
void CSoundMaker::SendObjectMessage(
  CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int
) {
  if (message != 14 || a != 0) {
    CRenderObject::SendObjectMessage(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const WorldSoundRecord& sound = *reinterpret_cast<const WorldSoundRecord*>(b);
  this->file = sound.file;
  this->volume = sound.volume;
  this->width = sound.width;
  this->height = sound.height;
  this->range = sound.range;
  this->render.position[0] = sound.position[0];
  this->render.position[1] = sound.position[1];
  this->render.position[2] = sound.position[2];
  this->radius = SquareRoot(static_cast<float>(this->width * this->width + this->height * this->height));
  this->cycle = static_cast<std::int64_t>(sound.cycle * 1000.0f);
  this->lastTick = timeGetTime() - this->cycle;
}

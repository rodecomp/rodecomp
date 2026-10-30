#include "Contents/Effects/StealEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B4CDF0
// FUNCTION: fn_18969
void CSteal::SpawnParticles() {
  for (int i = 0; i < 10; ++i) {
    const int life = this->unknown_002C;
    const int yaw = std::max(rand() % 360, 0);
    const int pitch = std::max(rand() % 100, 0) - 50;

    const float frameDelay = static_cast<float>(std::max(rand() % 50, 0) + 50) / 100.0f;
    const float shrink = -(frameDelay / static_cast<float>(life)) * g_depthAverageHalf;

    const float lift = -static_cast<float>(std::max(rand() % 90, 0) + 60) / 100.0f;
    const float liftStep = -(lift / static_cast<float>(life)) * 2.0f;

    const float scale = static_cast<float>(std::max(rand() % 100, 0) + 100) / 100.0f;
    const float scaleStep = -(scale / static_cast<float>(life));
    const int fadeStart = life - life / 3;

    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(yaw))));
    rotation.AppendXRotation(GetRadian(static_cast<int>(static_cast<float>(pitch))));

    const float distance = static_cast<float>(std::max(rand() % 4, 0) + 4);

    vector3d& offset = this->offsets[i];
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;

    const float velocityX = 0.0f * rotation.values[3] + 0.0f * rotation.values[0] + distance * rotation.values[6] + rotation.values[9] + 0.0f;
    const float velocityY = 0.0f * rotation.values[4] + 0.0f * rotation.values[1] + distance * rotation.values[7] + rotation.values[10] - 10.0f;
    const float velocityZ = 0.0f * rotation.values[5] + 0.0f * rotation.values[2] + distance * rotation.values[8] + rotation.values[11] + 0.0f;

    CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    particle->unknown_0004 = this->id;
    particle->unknown_000C = i;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->scale = scale;
    particle->frameDelay = frameDelay;

    float* params = &particle->unknown_0054;
    params[0] = shrink;
    params[1] = static_cast<float>(yaw);
    params[2] = static_cast<float>(pitch);
    params[3] = lift;
    params[4] = liftStep;
    params[5] = scaleStep;
    params[6] = static_cast<float>(fadeStart);

    particle->velocity.x = velocityX;
    particle->velocity.y = velocityY;
    particle->velocity.z = velocityZ;
    particle->SetSprite("이팩트\\particle7.spr");
    particle->SetAction("이팩트\\particle7.act");
    particle->alpha = 255;

    this->particles.push_back(particle);
  }
}

// OFFSET: 0x00B4D200
// FUNCTION: fn_18971
void CSteal::Close() {
  for (CSpriteParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B4D270
// FUNCTION: fn_18972
void CSteal::Slot01() {
  this->SpawnParticles();
}

// OFFSET: 0x00B4D280
// FUNCTION: fn_18973
bool CSteal::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B4D290
// FUNCTION: fn_18974
void CSteal::Slot03() {
  for (CSpriteParticle* particle : this->particles) {
    const vector3d origin = *reinterpret_cast<const vector3d*>(this->position);
    const float scale = particle->scale;
    float* params = &particle->unknown_0054;
    const float scaleStep = params[5];
    const int index = particle->unknown_000C;
    const vector3d velocity = particle->velocity;
    float lift = params[3] + params[4];
    const float distance = std::max(0.0f, params[0] + particle->frameDelay);

    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(params[1])));

    vector3d& offset = this->offsets[index];
    offset.y = lift + offset.y;
    offset.x = 0.0f * rotation.values[3] + 0.0f * rotation.values[0] + distance * rotation.values[6] + rotation.values[9] + offset.x;
    offset.z = 0.0f * rotation.values[5] + 0.0f * rotation.values[2] + distance * rotation.values[8] + rotation.values[11] + offset.z;

    const float x = velocity.x + origin.x + offset.x;
    const float y = velocity.y + origin.y + offset.y;
    const float z = velocity.z + origin.z + offset.z;
    const float newScale = scaleStep + scale;

    if (lift > 0.0f) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (y > mode->scene->terrain->Grid().Height(x, z)) {
        lift /= -1.5f;
      }
    }

    ++particle->unknown_00E8;
    particle->scale = newScale;
    particle->frameDelay = distance;
    params[3] = lift;
    particle->position.x = x;
    particle->position.y = y;
    particle->position.z = z;
  }
}

// OFFSET: 0x00B4D720
// FUNCTION: fn_18975
void CSteal::Slot04() {
  for (std::list<CSpriteParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->sub_B40F30(1);
  }
}

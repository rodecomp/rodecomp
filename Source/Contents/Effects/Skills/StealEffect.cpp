#include "Contents/Effects/Skills/StealEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B4CCA0
// FUNCTION: fn_18967
CSteal::~CSteal() {
}

// OFFSET: 0x00B4CDF0
// FUNCTION: fn_18969
void CSteal::SpawnParticles() {
  for (int i = 0; i < 10; ++i) {
    const int life = this->effectLifetime;
    const int yaw = std::max(0, rand() % 360);
    const int pitch = std::max(0, rand() % 100) - 50;

    const float frameDelay = static_cast<float>(std::max(0, rand() % 50) + 50) / 100.0f;
    const float shrink = -(frameDelay / static_cast<float>(life)) * g_depthAverageHalf;

    const float lift = -static_cast<float>(std::max(0, rand() % 90) + 60) / 100.0f;
    const float liftStep = -(lift / static_cast<float>(life)) * 2.0f;

    const float scale = static_cast<float>(std::max(0, rand() % 100) + 100) / 100.0f;
    const float scaleStep = -(scale / static_cast<float>(life));
    const int fadeStart = life - life / 3;

    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(static_cast<float>(yaw))));
    rotation.AppendXRotation(GetRadian(static_cast<int>(static_cast<float>(pitch))));

    const float distance = static_cast<float>(std::max(0, rand() % 4) + 4);

    vector3d& offset = this->offsets[i];
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;

    const float velocityX = 0.0f * rotation.values[3] + 0.0f * rotation.values[0] + distance * rotation.values[6] + rotation.values[9] + 0.0f;
    const float velocityY = 0.0f * rotation.values[4] + 0.0f * rotation.values[1] + distance * rotation.values[7] + rotation.values[10] - 10.0f;
    const float velocityZ = 0.0f * rotation.values[5] + 0.0f * rotation.values[2] + distance * rotation.values[8] + rotation.values[11] + 0.0f;

    CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 1));
    particle->effectId = this->id;
    particle->columnIndex = i;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->scale = scale;
    particle->frameDelay = frameDelay;

    float* params = &particle->animationParameter;
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
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B4D270
// FUNCTION: fn_18972
void CSteal::Open() {
  this->SpawnParticles();
}

// OFFSET: 0x00B4D280
// FUNCTION: fn_18973
bool CSteal::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B4D290
// FUNCTION: fn_18974
void CSteal::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    const vector3d origin = *reinterpret_cast<const vector3d*>(this->position);
    const float scale = particle->scale;
    float* params = &particle->animationParameter;
    const float scaleStep = params[5];
    const int index = particle->columnIndex;
    const float velocityX = particle->velocity.x;
    const float velocityY = particle->velocity.y;
    const float velocityZ = particle->velocity.z;
    float lift = params[3] + params[4];
    const float distance = std::max(0.0f, params[0] + particle->frameDelay);

    matrix rotation;
    rotation.MakeYRotation(GetRadian(static_cast<int>(params[1])));

    vector3d& offset = this->offsets[index];
    offset.x = 0.0f * rotation.values[3] + 0.0f * rotation.values[0] + distance * rotation.values[6] + rotation.values[9] + offset.x;
    offset.y = lift + offset.y;
    offset.z = 0.0f * rotation.values[5] + 0.0f * rotation.values[2] + distance * rotation.values[8] + rotation.values[11] + offset.z;

    const float x = velocityX + origin.x + offset.x;
    const float y = velocityY + origin.y + offset.y;
    const float z = velocityZ + origin.z + offset.z;
    const float newScale = scaleStep + scale;

    if (lift > 0.0f) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (y > mode->scene->terrain->Height(x, z)) {
        lift /= -1.5f;
      }
    }

    ++particle->elapsedFrames;
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
void CSteal::Render() {
  for (std::list<CSpriteParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    (*it)->Draw(1);
  }
}

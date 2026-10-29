#include "Contents/Effects/TalismanMarkEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/Vector3d.h"

// OFFSET: 0x0072E210
// FUNCTION: fn_07504
bool CTalismanMarkEffect::Slot06() {
  if (this->ActorExists() == true && !this->particles.empty()) {
    return true;
  }

  return false;
}

// OFFSET: 0x0072E3F0
// FUNCTION: fn_07506
void CTalismanMarkEffect::UpdateParticle(CEZ2STRParticle* particle) {
  if (!particle) {
    return;
  }

  const vector3d center = {this->position[0], this->position[1], this->position[2]};
  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  const float viewX = view[0] * center.x + view[3] * center.y + view[6] * center.z + view[9];
  const float viewY = view[1] * center.x + view[4] * center.y + view[7] * center.z + view[10];
  const float viewZ = view[2] * center.x + view[5] * center.y + view[8] * center.z + view[11];

  const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
  int angle = particle->unknown_004C;
  if (particle->unknown_00A8 == 1) {
    angle = (angle + 1) % 360;
    particle->unknown_004C = angle;
  }

  const float radians = static_cast<float>(angle) / 180.0f * 3.141592f;
  const float offsetY = ClientSin(radians) * 8.5f - 20.0f;
  const float offsetX = ClientCos(radians) * 8.5f;
  const float x = offsetX + viewX;
  const float y = offsetY + viewY;
  const float z = viewZ + 0.0f;
  const float worldX = x * inverse[0] + y * inverse[3] + z * inverse[6] + inverse[9];
  const float worldY = x * inverse[1] + y * inverse[4] + z * inverse[7] + inverse[10];
  const float worldZ = x * inverse[2] + y * inverse[5] + z * inverse[8] + inverse[11];
  particle->position.x = worldX;
  particle->position.y = worldY;
  particle->position.z = worldZ;
  particle->Process(0.0f, 0.0f, 0.0f);
}

// OFFSET: 0x0072E5F0
// FUNCTION: fn_07507
void CTalismanMarkEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CEZ2STRParticle* particle = *it;
    if (particle) {
      particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

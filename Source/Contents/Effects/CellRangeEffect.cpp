#include "Contents/Effects/CellRangeEffect.h"
#include "Contents/Effects/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <cstring>
#include <string>

// OFFSET: 0x00B43050
// FUNCTION: fn_18874
CCellRangeEffect::CCellRangeEffect() : CEffect(-1), cellSize(5.0f) {
}

// OFFSET: 0x00B431B0
// FUNCTION: fn_18876
void CCellRangeEffect::SetCellTexture(CParticle* particle) {
  if (!particle) {
    return;
  }

  const int frame = (particle->elapsedFrames + particle->columnIndex) / 10 % 3 + 1;
  std::string name;
  switch (this->id) {
  case EF_KO_ZENKAI_WATER:
    name = FormatOwnedString("effect\\water-%d.bmp", frame);
    break;
  case EF_KO_ZENKAI_LAND:
    name = FormatOwnedString("effect\\stone-%d.bmp", frame);
    break;
  case EF_KO_ZENKAI_FIRE:
    name = FormatOwnedString("effect\\fire-%d.bmp", frame);
    break;
  case EF_KO_ZENKAI_WIND:
    name = FormatOwnedString("effect\\lightning-%d.bmp", frame);
    break;
  case EF_KO_MAKIBISHI:
    name = "effect\\makibishi.bmp";
    particle->scale = 2.5f;
    particle->destinationBlend = 6;
    break;
  default:
    break;
  }

  if (!name.empty()) {
    particle->SetTexture(name);
  }
}

// OFFSET: 0x00B433E0
// FUNCTION: fn_18877
void CCellRangeEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B43420
// FUNCTION: fn_18878
void CCellRangeEffect::Open() {
  CSkill* owner = static_cast<CSkill*>(FindRenderObject(this->actor));
  if (!owner) {
    return;
  }

  const int range = owner->range;
  for (int row = -range; row <= range; ++row) {
    for (int column = -range; column <= range; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->effectId = this->id;
        particle->columnIndex = column;
        particle->rowIndex = row;
        particle->groupIndex = 1;
        particle->alpha = 255;
        particle->SetColor(255, 255, 255);
        particle->SetTexture("effect\\torch_violet08.bmp");
        particle->scale = 8.0f;
        this->particles.push_back(particle);
      }
    }
  }
}

// OFFSET: 0x00B43520
// FUNCTION: fn_18880
void CCellRangeEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = particle->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    if (particle) {
      vector3d position;
      std::memcpy(&position, this->position, sizeof(position));
      position.x = static_cast<float>(particle->columnIndex) * this->cellSize + position.x;
      position.z = static_cast<float>(particle->rowIndex) * this->cellSize + position.z;
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      position.y = mode->scene->terrain->Height(position.x, position.z) - 1.2f;
      particle->position.x = position.x;
      particle->position.y = position.y;
      particle->position.z = position.z;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.MakeXRotation(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch / 180.0f * 3.141592f);
      rotation.AppendXRotation(3.141592f);

      matrix scale;
      std::memcpy(scale.values, g_identityMatrix, sizeof(scale.values));
      scale.SetScale(particle->scale, particle->scale, 1.0f);

      matrix result;
      result.Compose(scale, rotation);
      particle->transform = result;
    }

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B43920
// FUNCTION: fn_18881
void CCellRangeEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    if (particle) {
      this->SetCellTexture(particle);
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

#include "Contents/Effects/CellRangeEffect.h"

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

  const int frame = (particle->unknown_00E8 + particle->unknown_000C) / 10 % 3 + 1;
  std::string name;
  switch (this->id) {
  case 992:
    name = FormatOwnedString("effect\\water-%d.bmp", frame);
    break;
  case 993:
    name = FormatOwnedString("effect\\stone-%d.bmp", frame);
    break;
  case 994:
    name = FormatOwnedString("effect\\fire-%d.bmp", frame);
    break;
  case 995:
    name = FormatOwnedString("effect\\lightning-%d.bmp", frame);
    break;
  case 1007:
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
  for (CParticle* particle : this->particles) {
    FreeParticle(particle);
  }

  this->particles.clear();
}

// OFFSET: 0x00B43420
// FUNCTION: fn_18878
void CCellRangeEffect::Slot01() {
  CSkill* owner = static_cast<CSkill*>(FindRenderObject(this->actor));
  if (!owner) {
    return;
  }

  const int range = owner->range;
  for (int row = -range; row <= range; ++row) {
    for (int column = -range; column <= range; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->unknown_0004 = this->id;
        particle->unknown_000C = column;
        particle->unknown_0010 = row;
        particle->unknown_0008 = 1;
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
void CCellRangeEffect::Slot03() {
  for (CParticle* particle : this->particles) {
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      vector3d position;
      std::memcpy(&position, this->position, sizeof(position));
      position.x = static_cast<float>(particle->unknown_000C) * this->cellSize + position.x;
      position.z = static_cast<float>(particle->unknown_0010) * this->cellSize + position.z;
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      position.y = mode->scene->terrain->Grid().Height(position.x, position.z) - 1.2f;
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

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B43920
// FUNCTION: fn_18881
void CCellRangeEffect::Slot04() {
  for (CParticle* particle : this->particles) {
    if (particle) {
      this->SetCellTexture(particle);
      particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

#include "Contents/Effects/RichManKim.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x0070B160
// FUNCTION: fn_07114
void CRichManKim::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }
  this->particles.clear();
}

// OFFSET: 0x0070B1A0
// FUNCTION: fn_07115
void CRichManKim::Slot01() {
  CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
  if (!particle) {
    return;
  }

  particle->SetTexture("effect\\pocket.bmp");
  particle->unknown_0004 = this->id;
  particle->unknown_000C = 0;
  particle->unknown_0010 = 0;
  particle->unknown_0008 = 1;
  particle->alpha = 200;
  particle->unknown_00C4 = 0;
  particle->unknown_004C = 0;
  particle->scale = 1.0f;
  *reinterpret_cast<float*>(particle->unknown_0058) = 0.0f;
  particle->unknown_0054 = -5.0f - static_cast<float>(rand() % 5);
  particle->SetColor(200, 200, 200);
  InitParticleQuad(particle->vertices);
  this->angleIndex = rand() % 359;
  this->particles.push_back(particle);
}

// OFFSET: 0x0070B370
// FUNCTION: fn_07116
bool CRichManKim::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x0070B380
// FUNCTION: fn_07117
void CRichManKim::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];

    if (particle) {
      int index = ++this->angleIndex;
      if (index > 360) {
        this->angleIndex = 0;
        index = 0;
      }

      float& width = *reinterpret_cast<float*>(particle->unknown_0058);
      const float height = static_cast<float>(static_cast<double>(particle->unknown_0054) - static_cast<double>(g_cosTable[index]) * 0.08);
      particle->unknown_0054 = height;
      const float stretch = static_cast<float>(static_cast<double>(g_cosTable[index]) * 0.003 + static_cast<double>(width));
      width = stretch;

      matrix rotation;
      std::memcpy(rotation.values, g_identityMatrix, sizeof(rotation.values));
      rotation.AppendXRotation(3.141592f);
      matrix scaling;
      std::memcpy(scaling.values, g_identityMatrix, sizeof(scaling.values));
      scaling.SetScale(stretch + 2.0f, 5.0f, 1.0f);
      matrix translation;
      translation.IsIdentity();
      translation.MakeTranslate(0.0f, height, 0.0f);

      matrix transform;
      transform.Compose(scaling, rotation);
      const matrix combined = transform;
      transform.Compose(combined, translation);
      particle->transform = transform;
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x0070B780
// FUNCTION: fn_07118
void CRichManKim::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

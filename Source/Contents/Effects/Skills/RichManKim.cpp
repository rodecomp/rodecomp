#include "Contents/Effects/Skills/RichManKim.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
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
void CRichManKim::Open() {
  CParticle* particle = CreateParticle(&this->positionVector, 5);
  if (!particle) {
    return;
  }

  particle->SetTexture("effect\\pocket.bmp");
  particle->effectId = this->id;
  particle->columnIndex = 0;
  particle->rowIndex = 0;
  particle->groupIndex = 1;
  particle->alpha = 200;
  particle->angle = 0;
  particle->phaseParameter = 0;
  particle->scale = 1.0f;
  float* values = &particle->animationParameter;
  values[0] = -5.0f - static_cast<float>(rand() % 5);
  values[1] = 0.0f;
  particle->SetColor(200, 200, 200);
  particle->InitParticleQuad(particle->vertices);
  this->angleIndex = rand() % 359;
  this->particles.push_back(particle);
}

// OFFSET: 0x0070B370
// FUNCTION: fn_07116
bool CRichManKim::IsAlive() {
  return this->ActorExists();
}

// OFFSET: 0x0070B380
// FUNCTION: fn_07117
void CRichManKim::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = (*it)->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];
    if (particle) {
      int index = this->angleIndex + 1;
      this->angleIndex = index;
      if (index > 360) {
        this->angleIndex = 0;
        index = 0;
      }

      float& width = particle->animationParameters[0];
      const float height = static_cast<float>(static_cast<double>(particle->animationParameter) - static_cast<double>(g_cosTable[index]) * 0.08);
      const float stretch = static_cast<float>(static_cast<double>(g_cosTable[index]) * 0.003 + static_cast<double>(width));
      particle->animationParameter = height;
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

    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x0070B780
// FUNCTION: fn_07118
void CRichManKim::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

#include "Contents/Effects/Skills/HealEffect.h"
#include "Contents/Effects/Core/EffectTypes.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <string>

// INLINED
static int ClampedRandom(int range) {
  const int value = rand() % range;
  return value >= 0 ? value : 0;
}

// OFFSET: 0x00B67310
// FUNCTION: fn_19065
CHealEffect::CHealEffect() : CEffect(-1) {
  this->spawnTick = 0;
}

// OFFSET: 0x00B67530
// FUNCTION: fn_19067
void CHealEffect::AddSparks(int kind) {
  for (int index = 0; index < 4; ++index) {
    CParticle* particle = CreateParticle(&this->positionVector, 5);
    if (!particle) {
      continue;
    }

    particle->effectId = this->id;
    particle->columnIndex = index;
    particle->rowIndex = 0;
    particle->groupIndex = 1;
    particle->SetTexture("effect\\pok1.tga");
    particle->alpha = 0;
    particle->angle = 0;
    particle->angleParameter = 0;

    const int driftZ = ClampedRandom(13);
    const int driftX = ClampedRandom(13);
    particle->offsetY = 0.0f;
    particle->offsetX = static_cast<float>(driftX - 6);
    particle->offsetZ = static_cast<float>(driftZ - 6);
    particle->SetColor(220, 250, 220);
    particle->InitParticleQuad(particle->vertices);
    this->sparkParticles.push_back(particle);

    switch (kind) {
    case 0:
    case 3:
    case 6:
      particle->scale = static_cast<float>(ClampedRandom(10)) * 0.1f + 2.0f;
      particle->animationParameter = static_cast<float>(ClampedRandom(40)) * 0.01f + 0.4f;
      break;
    case 1:
      particle->scale = static_cast<float>(ClampedRandom(10)) * 0.1f + 2.0f;
      particle->animationParameter = static_cast<float>(ClampedRandom(40)) * 0.01f + 0.6f;
      break;
    case 2:
      particle->scale = static_cast<float>(ClampedRandom(40)) * 0.1f + 0.4f;
      particle->animationParameter = static_cast<float>(ClampedRandom(40)) * 0.01f + 0.4f;
      break;
    case 4:
      particle->scale = static_cast<float>(ClampedRandom(10)) * 0.1f + 2.0f;
      particle->animationParameter = static_cast<float>(ClampedRandom(40)) * 0.01f + 0.3f;
      break;
    case 7:
      particle->scale = static_cast<float>(ClampedRandom(10)) * 0.1f + 2.0f;
      particle->animationParameter = static_cast<float>(ClampedRandom(40)) * 0.01f + 0.4f;
      particle->SetColor(0, 255, 255);
      break;
    case 8:
      particle->scale = static_cast<float>(ClampedRandom(10)) * 0.1f + 2.0f;
      particle->animationParameter = static_cast<float>(ClampedRandom(40)) * 0.01f + 0.4f;
      particle->SetColor(255, 255, 0);
      break;
    }
  }
}

// OFFSET: 0x00B678F0
// FUNCTION: fn_19068
void CHealEffect::AddColumn(int kind) {
  const float randomHeight = static_cast<float>(ClampedRandom(20)) + 40.0f;
  for (int row = 0, angle = 0; angle < 360; ++row, angle += 180) {
    const int randomAngle = (std::max)(rand() % 360, 0);
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = row;
      particle->groupIndex = 1;
      particle->angleParameter = 360;
      particle->alpha = 0;
      particle->phaseParameter = 90;
      particle->animationParameter = 0.0f;

      float& baseAngle = particle->angle;
      switch (kind) {
      case 0:
        particle->SetTexture("effect\\ring_white.tga");
        particle->speedParameter = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 60;
        break;
      case 1:
        particle->SetTexture("effect\\ring_white.tga");
        particle->speedParameter = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 40;
        break;
      case 2:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->speedParameter = 90.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.1f + 4.2f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 40;
        break;
      case 3:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->speedParameter = randomHeight;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>(ClampedRandom(100)) * 0.005f + 4.5f + static_cast<float>(row) * 0.2f;
        particle->SetColor(255, 255, 255);
        this->columnLifetime = 30;
        break;
      case 4:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->speedParameter = static_cast<float>((std::max)(rand() % 20, 0) * row) + 40.0f;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>(row) * 0.2f + 4.6f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 60;
        break;
      case 5:
        particle->SetTexture("effect\\ring_blue.tga");
        particle->speedParameter = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(100, 100, 255);
        this->columnLifetime = 40;
        break;
      case 6:
        particle->SetTexture("effect\\ring_white.tga");
        particle->speedParameter = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(115, 25, 115);
        this->columnLifetime = 60;
        break;
      case 7:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->speedParameter = static_cast<float>((std::max)(rand() % 20, 0) * row) + 40.0f;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>(row) * 0.2f + 4.6f;
        particle->SetColor(0, 255, 255);
        this->columnLifetime = 60;
        break;
      case 8:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->speedParameter = static_cast<float>((std::max)(rand() % 20, 0) * row) + 40.0f;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>(row) * 0.2f + 4.6f;
        particle->SetColor(255, 255, 0);
        this->columnLifetime = 60;
        break;
      }

      particle->InitParticleQuad(particle->vertices);
      this->columnParticles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B67D10
// FUNCTION: fn_19069
void CHealEffect::AddCone(int kind) {
  for (int row = 0, tilt = 50; tilt > 46; ++row, tilt -= 2) {
    const int randomAngle = (std::max)(rand() % 360, 0);
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(&this->positionVector, 5);
      if (!particle) {
        continue;
      }

      particle->effectId = this->id;
      particle->columnIndex = segment;
      particle->rowIndex = row;
      particle->groupIndex = 1;
      particle->angleParameter = 360;
      particle->alpha = 0;
      particle->animationParameter = 0.0f;

      float& baseAngle = particle->angle;
      switch (kind) {
      case 0:
        particle->SetTexture("effect\\ring_white.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->speedParameter = 12.0f;
        particle->scale = static_cast<float>(row) * 0.2f + 5.0f;
        particle->phaseParameter = tilt;
        particle->SetColor(140, 210, 140);
        this->coneLifetime = 60;
        break;
      case 1:
        particle->SetTexture("effect\\ring_white.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->speedParameter = 18.0f;
        particle->scale = static_cast<float>(row) * 0.2f + 5.0f;
        particle->phaseParameter = tilt;
        particle->SetColor(140, 210, 140);
        this->coneLifetime = 40;
        break;
      case 2:
        particle->SetTexture("effect\\alpha_down.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->speedParameter = 10.0f;
        particle->scale = static_cast<float>(row) * 0.1f + 5.3f;
        particle->phaseParameter = tilt - 4;
        particle->SetColor(140, 210, 140);
        this->coneLifetime = 30;
        break;
      case 5:
        particle->SetTexture("effect\\ring_blue.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->speedParameter = 18.0f;
        particle->scale = static_cast<float>(row) * 0.2f + 5.0f;
        particle->phaseParameter = tilt;
        particle->SetColor(100, 100, 255);
        this->coneLifetime = 40;
        break;
      }

      particle->InitParticleQuad(particle->vertices);
      this->coneParticles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B67F90
// FUNCTION: fn_19070
void CHealEffect::UpdateParticleVertices(CParticle* particle) {
  if (!particle) {
    return;
  }

  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (std::string(gameMode->mapName) == "ecl_fild01.rsw" && particle->alpha > 100) {
    particle->alpha = 100;
  }

  const float scale = particle->scale;
  EffectPoint points[4];

  const int angle = particle->angleParameter;
  for (int i = 0; i < 4; ++i) {
    points[i].x = scale * g_cosTable[(angle + i * 90) % 360];
    points[i].y = 0.0f;
    points[i].z = scale * g_sinTable[(angle + i * 90) % 360];
  }

  this->RotateToCamera(points);

  const float baseX = particle->offsetX;
  const float baseY = particle->offsetY;
  const float baseZ = particle->offsetZ;

  particle->vertices[0].x = baseX + points[3].x;
  particle->vertices[0].y = baseY + points[3].y;
  particle->vertices[0].z = baseZ + points[3].z;

  particle->vertices[1].x = baseX + points[0].x;
  particle->vertices[1].y = baseY + points[0].y;
  particle->vertices[1].z = baseZ + points[0].z;

  particle->vertices[2].x = baseX + points[2].x;
  particle->vertices[2].y = baseY + points[2].y;
  particle->vertices[2].z = baseZ + points[2].z;

  particle->vertices[3].x = baseX + points[1].x;
  particle->vertices[3].y = baseY + points[1].y;
  particle->vertices[3].z = baseZ + points[1].z;
}

// OFFSET: 0x00B686D0
// FUNCTION: fn_19071
void CHealEffect::Close() {
  for (auto it = this->columnParticles.begin(); it != this->columnParticles.end(); ++it) {
    FreeParticle(*it);
  }
  this->columnParticles.clear();

  for (auto it = this->coneParticles.begin(); it != this->coneParticles.end(); ++it) {
    FreeParticle(*it);
  }
  this->coneParticles.clear();

  for (auto it = this->sparkParticles.begin(); it != this->sparkParticles.end(); ++it) {
    FreeParticle(*it);
  }
  this->sparkParticles.clear();
}

// OFFSET: 0x00B69050
// FUNCTION: fn_19075
bool CHealEffect::IsAlive() {
  return !this->IsFinished();
}

// OFFSET: 0x00B68770
// FUNCTION: fn_19072
void CHealEffect::Open() {
  this->effectLifetime = 100;
}

// OFFSET: 0x00B68780
// FUNCTION: fn_19073
void CHealEffect::InitEvents() {
  switch (this->id) {
  case EF_ABSORBSPIRITS:
    this->AddEvent(0, "effect\\흡기.wav", 0, 0);
    break;
  case EF_HEAL:
  case EF_HEAL2:
  case EF_HEAL3:
  case EF_HEAL4:
    this->AddEvent(0, "_heal_effect.wav", 0, 0);
    break;
  }
}

// OFFSET: 0x00B68830
// FUNCTION: fn_19074
void CHealEffect::SpawnParticles() {
  switch (this->id) {
  case EF_HEAL:
    if (this->spawnTick == 0) {
      this->AddColumn(4);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(4);
    }
    break;
  case EF_HEAL2:
    if (this->spawnTick == 0) {
      this->AddColumn(0);
      this->AddCone(0);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(0);
    }
    break;
  case EF_HEAL3:
    if (this->spawnTick == 0) {
      this->AddColumn(3);
    }
    if (this->spawnTick <= 25 && this->spawnTick % 6 == 0) {
      this->AddSparks(3);
    }
    break;
  case EF_ABSORBSPIRITS:
    if (this->spawnTick == 0) {
      this->AddColumn(5);
      this->AddCone(5);
    }
    break;
  case EF_HEAL4:
    if (this->spawnTick == 0) {
      this->AddColumn(1);
      this->AddCone(1);
    }
    if (this->spawnTick == 1) {
      this->AddColumn(2);
      this->AddCone(2);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(1);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 9 == 2) {
      this->AddSparks(2);
    }
    break;
  case 1113:
    if (this->spawnTick == 0) {
      this->AddColumn(6);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(6);
    }
    break;
  case 1143:
    if (this->spawnTick == 0) {
      this->AddColumn(7);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(7);
    }
    break;
  case 1144:
    if (this->spawnTick == 0) {
      this->AddColumn(8);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(8);
    }
    break;
  }

  ++this->spawnTick;
}

// OFFSET: 0x00B69160
// FUNCTION: fn_19076
void CHealEffect::OnProcess() {
  this->SpawnParticles();

  for (auto it = this->columnParticles.begin(); it != this->columnParticles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      const int age = particle->elapsedFrames;
      int alpha = particle->alpha;
      if (this->columnLifetime < 0 || this->columnLifetime > age) {
        if (age < 16) {
          alpha = (std::min)(alpha + 5, 180);
        }
      } else {
        const int faded = alpha - 2;
        alpha = faded >= 0 ? faded : 0;
      }
      particle->alpha = alpha;

      const int segment = particle->columnIndex;
      float height = particle->speedParameter * 0.25f * g_sinTable[((4 - particle->rowIndex) * age + segment * 34) % 360] + particle->speedParameter * 0.75f;
      if (particle->elapsedFrames <= 90) {
        height *= g_sinTable[particle->elapsedFrames];
      }
      height = (std::max)(height, 0.0f);

      const float baseAngle = particle->angle;
      const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * segment) + baseAngle) % 360;
      const float tiltCos = g_cosTable[particle->phaseParameter];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float tiltSin = g_sinTable[particle->phaseParameter];

      EffectPoint points[2];
      const float bottomX = particle->scale * cosine;
      const float bottomZ = particle->scale * sine;
      particle->vertices[3].x = bottomX;
      particle->vertices[3].y = 0.0f;
      particle->vertices[3].z = bottomZ;

      const float topX = bottomX + height * tiltCos * cosine;
      const float topY = 0.0f - height * tiltSin;
      const float topZ = bottomZ + height * tiltCos * sine;
      particle->vertices[1].x = topX;
      particle->vertices[1].y = topY;
      particle->vertices[1].z = topZ;

      if (segment != 0) {
        particle->vertices[0].x = this->columnEdge[1].x;
        particle->vertices[0].y = this->columnEdge[1].y;
        particle->vertices[0].z = this->columnEdge[1].z;
        particle->vertices[2].x = this->columnEdge[0].x;
        particle->vertices[2].y = this->columnEdge[0].y;
        particle->vertices[2].z = this->columnEdge[0].z;
      }

      this->columnEdge[1].x = topX;
      this->columnEdge[1].y = topY;
      this->columnEdge[0].x = bottomX;
      this->columnEdge[0].y = 0.0f;
      this->columnEdge[0].z = bottomZ;
      this->columnEdge[1].z = topZ;
    }
    ++particle->elapsedFrames;
  }

  for (auto it = this->coneParticles.begin(); it != this->coneParticles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    if (particle) {
      const int age = particle->elapsedFrames;
      int alpha = particle->alpha;
      if (this->coneLifetime < 0 || this->coneLifetime > age) {
        if (age < 16) {
          alpha = (std::min)(alpha + 5, 180);
        }
      } else {
        const int faded = alpha - 2;
        alpha = faded >= 0 ? faded : 0;
      }
      particle->alpha = alpha;

      const int segment = particle->columnIndex;
      float height = particle->speedParameter * 0.25f * g_sinTable[((segment * 17 + age) * 2) % 360] + particle->speedParameter * 0.75f;
      if (age <= 90) {
        height *= g_sinTable[age];
      }
      height = (std::max)(height, 0.0f);

      const float baseAngle = particle->angle;
      const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->angleParameter) / 20.0f) * segment) + baseAngle) % 360;
      const float tiltCos = g_cosTable[particle->phaseParameter];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float tiltSin = g_sinTable[particle->phaseParameter];

      EffectPoint points[2];
      const float bottomX = particle->scale * cosine;
      const float bottomZ = particle->scale * sine;
      particle->vertices[3].y = 0.0f;
      particle->vertices[3].x = bottomX;
      particle->vertices[3].z = bottomZ;

      const float topY = 0.0f - height * tiltSin;
      const float topX = height * tiltCos * cosine + bottomX;
      const float topZ = bottomZ + height * tiltCos * sine;
      particle->vertices[1].y = topY;
      particle->vertices[1].x = topX;
      particle->vertices[1].z = topZ;

      if (segment != 0) {
        particle->vertices[0].x = this->coneEdge[1].x;
        particle->vertices[0].y = this->coneEdge[1].y;
        particle->vertices[0].z = this->coneEdge[1].z;
        particle->vertices[2].x = this->coneEdge[0].x;
        particle->vertices[2].y = this->coneEdge[0].y;
        particle->vertices[2].z = this->coneEdge[0].z;
      }

      this->coneEdge[0].x = bottomX;
      this->coneEdge[0].y = 0.0f;
      this->coneEdge[0].z = bottomZ;
      this->coneEdge[1].x = topX;
      this->coneEdge[1].y = topY;
      this->coneEdge[1].z = topZ;
    }
    ++particle->elapsedFrames;
  }

  for (auto it = this->sparkParticles.begin(); it != this->sparkParticles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& particlePosition = (*it)->position;
    particlePosition.x = this->position[0];
    particlePosition.y = this->position[1];
    particlePosition.z = this->position[2];
    if (particle) {
      particle->angleParameter += particle->angleParameter - 5 >= 0 ? -5 : 355;

      int alpha = particle->alpha;
      if (particle->elapsedFrames < 11) {
        alpha += 15;
      } else if (this->id != 312) {
        const int segment = particle->columnIndex;
        if ((5 - segment) * 7 == particle->elapsedFrames || 36 - segment * 7 == particle->elapsedFrames) {
          particle->SetTexture("effect\\pok3.tga");
        } else {
          particle->SetTexture("effect\\pok1.tga");
        }
      }
      const int faded = alpha - 3;
      particle->alpha = faded >= 0 ? faded : 0;

      particle->offsetX -= 0.0f;
      particle->offsetY -= particle->animationParameter;
      particle->offsetZ -= 0.0f;
    }
    this->UpdateParticleVertices(particle);
    ++particle->elapsedFrames;
  }
}

// OFFSET: 0x00B69BC0
// FUNCTION: fn_19077
void CHealEffect::Render() {
  for (auto it = this->columnParticles.begin(); it != this->columnParticles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->columnIndex > 0) {
      const int segment = particle->columnIndex;
      ParticleQuad quad;
      quad.billboard = false;
      quad.unused = false;
      quad.depth = true;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(segment - 1) / 21.0f;
      quad.right = static_cast<float>(segment) / 21.0f;
      particle->Render(quad);
    }
  }

  for (auto it = this->coneParticles.begin(); it != this->coneParticles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->columnIndex > 0) {
      const int segment = particle->columnIndex;
      ParticleQuad quad;
      quad.billboard = false;
      quad.unused = false;
      quad.depth = true;
      quad.top = 0.0f;
      quad.bottom = 1.0f;
      quad.left = static_cast<float>(segment - 1) / 21.0f;
      quad.right = static_cast<float>(segment) / 21.0f;
      particle->Render(quad);
    }
  }

  for (auto it = this->sparkParticles.begin(); it != this->sparkParticles.end(); ++it) {
    if (*it) {
      (*it)->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
    }
  }
}

// OFFSET: 0x00B69E30
// FUNCTION: fn_19078
void CHealEffect::RotateToCamera(EffectPoint* points) {
  const int yaw = 360 - static_cast<int>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw);
  const int pitch = (450 - static_cast<int>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch)) % 360;

  for (int i = 0; i < 4; ++i) {
    const float z = points[i].z;
    points[i].z = z * g_cosTable[pitch] - points[i].y * g_sinTable[pitch];
    points[i].y = points[i].y * g_cosTable[pitch] + z * g_sinTable[pitch];
  }

  for (int i = 0; i < 4; ++i) {
    const float x = points[i].x;
    points[i].x = x * g_cosTable[yaw] - points[i].z * g_sinTable[yaw];
    points[i].z = points[i].z * g_cosTable[yaw] + x * g_sinTable[yaw];
  }
}

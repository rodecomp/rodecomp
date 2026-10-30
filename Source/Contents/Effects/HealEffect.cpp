#include "Contents/Effects/HealEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>
#include <string>

// OFFSET: 0x00B67310
// FUNCTION: fn_19065
CHealEffect::CHealEffect() : CEffect(-1), columnEdge{}, coneEdge{} {
  this->spawnTick = 0;
}

// OFFSET: 0x00B67530
// FUNCTION: fn_19067
void CHealEffect::AddSparks(int kind) {
  for (int index = 0; index < 4; ++index) {
    CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
    if (!particle) {
      continue;
    }

    particle->unknown_0004 = this->id;
    particle->unknown_000C = index;
    particle->unknown_0010 = 0;
    particle->unknown_0008 = 1;
    particle->SetTexture("effect\\pok1.tga");
    particle->alpha = 0;
    particle->unknown_00C4 = 0;
    particle->unknown_0050 = 0;

    const int driftZ = (std::max)(rand() % 13, 0);
    const int driftX = (std::max)(rand() % 13, 0);
    particle->unknown_00D4 = 0.0f;
    particle->unknown_00D0 = static_cast<float>(driftX - 6);
    particle->unknown_00D8 = static_cast<float>(driftZ - 6);
    particle->SetColor(220, 250, 220);
    InitParticleQuad(particle->vertices);
    this->sparkParticles.push_back(particle);

    switch (kind) {
    case 0:
    case 3:
    case 6:
      particle->scale = static_cast<float>((std::max)(rand() % 10, 0)) * 0.1f + 2.0f;
      particle->unknown_0054 = static_cast<float>((std::max)(rand() % 40, 0)) * 0.01f + 0.4f;
      break;
    case 1:
      particle->scale = static_cast<float>((std::max)(rand() % 10, 0)) * 0.1f + 2.0f;
      particle->unknown_0054 = static_cast<float>((std::max)(rand() % 40, 0)) * 0.01f + 0.6f;
      break;
    case 2:
      particle->scale = static_cast<float>((std::max)(rand() % 40, 0)) * 0.1f + 0.4f;
      particle->unknown_0054 = static_cast<float>((std::max)(rand() % 40, 0)) * 0.01f + 0.4f;
      break;
    case 4:
      particle->scale = static_cast<float>((std::max)(rand() % 10, 0)) * 0.1f + 2.0f;
      particle->unknown_0054 = static_cast<float>((std::max)(rand() % 40, 0)) * 0.01f + 0.3f;
      break;
    case 7:
      particle->scale = static_cast<float>((std::max)(rand() % 10, 0)) * 0.1f + 2.0f;
      particle->unknown_0054 = static_cast<float>((std::max)(rand() % 40, 0)) * 0.01f + 0.4f;
      particle->SetColor(0, 255, 255);
      break;
    case 8:
      particle->scale = static_cast<float>((std::max)(rand() % 10, 0)) * 0.1f + 2.0f;
      particle->unknown_0054 = static_cast<float>((std::max)(rand() % 40, 0)) * 0.01f + 0.4f;
      particle->SetColor(255, 255, 0);
      break;
    }
  }
}

// OFFSET: 0x00B678F0
// FUNCTION: fn_19068
void CHealEffect::AddColumn(int kind) {
  const float randomHeight = static_cast<float>((std::max)(rand() % 20, 0)) + 40.0f;
  for (int row = 0, angle = 0; angle < 360; ++row, angle += 180) {
    const int randomAngle = (std::max)(rand() % 360, 0);
    for (int segment = 0; segment < 21; ++segment) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = row;
      particle->unknown_0008 = 1;
      particle->unknown_0050 = 360;
      particle->alpha = 0;
      particle->unknown_004C = 90;
      particle->unknown_0054 = 0.0f;

      float& baseAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      switch (kind) {
      case 0:
        particle->SetTexture("effect\\ring_white.tga");
        particle->unknown_00C0 = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 60;
        break;
      case 1:
        particle->SetTexture("effect\\ring_white.tga");
        particle->unknown_00C0 = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 40;
        break;
      case 2:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->unknown_00C0 = 90.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.1f + 4.2f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 40;
        break;
      case 3:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->unknown_00C0 = randomHeight;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>((std::max)(rand() % 100, 0)) * 0.005f + 4.5f + static_cast<float>(row) * 0.2f;
        particle->SetColor(255, 255, 255);
        this->columnLifetime = 30;
        break;
      case 4:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->unknown_00C0 = static_cast<float>((std::max)(rand() % 20, 0) * row) + 40.0f;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>(row) * 0.2f + 4.6f;
        particle->SetColor(140, 210, 140);
        this->columnLifetime = 60;
        break;
      case 5:
        particle->SetTexture("effect\\ring_blue.tga");
        particle->unknown_00C0 = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(100, 100, 255);
        this->columnLifetime = 40;
        break;
      case 6:
        particle->SetTexture("effect\\ring_white.tga");
        particle->unknown_00C0 = 60.0f;
        baseAngle = static_cast<float>(angle);
        particle->scale = static_cast<float>(row) * 0.4f + 4.6f;
        particle->SetColor(115, 25, 115);
        this->columnLifetime = 60;
        break;
      case 7:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->unknown_00C0 = static_cast<float>((std::max)(rand() % 20, 0) * row) + 40.0f;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>(row) * 0.2f + 4.6f;
        particle->SetColor(0, 255, 255);
        this->columnLifetime = 60;
        break;
      case 8:
        particle->SetTexture("effect\\alpha_down.tga");
        particle->unknown_00C0 = static_cast<float>((std::max)(rand() % 20, 0) * row) + 40.0f;
        baseAngle = static_cast<float>(randomAngle);
        particle->scale = static_cast<float>(row) * 0.2f + 4.6f;
        particle->SetColor(255, 255, 0);
        this->columnLifetime = 60;
        break;
      }

      InitParticleQuad(particle->vertices);
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
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (!particle) {
        continue;
      }

      particle->unknown_0004 = this->id;
      particle->unknown_000C = segment;
      particle->unknown_0010 = row;
      particle->unknown_0008 = 1;
      particle->unknown_0050 = 360;
      particle->alpha = 0;
      particle->unknown_0054 = 0.0f;

      float& baseAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
      switch (kind) {
      case 0:
        particle->SetTexture("effect\\ring_white.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->unknown_00C0 = 12.0f;
        particle->scale = static_cast<float>(row) * 0.2f + 5.0f;
        particle->unknown_004C = tilt;
        particle->SetColor(140, 210, 140);
        this->coneLifetime = 60;
        break;
      case 1:
        particle->SetTexture("effect\\ring_white.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->unknown_00C0 = 18.0f;
        particle->scale = static_cast<float>(row) * 0.2f + 5.0f;
        particle->unknown_004C = tilt;
        particle->SetColor(140, 210, 140);
        this->coneLifetime = 40;
        break;
      case 2:
        particle->SetTexture("effect\\alpha_down.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->unknown_00C0 = 10.0f;
        particle->scale = static_cast<float>(row) * 0.1f + 5.3f;
        particle->unknown_004C = tilt - 4;
        particle->SetColor(140, 210, 140);
        this->coneLifetime = 30;
        break;
      case 5:
        particle->SetTexture("effect\\ring_blue.tga");
        baseAngle = static_cast<float>(randomAngle);
        particle->unknown_00C0 = 18.0f;
        particle->scale = static_cast<float>(row) * 0.2f + 5.0f;
        particle->unknown_004C = tilt;
        particle->SetColor(100, 100, 255);
        this->coneLifetime = 40;
        break;
      }

      InitParticleQuad(particle->vertices);
      this->coneParticles.push_back(particle);
    }
  }
}

// OFFSET: 0x00B67F90
// FUNCTION: fn_19070
void CHealEffect::sub_B67F90(CParticle* particle) {
  if (!particle) {
    return;
  }

  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (std::string(gameMode->mapName) == "ecl_fild01.rsw") {
    if (particle->alpha > 100) {
      particle->alpha = 100;
    }
  }

  const float scale = particle->scale;
  vector3d points[4] = {};

  const int angle = particle->unknown_0050;
  points[0].x = scale * g_cosTable[angle % 360];
  points[0].y = 0.0f;
  points[0].z = scale * g_sinTable[angle % 360];

  points[1].x = scale * g_cosTable[(angle + 90) % 360];
  points[1].y = 0.0f;
  points[1].z = scale * g_sinTable[(angle + 90) % 360];

  points[2].x = scale * g_cosTable[(angle + 180) % 360];
  points[2].y = 0.0f;
  points[2].z = scale * g_sinTable[(angle + 180) % 360];

  points[3].x = scale * g_cosTable[(angle + 270) % 360];
  points[3].y = 0.0f;
  points[3].z = scale * g_sinTable[(angle + 270) % 360];

  this->sub_B69E30(points);

  const float baseX = particle->unknown_00D0;
  const float baseY = particle->unknown_00D4;
  const float baseZ = particle->unknown_00D8;

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
bool CHealEffect::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B68770
// FUNCTION: fn_19072
void CHealEffect::Slot01() {
  this->unknown_002C = 100;
}

// OFFSET: 0x00B68780
// FUNCTION: fn_19073
void CHealEffect::Slot12() {
  switch (this->id) {
  case 253:
    this->AddEvent(0, "effect\\흡기.wav", 0, 0);
    break;
  case 312:
  case 313:
  case 320:
  case 325:
    this->AddEvent(0, "_heal_effect.wav", 0, 0);
    break;
  }
}

// OFFSET: 0x00B68830
// FUNCTION: fn_19074
void CHealEffect::SpawnParticles() {
  switch (this->id) {
  case 253:
    if (this->spawnTick == 0) {
      this->AddColumn(5);
      this->AddCone(5);
    }
    break;
  case 312:
    if (this->spawnTick == 0) {
      this->AddColumn(4);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(4);
    }
    break;
  case 313:
    if (this->spawnTick == 0) {
      this->AddColumn(0);
      this->AddCone(0);
    }
    if (this->spawnTick <= 50 && this->spawnTick % 6 == 0) {
      this->AddSparks(0);
    }
    break;
  case 320:
    if (this->spawnTick == 0) {
      this->AddColumn(3);
    }
    if (this->spawnTick <= 25 && this->spawnTick % 6 == 0) {
      this->AddSparks(3);
    }
    break;
  case 325:
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
void CHealEffect::Slot03() {
  this->SpawnParticles();

  for (auto it = this->columnParticles.begin(); it != this->columnParticles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      const int age = particle->unknown_00E8;
      int alpha = particle->alpha;
      if (this->columnLifetime < 0 || age < this->columnLifetime) {
        if (age < 16) {
          alpha = (std::min)(alpha + 5, 180);
        }
      } else {
        alpha = (std::max)(alpha - 2, 0);
      }
      particle->alpha = alpha;

      const int segment = particle->unknown_000C;
      float height = particle->unknown_00C0 * 0.25f * g_sinTable[((4 - particle->unknown_0010) * age + segment * 34) % 360] + particle->unknown_00C0 * 0.75f;
      if (particle->unknown_00E8 < 91) {
        height *= g_sinTable[particle->unknown_00E8];
      }
      height = (std::max)(height, 0.0f);

      const float baseAngle = *reinterpret_cast<const float*>(&particle->unknown_00C4);
      const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * segment) + baseAngle) % 360;
      const float tiltCos = g_cosTable[particle->unknown_004C];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float tiltSin = g_sinTable[particle->unknown_004C];

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
    ++particle->unknown_00E8;
  }

  for (auto it = this->coneParticles.begin(); it != this->coneParticles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      const int age = particle->unknown_00E8;
      int alpha = particle->alpha;
      if (this->coneLifetime < 0 || age < this->coneLifetime) {
        if (age < 16) {
          alpha = (std::min)(alpha + 5, 180);
        }
      } else {
        alpha = (std::max)(alpha - 2, 0);
      }
      particle->alpha = alpha;

      const int segment = particle->unknown_000C;
      float height = particle->unknown_00C0 * 0.25f * g_sinTable[((segment * 17 + age) * 2) % 360] + particle->unknown_00C0 * 0.75f;
      if (age < 91) {
        height *= g_sinTable[age];
      }
      height = (std::max)(height, 0.0f);

      const float baseAngle = *reinterpret_cast<const float*>(&particle->unknown_00C4);
      const int angle = static_cast<int>(static_cast<float>(static_cast<int>(static_cast<float>(particle->unknown_0050) / 20.0f) * segment) + baseAngle) % 360;
      const float tiltCos = g_cosTable[particle->unknown_004C];
      const float cosine = g_cosTable[angle];
      const float sine = g_sinTable[angle];
      const float tiltSin = g_sinTable[particle->unknown_004C];

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
    ++particle->unknown_00E8;
  }

  for (auto it = this->sparkParticles.begin(); it != this->sparkParticles.end(); ++it) {
    CParticle* particle = *it;
    particle->position.x = this->position[0];
    particle->position.y = this->position[1];
    particle->position.z = this->position[2];
    if (particle) {
      particle->unknown_0050 += particle->unknown_0050 - 5 >= 0 ? -5 : 355;

      int alpha = particle->alpha;
      if (particle->unknown_00E8 < 11) {
        alpha += 15;
      } else if (this->id != 312) {
        const int segment = particle->unknown_000C;
        if ((5 - segment) * 7 == particle->unknown_00E8 || 36 - segment * 7 == particle->unknown_00E8) {
          particle->SetTexture("effect\\pok3.tga");
        } else {
          particle->SetTexture("effect\\pok1.tga");
        }
      }
      particle->alpha = (std::max)(alpha - 3, 0);

      particle->unknown_00D0 -= 0.0f;
      particle->unknown_00D4 -= particle->unknown_0054;
      particle->unknown_00D8 -= 0.0f;
    }
    this->sub_B67F90(particle);
    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B69BC0
// FUNCTION: fn_19077
void CHealEffect::Slot04() {
  for (auto it = this->columnParticles.begin(); it != this->columnParticles.end(); ++it) {
    CParticle* particle = *it;
    if (particle && particle->unknown_000C > 0) {
      const int segment = particle->unknown_000C;
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
    if (particle && particle->unknown_000C > 0) {
      const int segment = particle->unknown_000C;
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
void CHealEffect::sub_B69E30(vector3d* points) {
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

#include "Contents/Effects/HealEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"

#include <algorithm>
#include <cstdlib>

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

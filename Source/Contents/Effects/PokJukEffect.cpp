#include "Contents/Effects/PokJukEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

// OFFSET: 0x00B6F400
// FUNCTION: fn_19120
void CPokJukEffect::SpawnParticles() {
  int row = 0;
  for (int rowStart = 0; rowStart < 640; rowStart += 160) {
    for (int column = 0; column < 160; ++column) {
      CParticle* particle = CreateParticle(reinterpret_cast<const vector3d*>(this->position), 5);
      if (particle) {
        particle->unknown_0004 = this->id;
        particle->unknown_000C = column;
        particle->unknown_0010 = row;
        particle->unknown_0008 = 1;
        particle->unknown_00E8 = -300 - std::max(rand() % 50, 0);
        *reinterpret_cast<float*>(&particle->unknown_00C4) = static_cast<float>(std::max(rand() % 360, 0));
        particle->alpha = 0;
        particle->unknown_00C0 = 2.0f;
        particle->scale = static_cast<float>(std::max(rand() % 100, 0)) * 0.004f + 0.4f;
        particle->unknown_0050 = std::max(rand() % 360, 0);
        particle->unknown_004C = std::max(rand() % 360, 0);
        InitParticleQuad(particle->vertices);
        this->particles.push_back(particle);

        const int index = column + rowStart;
        switch (std::max(rand() % 5, 0)) {
        case 0:
          particle->SetColor(70, 70, 255);
          break;
        case 1:
          particle->SetColor(255, 70, 70);
          break;
        case 2:
          particle->SetColor(70, 255, 70);
          break;
        case 3:
          particle->SetColor(255, 255, 90);
          break;
        case 4:
          particle->SetColor(255, 70, 255);
          break;
        }

        this->states[index] = 0;
        this->beats[index] = 0;
        this->delays[index] = std::max(rand() % 4, 0) * 20;
        for (int corner = 0; corner < 4; ++corner) {
          this->trails[index][corner].x = -15.0f;
          this->trails[index][corner].y = 0.0f;
          this->trails[index][corner].z = 0.0f;
        }
      }
    }

    ++row;
  }
}

// OFFSET: 0x00B6F770
// FUNCTION: fn_19121
void CPokJukEffect::Close() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B6F7B0
// FUNCTION: fn_19122
void CPokJukEffect::Slot01() {
  this->SpawnParticles();
}

// OFFSET: 0x00B6F7C0
// FUNCTION: fn_19123
bool CPokJukEffect::Slot06() {
  return this->ActorExists();
}

// OFFSET: 0x00B6F7D0
// FUNCTION: fn_19124
void CPokJukEffect::Slot03() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;
    vector3d& position = particle->position;
    position.x = this->position[0];
    position.y = this->position[1];
    position.z = this->position[2];

    if (particle != nullptr) {
      const int index = particle->unknown_0010 * 160 + particle->unknown_000C;
      const int age = particle->unknown_00E8;

      if (age > 0) {
        vector3d* trail = this->trails[index];

        if (age % 3 == 0) {
          trail[3] = trail[2];
          trail[2] = trail[1];
          trail[1] = trail[0];
        }

        const int delay = this->delays[index];

        if (age >= delay + 160 && age <= delay + 180) {
          const char beat = static_cast<char>(age % 20);

          if (beat == 15 || beat == 19) {
            this->beats[index] = 1;
          } else {
            this->beats[index] = beat == 17 ? 2 : 0;
          }
        } else {
          this->beats[index] = 0;
        }

        float& spinAngle = *reinterpret_cast<float*>(&particle->unknown_00C4);
        spinAngle = static_cast<float>((static_cast<int>(spinAngle) + 5) % 360);

        if (this->states[index] == 0) {
          trail[0].y -= 0.25f;

          if (age > 154) {
            this->states[index] = 1;
            particle->alpha = 250;
          }
        } else if (this->states[index] == 1) {
          const int heading = particle->unknown_0050;
          const float speed = particle->scale;
          trail[0].y += speed * g_sinTable[heading];
          const float flat = speed * g_cosTable[heading];
          trail[0].x += flat * g_cosTable[particle->unknown_004C];
          trail[0].z += flat * g_sinTable[particle->unknown_004C];
          trail[0].y += 0.02f;
          particle->scale = speed * 0.98f;

          const int alpha = particle->alpha;
          const int coin = std::rand() % 2;

          if ((coin < 0 ? 0 : coin) == 0) {
            const int faded = alpha - 2;
            particle->alpha = faded < 0 ? 0 : faded;
          } else {
            const int faded = alpha - 1;
            particle->alpha = faded < 0 ? 0 : faded;
          }
        }

        if (age > 2000) {
          particle->unknown_00E8 = 0;
          const int heading = std::rand() % 360;
          particle->unknown_004C = heading < 0 ? 0 : heading;
          const int spread = std::rand() % 100;
          particle->scale = static_cast<float>(spread < 0 ? 0 : spread) * 0.004f + 0.4f;
          trail[0].x = -15.0f;
          trail[0].y = 0.0f;
          trail[0].z = 0.0f;
          this->states[index] = 0;
        }
      }
    }

    ++particle->unknown_00E8;
  }
}

// OFFSET: 0x00B6FEA0
// FUNCTION: fn_19125
void CPokJukEffect::Slot04() {
  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    CParticle* particle = *it;

    if (particle && particle->alpha > 0) {
      float shrink = 0.0f;
      const int index = particle->unknown_0010 * 160 + particle->unknown_000C;

      for (int layer = 0; layer < 2; ++layer) {
        vector3d corners[4]{};
        const float radius = particle->unknown_00C0 - shrink;

        for (int corner = 0; corner < 4; ++corner) {
          const int angle = static_cast<int>(static_cast<float>(corner * 90) + *reinterpret_cast<float*>(&particle->unknown_00C4)) % 360;
          corners[corner].x = radius * g_cosTable[angle];
          corners[corner].y = 0.0f;
          corners[corner].z = radius * g_sinTable[angle];
        }

        this->FaceCamera(corners);

        const vector3d& base = this->trails[index][layer];

        for (int corner = 0; corner < 4; ++corner) {
          corners[corner].x += base.x;
          corners[corner].y += base.y;
          corners[corner].z += base.z;
        }

        particle->vertices[0].x = corners[3].x;
        particle->vertices[0].y = corners[3].y;
        particle->vertices[0].z = corners[3].z;
        particle->vertices[1].x = corners[0].x;
        particle->vertices[1].y = corners[0].y;
        particle->vertices[1].z = corners[0].z;
        particle->vertices[2].x = corners[2].x;
        particle->vertices[2].y = corners[2].y;
        particle->vertices[2].z = corners[2].z;
        particle->vertices[3].x = corners[1].x;
        particle->vertices[3].y = corners[1].y;
        particle->vertices[3].z = corners[1].z;

        switch (this->beats[index]) {
        case 0:
          particle->SetTexture("effect\\pok1.tga");
          break;
        case 1:
          particle->SetTexture("effect\\pok3.tga");
          break;
        case 2:
          particle->SetTexture("effect\\pok2.tga");
          break;
        }

        const int alpha = particle->alpha;
        particle->destinationBlend = 2;
        particle->sourceBlend = 5;
        int shown = alpha / (layer + 1);

        if (this->beats[index] != 0) {
          shown /= 2;
        }

        particle->alpha = shown;
        particle->Render(false, 0.0f, 1.0f, 0.0f, 1.0f, false, true);
        particle->alpha = alpha;
        shrink += layer == 0 ? 0.5f : 0.25f;
      }
    }
  }
}

// OFFSET: 0x00B70540
// FUNCTION: fn_19126
void CPokJukEffect::FaceCamera(vector3d* corners) {
  const int yaw = 360 - static_cast<int>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw);
  const int pitch = (450 - static_cast<int>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch)) % 360;

  for (int corner = 0; corner < 4; ++corner) {
    const float y = corners[corner].y;
    const float z = corners[corner].z;
    corners[corner].z = z * g_cosTable[pitch] - y * g_sinTable[pitch];
    corners[corner].y = y * g_cosTable[pitch] + z * g_sinTable[pitch];
  }

  for (int corner = 0; corner < 4; ++corner) {
    const float x = corners[corner].x;
    const float z = corners[corner].z;
    corners[corner].x = x * g_cosTable[yaw] - z * g_sinTable[yaw];
    corners[corner].z = z * g_cosTable[yaw] + x * g_sinTable[yaw];
  }
}

#include "Contents/Effects/JupitelThunderStormEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

// INLINED
static bool ParticleAlive(const CParticle* particle) {
  return particle->life == 0.0f || static_cast<float>(particle->unknown_00E8) < particle->life * 60.0f;
}

// OFFSET: 0x00705F80
// FUNCTION: fn_07040
CJupitelThunderStormEffect::CJupitelThunderStormEffect() : CEffect(2366), particle(nullptr), step{}, duration(10) {
}

// OFFSET: 0x00706110
// FUNCTION: fn_07042
void CJupitelThunderStormEffect::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      FreeParticle(*it);
    }
  }
  this->particles.clear();
}

// OFFSET: 0x00706180
// FUNCTION: fn_07043
void CJupitelThunderStormEffect::Slot01() {
  const float dy = this->sourcePosition[1] - this->position[1];
  const float dx = this->sourcePosition[0] - this->position[0];
  const float dz = this->sourcePosition[2] - this->position[2];
  const float speed = SquareRoot(dy * dy + dx * dx + dz * dz) / static_cast<float>(this->duration);

  this->step.x = this->sourcePosition[0] - this->position[0];
  this->step.y = this->sourcePosition[1] - this->position[1];
  this->step.z = this->sourcePosition[2] - this->position[2];
  const float scale = 1.0f / SquareRoot(this->step.x * this->step.x + this->step.y * this->step.y + this->step.z * this->step.z);
  this->step.x = scale * this->step.x * speed;
  this->step.y = scale * this->step.y * speed;
  this->step.z = scale * this->step.z * speed;

  const float angle = ScreenAngle(this->position, this->sourcePosition, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  CEZ2STRParticle* ball = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  this->particle = ball;
  if (ball) {
    ball->Load(std::string("HYPER_NOVICE\\HN_JUPITEL_THUNDER_STORM\\jupitel_thunder_storm_ball\\jupitel_thunder_storm_ball.str"));
    static_cast<CEZ2STRParticle*>(this->particle)->m_angle = angle;
  }

  int corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
  int index = 0;
  const int rings = (this->unknown_00B0 - 1) / 4;
  const int lastDelay = ((this->unknown_00B0 - 1) / 2 + 6) * 150;
  for (int delay = 80; delay <= lastDelay; delay += 80, ++index) {
    CEZ2STRParticle* bolt = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
    if (!bolt) {
      continue;
    }

    const int range = (rings * 10 + 25) / 2;
    const int corner = index % 4;
    int offsetX = 0;
    float offsetZ;
    if (range == 0) {
      offsetZ = static_cast<float>(corners[corner][1]) * g_depthAverageHalf;
    } else {
      const int randomZ = rand() % range;
      offsetX = std::max(rand() % range, 0);
      offsetZ = (static_cast<float>(std::max(randomZ, 0)) + g_depthAverageHalf) * static_cast<float>(corners[corner][1]);
    }

    bolt->Load(FormatOwnedString("HYPER_NOVICE\\HN_JUPITEL_THUNDER_STORM\\jupitel_thunder_storm\\jupitel_thunder_storm_%02d.str", std::max(rand() % 3, 0)));
    bolt->position.z = offsetZ + this->sourcePosition[2];
    bolt->position.y = this->sourcePosition[1] - 15.0f;
    bolt->position.x = static_cast<float>(corners[corner][0]) * (static_cast<float>(offsetX) + g_depthAverageHalf) + this->sourcePosition[0];
    bolt->m_delay = delay;
    this->particles.push_back(bolt);

    if (corner == 0) {
      this->AddEvent(0, "effect\\HYPER_NOVICE\\hu_jupitel_thunder_storm.wav", delay / 16 + 1, 0);
    }
  }
}

// OFFSET: 0x007065F0
// FUNCTION: fn_07044
bool CJupitelThunderStormEffect::Slot06() {
  if (this->particle && ParticleAlive(this->particle)) {
    return true;
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it && ParticleAlive(*it)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00706690
// FUNCTION: fn_07045
void CJupitelThunderStormEffect::Slot03() {
  if (CParticle* particle = this->particle) {
    if (ParticleAlive(particle)) {
      vector3d position;
      if (this->frame < this->duration) {
        position.x = particle->position.x + this->step.x;
        position.y = particle->position.y + this->step.y;
        position.z = particle->position.z + this->step.z;
      } else {
        position.x = this->sourcePosition[0];
        position.y = this->sourcePosition[1];
        position.z = this->sourcePosition[2];
      }
      particle->position = position;
      static_cast<CEZ2STRParticle*>(this->particle)->Process(0.0f, 0.0f, 0.0f);
    } else {
      FreeParticle(particle);
      this->particle = nullptr;
    }
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      static_cast<CEZ2STRParticle*>(*it)->Process(0.0f, 0.0f, 0.0f);
    }
  }
}

// OFFSET: 0x007067C0
// FUNCTION: fn_07046
void CJupitelThunderStormEffect::Slot04() {
  if (this->particle) {
    static_cast<CEZ2STRParticle*>(this->particle)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }

  for (std::vector<CParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      static_cast<CEZ2STRParticle*>(*it)->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
    }
  }
}

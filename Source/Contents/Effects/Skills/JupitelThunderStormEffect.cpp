#include "Contents/Effects/Skills/JupitelThunderStormEffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdlib>
#include <string>

// GLOBAL: gl_26012
const volatile float g_jupitelThunderStormHeightOffset = 15.0f;

// INLINED
static __forceinline bool ParticleAlive(const CParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}

// INLINED
template <typename... Args>
void LoadFormatted(CEZ2STRParticle* particle, const char* format, Args... args) {
  particle->Load(FormatOwnedString(format, args...));
}

// INLINED
static vector3d Sum(const vector3d& left, const vector3d& right) {
  return vector3d{left.x + right.x, left.y + right.y, left.z + right.z};
}

// OFFSET: 0x00705F80
// FUNCTION: fn_07040
CJupitelThunderStormEffect::CJupitelThunderStormEffect() : CEffect(2366), particle(nullptr), step({0.0f, 0.0f, 0.0f}), duration(10) {
}

// OFFSET: 0x00706030
// FUNCTION: fn_46250
__declspec(noinline) CJupitelThunderStormEffect::~CJupitelThunderStormEffect() {
}

// OFFSET: 0x00706110
// FUNCTION: fn_07042
void CJupitelThunderStormEffect::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }

  for (std::vector<CEZ2STRParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      FreeParticle(*it);
    }
  }
  this->particles.clear();
}

// OFFSET: 0x00706180
// FUNCTION: fn_07043
void CJupitelThunderStormEffect::Open() {
  const float* origin = this->position;
  const float* target = this->sourcePosition;
  const float dx = target[0] - origin[0];
  const float dy = target[1] - origin[1];
  const float dz = target[2] - origin[2];
  const float distance = SquareRoot(dy * dy + dx * dx + dz * dz);
  const float speed = distance / static_cast<float>(this->duration);

  this->step = vector3d{target[0] - origin[0], target[1] - origin[1], target[2] - origin[2]};
  const float scale = 1.0f / SquareRoot(this->step.x * this->step.x + this->step.y * this->step.y + this->step.z * this->step.z);
  this->step = this->step * scale * speed;

  const float angle = ScreenAngle(origin, target, g_modeMgr.GetGameMode()->camera->frame.view.values);
  CEZ2STRParticle* ball = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  this->particle = ball;
  if (ball) {
    ball->Load(std::string("HYPER_NOVICE\\HN_JUPITEL_THUNDER_STORM\\jupitel_thunder_storm_ball\\jupitel_thunder_storm_ball.str"));
    static_cast<CEZ2STRParticle*>(this->particle)->m_angle = angle;
  }

  int corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
  int index = 0;
  const int lastDelay = ((this->effectLevel - 1) / 2 + 6) * 150;
  const int rings = (this->effectLevel - 1) / 4;
  for (int time = 80; time <= lastDelay; time += 80, ++index) {
    const int delay = time;
    CEZ2STRParticle* bolt = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
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
      const int randomX = rand() % range;
      offsetX = randomX >= 0 ? randomX : 0;
      offsetZ = (static_cast<float>(randomZ >= 0 ? randomZ : 0) + g_depthAverageHalf) * static_cast<float>(corners[corner][1]);
    }

    const int variant = rand() % 3;
    LoadFormatted(bolt, "HYPER_NOVICE\\HN_JUPITEL_THUNDER_STORM\\jupitel_thunder_storm\\jupitel_thunder_storm_%02d.str", variant >= 0 ? variant : 0);
    bolt->position.y = target[1] - g_jupitelThunderStormHeightOffset;
    bolt->position.x = static_cast<float>(corners[corner][0]) * (static_cast<float>(offsetX) + g_depthAverageHalf) + target[0];
    bolt->position.z = offsetZ + target[2];
    bolt->m_delay = delay;
    this->particles.push_back(bolt);

    if (corner == 0) {
      this->AddEvent(EFFECT_EVENT_SOUND, "effect\\HYPER_NOVICE\\hu_jupitel_thunder_storm.wav", delay / 16 + 1, 0);
    }
  }
}

// OFFSET: 0x007065F0
// FUNCTION: fn_07044
bool CJupitelThunderStormEffect::IsAlive() {
  if (this->particle && ParticleAlive(this->particle) == true) {
    return true;
  }

  const auto alive = std::find_if(this->particles.begin(), this->particles.end(),
                                  [](const CParticle* particle) { return particle && ParticleAlive(particle); });
  return this->particles.end() != alive;
}

// OFFSET: 0x00706690
// FUNCTION: fn_07045
void CJupitelThunderStormEffect::OnProcess() {
  if (CParticle* particle = this->particle) {
    if (ParticleAlive(particle) != true) {
      FreeParticle(particle);
      this->particle = nullptr;
    } else {
      particle->position = this->frame < this->duration ? Sum(particle->position, this->step) : this->sourcePositionVector;
      static_cast<CEZ2STRParticle*>(this->particle)->Process(0.0f, 0.0f, 0.0f);
    }
  }

  for (std::vector<CEZ2STRParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->Process(0.0f, 0.0f, 0.0f);
    }
  }
}

// OFFSET: 0x007067C0
// FUNCTION: fn_07046
void CJupitelThunderStormEffect::Render() {
  if (this->particle) {
    static_cast<CEZ2STRParticle*>(this->particle)->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }

  for (std::vector<CEZ2STRParticle*>::iterator it = this->particles.begin(); it != this->particles.end(); ++it) {
    if (*it) {
      (*it)->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
    }
  }
}

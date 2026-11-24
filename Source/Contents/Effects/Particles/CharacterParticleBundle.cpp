#include "Contents/Effects/Particles/CharacterParticleBundle.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/RadialGrid.h"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

  // INLINED
  static __forceinline vector3d TransformPoint(const matrix& transform, const vector3d point) {
    return vector3d{point.y * transform.values[3] + point.x * transform.values[0] + point.z * transform.values[6] + transform.values[9],
                    point.y * transform.values[4] + point.x * transform.values[1] + point.z * transform.values[7] + transform.values[10],
                    point.y * transform.values[5] + point.x * transform.values[2] + point.z * transform.values[8] + transform.values[11]};
  }

  // GLOBAL: gl_24870
  std::unordered_map<int, std::vector<RadialGridPoint>> g_clawMarkRings;
}

// OFFSET: 0x005A2E50
// FUNCTION: fn_03122
CharacterParticleBundle::CharacterParticleBundle() : CEffect(-1), startTime(0), elapsedFrames(0), particles() {
}

// OFFSET: 0x005A31F0
// FUNCTION: fn_03126
__declspec(noinline) CharacterParticleBundle::~CharacterParticleBundle() {
}

// OFFSET: 0x005A3360
// FUNCTION: fn_03129
void CChopChopClawMarks::UpdateParticlePosition(CEZ2STRParticle* particle) {
  if (particle) {
    const matrix& view = g_modeMgr.GetGameMode()->camera->frame.view;
    const matrix& inverse = g_modeMgr.GetGameMode()->camera->frame.inverse;
    const vector3d origin = this->positionVector;
    vector3d camera = TransformPoint(view, origin);
    camera.x += this->offsetX;
    camera.y -= this->offsetY;
    camera += particle->offset;
    particle->position = TransformPoint(inverse, camera);
  }
}

// OFFSET: 0x005A34E0
// FUNCTION: fn_03131
void CChopChopClawMarks::SpawnParticle() {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(&this->positionVector, 5));
  if (particle) {
    const std::vector<RadialGridPoint>& ring = this->GetRing(3);
    RadialGridPoint point;
    std::vector<RadialGridPoint>::const_iterator found;
    do {
      const int positiveX = std::ranges::max(rand() % 2, 0);
      const int x = std::max(rand() % 4, 1);
      int signedX = -x;
      if (positiveX) {
        signedX = x;
      }
      point.x = static_cast<float>(signedX);
      const int positiveY = std::ranges::max(rand() % 2, 0);
      const int y = std::max(rand() % 4, 1);
      int signedY = -y;
      if (positiveY) {
        signedY = y;
      }
      point.y = static_cast<float>(signedY);
      found = std::find_if(ring.begin(), ring.end(), [&point](const RadialGridPoint& candidate) {
        return candidate.x == point.x && candidate.y == point.y;
      });
    } while (found == ring.end());

    const float x = point.x * 5.0f;
    const float y = point.y * 5.0f;
    // GLOBAL: gl_24875
    particle->Load(OBFUSCATE_TEXT("DRUID\\KR_CHOP_CHOP\\chop_chop\\chop_chop.str", 0x1D6D75EDE7CD311FULL));
    particle->offset = vector3d{x, y, 0.0f};
    particle->m_angle = static_cast<float>(std::ranges::max(rand() % 360, 0));
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x005A3820
// FUNCTION: fn_03132
void CChopChopClawMarks::EmitParticles() {
  const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  const std::chrono::nanoseconds elapsed = now.time_since_epoch() - std::chrono::nanoseconds(this->startTime);
  const std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
  if (milliseconds >= std::chrono::milliseconds(this->lastSpawnTime + 30)) {
    this->SpawnParticle();
    this->lastSpawnTime = milliseconds.count();
  }
}

// OFFSET: 0x005A38A0
// FUNCTION: fn_03133
void CharacterParticleBundle::Close() {
  for (CEZ2STRParticle* particle : this->particles) {
    FreeParticle(particle);
  }
  this->particles.clear();
}

// OFFSET: 0x005A3A10
// FUNCTION: fn_03134
const std::vector<RadialGridPoint>& CChopChopClawMarks::GetRing(int radius) {
  const auto end = g_clawMarkRings.end();
  const auto found = g_clawMarkRings.find(radius);
  if (found != end) {
    return found->second;
  }

  const std::vector<RadialGridPoint> outer = GenerateRadialGrid(RadialGridPoint{0.0f, 0.0f}, radius);
  const std::vector<RadialGridPoint> inner = GenerateRadialGrid(RadialGridPoint{0.0f, 1.0f}, 1);
  std::vector<RadialGridPoint> ring;
  for (const RadialGridPoint& point : outer) {
    const auto match = std::find_if(inner.begin(), inner.end(), [&point](const RadialGridPoint& other) {
      return point.x == other.x && point.y == other.y;
    });
    if (match == inner.end()) {
      ring.push_back(point);
    }
  }

  g_clawMarkRings[radius] = std::move(ring);
  return g_clawMarkRings[radius];
}

// OFFSET: 0x005A3C70
// FUNCTION: fn_03135
void CChopChopClawMarks::OnInit() {
  const int id = CEZ2STREffectMgr::GetInstance()->EffectId(OBFUSCATE_ARRAY_TEXT("chop_chop", 0x59278F63D79F3DA1ULL, std::uint8_t));
  if (EZ2STREffectInfo* info = CEZ2STREffectMgr::GetInstance()->FindById(id)) {
    const float offsetX = info->offsetX;
    const float offsetY = info->offsetY;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
  }
}

// OFFSET: 0x005A3E40
// FUNCTION: fn_03136
void CharacterParticleBundle::OnInit() {
}

// OFFSET: 0x005A3E50
// FUNCTION: fn_03137
void CharacterParticleBundle::Open() {
  this->elapsedFrames = 0;
  this->startTime = std::chrono::steady_clock::now().time_since_epoch().count();
  this->OnInit();
}

// OFFSET: 0x005A3E90
// FUNCTION: fn_03138
bool CChopChopClawMarks::IsAlive() {
  if (this->ActorExists()) {
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    const std::chrono::milliseconds limit = std::chrono::seconds(this->effectLifetime);
    const std::chrono::nanoseconds elapsed = now.time_since_epoch() - std::chrono::nanoseconds(this->startTime);
    if (limit >= std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x005A3F10
// FUNCTION: fn_03139
void CharacterParticleBundle::OnProcess() {
  this->EmitParticles();
  std::erase_if(this->particles, [](CEZ2STRParticle* particle) {
    if (!particle) {
      return true;
    }
    if (particle->life < 0.0f) {
      FreeParticle(particle);
      return true;
    }
    return false;
  });

  for (CEZ2STRParticle*& particle : this->particles) {
    this->UpdateParticlePosition(particle);
    this->ProcessParticle(particle);
  }
  ++this->elapsedFrames;
}

// OFFSET: 0x005A4000
// FUNCTION: fn_03140
void CharacterParticleBundle::ProcessParticle(CEZ2STRParticle* particle) {
  if (particle) {
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x005A4030
// FUNCTION: fn_03141
void CharacterParticleBundle::Render() {
  for (CEZ2STRParticle* particle : this->particles) {
    this->RenderParticle(particle);
  }
}

// OFFSET: 0x005A4060
// FUNCTION: fn_03142
void CharacterParticleBundle::RenderParticle(CEZ2STRParticle* particle) {
  if (particle) {
    particle->DrawScreen(g_modeMgr.GetGameMode()->camera->frame.view);
  }
}

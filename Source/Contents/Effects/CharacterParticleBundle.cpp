#include "Contents/Effects/CharacterParticleBundle.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
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
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };

  // GLOBAL: gl_24870
  std::unordered_map<int, std::vector<RadialGridPoint>> g_clawMarkRings;
}

// OFFSET: 0x005A2E50
// FUNCTION: fn_03122
CharacterParticleBundle::CharacterParticleBundle() : CEffect(-1), startTime(0), unknown_00D8(0), particles() {
}

// OFFSET: 0x005A31F0
// FUNCTION: fn_03126
CharacterParticleBundle::~CharacterParticleBundle() {
}

// OFFSET: 0x005A3360
// FUNCTION: fn_03129
void CChopChopClawMarks::Slot36(CEZ2STRParticle* particle) {
  if (particle) {
    const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
    const float* inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse.values;
    const float x = this->position[0];
    const float y = this->position[1];
    const float z = this->position[2];
    const float viewX = view[0] * x + view[3] * y + view[6] * z + view[9] + this->offsetX + particle->unknown_00D0;
    const float viewY = view[1] * x + view[4] * y + view[7] * z + view[10] - this->offsetY + particle->unknown_00D4;
    const float viewZ = view[2] * x + view[5] * y + view[8] * z + view[11] + particle->unknown_00D8;
    particle->position.x = viewY * inverse[3] + viewX * inverse[0] + viewZ * inverse[6] + inverse[9];
    particle->position.y = viewX * inverse[1] + viewY * inverse[4] + viewZ * inverse[7] + inverse[10];
    particle->position.z = viewX * inverse[2] + viewY * inverse[5] + viewZ * inverse[8] + inverse[11];
  }
}

// OFFSET: 0x005A34E0
// FUNCTION: fn_03131
void CChopChopClawMarks::SpawnParticle() {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5));
  if (particle) {
    const std::vector<RadialGridPoint>& ring = this->GetRing(3);
    RadialGridPoint point;
    do {
      const int positiveX = std::max(rand() % 2, 0);
      const int x = std::max(rand() % 4, 1);
      point.x = static_cast<float>(positiveX ? x : -x);
      const int positiveY = std::max(rand() % 2, 0);
      const int y = std::max(rand() % 4, 1);
      point.y = static_cast<float>(positiveY ? y : -y);
    } while (std::find_if(ring.begin(), ring.end(), [&point](const RadialGridPoint& candidate) {
      return candidate.x == point.x && candidate.y == point.y;
    }) == ring.end());

    const float x = point.x * 5.0f;
    const float y = point.y * 5.0f;
    const std::uint8_t bytes[] = {
      0x5B, 0x63, 0x98, 0xAE, 0xA9, 0x29, 0x26, 0x4F, 0x40, 0x72, 0x85, 0xA8, 0xBD, 0x2A, 0x2E, 0x55,
      0x50, 0x61, 0x91, 0x84, 0x85, 0x1A, 0x1D, 0x42, 0x7C, 0x59, 0xA2, 0x97, 0xB1, 0x16, 0x05, 0x72,
      0x6F, 0x6E, 0xAE, 0x8F, 0x82, 0x05, 0x43, 0x6E, 0x6B, 0x43, 0xCD
    };
    // GLOBAL: gl_24875
    thread_local EncryptedName<sizeof(bytes), 0x1D6D75EDE7CD311FULL> name(bytes);
    particle->Open(name.Text());
    particle->unknown_00D0 = x;
    particle->unknown_00D4 = y;
    particle->unknown_00D8 = 0.0f;
    particle->m_angle = static_cast<float>(std::max(rand() % 360, 0));
    this->particles.push_back(particle);
  }
}

// OFFSET: 0x005A3820
// FUNCTION: fn_03132
void CChopChopClawMarks::Slot34() {
  const std::chrono::nanoseconds elapsed = std::chrono::steady_clock::now().time_since_epoch() - std::chrono::nanoseconds(this->startTime);
  const long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  if (milliseconds >= this->lastSpawnTime + 30) {
    this->SpawnParticle();
    this->lastSpawnTime = milliseconds;
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
  const auto found = g_clawMarkRings.find(radius);
  if (found != g_clawMarkRings.end()) {
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
void CChopChopClawMarks::Slot35() {
  const int id = CEZ2STREffectMgr::GetInstance()->EffectId([] {
    const std::uint8_t bytes[] = {0xC2, 0x55, 0xF0, 0xA7, 0x3C, 0xEC, 0x4F, 0x36, 0xD1, 0x3D};
    thread_local EncryptedName<sizeof(bytes), 0x59278F63D79F3DA1ULL> name(bytes);
    return name.Text();
  }());
  if (EZ2STREffectInfo* info = CEZ2STREffectMgr::GetInstance()->FindById(id)) {
    this->offsetX = info->offsetX;
    this->offsetY = info->offsetY;
  }
}

// OFFSET: 0x005A3E40
// FUNCTION: fn_03136
void CharacterParticleBundle::Slot35() {
}

// OFFSET: 0x005A3E50
// FUNCTION: fn_03137
void CharacterParticleBundle::Slot01() {
  this->unknown_00D8 = 0;
  this->startTime = std::chrono::steady_clock::now().time_since_epoch().count();
  this->Slot35();
}

// OFFSET: 0x005A3E90
// FUNCTION: fn_03138
bool CChopChopClawMarks::Slot06() {
  if (!this->ActorExists()) {
    return false;
  }

  const std::chrono::nanoseconds elapsed = std::chrono::steady_clock::now().time_since_epoch() - std::chrono::nanoseconds(this->startTime);
  return std::chrono::seconds(this->unknown_002C) >= std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
}

// OFFSET: 0x005A3F10
// FUNCTION: fn_03139
void CharacterParticleBundle::Slot03() {
  this->Slot34();
  const auto removed = std::remove_if(this->particles.begin(), this->particles.end(), [](CEZ2STRParticle* particle) {
    if (!particle) {
      return true;
    }
    if (particle->life < 0.0f) {
      FreeParticle(particle);
      return true;
    }
    return false;
  });
  this->particles.erase(removed, this->particles.end());

  for (CEZ2STRParticle* particle : this->particles) {
    this->Slot36(particle);
    this->Slot37(particle);
  }
  ++this->unknown_00D8;
}

// OFFSET: 0x005A4000
// FUNCTION: fn_03140
void CharacterParticleBundle::Slot37(CEZ2STRParticle* particle) {
  if (particle) {
    particle->Process(0.0f, 0.0f, 0.0f);
  }
}

// OFFSET: 0x005A4030
// FUNCTION: fn_03141
void CharacterParticleBundle::Slot04() {
  for (CEZ2STRParticle* particle : this->particles) {
    this->Slot38(particle);
  }
}

// OFFSET: 0x005A4060
// FUNCTION: fn_03142
void CharacterParticleBundle::Slot38(CEZ2STRParticle* particle) {
  if (particle) {
    particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
  }
}

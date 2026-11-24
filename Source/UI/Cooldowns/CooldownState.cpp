#include "UI/Cooldowns/CooldownState.h"
#include "Client/Replay/Replay.h"
#include "UI/Cooldowns/CooldownClock.h"

#include <cmath>
#include <cstring>
#include <new>
#include <windows.h>
#include <timeapi.h>

namespace {
  struct CooldownVertex : RenderVertex {
    // INLINED
    CooldownVertex() {
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->rhw = 0.0f;
      this->diffuse = 0;
      this->specular = 0xFF000000u;
      this->u = 0.0f;
      this->v = 0.0f;
    }
  };
}

// OFFSET: 0x00CA4F30
// FUNCTION: fn_20946
CooldownState::CooldownState(int slotValue, int xValue, int yValue, std::uint32_t durationValue, std::uint32_t startValue, std::uint32_t elapsedValue,
                             int reverseValue)
    : segment(0), vertexCount(24), angle(0), duration(durationValue), start(startValue), elapsed(elapsedValue), finalMinute(0), reverse(reverseValue) {
  new (this->vertices) CooldownVertex[24];
  this->x = xValue;
  this->y = yValue;
  this->slot = slotValue;
  this->ClearEdges();
  std::memset(this->vertices, 0, sizeof(this->vertices));
  if (this->reverse == 0) {
    this->color = 0xAA000000;
  } else {
    this->color = 0xAAFFFFFF;
  }
}

// OFFSET: 0x00910480
// FUNCTION: fn_12502
CooldownState::CooldownState(const CooldownState& other) = default;

// OFFSET: 0x00CA5120
// FUNCTION: fn_20947
CooldownState::~CooldownState() {
  this->ClearEdges();
}

// OFFSET: 0x00CA59C0
// FUNCTION: fn_20951
__declspec(noinline) void CooldownState::ClearEdges() noexcept {
  this->edges.clear();
}

// OFFSET: 0x00CA54E0
// FUNCTION: fn_20949
void CooldownState::AddEdge(unsigned key, float origin, int radius, unsigned char increasing) {
  this->edges.insert(std::make_pair(key, CooldownEdge{origin, origin, radius, increasing}));
}

// OFFSET: 0x00CA5480
// FUNCTION: fn_45875
float CooldownState::EdgePosition(float origin, int radius, unsigned char increasing) const {
  const float radians = this->angle * 0.017453292f;
  const float tangent = static_cast<float>(std::tan(static_cast<double>(radians)));
  const float distance = static_cast<float>(radius) / tangent;
  return increasing != 0 ? distance + origin : origin - distance;
}

// OFFSET: 0x00CA5600
// FUNCTION: fn_45876
void CooldownState::UpdateSegment(std::uint32_t progress) {
  if (this->duration != 0) {
    this->angle = progress == 0 ? 0.0f : static_cast<float>((static_cast<double>(progress) / static_cast<double>(this->duration)) * 360.0);
    int backwardCount = 0;
    int forwardCount = 0;
    if (this->angle >= 0 && this->angle < 45) {
      this->segment = 0;
      backwardCount = 24;
      forwardCount = 3;
    } else if (this->angle >= 45 && this->angle < 90) {
      this->segment = 1;
      backwardCount = 21;
      forwardCount = 6;
    } else if (this->angle >= 90 && this->angle < 135) {
      this->segment = 2;
      backwardCount = 18;
      forwardCount = 9;
    } else if (this->angle >= 135 && this->angle < 180) {
      this->segment = 3;
      backwardCount = 15;
      forwardCount = 12;
    } else if (this->angle >= 180 && this->angle < 225) {
      this->segment = 4;
      backwardCount = 12;
      forwardCount = 15;
    } else if (this->angle >= 225 && this->angle < 270) {
      this->segment = 5;
      backwardCount = 9;
      forwardCount = 18;
    } else if (this->angle >= 270 && this->angle < 315) {
      this->segment = 6;
      backwardCount = 6;
      forwardCount = 21;
    } else {
      this->segment = 7;
      backwardCount = 3;
      forwardCount = 24;
    }

    this->vertexCount = this->reverse != 0 ? forwardCount : backwardCount;
    this->angle = 90.0f - (this->angle - static_cast<float>(this->segment) * 45.0f);
  }
}

// OFFSET: 0x00CA5180
// FUNCTION: fn_20948
float CooldownState::Update() {
  std::uint32_t progress = CooldownNow();
  progress += this->elapsed - this->start;
  if (this->duration - progress <= 60000 && this->finalMinute == 0 && this->reverse == 1) {
    const std::uint32_t previous = this->duration;
    this->finalMinute = 1;
    this->duration = 60000;
    this->elapsed = progress - previous + 60000;
    this->start = CooldownNow();
    this->angle = 0;
    this->color = 0xAAFF6633u;
    progress = this->elapsed;
  }

  this->UpdateSegment(progress);

  if (this->duration > progress) {
    CooldownEdge& edge = this->edges.find(this->segment)->second;
    edge.current = this->EdgePosition(edge.origin, edge.radius, edge.increasing);
    return edge.current;
  }

  return this->edges.find(this->segment)->second.origin;
}

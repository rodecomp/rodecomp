#include "UI/Cooldowns/CooldownState.h"

#include <cmath>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// INLINED
std::uint32_t CooldownClock::Now() const {
  return this->restricted ? this->serverTick : timeGetTime();
}

// OFFSET: 0x00CA4F30
// FUNCTION: fn_20946
CooldownState::CooldownState(
  int slotValue, int xValue, int yValue, std::uint32_t durationValue, std::uint32_t startValue, std::uint32_t elapsedValue, int reverseValue
)
    : segment(0), vertexCount(24), angle(0), duration(durationValue), start(startValue), elapsed(elapsedValue), finalMinute(0), reverse(reverseValue),
      x(xValue), y(yValue), slot(slotValue), color(reverseValue != 0 ? 0xAAFFFFFFu : 0xAA000000u) {
  this->ClearEdges();
  std::memset(this->vertices, 0, sizeof(this->vertices));
}

// OFFSET: 0x00CA5120
// FUNCTION: fn_20947
CooldownState::~CooldownState() {
  this->ClearEdges();
}

// OFFSET: 0x00CA59C0
// FUNCTION: fn_20951
void CooldownState::ClearEdges() {
  this->edges.clear();
}

// OFFSET: 0x00CA54E0
// FUNCTION: fn_20949
void CooldownState::AddEdge(unsigned key, float origin, int radius, unsigned char increasing) {
  this->edges.emplace(key, CooldownEdge{origin, origin, radius, increasing});
}

// OFFSET: 0x00CA5180
// FUNCTION: fn_20948
float CooldownState::Update(const CooldownClock& clock) {
  std::uint32_t progress = clock.Now() + this->elapsed - this->start;
  if (this->duration - progress <= 60000 && this->finalMinute == 0 && this->reverse == 1) {
    const std::uint32_t previous = this->duration;
    this->finalMinute = 1;
    this->duration = 60000;
    this->elapsed = progress - previous + 60000;
    this->start = clock.Now();
    this->angle = 0;
    this->color = 0xAAFF6633u;
    progress = this->elapsed;
  }

  if (this->duration != 0) {
    this->angle = progress != 0 ? static_cast<float>((static_cast<double>(progress) / static_cast<double>(this->duration)) * 360.0) : 0.0f;
    const float value = this->angle;
    if (value >= 0 && value < 45) {
      this->segment = 0;
    } else if (value >= 45 && value < 90) {
      this->segment = 1;
    } else if (value >= 90 && value < 135) {
      this->segment = 2;
    } else if (value >= 135 && value < 180) {
      this->segment = 3;
    } else if (value >= 180 && value < 225) {
      this->segment = 4;
    } else if (value >= 225 && value < 270) {
      this->segment = 5;
    } else if (value >= 270 && value < 315) {
      this->segment = 6;
    } else {
      this->segment = 7;
    }

    this->vertexCount = this->reverse != 0 ? 3 * (this->segment + 1) : 24 - 3 * this->segment;
    this->angle = 90.0f - (this->angle - static_cast<float>(this->segment) * 45.0f);
  }

  auto entry = this->edges.find(this->segment);
  CooldownEdge& edge = entry->second;
  if (this->duration <= progress) {
    return edge.origin;
  }

  const float radians = this->angle * 0.017453292f;
  const float tangent = static_cast<float>(std::tan(static_cast<double>(radians)));
  const float distance = static_cast<float>(edge.radius) / tangent;
  edge.current = edge.increasing != 0 ? edge.origin + distance : edge.origin - distance;

  return edge.current;
}

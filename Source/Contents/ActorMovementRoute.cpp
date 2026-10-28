#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/TerrainPathSearch.h"

// GLOBAL: gl_24733
CPathFinder g_pathFinder;

// OFFSET: 0x00B84570
// FUNCTION: fn_19317
float Replay::DelayScale() const {
  if (this->speed == 0.25f) {
    return 4.0f;
  }
  if (this->speed == 0.5f) {
    return 2.0f;
  }
  if (this->speed == 1.0f) {
    return 1.0f;
  }
  if (this->speed == 2.0f) {
    return 0.5f;
  }
  if (this->speed == 4.0f) {
    return 0.25f;
  }
  if (this->speed == 8.0f) {
    return 0.125f;
  }
  if (this->speed == 16.0f) {
    return 0.0625f;
  }
  return 1.0f;
}

// OFFSET: 0x00CAB610
// FUNCTION: fn_21028
bool CGameActor::FindRoute(int startX, int startY, int fractionX, int fractionY, int destX, int destY, int speed) {
  float scaledSpeed = 0.0f;
  int stepSpeed;
  if (g_replayAnimationScaling != 0) {
    scaledSpeed = Replay::Instance().DelayScale() * static_cast<float>(speed);
    stepSpeed = static_cast<int>(scaledSpeed);
  } else {
    stepSpeed = speed;
  }

  const bool found =
    g_pathFinder.FindPath(this->routeStartTick, startX, startY, fractionX, fractionY, destX, destY, stepSpeed, this->route, this->unknown_0314);
  if (!found) {
    return found;
  }

  std::uint32_t endTick;
  this->route.EndTick(&endTick);
  const std::uint32_t now = g_session.GetServerTime();
  if (endTick > now) {
    return found;
  }

  const int baseSpeed = g_replayAnimationScaling != 0 ? static_cast<int>(scaledSpeed) : speed;
  this->routeStartTick = now;
  return g_pathFinder.FindPath(now, startX, startY, fractionX, fractionY, destX, destY, baseSpeed * 10 / 13, this->route, this->unknown_0314);
}

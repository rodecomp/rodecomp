#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Movement/TerrainPathSearch.h"

// GLOBAL: gl_24733
CPathFinder g_pathFinder;

// OFFSET: 0x00B84570
// FUNCTION: fn_19317
float Replay::DelayScale() const {
  if (0.25f == this->speed) {
    return 4.0f;
  }
  if (0.5f == this->speed) {
    return 2.0f;
  }
  if (1.0f == this->speed) {
    return 1.0f;
  }
  if (2.0f == this->speed) {
    return 0.5f;
  }
  if (4.0f == this->speed) {
    return 0.25f;
  }
  if (8.0f == this->speed) {
    return 0.125f;
  }
  if (16.0f == this->speed) {
    return 0.0625f;
  }
  return 1.0f;
}

// OFFSET: 0x00CAB610
// FUNCTION: fn_21028
bool CGameActor::FindRoute(int startX, int startY, int fractionX, int fractionY, int destX, int destY, int speed) {
  float scaledSpeed = 0.0f;
  MovementRouteInterpolation& route = this->route;
  bool found;
  if (g_replayAnimationScaling != 0) {
    scaledSpeed = Replay::Instance().DelayScale() * static_cast<float>(speed);
    found = g_pathFinder.FindPath(this->routeStartTick, startX, startY, fractionX, fractionY, destX, destY, static_cast<int>(scaledSpeed), route, this->objectType);
  } else {
    found = g_pathFinder.FindPath(this->routeStartTick, startX, startY, fractionX, fractionY, destX, destY, speed, route, this->objectType);
  }

  if (found) {
    std::uint32_t endTick;
    route.EndTick(&endTick);
    const std::uint32_t now = g_session.GetServerTime();
    if (endTick <= now) {
      const int retrySpeed = g_replayAnimationScaling != 0 ? static_cast<int>(scaledSpeed) * 10 / 13 : speed * 10 / 13;
      this->routeStartTick = now;
      found = g_pathFinder.FindPath(now, startX, startY, fractionX, fractionY, destX, destY, retrySpeed, route, this->objectType);
    }
  }

  return found;
}

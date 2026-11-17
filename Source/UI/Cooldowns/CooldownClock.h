#ifndef RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNCLOCK_H
#define RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNCLOCK_H

#include "Client/Replay/Replay.h"

#include <cstdint>
#include <windows.h>
#include <timeapi.h>

// INLINED
inline std::uint32_t CooldownNow() {
  return g_replayAnimationScaling ? Replay::Instance().elapsed : timeGetTime();
}

#endif

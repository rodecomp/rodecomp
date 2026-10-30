#include "Client/Mode/CGameMode.h"
#include "Client/Replay.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CED670
// FUNCTION: fn_21368
bool CGameMode::UpdateReplayPause(bool resume) {
  if (resume == true && this->replayPauseTick != 0) {
    if (Replay::Instance().state != 7) {
      const std::uint32_t paused = timeGetTime() - this->replayPauseTick;
      Replay::Instance().started += paused;
    }
  } else if (Replay::Instance().state == 7) {
    this->replayPauseTick = timeGetTime();
  }

  if (this->replayPauseTick == 0) {
    Replay::Instance().RestartClock();
  }

  return resume;
}

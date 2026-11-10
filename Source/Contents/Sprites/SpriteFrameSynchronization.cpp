#include "Client/Session.h"
#include "Resource/Sprites/ActionResource.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E12D70
// FUNCTION: fn_23819
const ActionFrame* CSession::GetSyncedFrame(CActRes* baseAct, CActRes* act, unsigned action, int baseFrame, std::uint32_t startTime, int* frame) {
  if (!baseAct || !act) {
    return nullptr;
  }

  int baseCount = baseAct->FrameCount(action);
  int count = act->FrameCount(action);
  if (baseCount <= 0 || count <= 0 || baseCount >= count || count % baseCount != 0) {
    return nullptr;
  }

  int ratio = count / baseCount;
  int speed = static_cast<int>(act->Speed(action));
  float elapsed = (timeGetTime() - startTime) / 24.0;
  if (speed == 0) {
    speed = 4;
  }

  *frame = static_cast<int>(elapsed / speed) % ratio + ratio * baseFrame;
  return &act->Frame(action, *frame);
}

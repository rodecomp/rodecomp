#include "Contents/SpriteRidingPolicy.h"
#include "Client/Session.h"
#include "Contents/Jobs/JobNames.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00DAB2C0
// FUNCTION: fn_22583
bool IsIgnoredRidingState(int id, int job, int effectState, bool force) {
  if (!force && job == JobNames::BaseJob(job)) {
    return false;
  }

  switch (effectState) {
  case 0:
  case 8:
  case 16:
  case 64: {
    bool ignored = false;
    g_session.m_hotkeyScript->CallTable("TB_Layer_Priority", "IsIgnoredRidingState", "d>b", id, &ignored);
    return ignored;
  }
  }

  return false;
}

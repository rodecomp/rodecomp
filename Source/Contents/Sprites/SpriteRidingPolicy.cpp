#include "Contents/Sprites/SpriteRidingPolicy.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/ActorEnums.h"
#include "Contents/Jobs/JobNames.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00DAB2C0
// FUNCTION: fn_22583
bool IsIgnoredRidingState(int id, int job, int baseAction, bool force) {
  if (!force && job == g_session.BaseJob(job)) {
    return false;
  }

  switch (baseAction) {
  case SPRITE_ACTION_STAND:
  case SPRITE_ACTION_MOVE:
  case SPRITE_ACTION_SIT:
  case SPRITE_ACTION_DEAD: {
    bool ignored = false;
    g_session.m_hotkeyScript->CallTable("TB_Layer_Priority", "IsIgnoredRidingState", "d>b", id, &ignored);
    return ignored;
  }
  }

  return false;
}

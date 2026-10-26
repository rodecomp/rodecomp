#include "Client/Session.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Script/LuaScript.h"

namespace {
  // OFFSET: 0x00DFBC00
  // FUNCTION: fn_23585
  bool IsTreeGroupTwo(int job) {
    if (job >= JT_RUNE_KNIGHT && job <= JT_MADOGEAR_B) {
      return true;
    }

    switch (job) {
    case JT_PIG_GENETIC:
    case JT_SHEEP_SURA:
    case JT_SHEEP_ARCB:
    case JT_FOX_SORCERER:
    case JT_FOX_WARLOCK:
    case JT_DOG_G_CROSS:
    case JT_DOG_CHASER:
    case JT_OSTRICH_MINSTREL:
    case JT_OSTRICH_WANDER:
    case JT_FOX_WARLOCK_B:
    case JT_SHEEP_ARCB_B:
    case JT_DOG_G_CROSS_B:
    case JT_FOX_SORCERER_B:
    case JT_OSTRICH_MINSTREL_B:
    case JT_OSTRICH_WANDER_B:
    case JT_SHEEP_SURA_B:
    case JT_PIG_GENETIC_B:
    case JT_DOG_CHASER_B:
    case JT_PIG_MECHANIC:
    case JT_OSTRICH_RANGER:
    case JT_LION_ROYAL_GUARD:
    case JT_LION_RUNE_KNIGHT:
    case JT_PIG_MECHANIC_B:
    case JT_OSTRICH_RANGER_B:
    case JT_LION_ROYAL_GUARD_B:
    case JT_LION_RUNE_KNIGHT_B:
      return true;
    default:
      return false;
    }
  }

  // OFFSET: 0x00DFBCA0
  // FUNCTION: fn_23586
  bool IsTreeGroupThree(int job) {
    const auto value = static_cast<unsigned>(job);
    return value - JT_DRAGON_KNIGHT <= unsigned(JT_FOURTH_JOB_END - JT_DRAGON_KNIGHT) ||
      value - JT_SKY_EMPEROR <= unsigned(JT_UPPER_EXTENDED_JOB2_END - 1 - JT_SKY_EMPEROR) || job == JT_ALITEA || job == JT_ALITEA_RIDING;
  }
}

// OFFSET: 0x00DFBCE0
// FUNCTION: fn_23587
bool __stdcall IsPlayerJob(int job) {
  const auto value = static_cast<unsigned>(job);
  return value - JT_NOVICE_H <= unsigned(JT_MER_BEGIN - 1 - JT_NOVICE_H) || value <= unsigned(JT_SUMMER2);
}

// OFFSET: 0x00E19110
// FUNCTION: fn_23835
int JobInheritance::Lookup(int job) {
  const int inherited = this->script.TableInteger("JobInheritTable", 0, job);
  const int replacement = this->script.TableInteger("JOB_INHERIT_LIST2", 0, inherited);
  return replacement == -1 ? inherited : replacement;
}

// OFFSET: 0x00E25B70
// FUNCTION: fn_23907
void JobInheritance::InitializeSkillTree(CPlayerSkillBundle& bundle, int job) {
  int count = 0;
  if (!this->script.Call("GetInheritJob", "d>d", job, &count) || count == -1) {
    return;
  }

  bundle.ClearGroups();
  bundle.ClearExtra();
  bundle.ClearLevelChanges();

  for (int index = count; index > 0; --index) {
    const int inherited = this->Lookup(index);
    int group = 1;
    if (inherited == JT_NOVICE || this->Lookup(index + 1) == JT_NOVICE || inherited == JT_DO_SUMMONER || inherited == JT_DO_SUMMONER_B) {
      group = 0;
    } else if (IsTreeGroupTwo(inherited) || inherited == JT_STAR_EMPEROR || inherited == JT_SOUL_REAPER || inherited == JT_STAR_EMPEROR_B ||
      inherited == JT_SOUL_REAPER_B) {
      group = 2;
    } else if (IsTreeGroupThree(inherited)) {
      group = 3;
    }

    switch (inherited) {
    case JT_SKY_EMPEROR:
    case JT_SOUL_ASCETIC:
      group = 3;
      break;
    case JT_SHINKIRO:
    case JT_SHIRANUI:
    case JT_NIGHT_WATCH:
    case JT_HYPER_NOVICE:
    case JT_ALITEA:
      group = 2;
      break;
    case JT_SPIRIT_HANDLER:
      group = 1;
      break;
    default:
      break;
    }

    this->script.Call("InitSkillTreeView", "dd", inherited, group);
  }
}

// FUNCTION: fn_23461
int __stdcall GetRealWeaponId(int weapon) {
  int real = 0;
  if (g_session.m_hotkeyScript->Call("GetRealWeaponId", "d>d", weapon, &real)) {
    return real;
  }
  return weapon;
}

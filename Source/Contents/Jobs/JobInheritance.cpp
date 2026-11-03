#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/ClientItemLists.h"
#include "Contents/ClientState.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/WeaponType.h"
#include "Contents/MessageTable.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/MapPropertyMgr.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Contents/World/World.h"
#include "File/MapName.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/AMachineEffect.h"
#include "Contents/Effects/RagEffect.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>
#include <list>
#include <new>
#include <string>
#include <unordered_map>
#include <vector>

// GLOBAL: gl_24496
int g_screenShakeDuration;

// GLOBAL: gl_24497
char g_screenShakeMode;

// GLOBAL: gl_24702
const char g_playerNameKeyData[] = "가나다라마바사아자차카타파하가나다라마바사아자차카타파하";

// GLOBAL: gl_24649
const char* g_playerNameKey = g_playerNameKeyData;

// OFFSET: 0x00DFBC00
// FUNCTION: fn_23585
__declspec(noinline) bool CSession::IsTreeGroupTwo(int job) const {
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
__declspec(noinline) bool CSession::IsTreeGroupThree(int job) const {
  const auto value = static_cast<unsigned>(job);
  return value - JT_DRAGON_KNIGHT <= unsigned(JT_FOURTH_JOB_END - JT_DRAGON_KNIGHT) ||
    value - JT_SKY_EMPEROR <= unsigned(JT_UPPER_EXTENDED_JOB2_END - 1 - JT_SKY_EMPEROR) || job == JT_ALITEA || job == JT_ALITEA_RIDING;
}

// OFFSET: 0x00DFA600
// FUNCTION: fn_23576
bool CSession::IsNpcJob(int job) const {
  return (job >= JT_WARPNPC && job < JT_MON_BEGIN) || static_cast<unsigned>(job - JT_4_TOWER_14) <= unsigned(JT_NPC_END - 1 - JT_4_TOWER_14);
}

// OFFSET: 0x00DFA630
// FUNCTION: fn_23577
bool __stdcall ValidateCharacterName(const char* name) {
  const int length = static_cast<int>(std::strlen(name));
  for (int index = 0; index < length; ++index) {
    if (name[index] == 0) {
      return false;
    }
  }
  return true;
}

// OFFSET: 0x00DFA670
// FUNCTION: fn_23578
bool CSession::IsNavigationAllowed() const {
  const std::string name = ScriptMapName(this->m_navigationPlayerMap);
  for (const auto& forbidden : this->m_navigationForbiddenMaps) {
    if (forbidden == name) {
      return false;
    }
  }
  return true;
}

// OFFSET: 0x00DFA810
// FUNCTION: fn_23579
bool IsNavigationActive() {
  if (g_windowMgr.unknown_0250 != 0) {
    return false;
  }
  if (g_windowMgr.window_4F260 != nullptr || g_windowMgr.window_4F264 != nullptr ||
      g_windowMgr.window_4F268 != nullptr || g_windowMgr.window_4F26C != nullptr) {
    return false;
  }
  if (g_windowMgr.unknown_01E4 != 0) {
    return false;
  }
  return true;
}

// OFFSET: 0x00DFA8D0
// FUNCTION: fn_23580
bool CSession::HasJobShieldSprite(int job, int shield) const {
  if (shield > 2000) {
    switch (job) {
    case JT_CHICKEN:
    case JT_CHICKEN2:
    case JT_MARRIED:
    case JT_SANTA:
    case JT_SUMMER:
    case JT_HANBOK:
    case JT_OKTOBERFEST:
    case JT_SUMMER2:
    case JT_CHICKEN_H:
    case JT_CHICKEN2_H:
    case JT_CHICKEN_B:
    case JT_CHICKEN2_B:
    case JT_GANGSI:
    case JT_DEATHKNIGHT:
    case JT_COLLECTOR:
    case JT_RUNE_CHICKEN:
    case JT_RUNE_CHICKEN_H:
    case JT_ROYAL_CHICKEN:
    case JT_ROYAL_CHICKEN_H:
    case JT_WOLF_RANGER:
    case JT_WOLF_RANGER_H:
    case JT_MADOGEAR:
    case JT_MADOGEAR_H:
    case JT_RUNE_CHICKEN2:
    case JT_RUNE_CHICKEN2_H:
    case JT_RUNE_CHICKEN3:
    case JT_RUNE_CHICKEN3_H:
    case JT_RUNE_CHICKEN4:
    case JT_RUNE_CHICKEN4_H:
    case JT_RUNE_CHICKEN5:
    case JT_RUNE_CHICKEN5_H:
    case JT_RUNE_CHICKEN_B:
    case JT_ROYAL_CHICKEN_B:
    case JT_WOLF_RANGER_B:
    case JT_MADOGEAR_B:
    case JT_DO_CART_SUMMONER:
    case JT_WOLF_WINDHAWK:
    case JT_MEISTER_MADOGEAR:
    case JT_DRAGON_KNIGHT_CHICKEN:
    case JT_IMPERIAL_GUARD_CHICKEN:
      return false;
    }

    return true;
  }

  switch (job) {
  case JT_NOVICE:
  case JT_MAGICIAN:
  case JT_ARCHER:
  case JT_WIZARD:
  case JT_HUNTER:
  case JT_SAGE:
  case JT_BARD:
  case JT_DANCER:
  case JT_SUPERNOVICE:
  case JT_NOVICE_H:
  case JT_MAGICIAN_H:
  case JT_ARCHER_H:
  case JT_WIZARD_H:
  case JT_HUNTER_H:
  case JT_SAGE_H:
  case JT_BARD_H:
  case JT_DANCER_H:
  case JT_NOVICE_B:
  case JT_MAGICIAN_B:
  case JT_ARCHER_B:
  case JT_WIZARD_B:
  case JT_HUNTER_B:
  case JT_SAGE_B:
  case JT_BARD_B:
  case JT_DANCER_B:
  case JT_SUPERNOVICE_B:
  case JT_WARLOCK:
  case JT_RANGER:
  case JT_WARLOCK_H:
  case JT_RANGER_H:
  case JT_SORCERER:
  case JT_MINSTREL:
  case JT_WANDERER:
  case JT_SORCERER_H:
  case JT_MINSTREL_H:
  case JT_WANDERER_H:
  case JT_WARLOCK_B:
  case JT_RANGER_B:
  case JT_SORCERER_B:
  case JT_MINSTREL_B:
  case JT_WANDERER_B:
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_ARCH_MAGE:
  case JT_WINDHAWK:
  case JT_ELEMENTAL_MASTER:
  case JT_TROUBADOUR:
  case JT_TROUVERE:
  case JT_HYPER_NOVICE:
    return shield == 1;
  case JT_ACOLYTE:
  case JT_MERCHANT:
  case JT_THIEF:
  case JT_PRIEST:
  case JT_BLACKSMITH:
  case JT_ASSASSIN:
  case JT_ROGUE:
  case JT_ALCHEMIST:
  case JT_ACOLYTE_H:
  case JT_MERCHANT_H:
  case JT_THIEF_H:
  case JT_PRIEST_H:
  case JT_BLACKSMITH_H:
  case JT_ASSASSIN_H:
  case JT_ROGUE_H:
  case JT_ALCHEMIST_H:
  case JT_ACOLYTE_B:
  case JT_MERCHANT_B:
  case JT_THIEF_B:
  case JT_PRIEST_B:
  case JT_BLACKSMITH_B:
  case JT_ASSASSIN_B:
  case JT_ROGUE_B:
  case JT_ALCHEMIST_B:
  case JT_ARCHBISHOP:
  case JT_MECHANIC:
  case JT_GUILLOTINE_CROSS:
  case JT_ARCHBISHOP_H:
  case JT_MECHANIC_H:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GENETIC:
  case JT_SHADOW_CHASER:
  case JT_GENETIC_H:
  case JT_SHADOW_CHASER_H:
  case JT_ARCHBISHOP_B:
  case JT_MECHANIC_B:
  case JT_GUILLOTINE_CROSS_B:
  case JT_GENETIC_B:
  case JT_SHADOW_CHASER_B:
  case JT_MEISTER:
  case JT_SHADOW_CROSS:
  case JT_CARDINAL:
  case JT_BIOLO:
  case JT_ABYSS_CHASER:
    switch (shield) {
    case 1:
    case 2:
      return true;
    }
    return false;
  case JT_SWORDMAN:
  case JT_KNIGHT:
  case JT_CRUSADER:
  case JT_SWORDMAN_H:
  case JT_KNIGHT_H:
  case JT_CRUSADER_H:
  case JT_SWORDMAN_B:
  case JT_KNIGHT_B:
  case JT_CRUSADER_B:
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_RUNE_KNIGHT_B:
  case JT_ROYAL_GUARD_B:
  case JT_DO_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_DRAGON_KNIGHT:
  case JT_IMPERIAL_GUARD:
  case JT_DRAGON_KNIGHT_CHICKEN:
  case JT_IMPERIAL_GUARD_CHICKEN:
  case JT_SPIRIT_HANDLER:
    switch (shield) {
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    }
    return false;
  case JT_DRUID:
  case JT_DRUID_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
  case JT_ALITEA:
    switch (shield) {
    case 1:
    case 2:
    case 4:
      return true;
    }
    return false;
  }

  return false;
}

// OFFSET: 0x00DFAC70
// FUNCTION: fn_23581
bool CSession::IsJobWeaponTypeAllowed(int job, std::int64_t weaponType) const {
  switch (job) {
  case JT_NOVICE:
  case JT_SUPERNOVICE:
  case JT_NOVICE_H:
  case JT_NOVICE_B:
  case JT_SUPERNOVICE_B:
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_HYPER_NOVICE:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_1HAXE:
    case W_MACE:
    case W_STAFF:
    case W_2HSTAFF:
      return true;
    }
    return false;
  case JT_SWORDMAN:
  case JT_KNIGHT:
  case JT_CHICKEN:
  case JT_SWORDMAN_H:
  case JT_KNIGHT_H:
  case JT_CHICKEN_H:
  case JT_SWORDMAN_B:
  case JT_KNIGHT_B:
  case JT_CHICKEN_B:
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_RUNE_CHICKEN:
  case JT_RUNE_CHICKEN_H:
  case JT_RUNE_CHICKEN2:
  case JT_RUNE_CHICKEN2_H:
  case JT_RUNE_CHICKEN3:
  case JT_RUNE_CHICKEN3_H:
  case JT_RUNE_CHICKEN4:
  case JT_RUNE_CHICKEN4_H:
  case JT_RUNE_CHICKEN5:
  case JT_RUNE_CHICKEN5_H:
  case JT_RUNE_KNIGHT_B:
  case JT_RUNE_CHICKEN_B:
  case JT_DRAGON_KNIGHT:
  case JT_DRAGON_KNIGHT_CHICKEN:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_2HSWORD:
    case W_1HSPEAR:
    case W_2HSPEAR:
    case W_1HAXE:
    case W_2HAXE:
    case W_MACE:
    case W_2HMACE:
      return true;
    }
    return false;
  case JT_MAGICIAN:
  case JT_WIZARD:
  case JT_MAGICIAN_H:
  case JT_WIZARD_H:
  case JT_MAGICIAN_B:
  case JT_WIZARD_B:
  case JT_LINKER:
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
  case JT_LINKER_B:
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
  case JT_ARCH_MAGE:
  case JT_SOUL_ASCETIC:
    switch (weaponType) {
    case W_DAGGER:
    case W_STAFF:
    case W_2HSTAFF:
      return true;
    }
    return false;
  case JT_ARCHER:
  case JT_HUNTER:
  case JT_ARCHER_H:
  case JT_HUNTER_H:
  case JT_ARCHER_B:
  case JT_HUNTER_B:
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_WOLF_RANGER:
  case JT_WOLF_RANGER_H:
  case JT_RANGER_B:
  case JT_WOLF_RANGER_B:
  case JT_WINDHAWK:
    switch (weaponType) {
    case W_DAGGER:
    case W_BOW:
      return true;
    }
    return false;
  case JT_ACOLYTE:
  case JT_ACOLYTE_H:
  case JT_ACOLYTE_B:
    switch (weaponType) {
    case W_MACE:
    case W_STAFF:
    case W_2HSTAFF:
      return true;
    }
    return false;
  case JT_MERCHANT:
  case JT_BLACKSMITH:
  case JT_MERCHANT_H:
  case JT_BLACKSMITH_H:
  case JT_MERCHANT_B:
  case JT_BLACKSMITH_B:
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MADOGEAR:
  case JT_MADOGEAR_H:
  case JT_MECHANIC_B:
  case JT_MADOGEAR_B:
  case JT_MEISTER:
  case JT_MEISTER_MADOGEAR:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_1HAXE:
    case W_2HAXE:
    case W_MACE:
      return true;
    }
    return false;
  case JT_THIEF:
  case JT_ROGUE:
  case JT_THIEF_H:
  case JT_ROGUE_H:
  case JT_THIEF_B:
  case JT_ROGUE_B:
  case JT_SHADOW_CHASER:
  case JT_SHADOW_CHASER_H:
  case JT_SHADOW_CHASER_B:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_BOW:
      return true;
    }
    return false;
  case JT_PRIEST:
  case JT_PRIEST_H:
  case JT_PRIEST_B:
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
  case JT_CARDINAL:
    switch (weaponType) {
    case W_MACE:
    case W_STAFF:
    case W_BOOK:
    case W_2HSTAFF:
      return true;
    }
    return false;
  case JT_ASSASSIN:
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CROSS:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_1HAXE:
    case W_KATAR:
    case W_DOUBLE_DD:
    case W_DOUBLE_SS:
    case W_DOUBLE_AA:
    case W_DOUBLE_DS:
    case W_DOUBLE_DA:
    case W_DOUBLE_SA:
      return true;
    }
    return false;
  case JT_CRUSADER:
  case JT_CHICKEN2:
  case JT_CRUSADER_H:
  case JT_CHICKEN2_H:
  case JT_CRUSADER_B:
  case JT_CHICKEN2_B:
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_ROYAL_CHICKEN:
  case JT_ROYAL_CHICKEN_H:
  case JT_ROYAL_GUARD_B:
  case JT_ROYAL_CHICKEN_B:
  case JT_IMPERIAL_GUARD:
  case JT_IMPERIAL_GUARD_CHICKEN:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_2HSWORD:
    case W_1HSPEAR:
    case W_2HSPEAR:
    case W_1HAXE:
    case W_2HAXE:
    case W_MACE:
      return true;
    }
    return false;
  case JT_MONK:
  case JT_MONK_H:
  case JT_MONK_B:
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
  case JT_INQUISITOR:
    switch (weaponType) {
    case W_MACE:
    case W_STAFF:
    case W_KNUCKLE:
    case W_2HSTAFF:
      return true;
    }
    return false;
  case JT_SAGE:
  case JT_SAGE_H:
  case JT_SAGE_B:
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
  case JT_ELEMENTAL_MASTER:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_STAFF:
    case W_BOOK:
    case W_2HSTAFF:
      return true;
    }
    return false;
  case JT_ALCHEMIST:
  case JT_ALCHEMIST_H:
  case JT_ALCHEMIST_B:
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
  case JT_BIOLO:
    switch (weaponType) {
    case W_DAGGER:
    case W_1HSWORD:
    case W_1HAXE:
    case W_2HAXE:
    case W_MACE:
    case W_STAFF:
    case W_2HSTAFF:
      return true;
    }
    return false;
  case JT_BARD:
  case JT_BARD_H:
  case JT_BARD_B:
  case JT_MINSTREL:
  case JT_MINSTREL_H:
  case JT_MINSTREL_B:
  case JT_TROUBADOUR:
    switch (weaponType) {
    case W_DAGGER:
    case W_BOW:
    case W_MUSICAL:
      return true;
    }
    return false;
  case JT_GUNSLINGER:
  case JT_GUNSLINGER_B:
    switch (weaponType) {
    case W_REVOLVER:
    case W_RIFLE:
    case W_GATLING:
    case W_SHOTGUN:
    case W_GRENADE:
      return true;
    }
    return false;
  case JT_NINJA:
  case JT_NINJA_B:
    return weaponType == W_HUUMA;
  case JT_STAR:
  case JT_STAR2:
  case JT_STAR_B:
  case JT_STAR2_B:
  case JT_STAR_EMPEROR:
  case JT_STAR_EMPEROR_B:
  case JT_STAR2_EMPEROR:
  case JT_STAR2_EMPEROR_B:
  case JT_SKY_EMPEROR:
  case JT_SKY_EMPEROR2:
    return weaponType == W_BOOK;
  case JT_GANGSI:
  case JT_DEATHKNIGHT:
  case JT_COLLECTOR:
    return true;
  }

  return weaponType == W_DAGGER;
}

// OFFSET: 0x00DFBCE0
// FUNCTION: fn_23587
bool __stdcall IsPlayerJob(int job) {
  const auto value = static_cast<unsigned>(job);
  return value - JT_NOVICE_H <= unsigned(JT_MER_BEGIN - 1 - JT_NOVICE_H) || value <= unsigned(JT_SUMMER2);
}

// OFFSET: 0x00E19110
// FUNCTION: fn_23835
int CSession::InheritedJob(int job) {
  const int inherited = this->m_hotkeyScript->TableInteger("JobInheritTable", 0, job);
  const int replacement = this->m_hotkeyScript->TableInteger("JOB_INHERIT_LIST2", 0, inherited);
  return replacement == -1 ? inherited : replacement;
}

// OFFSET: 0x00E25B70
// FUNCTION: fn_23907
void CSession::InitializeSkillTree() {
  int count = 0;
  if (!this->m_hotkeyScript->Call("GetInheritJob", "d>d", this->m_job, &count) || count == -1) {
    return;
  }

  this->m_skillBundle.ClearGroups();
  this->m_skillBundle.ClearExtra();
  this->m_skillBundle.ClearLevelChanges();

  for (int index = count; index > 0; --index) {
    const int inherited = this->InheritedJob(index);
    int group = 1;
    if (inherited == JT_NOVICE || this->InheritedJob(index + 1) == JT_NOVICE || inherited == JT_DO_SUMMONER || inherited == JT_DO_SUMMONER_B) {
      group = 0;
    } else if (this->IsTreeGroupTwo(inherited) || inherited == JT_STAR_EMPEROR || inherited == JT_SOUL_REAPER || inherited == JT_STAR_EMPEROR_B ||
      inherited == JT_SOUL_REAPER_B) {
      group = 2;
    } else if (this->IsTreeGroupThree(inherited)) {
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

    this->m_hotkeyScript->Call("InitSkillTreeView", "dd", inherited, group);
  }
}

// OFFSET: 0x00DE0830
// FUNCTION: fn_23461
int __stdcall GetRealWeaponId(int weapon) {
  int real = 0;
  if (g_session.m_hotkeyScript->Call("GetRealWeaponId", "d>d", weapon, &real)) {
    return real;
  }
  return weapon;
}

// OFFSET: 0x00DFB4B0
// FUNCTION: fn_23583
bool __stdcall IsJobWeaponAllowed(int job, int slot, std::int64_t weapon) {
  if (weapon >= 105) {
    if (job == JT_ASSASSIN || job == JT_ASSASSIN_H || job == JT_ASSASSIN_B || job == JT_GUILLOTINE_CROSS || job == JT_GUILLOTINE_CROSS_H ||
      job == JT_GUILLOTINE_CROSS_B || job == JT_SHADOW_CROSS) {
      if (GetWeaponViewValue(static_cast<int>(weapon >> 32)) > 0) {
        weapon = GetDualWieldWeaponType(static_cast<int>(weapon), static_cast<int>(weapon >> 32));
      } else {
        weapon = GetWeaponViewValue(static_cast<int>(weapon));
      }
    } else {
      weapon = GetWeaponViewValue(static_cast<int>(weapon));
    }
  }

  const std::int64_t view = GetRealWeaponId(static_cast<int>(weapon));

  switch (job) {
  case JT_SWORDMAN:
  case JT_KNIGHT:
  case JT_CHICKEN:
  case JT_CRUSADER:
  case JT_CHICKEN2:
  case JT_SWORDMAN_H:
  case JT_KNIGHT_H:
  case JT_CHICKEN_H:
  case JT_CRUSADER_H:
  case JT_CHICKEN2_H:
  case JT_SWORDMAN_B:
  case JT_KNIGHT_B:
  case JT_CHICKEN_B:
  case JT_CRUSADER_B:
  case JT_CHICKEN2_B:
  case JT_RUNE_KNIGHT:
  case JT_RUNE_KNIGHT_H:
  case JT_ROYAL_GUARD:
  case JT_ROYAL_GUARD_H:
  case JT_RUNE_CHICKEN:
  case JT_RUNE_CHICKEN_H:
  case JT_ROYAL_CHICKEN:
  case JT_ROYAL_CHICKEN_H:
  case JT_RUNE_CHICKEN2:
  case JT_RUNE_CHICKEN2_H:
  case JT_RUNE_CHICKEN3:
  case JT_RUNE_CHICKEN3_H:
  case JT_RUNE_CHICKEN4:
  case JT_RUNE_CHICKEN4_H:
  case JT_RUNE_CHICKEN5:
  case JT_RUNE_CHICKEN5_H:
  case JT_RUNE_KNIGHT_B:
  case JT_ROYAL_GUARD_B:
  case JT_RUNE_CHICKEN_B:
  case JT_ROYAL_CHICKEN_B:
  case JT_DRAGON_KNIGHT:
  case JT_IMPERIAL_GUARD:
  case JT_DRAGON_KNIGHT_CHICKEN:
  case JT_IMPERIAL_GUARD_CHICKEN:
    return view == 4 || view == 5;
  case JT_MAGICIAN:
  case JT_MERCHANT:
  case JT_MAGICIAN_H:
  case JT_MERCHANT_H:
  case JT_MAGICIAN_B:
  case JT_MERCHANT_B:
    return view == 1;
  case JT_ARCHER:
  case JT_ARCHER_H:
  case JT_ARCHER_B:
    return view != 11;
  case JT_THIEF:
  case JT_HUNTER:
  case JT_ROGUE:
  case JT_BARD:
  case JT_DANCER:
  case JT_THIEF_H:
  case JT_HUNTER_H:
  case JT_ROGUE_H:
  case JT_BARD_H:
  case JT_DANCER_H:
  case JT_THIEF_B:
  case JT_HUNTER_B:
  case JT_ROGUE_B:
  case JT_BARD_B:
  case JT_DANCER_B:
  case JT_RANGER:
  case JT_RANGER_H:
  case JT_MINSTREL:
  case JT_WANDERER:
  case JT_SHADOW_CHASER:
  case JT_MINSTREL_H:
  case JT_WANDERER_H:
  case JT_SHADOW_CHASER_H:
  case JT_WOLF_RANGER:
  case JT_WOLF_RANGER_H:
  case JT_RANGER_B:
  case JT_MINSTREL_B:
  case JT_WANDERER_B:
  case JT_SHADOW_CHASER_B:
  case JT_WOLF_RANGER_B:
  case JT_WINDHAWK:
  case JT_ABYSS_CHASER:
  case JT_TROUBADOUR:
  case JT_TROUVERE:
  case JT_WOLF_WINDHAWK:
    return view == 11;
  case JT_PRIEST:
  case JT_PRIEST_H:
  case JT_PRIEST_B:
  case JT_ARCHBISHOP:
  case JT_ARCHBISHOP_H:
  case JT_ARCHBISHOP_B:
  case JT_CARDINAL:
    return view == 15;
  case JT_WIZARD:
  case JT_WIZARD_H:
  case JT_WIZARD_B:
  case JT_LINKER:
  case JT_WARLOCK:
  case JT_WARLOCK_H:
  case JT_WARLOCK_B:
  case JT_LINKER_B:
  case JT_SOUL_REAPER:
  case JT_SOUL_REAPER_B:
  case JT_ARCH_MAGE:
  case JT_SOUL_ASCETIC:
    if (view == 1) {
      return slot == 1;
    }

    if (view == 10 || view == 23) {
      return slot == 0;
    }

    return false;
  case JT_BLACKSMITH:
  case JT_BLACKSMITH_H:
  case JT_BLACKSMITH_B:
  case JT_MECHANIC:
  case JT_MECHANIC_H:
  case JT_MADOGEAR:
  case JT_MADOGEAR_H:
  case JT_MECHANIC_B:
  case JT_MADOGEAR_B:
  case JT_MEISTER:
  case JT_MEISTER_MADOGEAR:
  case JT_DRUID:
  case JT_DRUID_B:
  case JT_KARNOS:
  case JT_KARNOS_B:
  case JT_ALITEA:
    switch (view) {
    case 2:
    case 6:
    case 7:
    case 8:
      return true;
    default:
      return false;
    }
  case JT_ASSASSIN:
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CROSS:
    switch (view) {
    case 16:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
      return true;
    default:
      return false;
    }
  case JT_MONK:
  case JT_MONK_H:
  case JT_MONK_B:
  case JT_SURA:
  case JT_SURA_H:
  case JT_SURA_B:
  case JT_INQUISITOR:
    return view == 0 || view == 12;
  case JT_SAGE:
  case JT_SAGE_H:
  case JT_SAGE_B:
  case JT_SORCERER:
  case JT_SORCERER_H:
  case JT_SORCERER_B:
  case JT_ELEMENTAL_MASTER:
    switch (view) {
    case 5:
    case 10:
    case 15:
    case 23:
      return true;
    default:
      return false;
    }
  case JT_ALCHEMIST:
  case JT_ALCHEMIST_H:
  case JT_ALCHEMIST_B:
  case JT_GENETIC:
  case JT_GENETIC_H:
  case JT_GENETIC_B:
  case JT_BIOLO:
    switch (view) {
    case 2:
    case 6:
    case 7:
    case 8:
      return true;
    default:
      return false;
    }
  case JT_NOVICE:
  case JT_SUPERNOVICE:
  case JT_NOVICE_H:
  case JT_NOVICE_B:
  case JT_SUPERNOVICE_B:
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
  case JT_HYPER_NOVICE:
    if (slot == 0) {
      return view == 1;
    }

    if (slot != 1) {
      return false;
    }

    switch (view) {
    case 2:
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 23:
      return true;
    default:
      return false;
    }
  case JT_GUNSLINGER:
  case JT_REBELLION:
  case JT_GUNSLINGER_B:
  case JT_REBELLION_B:
  case JT_NIGHT_WATCH:
    return view >= 18 && view <= 21;
  case JT_NINJA:
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_NINJA_B:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
    return view == 22;
  default:
    return false;
  }
}


// OFFSET: 0x00E0AB60
// FUNCTION: fn_23803
std::string CSession::GetWeaponName(int weapon) {
  const unsigned index = GetRealWeaponId(weapon);
  if (index < static_cast<unsigned>(this->m_weaponNamesEnd - this->m_weaponNamesBegin)) {
    return this->m_weaponNamesBegin[index];
  }

  return this->m_weaponNamesBegin[0];
}

// OFFSET: 0x00DE3800
// FUNCTION: fn_23482
const char* CSession::GetWeaponClassName(int item) {
  const char* name = nullptr;
  if (g_session.m_hotkeyScript->Call("ReqWeaponNameByClassNum", "d>s", FindItemDescription(item).unknown_0040, &name)) {
    return name;
  }

  return "";
}

// OFFSET: 0x00BAD420
// FUNCTION: fn_19759
std::string EquipmentResourceMgr::GetWeaponResourceName(int type, int job, int sex, int weapon, int weaponItem, int costume) {
  std::string result;
  std::string jobName;
  std::string sexName;
  std::string weaponName;

  const int spriteJob = g_session.GetSpriteJob(job <= 31 ? job : job + 3950, costume, true);
  jobName = g_session.m_jobSpriteNames[spriteJob];
  sexName = g_session.GetSexName(sex);

  if (weaponItem == -1) {
    weaponName = g_session.GetWeaponName(weapon);
    result = this->GetBodyRacePrefix(spriteJob) +
      FormatOwnedString("\\%s_%s%s.%s", jobName.c_str(), sexName.c_str(), weaponName.c_str(), g_equipmentResourceExtensions[type]);
  } else {
    result = this->GetBodyRacePrefix(spriteJob + 3950) +
      FormatOwnedString("\\%s_%s%s.%s", jobName.c_str(), sexName.c_str(), g_session.GetWeaponClassName(weaponItem), g_equipmentResourceExtensions[type]);
  }

  return result;
}

// OFFSET: 0x00BACEE0
// FUNCTION: fn_19758
std::string EquipmentResourceMgr::GetWeaponEffectResourceName(int type, int job, int sex, int weapon, int weaponItem, int effect, int costume) {
  std::string result;
  std::string jobName;
  std::string sexName;
  std::string weaponName;
  const char* const effects[] = {"_검광", "_발광"};

  const int spriteJob = g_session.GetSpriteJob(job <= 31 ? job : job + 3950, costume, true);
  jobName = g_session.m_jobSpriteNames[spriteJob];
  sexName = g_session.GetSexName(sex);
  weaponName = g_session.GetWeaponName(weapon);

  const char* const suffix = effects[effect];
  if (weaponItem == -1) {
    result = this->GetBodyRacePrefix(spriteJob) +
      FormatOwnedString("\\%s_%s%s%s.%s", jobName.c_str(), sexName.c_str(), weaponName.c_str(), suffix, g_equipmentResourceExtensions[type]);
  } else {
    result = this->GetBodyRacePrefix(spriteJob) +
      FormatOwnedString("\\%s_%s%s%s.%s", jobName.c_str(), sexName.c_str(), g_session.GetWeaponClassName(weaponItem), suffix, g_equipmentResourceExtensions[type]);
  }

  return result;
}

// OFFSET: 0x00E19370
// FUNCTION: fn_23837
std::string CSession::GetWeaponActName(int job, int sex, int weapon, int weaponItem, int costume) {
  return g_equipmentResourceMgr->GetWeaponResourceName(0, job, sex, weapon, weaponItem, costume);
}

// OFFSET: 0x00E193E0
// FUNCTION: fn_23838
std::string CSession::GetWeaponEffectActName(int job, int sex, int weapon, int weaponItem, int effect, int costume) {
  return std::string(g_equipmentResourceMgr->GetWeaponEffectResourceName(0, job, sex, weapon, weaponItem, effect, costume));
}

// OFFSET: 0x00E19450
// FUNCTION: fn_23839
std::string CSession::GetWeaponEffectSprName(int job, int sex, int weapon, int weaponItem, int effect, int costume) {
  std::string name = g_equipmentResourceMgr->GetWeaponEffectResourceName(1, job, sex, weapon, weaponItem, effect, costume);
  return name;
}

// OFFSET: 0x00E194C0
// FUNCTION: fn_23840
std::string CSession::GetWeaponSprName(int job, int sex, int weapon, int weaponItem, int costume) {
  std::string name = g_equipmentResourceMgr->GetWeaponResourceName(1, job, sex, weapon, weaponItem, costume);
  return name;
}

// OFFSET: 0x00DFBE10
// FUNCTION: fn_23590
bool __stdcall IsVisibleJob(int job) {
  return job != JT_WARPNPC && job != JT_EFFECTLAUNCHER && job != JT_HIDDEN_NPC;
}

// OFFSET: 0x00E19230
// FUNCTION: fn_23836
std::string GetJobTitleString(int job) {
  const char* title = nullptr;

  if (!g_session.m_hotkeyScript->Call("GetTitleString", "d>s", job, &title)) {
    return "";
  }

  return title ? title : "";
}

// OFFSET: 0x00E19530
// FUNCTION: fn_23841
int __stdcall GetWeaponViewValue(int weapon) {
  const int value = FindItemDescription(weapon).unknown_0040;

  if (weapon > 2100 && weapon < 2200 || static_cast<unsigned>(weapon - 28901) <= 0x61 || static_cast<unsigned>(weapon - 460000) <= 0x270F) {
    return -1;
  }

  return value;
}

// OFFSET: 0x00E248F0
// FUNCTION: fn_23898
int __stdcall GetDualWieldWeaponType(int first, int second) {
  int main = first;
  int result = 0;
  int sub;

  if (first != 0) {
    sub = second;
  } else {
    main = second;

    if (second == 0) {
      return result;
    }

    sub = 0;
  }

  if (main >= 1100 && main < 1150 || main >= 13400 && main < 13500 || static_cast<unsigned>(main - 500000) <= 0x270F) {
    result = 2;

    if (sub >= 1100 && sub < 1150 || sub >= 13400 && sub < 13500 || static_cast<unsigned>(sub - 500000) <= 0x270F) {
      return 26;
    }

    if ((sub < 1200 || sub >= 1250) && (sub < 13000 || sub >= 13100) && (sub < 28700 || sub >= 28900) &&
      static_cast<unsigned>(sub - 510000) > 0x270F) {
      if (sub >= 1300 && sub < 1350 || static_cast<unsigned>(sub - 520000) <= 0x270F) {
        return 30;
      }

      return result;
    }

    return 28;
  }

  if (main >= 1200 && main < 1250 || main >= 13000 && main < 13100 || main >= 28700 && main < 28900 ||
    static_cast<unsigned>(main - 510000) <= 0x270F) {
    result = 1;

    if ((sub < 1100 || sub >= 1150) && (sub < 13400 || sub >= 13500) && static_cast<unsigned>(sub - 500000) > 0x270F) {
      if (sub >= 1200 && sub < 1250 || sub >= 13000 && sub < 13100 || sub >= 28700 && sub < 28900 ||
        static_cast<unsigned>(sub - 510000) <= 0x270F) {
        return 25;
      }

      if (sub >= 1300 && sub < 1350 || static_cast<unsigned>(sub - 520000) <= 0x270F) {
        return 29;
      }

      return result;
    }

    return 28;
  }

  if (main >= 1300 && main < 1350 || static_cast<unsigned>(main - 520000) <= 0x270F) {
    result = 6;

    if (sub >= 1300 && sub < 1350) {
      return 27;
    }

    if (static_cast<unsigned>(sub - 520000) <= 0x270F) {
      return 27;
    }
  }

  return result;
}

// OFFSET: 0x00E25970
// FUNCTION: fn_23906
bool ResetHotkeySlots() {
  g_session.m_hotkeyScript->Call("ResetTheHotKey", "dddd", 1, 132, 192, 18);
  return g_session.m_hotkeyScript->Call("ResetTheHotKey", "dddd", 1, 135, 36, 18);
}

// OFFSET: 0x00E25E60
// FUNCTION: fn_23909
std::pair<int, int>* CSession::InsertPair(int key, int value) {
  for (auto& entry : this->unknown_0778) {
    if (entry.first == key) {
      entry.second = value;
      return &entry;
    }
  }

  this->unknown_0778.push_back(std::make_pair(key, value));
  return &this->unknown_0778.back();
}

// OFFSET: 0x00E0A1F0
// FUNCTION: fn_23792
int CSession::GetPairValue() const {
  const int first = this->m_pairValues[0] != -1 ? this->m_pairValues[0] : -1;
  const int second = this->m_pairValues[1] == -1 ? first : this->m_pairValues[1];
  const int third = this->m_pairValues[2];
  return third == -1 ? second : third;
}

// OFFSET: 0x00E0A220
// FUNCTION: fn_23793
int CSession::GetPetTargetAid() const {
  int target = 0;
  std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(&this->unknown_5570[8]);
  CGameMode* mode;
  CRenderObject* actor;
  if (aid != 0 && (mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) != nullptr &&
      (actor = mode->scene->GetGameActorByAID(aid)) != nullptr) {
    target = static_cast<CGameActor*>(actor)->hitTargetAid;
  }

  aid = *reinterpret_cast<const std::uint32_t*>(&this->unknown_5620[8]);
  if (aid != 0 && (mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) != nullptr &&
      (actor = mode->scene->GetGameActorByAID(aid)) != nullptr) {
    target = static_cast<CGameActor*>(actor)->hitTargetAid;
  }

  if (target > 0) {
    mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode == nullptr || mode->scene->GetGameActorByAID(target) == nullptr) {
      return 0;
    }
  }
  return target;
}

// OFFSET: 0x00E1D950
// FUNCTION: fn_23854
int CSession::WorldUnknown0050() const {
  return static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->unknown_0050;
}

// OFFSET: 0x00E1D970
// FUNCTION: fn_23855
bool CSession::IsShadowItemType(int type) const {
  return type == 18;
}

// OFFSET: 0x00E1D980
// FUNCTION: fn_23856
bool CSession::IsConsumableItemType(int type) const {
  if (type == 0 || type == 1 || type == 2 || type == 18) {
    return true;
  }
  return false;
}

// OFFSET: 0x00E1D9B0
// FUNCTION: fn_23857
bool CSession::IsCostumeLocation(int location) const {
  if (location == 0x400 || location == 0x800 || location == 0xC00 || location == 0x1000 || location == 0x1400 || location == 0x1800 ||
      location == 0x1C00 || location == 0x2000 || location == 0x4000 || location == 0x10000 || location == 0x20000 || location == 0x40000 ||
      location == 0x80000 || location == 0x100000 || location == 0x200000) {
    return true;
  }
  return false;
}

// OFFSET: 0x00E1DA40
// FUNCTION: fn_23858
int CSession::IsBerserkActive() const {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == EFST_BERSERK) {
      return 1;
    }
  }
  return 0;
}

// OFFSET: 0x00E1DA70
// FUNCTION: fn_23859
int CSession::IsTrickDeadActive() const {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == EFST_TRICKDEAD) {
      return 1;
    }
  }
  return 0;
}

// OFFSET: 0x00E1DAA0
// FUNCTION: fn_23860
int CSession::GetMapProperty08() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 8);
}

// OFFSET: 0x00E1DAB0
// FUNCTION: fn_23861
int CSession::GetMapProperty07() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 7);
}

// OFFSET: 0x00E1DAC0
// FUNCTION: fn_23862
int CSession::GetMapProperty13() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 0x13);
}

// OFFSET: 0x00E1DAD0
// FUNCTION: fn_23863
int CSession::GetMapProperty11() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 0x11);
}

// OFFSET: 0x00E1DAE0
// FUNCTION: fn_23864
int CSession::GetMapProperty10() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 0x10);
}

// OFFSET: 0x00E1DAF0
// FUNCTION: fn_23865
int CSession::GetMapProperty12() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 0x12);
}

// OFFSET: 0x00E1DB00
// FUNCTION: fn_23866
int CSession::GetMapProperty09() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 9);
}

// OFFSET: 0x00E1DB10
// FUNCTION: fn_23867
int CSession::GetMapProperty0F() const {
  return g_mapPropertyMgr->GetValue(this->unknown_0000, 0xF);
}

// OFFSET: 0x00E1DB20
// FUNCTION: fn_23868
bool CSession::IsEquipItemType(int type) const {
  if (type == 4 || type == 11 || type == 12 || type == 13 || type == 14 || type == 5 || type == 8 || type == 9 || type == 15) {
    return true;
  }
  return false;
}

// OFFSET: 0x00E1DB60
// FUNCTION: fn_23869
bool CSession::IsMiscItemType(int type) const {
  if (type == 6 || type == 3 || type == 7 || type == 10 || type == 16 || type == 17 || type == 19) {
    return true;
  }
  return false;
}

// OFFSET: 0x00E1DBA0
// FUNCTION: fn_23870
bool CSession::HasUnknown6178Records() const {
  return this->unknown_6178.size() != 0;
}

// OFFSET: 0x00E1DBD0
// FUNCTION: fn_23871
int CSession::HasActiveSkillEffect(int id) const {
  for (const ActiveSkillEffect& effect : g_activeSkillEffects) {
    if (effect.id == id) {
      return 1;
    }
  }
  return 0;
}

// OFFSET: 0x00E1DC00
// FUNCTION: fn_23872
int CSession::IsGuardianJob(int job) const {
  return job == JT_ARCHER_GUARDIAN || job == JT_KNIGHT_GUARDIAN || job == JT_SOLDIER_GUARDIAN ? 1 : 0;
}

// OFFSET: 0x00E1DF50
// FUNCTION: fn_23887
int CSession::IsPlayerJobId(int job) const {
  return IsPlayerJob(job);
}

// OFFSET: 0x00E1E050
// FUNCTION: fn_23892
bool CSession::IsSuperNoviceJob(int job) const {
  if (job == JT_SUPERNOVICE || job == JT_SUPERNOVICE_B || job == JT_PORING_SNOVICE || job == JT_PORING_SNOVICE_B) {
    return true;
  }
  return false;
}

// OFFSET: 0x00E1E080
// FUNCTION: fn_23893
bool CSession::IsSuperNoviceClass(int job) const {
  if (job == JT_SUPERNOVICE || job == JT_SUPERNOVICE_B || job == JT_PORING_SNOVICE || job == JT_PORING_SNOVICE_B || job == JT_SUPERNOVICE2 ||
      job == JT_SUPERNOVICE2_B || job == JT_PORING_SNOVICE2 || job == JT_PORING_SNOVICE2_B || job == JT_HYPER_NOVICE || job == JT_HYPER_NOVICE_RIDING) {
    return true;
  }
  return false;
}

// OFFSET: 0x00E24550
// FUNCTION: fn_23896
bool CSession::PlaySkillTargetEffect(int skillId, int level, CGameActor* source, CGameActor* target) {
  bool hasInfo = false;
  if (!g_session.m_hotkeyScript->Call("HaveSkillEffectInfo", "d>b", skillId, &hasInfo) || !hasInfo) {
    return false;
  }

  const char* waveName = nullptr;
  if (g_session.m_hotkeyScript->Call("GetTargetEffectWaveFileName", "d>s", skillId, &waveName) && std::strcmp(waveName, "") != 0) {
    g_soundMgr->PlayWave(waveName, 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  int count = 0;
  if (g_session.m_hotkeyScript->Call("GetTargetEffectNum", "d>d", skillId, &count)) {
    for (int index = 0; index < count; ++index) {
      int effectId = -1;
      g_session.m_hotkeyScript->Call("GetTargetEffectID", "dd>d", skillId, index + 1, &effectId);
      if (effectId == -1) {
        continue;
      }

      if (g_effectMgr->HasAllocator(effectId)) {
        g_effectMgr->Spawn(
          target->render.accountId, effectId, target->render.position.x, target->render.position.y, target->render.position.z, 0, -1, 0, 0
        );
      } else {
        target->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
  }
  return true;
}

// OFFSET: 0x00E248E0
// FUNCTION: fn_23897
void CSession::ApplySaveDataOptions() {
}

// OFFSET: 0x00E25EE0
// FUNCTION: fn_23910
int CSession::SetPairValue(int value, int slot) {
  if (slot == 0) {
    this->m_pairValues[0] = value;
    return value;
  }

  if (slot == 1) {
    this->m_pairValues[1] = value;
    return value;
  }

  if (slot == 2) {
    this->m_pairValues[2] = value;
    return value;
  }

  return slot;
}

// OFFSET: 0x00E25F20
// FUNCTION: fn_23911
int CSession::SetHotkeyEntry(char enabled, int id, int value, int slot) {
  for (int index = 0; index < 2; ++index) {
    NormalItemSlot& entry = this->SkillHotkey(index);

    if (id == static_cast<int>(entry.itemId) && value == entry.unknown_0005 && enabled == static_cast<char>(entry.unknown_0000) ||
      !CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, static_cast<int>(entry.itemId)) && id == static_cast<int>(entry.itemId)) {
      entry.unknown_0000 = 0;
      entry.itemId = 0;
      entry.unknown_0005 = 0;
      g_modeMgr.GetGameMode()->SendMsg(313, index + 36, 0, 0, 0);
    }
  }

  if (id == 0) {
    enabled = 0;
    value = 0;
  }

  NormalItemSlot& target = this->SkillHotkey(slot);
  target.unknown_0000 = static_cast<unsigned char>(enabled);
  target.itemId = static_cast<std::uint32_t>(id);
  target.unknown_0005 = static_cast<std::int16_t>(value);

  return g_modeMgr.GetGameMode()->SendMsg(313, slot + 36, 0, 0, 0);
}

// OFFSET: 0x00E26030
// FUNCTION: fn_23912
int CSession::SetFlag(char value) {
  const int result = value & 1;
  this->unknown_0E98 = result;
  this->unknown_5558 = result;
  return result;
}

// OFFSET: 0x00DFB490
// FUNCTION: fn_23582
bool IsScreenShakeActive() {
  return g_screenShakeDuration != 0 && g_screenShakeMode == 2;
}

// OFFSET: 0x00E25DD0
// FUNCTION: fn_23908
void CSession::SetPlayerNameKey(const unsigned char* key) {
  std::memcpy(this->m_playerNameKey, key, sizeof(this->m_playerNameKey));

  for (unsigned int index = 0; index < sizeof(this->m_playerNameKey); ++index) {
    this->m_playerNameKey[index] ^= g_playerNameKey[index];
  }
}

// OFFSET: 0x00E1D870
// FUNCTION: fn_23853
bool CSession::IsBabyJob(int job) const {
  return (job >= JT_NOVICE_B && job <= JT_THIEF_B) || job == JT_SUPERNOVICE_B || job == JT_SUPERNOVICE2_B ||
         (job >= JT_KNIGHT_B && job <= JT_CHICKEN2_B) || (job >= JT_RUNE_KNIGHT_B && job <= JT_MADOGEAR_B) ||
         (job >= JT_PORING_NOVICE_B && job <= JT_DOG_CHASER_B) || (job >= JT_SHEEP_PRIEST_B && job <= JT_PIG_BLACKSMITH_B) ||
         job == JT_PORING_SNOVICE2_B || (job >= JT_PIG_MECHANIC_B && job <= JT_LION_CRUSADER_B) || job == JT_DO_SUMMONER_B ||
         (job >= JT_NINJA_B && job <= JT_STAR2_B) || job == JT_STAR_EMPEROR_B || job == JT_SOUL_REAPER_B ||
         job == JT_STAR2_EMPEROR_B || job == JT_HAETAE_STAR_EMPEROR_B || job == JT_HAETAE_SOUL_REAPER_B || job == JT_DRUID_B ||
         job == JT_KARNOS_B;
}

// OFFSET: 0x00E1DC50
// FUNCTION: fn_23873
int CSession::IsMercenaryJob(int job) const {
  return (job >= JT_MER_LIF && job <= JT_MER_VANILMIRTH_H2) || (job >= JT_MER_EIRA && job <= JT_MER_ELEANOR) ? 1 : 0;
}

// OFFSET: 0x00E1DC80
// FUNCTION: fn_23874
int CSession::IsNewMercenaryJob(int job) const {
  return job >= JT_MER_EIRA && job <= JT_MER_ELEANOR;
}

// OFFSET: 0x00E1E020
// FUNCTION: fn_23891
bool CSession::IsSuperNovice2Job(int job) const {
  return job == JT_SUPERNOVICE2 || job == JT_SUPERNOVICE2_B || job == JT_PORING_SNOVICE2 || job == JT_PORING_SNOVICE2_B;
}

// OFFSET: 0x00E1E0E0
// FUNCTION: fn_23894
bool CSession::IsExpandedJob(int job) const {
  switch (job) {
  case JT_KAGEROU:
  case JT_OBORO:
  case JT_FROG_KAGEROU:
  case JT_FROG_OBORO:
  case JT_REBELLION:
  case JT_PECO_REBELLION:
  case JT_KAGEROU_B:
  case JT_OBORO_B:
  case JT_REBELLION_B:
  case JT_FROG_KAGEROU_B:
  case JT_FROG_OBORO_B:
  case JT_PECO_REBELLION_B:
  case JT_STAR_EMPEROR:
  case JT_SOUL_REAPER:
  case JT_STAR_EMPEROR_B:
  case JT_SOUL_REAPER_B:
  case JT_STAR2_EMPEROR:
  case JT_STAR2_EMPEROR_B:
  case JT_HAETAE_STAR_EMPEROR:
  case JT_HAETAE_SOUL_REAPER:
  case JT_HAETAE_STAR_EMPEROR_B:
  case JT_HAETAE_SOUL_REAPER_B:
  case JT_SKY_EMPEROR:
  case JT_SOUL_ASCETIC:
  case JT_SHINKIRO:
  case JT_SHIRANUI:
  case JT_NIGHT_WATCH:
  case JT_HYPER_NOVICE:
  case JT_SPIRIT_HANDLER:
  case JT_SKY_EMPEROR_RIDING:
  case JT_SOUL_ASCETIC_RIDING:
  case JT_SHINKIRO_RIDING:
  case JT_SHIRANUI_RIDING:
  case JT_NIGHT_WATCH_RIDING:
  case JT_HYPER_NOVICE_RIDING:
  case JT_SPIRIT_HANDLER_RIDING:
  case JT_SKY_EMPEROR2:
  case JT_KARNOS:
  case JT_KARNOS_B:
  case JT_KARNOS_RIDING:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00E3D350
// FUNCTION: fn_24188
bool CSession::IsCostumeJob(int job) const {
  switch (job) {
  case JT_MARRIED:
  case JT_SANTA:
  case JT_SUMMER:
  case JT_HANBOK:
  case JT_OKTOBERFEST:
  case JT_SUMMER2:
    return true;
  default:
    return false;
  }
}

int GetEquipLocationIndex(const int& mask);

namespace EquipmentProperties {
class CEquipmentProperties;

class CEquipment {
public:
  virtual ~CEquipment();
  virtual void Load(bool force) = 0;
  virtual void Clear();
  virtual int GetType() = 0;
  virtual int GetValue(const int& key);
  int& GetId();

  /* 0004 */ int m_id;
  /* 0008 */ void* m_info;
  /* 000C */ unsigned char m_properties[0x28];
  /* 0034 */ bool m_pretend;
};

class CEquipmentAttachable : public CEquipment {
public:
  void Load(bool force) override;
  int GetType() override;
  CEquipment* GetOwner();

  /* 0038 */ int unknown_0038;
  /* 003C */ CEquipment* m_owner;
};

class CEquipmentWearable : public CEquipment {
public:
  void Load(bool force) override;
  void Clear() override;
  int GetType() override;
  std::vector<CEquipmentAttachable>& GetAttachables();
  bool IsActive();

  /* 0038 */ bool m_active;
  /* 003C */ int m_type;
  /* 0040 */ int unknown_0040;
  /* 0044 */ std::vector<CEquipmentAttachable> m_attachables;
  /* 0050 */ std::list<CEquipmentProperties*> m_children;
};

class CEquipmentPropertiesMgr {
public:
  virtual ~CEquipmentPropertiesMgr();
  /* 0004 */ unsigned char unknown_0004[0x9];
  /* 000D */ bool m_dirty;
  /* 000E */ unsigned char unknown_000E[0x12];
  /* 0020 */ UIWindow* unknown_0020;
  /* 0024 */ std::unordered_map<int, CEquipmentWearable*> properties1;

  CEquipmentPropertiesMgr();
  CEquipmentWearable* FindProperty1(const int& key);
  void UnequipItem(const int& itemId, bool notify);
  void RemoveWearable(const int& location);
};

void CEquipmentPropertiesMgr::RemoveWearable(const int& location) {
  this->m_dirty = true;
  const int key = GetEquipLocationIndex(location);
  CEquipmentWearable* wearable = this->FindProperty1(key);
  if (wearable != nullptr) {
    for (CEquipmentAttachable& attachable : wearable->GetAttachables()) {
      this->UnequipItem(attachable.GetId(), true);
    }
    this->UnequipItem(wearable->GetId(), true);
    wearable->Clear();
    delete wearable;
    this->properties1.erase(key);
  }
  if (this->unknown_0020 != nullptr) {
    this->unknown_0020->SendMsg(nullptr, 0x3C, 0, 0, 0, 0);
  }
}
}

// OFFSET: 0x00E24B30
// FUNCTION: fn_23899
bool CSession::UnequipItem2B50(int location) {
  if (location == 0x8000) {
    this->SetUnknown16EC(0);
  } else {
    const EquipSlots slots = g_session.GetEquipSlots(location);
    if (slots.values[2] < 10) {
      if (slots.values[2] != -1) {
        ITEM_INFO& item = this->unknown_2B50[slots.values[2]];
        const char* message = LookupMessage(0xAB);
        const std::string name = item.DisplayName(0);
        const std::string text = FormatOwnedString("%s%s", name.c_str(), message);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 7, 0, 0);
        item.unknown_0000[3] = 0;
        reinterpret_cast<ClientItemLists*>(this)->AddItem1710(item, 1);
        item.Reset();
      }
      if (slots.values[1] != -1 && slots.values[1] < 10) {
        this->unknown_2B50[slots.values[1]].Reset();
      }
      if (slots.values[0] != -1 && slots.values[0] < 10) {
        this->unknown_2B50[slots.values[0]].Reset();
      }
    }
    RequestBasicInfoRefresh();
  }

  this->InvalidateWindowFrames();
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_022C)) {
    window->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
  if (g_equipmentPropertiesMgr == nullptr) {
    g_equipmentPropertiesMgr = new (std::nothrow) EquipmentProperties::CEquipmentPropertiesMgr;
  }
  g_equipmentPropertiesMgr->RemoveWearable(location);
  return true;
}

// OFFSET: 0x00E1E250
// FUNCTION: fn_45483
bool LaunchLuaSkillEffect(int skillId, int level, CGameActor* source, CGameActor* target) {
  vector3d position{0.0f, 0.0f, 0.0f};
  std::uint32_t sourceId = 0;
  if (source != nullptr) {
    position = source->render.position;
    sourceId = source->render.accountId;
  }
  const std::uint32_t targetId = target != nullptr ? target->render.accountId : 0;

  bool hasInfo = false;
  if (!g_session.m_hotkeyScript->Call("HaveSkillEffectInfo", "d>b", skillId, &hasInfo) || !hasInfo) {
    return false;
  }

  const char* waveName = nullptr;
  if (g_session.m_hotkeyScript->Call("GetEffectWaveFileName", "d>s", skillId, &waveName) && std::strcmp(waveName, "") != 0) {
    g_soundMgr->PlayWave(waveName, 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  int count = 0;
  if (!g_session.m_hotkeyScript->Call("GetEffectNum", "d>d", skillId, &count)) {
    return false;
  }

  for (int index = 0; index < count; ++index) {
    int effectId = -1;
    if (!g_session.m_hotkeyScript->Call("GetEffectID", "dd>d", skillId, index + 1, &effectId) || effectId == -1) {
      continue;
    }

    if (g_effectMgr->HasAllocator(effectId)) {
      g_effectMgr->Spawn(sourceId, effectId, position.x, position.y, position.z, sourceId != targetId ? targetId : 0, -1, 0, 0);
    } else if (source != nullptr) {
      CRagEffect* effect = source->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      if (effect != nullptr) {
        bool onTarget = false;
        g_session.m_hotkeyScript->Call("IsOnTarget", "d>b", skillId, &onTarget);
        if (onTarget && target != nullptr) {
          effect->SendMsg(nullptr, 14, 0, reinterpret_cast<std::intptr_t>(&target->render.position), 0, 0, 0, 0);
        }
      }
    }
  }
  return true;
}

// OFFSET: 0x00E24DE0
// FUNCTION: fn_23901
bool CSession::PlaySkillGroundEffect(int skillId, vector3d position) {
  bool hasInfo = false;
  if (!g_session.m_hotkeyScript->Call("HaveSkillEffectInfo", "d>b", skillId, &hasInfo) || !hasInfo) {
    return false;
  }

  const char* waveName = nullptr;
  if (g_session.m_hotkeyScript->Call("GetEffectWaveFileName", "d>s", skillId, &waveName) && std::strcmp(waveName, "") != 0) {
    g_soundMgr->PlayWave(waveName, 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }

  int count = 0;
  if (g_session.m_hotkeyScript->Call("GetGroundEffectNum", "d>d", skillId, &count)) {
    for (int index = 0; index < count; ++index) {
      int effectId = -1;
      g_session.m_hotkeyScript->Call("GetGroundEffectID", "dd>d", skillId, index + 1, &effectId);
      if (effectId == -1) {
        continue;
      }

      if (g_effectMgr->HasAllocator(effectId)) {
        g_effectMgr->Spawn(0, effectId, position.x, position.y, position.z, 0, -1, 0, 0);
      } else {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())
          ->LaunchDummyEffect(&position, effectId, EffectLifetime(effectId) + 10, 0);
      }
    }
  }
  return true;
}

// OFFSET: 0x00E2F9F0
// FUNCTION: fn_24162
int CSession::GetMaxBaseLevel(int job) const {
  if (g_session.IsTreeGroupThree(job) == true) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x74]);
  }
  if (g_session.IsTreeGroupTwo(job)) {
    return g_session.m_maxLevel;
  }
  if (g_session.IsSuperNovice2Job(job)) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0]);
  }
  if (g_session.IsExpandedJob(job)) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[4]);
  }
  return g_session.IsDoramJob(job) == true ? *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x28])
                                           : *reinterpret_cast<const int*>(g_session.unknown_7EE8);
}

// OFFSET: 0x00E2FA80
// FUNCTION: fn_24163
int CSession::GetMaxJobLevel(int job) const {
  if (g_session.IsSuperNoviceJob(job)) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x10]);
  }
  if ((job >= JT_KNIGHT_H && job < JT_NOVICE_B) || job == JT_GUNSLINGER || job == JT_NINJA || job == JT_GUNSLINGER_B || job == JT_NINJA_B ||
      job == JT_DRUID || job == JT_DRUID_B) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x18]);
  }
  if (job == JT_NOVICE || job == JT_NOVICE_B || job == JT_NOVICE_H) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x0C]);
  }
  if (g_session.IsTreeGroupThree(job) == true) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x78]);
  }
  if (g_session.IsTreeGroupTwo(job)) {
    return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x1C]);
  }
  if (!g_session.IsExpandedJob(job)) {
    if (g_session.IsDoramJob(job) == true) {
      return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x2C]);
    }
    if (!g_session.IsSuperNovice2Job(job)) {
      return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x14]);
    }
  }
  return *reinterpret_cast<const int*>(&g_session.unknown_7EF0[0x24]);
}

// OFFSET: 0x00E1E690
// FUNCTION: fn_23895
void CSession::PlaySkillEffect(int skillId, int level, CGameActor* source, CGameActor* target) {
  if (source == nullptr || target == nullptr) {
    return;
  }

  if (this->PlaySkillTargetEffect(skillId, level, source, target) == true) {
    return;
  }

  CRagEffect* effect;
  vector3d position;

  switch (skillId) {
  case 0xE:
    effect = target->LaunchEffect(0x1A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, level, 0, 0, 0, 0);
    }
    break;
  case 0xF:
    effect = source->LaunchEffect(0x1B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x11:
    effect = source->LaunchEffect(0x18, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x13:
    effect = target->LaunchEffect(0x1F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, level, 0, 0, 0, 0);
    }
    break;
  case 0x14:
    effect = target->LaunchEffect(0x1D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, level, 0, 0, 0, 0);
    }
    break;
  case 0x15:
    target->LaunchEffect(0x1E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1C:
    target->LaunchEffect(0x140, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2F: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    vector3d* sourcePosition = &source->render.position;
    mode->SpawnArrowEffect64(sourcePosition, &target->render.position, 18.0f);
    int column;
    int row;
    mode->WorldCell(target->render.position.x, target->render.position.z, column, row);
    const int offsets[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
    for (int index = 0; index < 8; ++index) {
      float x;
      float z;
      mode->scene->terrain->Grid().CellCenter(offsets[index][0] + column, offsets[index][1] + row, x, z);
      position.x = x;
      position.y = mode->scene->terrain->Grid().Height(x, z);
      position.z = z;
      mode->SpawnArrowEffect64(sourcePosition, &position, 18.0f);
    }
    break;
  }
  case 0x38:
    effect = source->LaunchEffect(0x94, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, level, 0, 0, 0, 0);
      position = source->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x39:
    effect = target->LaunchEffect(0x46, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x3A:
    effect = source->LaunchEffect(0x96, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x3B:
    effect = source->LaunchEffect(0x50, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    effect = source->LaunchEffect(0x97, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = source->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x3E:
    effect = source->LaunchEffect(0x95, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = source->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x4D:
    effect = source->LaunchEffect(0x52, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x54:
    effect = source->LaunchEffect(0x5D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x56:
    effect = source->LaunchEffect(0x75, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x5A:
    effect = target->LaunchEffect(0x4F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x88:
    effect = source->LaunchEffect(0x79, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    effect = source->LaunchEffect(0x8F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = source->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x89:
    effect = source->LaunchEffect(0x7B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x95:
    target->LaunchEffect(0x136, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x98:
    if (g_modeMgr.GetGameMode() != nullptr) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchTargetEffect(0x134, source, target, -1);
    }
    break;
  case 0x99:
    source->LaunchEffect(0xAA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0xA0:
    effect = source->LaunchEffect(0x50, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0xD4:
    effect = source->LaunchEffect(0x113, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0xDB:
    effect = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(0xE3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = source->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0xE6:
    if (g_modeMgr.GetGameMode() != nullptr) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchTargetEffect(0x12A, source, target, -1);
    }
    break;
  case 0xFA:
    target->LaunchEffect(0xF6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0xFB:
    effect = source->LaunchEffect(0xF9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0xFD:
  case 0x152:
    target->LaunchEffect(0xF5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0xFE:
    source->LaunchEffect(0xE2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x107:
    effect = source->LaunchEffect(0x149, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x10A:
    effect = source->LaunchEffect(0x108, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    effect = source->LaunchEffect(0x10B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x10B:
    effect = source->LaunchEffect(0x109, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      effect->SendMsg(nullptr, 44, 0, level, 0, 0, 0, 0);
    }
    break;
  case 0x10F:
    g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    g_soundMgr->PlayWave("effect\\mon_아수라 패황권.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    target->LaunchEffect(0x10A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x151:
    effect = source->LaunchEffect(0x1EE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x153:
    source->LaunchEffect(0x1C2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x155:
    effect = source->LaunchEffect(0x5D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x158:
    target->LaunchEffect(0x10F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x159:
    target->LaunchEffect(0x110, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x15A:
    target->LaunchEffect(0x10E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x16D:
    target->LaunchEffect(0x17C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x16F:
    target->LaunchEffect(0x16D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x170:
    source->LaunchEffect(0x16E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x17B:
    effect = source->LaunchEffect(0x169, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x17D:
    target->LaunchEffect(0x183, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x17E:
    effect = source->LaunchEffect(0x184, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x18A:
    effect = source->LaunchEffect(0x189, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x18D:
    target->LaunchEffect(0x153, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x18E:
    source->LaunchEffect(0x18F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x18F:
    source->LaunchEffect(0x190, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x190:
    target->LaunchEffect(0x191, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x19F:
    target->LaunchEffect(0x19D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x1AF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1A1:
    target->LaunchEffect(0x1D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x1AD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1A3:
  case 0xA14:
    target->LaunchEffect(0x19F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effect = target->LaunchEffect(0x1AE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = source->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x1A5:
  case 0x7E7:
    source->LaunchEffect(0x1C9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effect = source->LaunchEffect(0x1B7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x1CA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1D3:
    effect = source->LaunchEffect(0x223, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x226, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1D4:
    effect = source->LaunchEffect(0x22B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x1AD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1D5:
    target->LaunchEffect(0x1A9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effect = source->LaunchEffect(0x229, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = vector3d{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x1AF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x22A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1E0:
    target->LaunchEffect(0x208, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1E5:
    target->LaunchEffect(0x206, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1EA:
    position = target->render.position;
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchDummyEffect(&position, 0x219, 100, 0);
    break;
  case 0x1F5:
    target->LaunchEffect(0x224, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1F6:
    target->LaunchEffect(0x288, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1F7:
    target->LaunchEffect(0x289, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1FB:
    target->LaunchEffect(0x284, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x200:
    target->LaunchEffect(0x287, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x202:
    effect = source->LaunchEffect(0x26F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x203:
    target->LaunchEffect(0x283, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x206:
    target->LaunchEffect(0x274, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x207:
    effect = source->LaunchEffect(0x248, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x208:
    effect = source->LaunchEffect(0x285, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x20B:
    if (g_modeMgr.GetGameMode() != nullptr) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchTargetEffect(0x265, source, target, -1);
    }
    break;
  case 0x20C:
    if (g_modeMgr.GetGameMode() != nullptr) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchTargetEffect(0x266, source, target, -1);
    }
    break;
  case 0x20D:
    if (g_modeMgr.GetGameMode() != nullptr) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchTargetEffect(0x267, source, target, -1);
    }
    break;
  case 0x20E:
    if (g_modeMgr.GetGameMode() != nullptr) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchTargetEffect(0x268, source, target, -1);
    }
    break;
  case 0x210:
    target->LaunchEffect(0x278, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x212:
    target->LaunchEffect(0x276, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x216:
    target->LaunchEffect(0x26A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x219:
    effect = target->LaunchEffect(0x26B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x21C:
    effect = source->LaunchEffect(0x26D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = vector3d{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      effect->SendMsg(nullptr, 44, 0, level, 0, 0, 0, 0);
    }
    break;
  case 0x21E:
    effect = source->LaunchEffect(0x270, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x220:
    target->LaunchEffect(0x279, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x28D:
  case 0x2EE:
    source->LaunchEffect(0x29A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x28E:
    g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x293:
    source->LaunchEffect(0x29C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x295:
  case 0x2C8:
    source->LaunchEffect(0x199, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x299:
  case 0x2F4:
    source->LaunchEffect(0x29D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x29A:
  case 0x2F7:
    g_soundMgr->PlayWave("_stonecurse.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x29B:
  case 0x2FA:
    g_soundMgr->PlayWave("amon_ra_die01.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x29E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x29C:
  case 0x2F8:
    g_soundMgr->PlayWave("effect\\widesleep.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x29D:
    source->LaunchEffect(0x16, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2A0:
    target->LaunchEffect(0x2A4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2A1:
    target->LaunchEffect(0x2A5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2A2:
    effect = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player->LaunchEffect(0xE3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x2A3:
    source->LaunchEffect(0xD6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2A4:
    source->LaunchEffect(0x2A3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2CA:
    target->LaunchEffect(0x405, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2CB: {
    CRagEffect* first = target->LaunchEffect(0x406, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    CRagEffect* second = target->LaunchEffect(0x406, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (first) {
      position = source->render.position;
      first->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    if (second) {
      position = source->render.position;
      second->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\LG_CANNONSPEAR.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  }
  case 0x2CC:
    source->LaunchEffect(0x408, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2D8:
  case 0x8B9: {
    if (source == target) {
      return;
    }
    g_soundMgr->PlayWave("arrowstorm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    const int effectId = g_serviceOptions.Get(0xE5) ? 0x5B1 : 0x50D;
    g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    position = vector3d{0.0f, 0.0f, 0.0f};
    for (int delay = 600; delay < 1200; delay += 200) {
      CEffect* arrow = g_effectMgr->Spawn(target->render.accountId, effectId + 1, position.x, position.y, position.z, 0, -1, 0, 0);
      if (arrow) {
        arrow->Slot16(delay);
      }
    }
    break;
  }
  case 0x2DB:
  case 0x7D8:
  case 0x138C:
    g_soundMgr->PlayWave("dragon_breath.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x2DC:
  case 0x8EC: {
    g_effectMgr->Spawn(source->render.accountId, 0x574, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, 0x575, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    const int duration = static_cast<int>(source->render.speed * 100.0f + 150.0f);
    CEffect* spawned = g_effectMgr->Spawn(target->render.accountId, 0x572, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (spawned) {
      spawned->Slot16(duration);
    }
    spawned = g_effectMgr->Spawn(target->render.accountId, 0x573, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (spawned) {
      spawned->Slot16(duration);
    }
    break;
  }
  case 0x2E6:
  case 0x990:
    g_soundMgr->PlayWave("effect\\S포이즌버스터.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    target->LaunchEffect(0x39B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x3EC:
    if (g_modeMgr.GetGameMode() != nullptr) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchTargetEffect(0x258, source, target, -1);
    }
    break;
  case 0x7D2:
    g_soundMgr->PlayWave("effect\\기공포.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effect = source->LaunchEffect(0x340, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    effect = source->LaunchEffect(0x68E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x7D4:
    effect = source->LaunchEffect(0x2D3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    effect = source->LaunchEffect(0x343, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x7DC:
    g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    target->LaunchEffect(0x24D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("_enemy_hit_wind2.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x7E6:
    g_effectMgr->Spawn(target->render.accountId, 0x61E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effect = source->LaunchEffect(0x620, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x7EE:
    effect = target->LaunchEffect(0x302, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x7EF:
    source->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x24D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\EF_PoisonAttack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x7F2:
    source->LaunchEffect(0x300, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\mon_맹룡과강.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x305, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x7F5: {
    float angle = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CameraDesiredYaw() + source->render.angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
    }
    int effectId = 0;
    if (angle >= 0.0f && angle < 23.0f) {
      effectId = 0x527;
    } else if (angle >= 23.0f && angle < 67.0f) {
      effectId = 0x526;
    } else if (angle >= 67.0f && angle < 112.0f) {
      effectId = 0x525;
    } else if (angle >= 112.0f && angle < 157.0f) {
      effectId = 0x524;
    } else if (angle >= 157.0f && angle < 202.0f) {
      effectId = 0x523;
    } else if (angle >= 202.0f && angle < 247.0f) {
      effectId = 0x52A;
    } else if (angle >= 247.0f && angle < 292.0f) {
      effectId = 0x529;
    } else if (angle >= 292.0f && angle < 337.0f) {
      effectId = 0x528;
    } else if (angle >= 337.0f && angle <= 360.0f) {
      effectId = 0x527;
    }
    if (g_serviceOptions.Get(0xE5)) {
      effectId += 0xA3;
    }
    g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, g_serviceOptions.Get(0xE5) ? 0x5C5 : 0x522, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effect = source->LaunchEffect(0x301, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = vector3d{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x226, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  case 0x7F6:
    g_soundMgr->PlayWave("effect\\AB_judex.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    g_effectMgr->Spawn(source->render.accountId, 0x65B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, 0x65C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, 0x65F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, 0x660, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    for (int count = 0; count < 12; ++count) {
      position = target->render.position;
      const float radius = static_cast<float>(std::max(std::rand() % 15, 2));
      matrix rotation;
      rotation.MakeYRotation(static_cast<float>(std::max(std::rand() % 36, 0)) * 10.0f);
      vector3d spot;
      spot.x = rotation.values[0] * radius + rotation.values[3] * 0.0f + rotation.values[6] * 0.0f + rotation.values[9] + position.x;
      spot.y = rotation.values[1] * radius + rotation.values[4] * 0.0f + rotation.values[7] * 0.0f + rotation.values[10] + position.y;
      spot.z = rotation.values[2] * radius + rotation.values[5] * 0.0f + rotation.values[8] * 0.0f + rotation.values[11] + position.z;
      const int delay = std::max(std::rand() % 10, 0) * 50;
      CEffect* spawned = g_effectMgr->Spawn(0, 0x661, spot.x, spot.y, spot.z, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(delay);
      }
      spawned = g_effectMgr->Spawn(0, 0x662, spot.x, spot.y, spot.z, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(delay);
      }
    }
    break;
  case 0x7F8:
    g_soundMgr->PlayWave("effect\\AB_adoramus.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(
        target->render.accountId, 0x594, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
      g_effectMgr->Spawn(
        target->render.accountId, 0x595, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
    } else {
      g_effectMgr->Spawn(
        target->render.accountId, 0x4EA, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
      g_effectMgr->Spawn(
        target->render.accountId, 0x4EB, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
    }
    break;
  case 0x807:
  case 0x808:
    effect = source->LaunchEffect(0x109, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      effect->SendMsg(nullptr, 44, 0, 3, 0, 0, 0, 0);
    }
    break;
  case 0x8A3:
    g_soundMgr->PlayWave(
      "crimson_r.wav", target->render.position.x - source->render.position.x, 0.0f, target->render.position.z - source->render.position.z, 250, 40,
      1.0f, 0
    );
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(
        target->render.accountId, 0x5A1, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
      g_effectMgr->Spawn(
        target->render.accountId, 0x5A2, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
    } else {
      g_effectMgr->Spawn(
        target->render.accountId, 0x4FA, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
      g_effectMgr->Spawn(
        target->render.accountId, 0x4FB, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, level, 0, 0
      );
    }
    break;
  case 0x8A4:
    g_effectMgr->Spawn(target->render.accountId, 0x60E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, 0x60F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    for (int index = 0; index < 4; ++index) {
      matrix rotation;
      rotation.MakeYRotation((static_cast<float>(index) * 90.0f + 45.0f) / 180.0f * 3.141592f);
      vector3d spot;
      spot.x = rotation.values[0] * 15.0f + rotation.values[3] * 0.0f + rotation.values[6] * 0.0f + rotation.values[9] + target->render.position.x;
      spot.y = rotation.values[1] * 15.0f + rotation.values[4] * 0.0f + rotation.values[7] * 0.0f + rotation.values[10] + target->render.position.y;
      spot.z = rotation.values[2] * 15.0f + rotation.values[5] * 0.0f + rotation.values[8] * 0.0f + rotation.values[11] + target->render.position.z;
      g_effectMgr->Spawn(0, 0x611, spot.x, spot.y, spot.z, 0, -1, 0, 0);
      g_effectMgr->Spawn(0, 0x612, spot.x, spot.y, spot.z, 0, -1, 0, 0);
    }
    break;
  case 0x8A6:
  case 0x8A7:
    g_effectMgr->Spawn(target->render.accountId, 0x666, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 0x8AA:
    effect = source->LaunchEffect(0x324, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x8B1:
    target->LaunchEffect(0x31, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8B2:
    target->LaunchEffect(0x34, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8B3:
    target->LaunchEffect(0x33, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8B4:
    target->LaunchEffect(0x93, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8BC: {
    g_effectMgr->Spawn(target->render.accountId, 0x672, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    float angle = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CameraDesiredYaw() + source->render.angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
    }
    int effectId;
    if (angle > 0.0f && angle < 90.0f) {
      effectId = 0x66E;
    } else if (angle >= 90.0f && angle < 180.0f) {
      effectId = 0x66D;
    } else if (angle >= 180.0f && angle < 270.0f) {
      effectId = 0x670;
    } else {
      effectId = 0x66F;
    }
    g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  }
  case 0x8BD:
    source->LaunchEffect(0x2EE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8C3:
    g_soundMgr->PlayWave("wug_strike.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8C4:
    g_soundMgr->PlayWave("wug_bite.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8C6:
    g_soundMgr->PlayWave("were_wolf_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8D0:
    g_soundMgr->PlayWave("effect\\맹호경파산.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effect = source->LaunchEffect(0x326, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x8D1:
    g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    g_soundMgr->PlayWave("effect\\맹호경파산.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x8D2: {
    g_effectMgr->Spawn(source->render.accountId, 0x6A7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, 0x6A8, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    CEffect* spawned = g_effectMgr->Spawn(target->render.accountId, 0x6A9, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (spawned) {
      spawned->Slot17(static_cast<float>(std::max(std::rand() % 36, 0)) * 10.0f);
    }
    source->LaunchEffect(0x308, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
  case 0x8DE:
    source->LaunchEffect(0x30C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    source->LaunchEffect(0x31A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8DF:
    target->LaunchEffect(0x312, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8E0:
    source->LaunchEffect(0x30D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x8E6:
    g_effectMgr->Spawn(
      source->render.accountId, 0x679, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
    );
    g_effectMgr->Spawn(target->render.accountId, 0x67C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  case 0x8E7: {
    float angle = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CameraDesiredYaw() + source->render.angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
    }
    int effectId;
    if (angle > 0.0f && angle < 90.0f) {
      effectId = 0x6A5;
    } else if (angle >= 90.0f && angle < 180.0f) {
      effectId = 0x6A4;
    } else if (angle >= 180.0f && angle < 270.0f) {
      effectId = 0x6A3;
    } else {
      effectId = 0x6A2;
    }
    g_effectMgr->Spawn(target->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, 0x6A6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    break;
  }
  case 0x8E8:
    if (g_serviceOptions.Get(0xE5)) {
      if (g_effectMgr->Find(source->render.accountId, 0x5D6)) {
        return;
      }
      g_effectMgr->Spawn(source->render.accountId, 0x5D6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x5D7, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    } else {
      if (g_effectMgr->Find(source->render.accountId, 0x553)) {
        return;
      }
      g_effectMgr->Spawn(source->render.accountId, 0x553, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x554, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;
  case 0x8F0: {
    const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
    CEffect* spawned;
    if (g_serviceOptions.Get(0xE5)) {
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x5B3, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(duration);
      }
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x5B4, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    } else {
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x510, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(duration);
      }
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x511, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    if (spawned) {
      spawned->Slot16(duration);
    }
    spawned = g_effectMgr->Spawn(
      0, 0x52C, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
    );
    if (spawned) {
      spawned->Slot16(duration);
    }
    break;
  }
  case 0x903: {
    float angle = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CameraDesiredYaw() + source->render.angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
    }
    int effectId = 0;
    if (angle >= 0.0f && angle < 23.0f) {
      effectId = 0x57C;
    } else if (angle >= 23.0f && angle < 67.0f) {
      effectId = 0x57B;
    } else if (angle >= 67.0f && angle < 112.0f) {
      effectId = 0x57A;
    } else if (angle >= 112.0f && angle < 157.0f) {
      effectId = 0x579;
    } else if (angle >= 157.0f && angle < 202.0f) {
      effectId = 0x578;
    } else if (angle >= 202.0f && angle < 247.0f) {
      effectId = 0x57F;
    } else if (angle >= 247.0f && angle < 292.0f) {
      effectId = 0x57E;
    } else if (angle >= 292.0f && angle < 337.0f) {
      effectId = 0x57D;
    } else if (angle >= 337.0f && angle <= 360.0f) {
      effectId = 0x57C;
    }
    if (g_serviceOptions.Get(0xE5)) {
      if (!g_effectMgr->Find(source->render.accountId, 0x5EF)) {
        g_effectMgr->Spawn(source->render.accountId, 0x5EF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
        CEffect* spawned = g_effectMgr->Spawn(source->render.accountId, effectId + 0x6F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (spawned) {
          spawned->Slot16(duration);
        }
      }
    } else {
      if (!g_effectMgr->Find(source->render.accountId, 0x580)) {
        g_effectMgr->Spawn(source->render.accountId, 0x580, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
        CEffect* spawned = g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
        if (spawned) {
          spawned->Slot16(duration);
        }
      }
    }
    effect = source->LaunchEffect(0x581, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\LG_CANNONSPEAR.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  }
  case 0x904: {
    float angle = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->CameraDesiredYaw() + source->render.angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
    }
    int effectId;
    if (angle > 0.0f && angle < 90.0f) {
      effectId = 0x55E;
    } else if (angle >= 90.0f && angle < 180.0f) {
      effectId = 0x55D;
    } else if (angle >= 180.0f && angle < 270.0f) {
      effectId = 0x560;
    } else {
      effectId = 0x55F;
    }
    const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
    CEffect* spawned;
    if (g_serviceOptions.Get(0xE5)) {
      spawned = g_effectMgr->Spawn(source->render.accountId, effectId + 0x86, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(duration);
      }
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x5DF, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(duration);
      }
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x5E0, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    } else {
      spawned = g_effectMgr->Spawn(source->render.accountId, effectId, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(duration);
      }
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x559, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      if (spawned) {
        spawned->Slot16(duration);
      }
      spawned = g_effectMgr->Spawn(target->render.accountId, 0x55A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    if (spawned) {
      spawned->Slot16(duration);
    }
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(source->render.accountId, 0x5E1, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x5E2, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    } else {
      g_effectMgr->Spawn(source->render.accountId, 0x55B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x55C, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\LG_BANISHINGPOINT.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  }
  case 0x905:
    g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    source->LaunchEffect(0x389, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\LG_TRAMPLE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x906:
    source->LaunchEffect(0x38A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x908:
    effect = source->LaunchEffect(0x38B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x90A:
    source->LaunchEffect(0x3A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    source->LaunchEffect(0x179, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\LG_RAGEBURST.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x910:
    if (!source->state.Attachments().HasId(0x37A)) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x37A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\LG_MOONSLASHER.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    source->LaunchEffect(0x3A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    source->LaunchEffect(0x375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x912:
    target->LaunchEffect(0x5B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x913:
    g_soundMgr->PlayWave("effect\\LG_EARTHDRIVE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x914:
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x383, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x37E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x38A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\LG_HESPERUSLIT.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x916:
    source->LaunchEffect(0x375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\SR_DRAGONCOMBO.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x917:
    if (!source->state.Attachments().HasId(0x37A)) {
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      source->LaunchEffect(0x37A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\SR_SKYNETBLOW.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    target->LaunchEffect(0x224, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x226, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x91A:
    g_effectMgr->Spawn(source->render.accountId, g_serviceOptions.Get(0xE5) ? 0x5FC : 0x52B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    target->LaunchEffect(0x379, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\SR_TIGERCANNON.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x91C:
    if (g_serviceOptions.Get(0xE5)) {
      g_effectMgr->Spawn(source->render.accountId, 0x5B5, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x5B6, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->LaunchEffect(0x609, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    } else {
      g_effectMgr->Spawn(source->render.accountId, 0x51E, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(source->render.accountId, 0x51F, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    g_soundMgr->PlayWave("effect\\SR_RAMPAGEBLASTER.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x920:
    source->LaunchEffect(0x1C9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effect = source->LaunchEffect(0x37D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x1CA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\SR_KNUCKLEARROW.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x924:
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\흡기.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x927:
    g_effectMgr->Spawn(target->render.accountId, 0x626, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effect = source->LaunchEffect(0x627, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    g_soundMgr->PlayWave("effect\\SR_GATEOFHELL.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x928:
    source->LaunchEffect(0x375, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\mon_탄지신통.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x96D:
    effect = source->LaunchEffect(0x630, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      g_soundMgr->PlayWave("effect\\metalic.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  case 0x96E:
    target->LaunchEffect(0x621, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x63A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\REVERBERATION.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x97A:
    effect = source->LaunchEffect(0x35F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    }
    target->LaunchEffect(0x1D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\LG_EARTHDRIVE.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x97D:
    effect = target->LaunchEffect(0x365, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x1A9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\metalic.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0xA21:
    target->LaunchEffect(0x4C8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x1AD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0xA2C:
    target->LaunchEffect(0x1A9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effect = source->LaunchEffect(0x4B6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = vector3d{0.0f, static_cast<float>(target->render.accountId), 0.0f};
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    target->LaunchEffect(0x1AF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x1A1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x13A2:
    g_effectMgr->Spawn(0, 0x44D, source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0);
    g_effectMgr->Spawn(
      source->render.accountId, 0x44C, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
    );
    break;
  case 0x13B6:
    g_effectMgr->Spawn(
      source->render.accountId, 0x468, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, -1, 0, 0
    );
    g_effectMgr->Spawn(
      target->render.accountId, 0x469, target->render.position.x, target->render.position.y, target->render.position.z, target->render.accountId, -1, 0, 0
    );
    break;
  case 0x14B2: {
    CEffect* machine = g_effectMgr->Find(source->render.accountId, 0x72F);
    if (machine) {
      static_cast<CAMachineEffect*>(machine)->PlayAttack();
    }
    CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    g_soundMgr->PlayWave(
      "effect\\MT_A_MACHINE.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z,
      250, 40, 1.0f, 0
    );
    break;
  }
  case 0x1F49:
    if (level == 1) {
      target->LaunchEffect(0x235, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    } else if (level == 2) {
      target->LaunchEffect(0x236, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    } else {
      target->LaunchEffect(0x237, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x1F4C:
    target->LaunchEffect(0x237, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x1A9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1F58:
    target->LaunchEffect(0x3C0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1F59:
    effect = source->LaunchEffect(0x43A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x1F5C:
    target->LaunchEffect(0x3C5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1F5E:
    target->LaunchEffect(0x3C7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x1F64:
  case 0x1F65:
  case 0x1F66:
    target->LaunchEffect(0x3C6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x2031:
    target->LaunchEffect(0x407, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20D1:
    target->LaunchEffect(0xFC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x150, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20D4:
    source->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x1B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20D7:
    target->LaunchEffect(0x185, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20DA:
    source->LaunchEffect(0xD6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0xD6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20DD:
    g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3A8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3BC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20DE:
    g_soundMgr->PlayWave("effect\\ef_firehit.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3A8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3BE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20DF:
    g_soundMgr->PlayWave("effect\\EF_FireArrow1.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3A8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    target->LaunchEffect(0x3BB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E0:
    g_soundMgr->PlayWave("effect\\EF_FrostDiver2.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3B5, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E1:
    g_soundMgr->PlayWave("effect\\EF_FrostDiver.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3BA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E2:
    g_soundMgr->PlayWave("effect\\EF_IceArrow1.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x376, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    source->LaunchEffect(0x37E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3B8, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E3:
    g_soundMgr->PlayWave("effect\\EF_ThunderStorm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3A9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3B3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E4:
    g_soundMgr->PlayWave("effect\\EF_LightBolt1.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3A9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3B4, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E5:
    g_soundMgr->PlayWave("effect\\EF_LightBolt3.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3A9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    target->LaunchEffect(0x3B7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E6:
    g_soundMgr->PlayWave("_stonecurse.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3AA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3BD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E7:
    g_soundMgr->PlayWave("evil_cloud_hermit_attack.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3AA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x3B9, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E8:
    g_soundMgr->PlayWave("effect\\wizard_earthspike.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    source->LaunchEffect(0x3AA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    target->LaunchEffect(0x3B6, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20E9:
    effect = target->LaunchEffect(0x1F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
    }
    break;
  case 0x20EA:
    g_soundMgr->PlayWave("effect\\화염진.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effect = source->LaunchEffect(0x18, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      target->LaunchEffect(0x11, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x20EC:
    target->LaunchEffect(0x385, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    target->LaunchEffect(0x11, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_soundMgr->PlayWave("effect\\폭염룡.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 0x20EE:
    effect = target->LaunchEffect(0x1A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      effect->SendMsg(nullptr, 44, 0, 1, 0, 0, 0, 0);
    }
    break;
  case 0x20EF:
    target->LaunchEffect(0x3AD, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  case 0x20F1:
    target->LaunchEffect(0x3AE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    effect = source->LaunchEffect(0x1B7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x20F2:
    g_soundMgr->PlayWave("effect\\기공포.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effect = source->LaunchEffect(0x3AF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x20F3:
    target->LaunchEffect(0x1D2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (!target->state.Attachments().HasId(0x1CC)) {
      target->LaunchEffect(0x1CC, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;
  case 0x20F5:
    g_soundMgr->PlayWave("effect\\맹호경파산.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effect = source->LaunchEffect(0x3A7, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    if (effect) {
      position = target->render.position;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
    break;
  case 0x20FA:
    target->LaunchEffect(0x3B0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    break;
  }
}

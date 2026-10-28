#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CMode.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Session.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"

#include <cstring>
#include <string>

// GLOBAL: gl_24496
int g_screenShakeDuration;

// GLOBAL: gl_24497
char g_screenShakeMode;

// GLOBAL: gl_24702
const char g_playerNameKeyData[] = "가나다라마바사아자차카타파하가나다라마바사아자차카타파하";

// GLOBAL: gl_24649
const char* g_playerNameKey = g_playerNameKeyData;

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
      g_world.ActiveGameMode()->SendMsg(313, index + 36, 0, 0, 0);
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

  return g_world.ActiveGameMode()->SendMsg(313, slot + 36, 0, 0, 0);
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

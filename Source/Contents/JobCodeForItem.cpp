#include "Contents/JobCodeForItem.h"
#include "Contents/Jobs/JobId.h"

#include <new>

// OFFSET: 0x006C7720
// OFFSET: 0x006C7920
// OFFSET: 0x006C7B30
// OFFSET: 0x006C7D40
// OFFSET: 0x006C7F60
// OFFSET: 0x006C8180
// OFFSET: 0x006C83A0
// OFFSET: 0x006C85D0
// OFFSET: 0x006C8810
// FUNCTION: fn_06421
// FUNCTION: fn_06422
// FUNCTION: fn_06423
// FUNCTION: fn_06424
// FUNCTION: fn_06425
// FUNCTION: fn_06426
// FUNCTION: fn_06427
// FUNCTION: fn_06428
// FUNCTION: fn_06429
template <typename... Groups>
void CJobCodeForItem::AddJobCodeFromGroups(unsigned long code, Groups... groups) {
  std::set<unsigned int> groupSet = {static_cast<unsigned int>(groups)...};
  if (!groupSet.empty()) {
    std::set<int> jobs;
    for (const unsigned int& group : groupSet) {
      auto groupIt = this->m_jobGroups.find(group);
      if (groupIt != this->m_jobGroups.end()) {
        jobs.merge(std::set<int>(groupIt->second));
      }
    }

    auto it = this->m_jobCodes.find(code);
    if (it == this->m_jobCodes.end()) {
      this->m_jobCodes[code] = jobs;
    } else {
      it->second.merge(jobs);
    }
  }
}

template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int, int);
template void CJobCodeForItem::AddJobCodeFromGroups(unsigned long, int);

// OFFSET: 0x006C8A50
// OFFSET: 0x006C8B10
// OFFSET: 0x006C8BD0
// OFFSET: 0x006C8CB0
// OFFSET: 0x006C8D90
// OFFSET: 0x006C8F60
// OFFSET: 0x006C9050
// FUNCTION: fn_06430
// FUNCTION: fn_06431
// FUNCTION: fn_06432
// FUNCTION: fn_06433
// FUNCTION: fn_06434
// FUNCTION: fn_06435
// FUNCTION: fn_06436
template <typename... Jobs>
void CJobCodeForItem::AddJobCode(unsigned long code, Jobs... jobs) {
  std::set<int> jobSet = {jobs...};
  if (!jobSet.empty()) {
    auto it = this->m_jobCodes.find(code);
    if (it == this->m_jobCodes.end()) {
      this->m_jobCodes[code] = jobSet;
    } else {
      it->second.merge(jobSet);
    }
  }
}

template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId, JobId);
template void CJobCodeForItem::AddJobCode(unsigned long, JobId);

// OFFSET: 0x006C9270
// OFFSET: 0x006C9350
// OFFSET: 0x006C9430
// OFFSET: 0x006C9510
// OFFSET: 0x006C9600
// OFFSET: 0x006C96F0
// OFFSET: 0x006C98F0
// OFFSET: 0x006C9A00
// FUNCTION: fn_06437
// FUNCTION: fn_06438
// FUNCTION: fn_06439
// FUNCTION: fn_06440
// FUNCTION: fn_06441
// FUNCTION: fn_06442
// FUNCTION: fn_06443
// FUNCTION: fn_06444
template <typename... Jobs>
void CJobCodeForItem::AddJobGroup(unsigned int group, Jobs... jobs) {
  std::set<int> jobSet = {jobs...};
  if (!jobSet.empty()) {
    auto it = this->m_jobGroups.find(group);
    if (it == this->m_jobGroups.end()) {
      this->m_jobGroups[group] = jobSet;
    } else {
      it->second.merge(jobSet);
    }
  }
}

template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId,
                                            JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId, JobId);
template void CJobCodeForItem::AddJobGroup(unsigned int, JobId, JobId, JobId);

// GLOBAL: gl_24704
template <>
CJobCodeForItem* Singleton<CJobCodeForItem>::s_instance = nullptr;

// OFFSET: 0x00E5CED0
// FUNCTION: fn_24374
template <>
CJobCodeForItem* Singleton<CJobCodeForItem>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CJobCodeForItem;
  }

  return s_instance;
}

// OFFSET: 0x006CB0A0
// FUNCTION: fn_06464
void CJobCodeForItem::InitJobGroups() {
  this->AddJobGroup(19, JT_SWORDMAN_B, JT_MAGICIAN_B, JT_ARCHER_B, JT_ACOLYTE_B, JT_MERCHANT_B, JT_THIEF_B);
  this->AddJobGroup(20, JT_KNIGHT_B, JT_PRIEST_B, JT_WIZARD_B, JT_BLACKSMITH_B, JT_HUNTER_B, JT_ASSASSIN_B, JT_CRUSADER_B, JT_MONK_B, JT_SAGE_B, JT_ROGUE_B,
                    JT_ALCHEMIST_B, JT_BARD_B, JT_DANCER_B);
  this->AddJobGroup(21, JT_RUNE_KNIGHT_B, JT_ROYAL_GUARD_B, JT_MECHANIC_B, JT_GENETIC_B, JT_WARLOCK_B, JT_SORCERER_B, JT_RANGER_B, JT_MINSTREL_B, JT_WANDERER_B,
                    JT_ARCHBISHOP_B, JT_SURA_B, JT_GUILLOTINE_CROSS_B, JT_SHADOW_CHASER_B);
  this->AddJobGroup(15, JT_SWORDMAN_H, JT_MERCHANT_H, JT_MAGICIAN_H, JT_ARCHER_H, JT_ACOLYTE_H, JT_THIEF_H);
  this->AddJobGroup(13, JT_KNIGHT, JT_CRUSADER, JT_BLACKSMITH, JT_ALCHEMIST, JT_WIZARD, JT_SAGE, JT_HUNTER, JT_BARD, JT_DANCER, JT_PRIEST, JT_MONK, JT_ASSASSIN,
                    JT_ROGUE);
  this->AddJobGroup(16, JT_KNIGHT_H, JT_CRUSADER_H, JT_BLACKSMITH_H, JT_ALCHEMIST_H, JT_WIZARD_H, JT_SAGE_H, JT_HUNTER_H, JT_BARD_H, JT_DANCER_H, JT_PRIEST_H,
                    JT_MONK_H, JT_ASSASSIN_H, JT_ROGUE_H);
  this->AddJobGroup(14, JT_RUNE_KNIGHT, JT_ROYAL_GUARD, JT_MECHANIC, JT_GENETIC, JT_WARLOCK, JT_SORCERER, JT_RANGER, JT_MINSTREL, JT_WANDERER, JT_ARCHBISHOP,
                    JT_SURA, JT_GUILLOTINE_CROSS, JT_SHADOW_CHASER);
  this->AddJobGroup(17, JT_RUNE_KNIGHT_H, JT_ROYAL_GUARD_H, JT_MECHANIC_H, JT_GENETIC_H, JT_WARLOCK_H, JT_SORCERER_H, JT_RANGER_H, JT_MINSTREL_H, JT_WANDERER_H,
                    JT_ARCHBISHOP_H, JT_SURA_H, JT_GUILLOTINE_CROSS_H, JT_SHADOW_CHASER_H);
  this->AddJobGroup(18, JT_DRAGON_KNIGHT, JT_MEISTER, JT_SHADOW_CROSS, JT_ARCH_MAGE, JT_CARDINAL, JT_WINDHAWK, JT_IMPERIAL_GUARD, JT_BIOLO, JT_ABYSS_CHASER,
                    JT_ELEMENTAL_MASTER, JT_INQUISITOR, JT_TROUBADOUR, JT_TROUVERE);
  this->AddJobGroup(0, JT_NOVICE, JT_SUPERNOVICE, JT_NOVICE_H, JT_NOVICE_B, JT_SUPERNOVICE_B, JT_SUPERNOVICE2, JT_SUPERNOVICE2_B, JT_HYPER_NOVICE);
  this->AddJobGroup(1, JT_SWORDMAN, JT_KNIGHT, JT_CRUSADER, JT_SWORDMAN_H, JT_KNIGHT_H, JT_CRUSADER_H, JT_SWORDMAN_B, JT_KNIGHT_B, JT_CRUSADER_B,
                    JT_RUNE_KNIGHT, JT_RUNE_KNIGHT_H, JT_ROYAL_GUARD, JT_ROYAL_GUARD_H, JT_RUNE_KNIGHT_B, JT_ROYAL_GUARD_B, JT_DRAGON_KNIGHT,
                    JT_IMPERIAL_GUARD);
  this->AddJobGroup(2, JT_MERCHANT, JT_BLACKSMITH, JT_ALCHEMIST, JT_MERCHANT_H, JT_BLACKSMITH_H, JT_ALCHEMIST_H, JT_MERCHANT_B, JT_BLACKSMITH_B, JT_ALCHEMIST_B,
                    JT_MECHANIC, JT_MECHANIC_H, JT_MECHANIC_B, JT_GENETIC, JT_GENETIC_H, JT_GENETIC_B, JT_MEISTER, JT_BIOLO);
  this->AddJobGroup(3, JT_MAGICIAN, JT_WIZARD, JT_SAGE, JT_MAGICIAN_H, JT_WIZARD_H, JT_SAGE_H, JT_MAGICIAN_B, JT_WIZARD_B, JT_SAGE_B, JT_WARLOCK, JT_WARLOCK_H,
                    JT_WARLOCK_B, JT_SORCERER, JT_SORCERER_H, JT_SORCERER_B, JT_ARCH_MAGE, JT_ELEMENTAL_MASTER);
  this->AddJobGroup(4, JT_ARCHER, JT_HUNTER, JT_BARD, JT_DANCER, JT_ARCHER_H, JT_HUNTER_H, JT_BARD_H, JT_DANCER_H, JT_ARCHER_B, JT_HUNTER_B, JT_BARD_B,
                    JT_DANCER_B, JT_RANGER, JT_RANGER_H, JT_RANGER_B, JT_MINSTREL, JT_WANDERER, JT_MINSTREL_H, JT_MINSTREL_B, JT_WANDERER_H, JT_WANDERER_B,
                    JT_WINDHAWK, JT_TROUBADOUR, JT_TROUVERE);
  this->AddJobGroup(5, JT_ACOLYTE, JT_PRIEST, JT_MONK, JT_ACOLYTE_H, JT_PRIEST_H, JT_MONK_H, JT_ACOLYTE_B, JT_PRIEST_B, JT_MONK_B, JT_ARCHBISHOP,
                    JT_ARCHBISHOP_H, JT_SURA, JT_SURA_H, JT_ARCHBISHOP_B, JT_SURA_B, JT_CARDINAL, JT_INQUISITOR);
  this->AddJobGroup(6, JT_THIEF, JT_ASSASSIN, JT_ROGUE, JT_THIEF_H, JT_ASSASSIN_H, JT_ROGUE_H, JT_THIEF_B, JT_ASSASSIN_B, JT_ROGUE_B, JT_GUILLOTINE_CROSS,
                    JT_GUILLOTINE_CROSS_H, JT_SHADOW_CHASER, JT_SHADOW_CHASER_H, JT_GUILLOTINE_CROSS_B, JT_SHADOW_CHASER_B, JT_SHADOW_CROSS, JT_ABYSS_CHASER);
  this->AddJobGroup(7, JT_TAEKWON, JT_STAR, JT_TAEKWON_B, JT_STAR_B, JT_STAR_EMPEROR, JT_STAR_EMPEROR_B, JT_SKY_EMPEROR);
  this->AddJobGroup(8, JT_TAEKWON, JT_STAR, JT_LINKER, JT_TAEKWON_B, JT_STAR_B, JT_LINKER_B, JT_STAR_EMPEROR, JT_STAR_EMPEROR_B, JT_SOUL_REAPER,
                    JT_SOUL_REAPER_B, JT_SKY_EMPEROR, JT_SOUL_ASCETIC);
  this->AddJobGroup(9, JT_STAR, JT_STAR_B, JT_STAR_EMPEROR, JT_STAR_EMPEROR_B, JT_SKY_EMPEROR);
  this->AddJobGroup(10, JT_STAR_EMPEROR, JT_STAR_EMPEROR_B, JT_SKY_EMPEROR);
  this->AddJobGroup(11, JT_LINKER, JT_LINKER_B, JT_SOUL_REAPER, JT_SOUL_REAPER_B, JT_SOUL_ASCETIC);
  this->AddJobGroup(12, JT_SOUL_REAPER, JT_SOUL_REAPER_B, JT_SOUL_ASCETIC);
  this->AddJobGroup(61, JT_NINJA, JT_KAGEROU, JT_OBORO, JT_NINJA_B, JT_KAGEROU_B, JT_OBORO_B, JT_SHINKIRO, JT_SHIRANUI);
  this->AddJobGroup(62, JT_KAGEROU, JT_OBORO, JT_KAGEROU_B, JT_OBORO_B, JT_SHINKIRO, JT_SHIRANUI);
  this->AddJobGroup(63, JT_GUNSLINGER, JT_REBELLION, JT_GUNSLINGER_B, JT_REBELLION_B, JT_NIGHT_WATCH);
  this->AddJobGroup(64, JT_REBELLION, JT_REBELLION_B, JT_NIGHT_WATCH);
  this->AddJobGroup(65, JT_DO_SUMMONER, JT_DO_SUMMONER_B, JT_SPIRIT_HANDLER);
  this->AddJobGroup(66, JT_DRUID, JT_KARNOS, JT_ALITEA, JT_DRUID_B, JT_KARNOS_B);
  this->AddJobGroup(22, JT_KNIGHT, JT_KNIGHT_H, JT_KNIGHT_B, JT_RUNE_KNIGHT, JT_RUNE_KNIGHT_H, JT_RUNE_KNIGHT_B, JT_DRAGON_KNIGHT);
  this->AddJobGroup(23, JT_CRUSADER, JT_CRUSADER_H, JT_CRUSADER_B, JT_ROYAL_GUARD, JT_ROYAL_GUARD_H, JT_ROYAL_GUARD_B, JT_IMPERIAL_GUARD);
  this->AddJobGroup(24, JT_BLACKSMITH, JT_BLACKSMITH_H, JT_BLACKSMITH_B, JT_MECHANIC, JT_MECHANIC_H, JT_MECHANIC_B, JT_MEISTER);
  this->AddJobGroup(25, JT_ALCHEMIST, JT_ALCHEMIST_H, JT_ALCHEMIST_B, JT_GENETIC, JT_GENETIC_H, JT_GENETIC_B, JT_BIOLO);
  this->AddJobGroup(26, JT_ASSASSIN, JT_ASSASSIN_H, JT_ASSASSIN_B, JT_GUILLOTINE_CROSS, JT_GUILLOTINE_CROSS_H, JT_GUILLOTINE_CROSS_B, JT_SHADOW_CROSS);
  this->AddJobGroup(27, JT_ROGUE, JT_ROGUE_H, JT_ROGUE_B, JT_SHADOW_CHASER, JT_SHADOW_CHASER_H, JT_SHADOW_CHASER_B, JT_ABYSS_CHASER);
  this->AddJobGroup(28, JT_WIZARD, JT_WIZARD_H, JT_WIZARD_B, JT_WARLOCK, JT_WARLOCK_H, JT_WARLOCK_B, JT_ARCH_MAGE);
  this->AddJobGroup(29, JT_SAGE, JT_SAGE_H, JT_SAGE_B, JT_SORCERER, JT_SORCERER_H, JT_SORCERER_B, JT_ELEMENTAL_MASTER);
  this->AddJobGroup(30, JT_PRIEST, JT_PRIEST_H, JT_PRIEST_B, JT_ARCHBISHOP, JT_ARCHBISHOP_H, JT_ARCHBISHOP_B, JT_CARDINAL);
  this->AddJobGroup(31, JT_MONK, JT_MONK_H, JT_MONK_B, JT_SURA, JT_SURA_H, JT_SURA_B, JT_INQUISITOR);
  this->AddJobGroup(32, JT_HUNTER, JT_HUNTER_H, JT_HUNTER_B, JT_RANGER, JT_RANGER_H, JT_RANGER_B, JT_WINDHAWK);
  this->AddJobGroup(33, JT_BARD, JT_BARD_H, JT_BARD_B, JT_MINSTREL, JT_MINSTREL_H, JT_MINSTREL_B, JT_TROUBADOUR);
  this->AddJobGroup(34, JT_DANCER, JT_DANCER_H, JT_DANCER_B, JT_WANDERER, JT_WANDERER_H, JT_WANDERER_B, JT_TROUVERE);
  this->AddJobGroup(35, JT_KNIGHT_H, JT_RUNE_KNIGHT, JT_RUNE_KNIGHT_H, JT_RUNE_KNIGHT_B, JT_DRAGON_KNIGHT);
  this->AddJobGroup(36, JT_CRUSADER_H, JT_ROYAL_GUARD, JT_ROYAL_GUARD_H, JT_ROYAL_GUARD_B, JT_IMPERIAL_GUARD);
  this->AddJobGroup(37, JT_BLACKSMITH_H, JT_MECHANIC, JT_MECHANIC_H, JT_MECHANIC_B, JT_MEISTER);
  this->AddJobGroup(38, JT_ALCHEMIST_H, JT_GENETIC, JT_GENETIC_H, JT_GENETIC_B, JT_BIOLO);
  this->AddJobGroup(39, JT_ASSASSIN_H, JT_GUILLOTINE_CROSS, JT_GUILLOTINE_CROSS_H, JT_GUILLOTINE_CROSS_B, JT_SHADOW_CROSS);
  this->AddJobGroup(40, JT_ROGUE_H, JT_SHADOW_CHASER, JT_SHADOW_CHASER_H, JT_SHADOW_CHASER_B, JT_ABYSS_CHASER);
  this->AddJobGroup(41, JT_WIZARD_H, JT_WARLOCK, JT_WARLOCK_H, JT_WARLOCK_B, JT_ARCH_MAGE);
  this->AddJobGroup(42, JT_SAGE_H, JT_SORCERER, JT_SORCERER_H, JT_SORCERER_B, JT_ELEMENTAL_MASTER);
  this->AddJobGroup(43, JT_PRIEST_H, JT_ARCHBISHOP, JT_ARCHBISHOP_H, JT_ARCHBISHOP_B, JT_CARDINAL);
  this->AddJobGroup(44, JT_MONK_H, JT_SURA, JT_SURA_H, JT_SURA_B, JT_INQUISITOR);
  this->AddJobGroup(45, JT_HUNTER_H, JT_RANGER, JT_RANGER_H, JT_RANGER_B, JT_WINDHAWK);
  this->AddJobGroup(46, JT_BARD_H, JT_MINSTREL, JT_MINSTREL_H, JT_MINSTREL_B, JT_TROUBADOUR, JT_TROUVERE);
  this->AddJobGroup(47, JT_DANCER_H, JT_WANDERER, JT_WANDERER_H, JT_WANDERER_B, JT_TROUBADOUR, JT_TROUVERE);
  this->AddJobGroup(48, JT_RUNE_KNIGHT, JT_RUNE_KNIGHT_H, JT_RUNE_KNIGHT_B, JT_DRAGON_KNIGHT);
  this->AddJobGroup(49, JT_ROYAL_GUARD, JT_ROYAL_GUARD_H, JT_ROYAL_GUARD_B, JT_IMPERIAL_GUARD);
  this->AddJobGroup(50, JT_MECHANIC, JT_MECHANIC_H, JT_MECHANIC_B, JT_MEISTER);
  this->AddJobGroup(51, JT_GENETIC, JT_GENETIC_H, JT_GENETIC_B, JT_BIOLO);
  this->AddJobGroup(52, JT_GUILLOTINE_CROSS, JT_GUILLOTINE_CROSS_H, JT_GUILLOTINE_CROSS_B, JT_SHADOW_CROSS);
  this->AddJobGroup(53, JT_SHADOW_CHASER, JT_SHADOW_CHASER_H, JT_SHADOW_CHASER_B, JT_ABYSS_CHASER);
  this->AddJobGroup(54, JT_WARLOCK, JT_WARLOCK_H, JT_WARLOCK_B, JT_ARCH_MAGE);
  this->AddJobGroup(55, JT_SORCERER, JT_SORCERER_H, JT_SORCERER_B, JT_ELEMENTAL_MASTER);
  this->AddJobGroup(56, JT_ARCHBISHOP, JT_ARCHBISHOP_H, JT_ARCHBISHOP_B, JT_CARDINAL);
  this->AddJobGroup(57, JT_SURA, JT_SURA_H, JT_SURA_B, JT_INQUISITOR);
  this->AddJobGroup(58, JT_RANGER, JT_RANGER_H, JT_RANGER_B, JT_WINDHAWK);
  this->AddJobGroup(59, JT_MINSTREL, JT_MINSTREL_H, JT_MINSTREL_B, JT_TROUBADOUR);
  this->AddJobGroup(60, JT_WANDERER, JT_WANDERER_H, JT_WANDERER_B, JT_TROUVERE);
  this->AddJobGroup(67, JT_KARNOS, JT_ALITEA, JT_KARNOS_B);
}

// OFFSET: 0x006CBD30
// FUNCTION: fn_06465
void CJobCodeForItem::InitJobCodes() {
  this->AddJobCodeFromGroups(0x1, 0);
  this->AddJobCodeFromGroups(0x40, 27);
  this->AddJobCode(0x40, JT_THIEF, JT_THIEF_H, JT_THIEF_B);
  this->AddJobCodeFromGroups(0x200, 32);
  this->AddJobCodeFromGroups(0x1000, 26);
  this->AddJobCodeFromGroups(0x2000, 23);
  this->AddJobCodeFromGroups(0x2080, 22, 23);
  this->AddJobCodeFromGroups(0x2082, 1);
  this->AddJobCodeFromGroups(0x4100, 28, 29, 11);
  this->AddJobCodeFromGroups(0x4104, 3, 11);
  this->AddJobCodeFromGroups(0x4400, 30, 29, 9);
  this->AddJobCodeFromGroups(0x8000, 33);
  this->AddJobCodeFromGroups(0x10000, 34);
  this->AddJobCodeFromGroups(0x18200, 32, 33, 34);
  this->AddJobCodeFromGroups(0x18208, 4);
  this->AddJobCodeFromGroups(0x20400, 30, 31);
  this->AddJobCodeFromGroups(0x20410, 5);
  this->AddJobCodeFromGroups(0x24514, 3, 5, 11);
  this->AddJobCodeFromGroups(0x24515, 0, 3, 5, 11);
  this->AddJobCodeFromGroups(0x40000, 25);
  this->AddJobCodeFromGroups(0x40800, 24, 25);
  this->AddJobCodeFromGroups(0x40820, 2);
  this->AddJobCodeFromGroups(0x428A2, 1, 2, 66);
  this->AddJobCodeFromGroups(0x60C30, 5, 2);
  this->AddJobCodeFromGroups(0x66DB6, 1, 3, 5, 2, 11);
  this->AddJobCodeFromGroups(0x80240, 32, 27);
  this->AddJobCode(0x80240, JT_THIEF, JT_THIEF_H, JT_THIEF_B);
  this->AddJobCodeFromGroups(0x81040, 6, 61);
  this->AddJobCodeFromGroups(0x98208, 4, 27);
  this->AddJobCodeFromGroups(0x98248, 4, 27);
  this->AddJobCode(0x98248, JT_THIEF, JT_THIEF_H, JT_THIEF_B);
  this->AddJobCodeFromGroups(0xC38E2, 1, 2, 6);
  this->AddJobCodeFromGroups(0xC38E3, 0, 1, 2, 6);
  this->AddJobCodeFromGroups(0xDBAEA, 1, 4, 2, 6);
  this->AddJobCodeFromGroups(0xDFBEE, 1, 3, 4, 2, 6, 11, 61);
  this->AddJobCodeFromGroups(0xDFBEF, 0, 1, 3, 4, 2, 6, 11, 61, 66);
  this->AddJobCodeFromGroups(0xE3CF2, 1, 5, 2, 6);
  this->AddJobCodeFromGroups(0xFBCF2, 1, 5, 2, 6, 33, 34);
  this->AddJobCodeFromGroups(0xFFF80, 13, 16, 14, 17, 20, 21, 18, 9, 11);
  this->AddJobCodeFromGroups(0x438A3, 0, 1, 2, 26, 66);
  this->AddJobCodeFromGroups(0x62CB2, 1, 5, 2, 66);
  this->AddJobCodeFromGroups(0x62CB3, 1, 5, 2, 0, 66);
  this->AddJobCodeFromGroups(0x42, 1, 9);
  this->AddJobCodeFromGroups(0x43, 1, 2, 6, 7);
  this->AddJobCodeFromGroups(0x44, 1, 2, 6, 4, 7, 63);
  this->AddJobCodeFromGroups(0x45, 1, 5, 2, 6, 7);
  this->AddJobCodeFromGroups(0x46, 1, 2, 7);
  this->AddJobCodeFromGroups(0x47, 30, 26);
  this->AddJobCodeFromGroups(0x48, 16, 14, 17, 21, 18);
  this->AddJobCodeFromGroups(0x49, 63);
  this->AddJobCodeFromGroups(0x4A, 61);
  this->AddJobCodeFromGroups(0x4B, 15, 16, 14, 17, 21, 18);
  this->AddJobCodeFromGroups(0x50, 35, 36);
  this->AddJobCode(0x50, JT_SWORDMAN_H);
  this->AddJobCodeFromGroups(0x51, 37, 38);
  this->AddJobCode(0x51, JT_MERCHANT_H);
  this->AddJobCodeFromGroups(0x52, 39, 40);
  this->AddJobCode(0x52, JT_THIEF_H);
  this->AddJobCodeFromGroups(0x53, 41, 42);
  this->AddJobCode(0x53, JT_MAGICIAN_H);
  this->AddJobCodeFromGroups(0x54, 43, 44);
  this->AddJobCode(0x54, JT_ACOLYTE_H);
  this->AddJobCodeFromGroups(0x55, 45, 46, 47);
  this->AddJobCode(0x55, JT_ARCHER_H);
  this->AddJobCodeFromGroups(0x56, 41, 42, 43, 44);
  this->AddJobCode(0x56, JT_MAGICIAN_H, JT_ACOLYTE_H);
  this->AddJobCodeFromGroups(0x57, 35, 36, 37, 38, 39, 40);
  this->AddJobCode(0x57, JT_SWORDMAN_H, JT_MERCHANT_H, JT_THIEF_H);
  this->AddJobCodeFromGroups(0x58, 41, 42, 43, 44, 45, 46, 47);
  this->AddJobCode(0x58, JT_MAGICIAN_H, JT_ACOLYTE_H, JT_ARCHER_H);
  this->AddJobCodeFromGroups(0x59, 35, 36, 37, 38);
  this->AddJobCode(0x59, JT_SWORDMAN_H, JT_MERCHANT_H);
  this->AddJobCodeFromGroups(0x5A, 35, 36, 37, 38, 43, 44);
  this->AddJobCode(0x5A, JT_SWORDMAN_H, JT_MERCHANT_H, JT_ACOLYTE_H);
  this->AddJobCodeFromGroups(0x5B, 35, 36, 37, 38, 39, 40, 45, 46, 47, 41, 42);
  this->AddJobCode(0x5B, JT_SWORDMAN_H, JT_MERCHANT_H, JT_THIEF_H, JT_ARCHER_H, JT_MAGICIAN_H);
  this->AddJobCodeFromGroups(0x5C, 40, 45, 46, 47);
  this->AddJobCode(0x5C, JT_ARCHER_H);
  this->AddJobCodeFromGroups(0x5D, 43, 42);
  this->AddJobCodeFromGroups(0x5E, 43, 44);
  this->AddJobCodeFromGroups(0x5F, 35);
  this->AddJobCodeFromGroups(0x60, 36);
  this->AddJobCodeFromGroups(0x61, 37);
  this->AddJobCodeFromGroups(0x62, 38);
  this->AddJobCodeFromGroups(0x63, 41);
  this->AddJobCodeFromGroups(0x64, 42);
  this->AddJobCodeFromGroups(0x65, 45);
  this->AddJobCodeFromGroups(0x66, 39);
  this->AddJobCodeFromGroups(0x67, 46);
  this->AddJobCodeFromGroups(0x68, 47);
  this->AddJobCodeFromGroups(0x69, 43);
  this->AddJobCodeFromGroups(0x6A, 44);
  this->AddJobCodeFromGroups(0x6B, 40);
  this->AddJobCodeFromGroups(0x6C, 3, 5, 4, 11);
  this->AddJobCodeFromGroups(0x6D, 1, 2, 6, 7, 61);
  this->AddJobCodeFromGroups(0x6E, 14, 17, 21, 18);
  this->AddJobCodeFromGroups(0x6F, 48);
  this->AddJobCodeFromGroups(0x70, 49);
  this->AddJobCodeFromGroups(0x71, 56, 55);
  this->AddJobCodeFromGroups(0x72, 52);
  this->AddJobCodeFromGroups(0x73, 50);
  this->AddJobCodeFromGroups(0x74, 58);
  this->AddJobCodeFromGroups(0x75, 51);
  this->AddJobCodeFromGroups(0x76, 54);
  this->AddJobCodeFromGroups(0x77, 55);
  this->AddJobCodeFromGroups(0x78, 59);
  this->AddJobCodeFromGroups(0x79, 60);
  this->AddJobCodeFromGroups(0x7A, 56);
  this->AddJobCodeFromGroups(0x7B, 57);
  this->AddJobCodeFromGroups(0x7C, 53);
  this->AddJobCodeFromGroups(0x7D, 48, 49);
  this->AddJobCodeFromGroups(0x7E, 54, 55, 56, 57);
  this->AddJobCodeFromGroups(0x7F, 48, 49, 50, 51, 56, 57);
  this->AddJobCodeFromGroups(0x80, 58, 59, 60);
  this->AddJobCodeFromGroups(0x81, 55, 51);
  this->AddJobCodeFromGroups(0x82, 9);
  this->AddJobCodeFromGroups(0x83, 61, 63);
  this->AddJobCodeFromGroups(0x84, 54, 56);
  this->AddJobCodeFromGroups(0x85, 1, 2, 6, 4, 7, 61, 63);
  this->AddJobCodeFromGroups(0x86, 13, 16, 14, 17, 20, 21, 18, 9, 11, 62, 64);
  this->AddJobCodeFromGroups(0x87, 59, 60);
  this->AddJobCodeFromGroups(0x88, 54, 55);
  this->AddJobCodeFromGroups(0x89, 50, 51);
  this->AddJobCodeFromGroups(0x8A, 56, 57);
  this->AddJobCodeFromGroups(0x8B, 52, 53);
  this->AddJobCodeFromGroups(0x8C, 45, 46, 47, 39, 40);
  this->AddJobCode(0x8C, JT_ARCHER_H, JT_THIEF_H);
  this->AddJobCodeFromGroups(0x8D, 11);
  this->AddJobCodeFromGroups(0x8E, 65);
  this->AddJobCodeFromGroups(0x8F, 8);
  this->AddJobCodeFromGroups(0x90, 64, 62, 10, 12, 67);
  this->AddJobCodeFromGroups(0x91, 10);
  this->AddJobCodeFromGroups(0x92, 12);
  this->AddJobCodeFromGroups(0x93, 58, 53);
  this->AddJobCodeFromGroups(0x94, 4, 6, 61, 63);
  this->AddJobCodeFromGroups(0x95, 0, 1, 2, 3, 4, 5, 6, 8, 61, 63, 66);
  this->AddJobCodeFromGroups(0x96, 22);
  this->AddJobCodeFromGroups(0x97, 24);
  this->AddJobCodeFromGroups(0x98, 28);
  this->AddJobCodeFromGroups(0x99, 29);
  this->AddJobCodeFromGroups(0x9A, 30);
  this->AddJobCodeFromGroups(0x9B, 31);
  this->AddJobCodeFromGroups(0x9C, 27);
  this->AddJobCode(0x9D, JT_DRAGON_KNIGHT);
  this->AddJobCode(0x9E, JT_MEISTER);
  this->AddJobCode(0x9F, JT_SHADOW_CROSS);
  this->AddJobCode(0xA0, JT_ARCH_MAGE);
  this->AddJobCode(0xA1, JT_CARDINAL);
  this->AddJobCode(0xA2, JT_WINDHAWK);
  this->AddJobCode(0xA3, JT_IMPERIAL_GUARD);
  this->AddJobCode(0xA4, JT_BIOLO);
  this->AddJobCode(0xA5, JT_ABYSS_CHASER);
  this->AddJobCode(0xA6, JT_ELEMENTAL_MASTER);
  this->AddJobCode(0xA7, JT_INQUISITOR);
  this->AddJobCode(0xA8, JT_TROUBADOUR, JT_TROUVERE);
  this->AddJobCodeFromGroups(0xA9, 18);
  this->AddJobCode(0xAA, JT_TROUBADOUR);
  this->AddJobCode(0xAB, JT_TROUVERE);
  this->AddJobCodeFromGroups(0xAC, 3);
  this->AddJobCode(0xAD, JT_SKY_EMPEROR);
  this->AddJobCode(0xAE, JT_SOUL_ASCETIC);
  this->AddJobCode(0xAF, JT_SHINKIRO, JT_SHIRANUI);
  this->AddJobCode(0xB0, JT_NIGHT_WATCH);
  this->AddJobCode(0xB1, JT_HYPER_NOVICE);
  this->AddJobCode(0xB2, JT_SPIRIT_HANDLER);
  this->AddJobCode(0xB3, JT_SKY_EMPEROR, JT_SOUL_ASCETIC, JT_SHINKIRO, JT_SHIRANUI, JT_NIGHT_WATCH, JT_HYPER_NOVICE, JT_SPIRIT_HANDLER, JT_ALITEA);
  this->AddJobCodeFromGroups(0xB4, 18);
  this->AddJobCode(0xB4, JT_SKY_EMPEROR, JT_SOUL_ASCETIC, JT_SHINKIRO, JT_SHIRANUI, JT_NIGHT_WATCH, JT_HYPER_NOVICE, JT_SPIRIT_HANDLER, JT_ALITEA);
  this->AddJobCode(0xB5, JT_DRAGON_KNIGHT, JT_IMPERIAL_GUARD);
  this->AddJobCode(0xB6, JT_HYPER_NOVICE, JT_ARCH_MAGE, JT_ELEMENTAL_MASTER, JT_CARDINAL, JT_INQUISITOR, JT_SOUL_ASCETIC);
  this->AddJobCode(0xB7, JT_WINDHAWK, JT_TROUBADOUR, JT_TROUVERE, JT_ABYSS_CHASER);
  this->AddJobCode(0xB8, JT_ARCH_MAGE, JT_ELEMENTAL_MASTER, JT_CARDINAL, JT_INQUISITOR, JT_SOUL_ASCETIC);
  this->AddJobCode(0xB9, JT_CARDINAL, JT_ELEMENTAL_MASTER, JT_SKY_EMPEROR);
  this->AddJobCode(0xBA, JT_DRAGON_KNIGHT, JT_IMPERIAL_GUARD, JT_MEISTER, JT_BIOLO);
  this->AddJobCode(0xBB, JT_CARDINAL, JT_INQUISITOR);
  this->AddJobCode(0xBC, JT_HYPER_NOVICE, JT_DRAGON_KNIGHT, JT_IMPERIAL_GUARD, JT_ARCH_MAGE, JT_ELEMENTAL_MASTER, JT_WINDHAWK, JT_TROUBADOUR, JT_TROUVERE,
                   JT_SHADOW_CROSS, JT_ABYSS_CHASER, JT_MEISTER, JT_BIOLO, JT_SOUL_ASCETIC, JT_SHINKIRO, JT_SHIRANUI, JT_HYPER_NOVICE, JT_ALITEA);
  this->AddJobCode(0xBD, JT_HYPER_NOVICE, JT_DRAGON_KNIGHT, JT_IMPERIAL_GUARD, JT_CARDINAL, JT_INQUISITOR, JT_MEISTER, JT_BIOLO);
  this->AddJobCodeFromGroups(0xBE, 18);
  this->AddJobCode(0xBE, JT_SKY_EMPEROR, JT_SOUL_ASCETIC, JT_SHINKIRO, JT_SHIRANUI, JT_NIGHT_WATCH, JT_HYPER_NOVICE, JT_ALITEA);
  this->AddJobCodeFromGroups(0xBF, 66);
  this->AddJobCode(0xC0, JT_ALITEA);
  this->AddJobCodeFromGroups(0xC1, 67);
  this->AddJobCodeFromGroups(0xC2, 1, 2);
}

// OFFSET: 0x006CCB30
// FUNCTION: fn_06466
void CJobCodeForItem::Init() {
  this->InitJobGroups();
  this->InitJobCodes();
}

// OFFSET: 0x006CCBE0
// FUNCTION: fn_06467
bool CJobCodeForItem::IsJobAllowed(int job, unsigned long code) {
  if (code == 0xFFFFE) {
    auto group = this->m_jobGroups.find(0);
    if (group == this->m_jobGroups.end()) {
      return false;
    }
    return group->second.find(job) == group->second.end();
  }
  if (code == 0xFFFFF) {
    return true;
  }

  auto it = this->m_jobCodes.find(code);
  if (it == this->m_jobCodes.end()) {
    return false;
  }
  return it->second.find(job) != it->second.end();
}

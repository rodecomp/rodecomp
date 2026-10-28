#include "Client/Session.h"
#include "Contents/Jobs/JobId.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00DE0A70
// FUNCTION: fn_23467
float CSession::GetNpcLocationRadius(int job, bool riding) {
  if (riding == true) {
    job = this->GetRidingJob(job);
  }

  int radius = 0;
  g_session.m_hotkeyScript->Call("GetNpcLocationRadius", "d>d", job, &radius);
  if (radius != 0) {
    return static_cast<float>(radius * 2 + 1);
  }

  if (this->m_npcLocationRadius.find(job) == this->m_npcLocationRadius.end()) {
    return 1.0f;
  }

  return this->m_npcLocationRadius[job];
}

// OFFSET: 0x00E130A0
// FUNCTION: fn_23823
int CSession::GetRidingJob(int job) {
  if (job == JT_NINJA) {
    return JT_FROG_NINJA;
  }
  if (job == JT_LINKER) {
    return JT_FROG_LINKER;
  }
  if (job == JT_GUNSLINGER) {
    return JT_PECO_GUNNER;
  }
  if (job == JT_SWORDMAN) {
    return JT_PECO_SWORD;
  }
  if (job == JT_BLACKSMITH_H) {
    return JT_PIG_WHITESMITH;
  }
  if (job == JT_MERCHANT) {
    return JT_PIG_MERCHANT;
  }
  if (job == JT_GENETIC || job == JT_GENETIC_H) {
    return JT_PIG_GENETIC;
  }
  if (job == JT_ALCHEMIST_H) {
    return JT_PIG_CREATOR;
  }
  if (job == JT_ARCHER) {
    return JT_OSTRICH_ARCHER;
  }
  if (job == JT_STAR || job == JT_STAR2) {
    return JT_PORING_STAR;
  }
  if (job == JT_NOVICE) {
    return JT_PORING_NOVICE;
  }
  if (job == JT_MONK) {
    return JT_SHEEP_MONK;
  }
  if (job == JT_ACOLYTE) {
    return JT_SHEEP_ACO;
  }
  if (job == JT_SURA || job == JT_SURA_H) {
    return JT_SHEEP_SURA;
  }
  if (job == JT_SUPERNOVICE) {
    return JT_PORING_SNOVICE;
  }
  if (job == JT_ARCHBISHOP || job == JT_ARCHBISHOP_H) {
    return JT_SHEEP_ARCB;
  }
  if (job == JT_MAGICIAN) {
    return JT_FOX_MAGICIAN;
  }
  if (job == JT_SAGE) {
    return JT_FOX_SAGE;
  }
  if (job == JT_SORCERER || job == JT_SORCERER_H) {
    return JT_FOX_SORCERER;
  }
  if (job == JT_WARLOCK || job == JT_WARLOCK_H) {
    return JT_FOX_WARLOCK;
  }
  if (job == JT_WIZARD) {
    return JT_FOX_WIZ;
  }
  if (job == JT_SAGE_H) {
    return JT_FOX_PROF;
  }
  if (job == JT_WIZARD_H) {
    return JT_FOX_HWIZ;
  }
  if (job == JT_ALCHEMIST) {
    return JT_PIG_ALCHE;
  }
  if (job == JT_BLACKSMITH) {
    return JT_PIG_BLACKSMITH;
  }
  if (job == JT_MONK_H) {
    return JT_SHEEP_CHAMP;
  }
  if (job == JT_GUILLOTINE_CROSS || job == JT_GUILLOTINE_CROSS_H) {
    return JT_DOG_G_CROSS;
  }
  if (job == JT_THIEF) {
    return JT_DOG_THIEF;
  }
  if (job == JT_ROGUE) {
    return JT_DOG_ROGUE;
  }
  if (job == JT_SHADOW_CHASER || job == JT_SHADOW_CHASER_H) {
    return JT_DOG_CHASER;
  }
  if (job == JT_ROGUE_H) {
    return JT_DOG_STALKER;
  }
  if (job == JT_ASSASSIN) {
    return JT_DOG_ASSASSIN;
  }
  if (job == JT_ASSASSIN_H) {
    return JT_DOG_ASSA_X;
  }
  if (job == JT_DANCER) {
    return JT_OSTRICH_DANCER;
  }
  if (job == JT_MINSTREL || job == JT_MINSTREL_H) {
    return JT_OSTRICH_MINSTREL;
  }
  if (job == JT_BARD) {
    return JT_OSTRICH_BARD;
  }
  if (job == JT_HUNTER_H) {
    return JT_OSTRICH_SNIPER;
  }
  if (job == JT_WANDERER || job == JT_WANDERER_H) {
    return JT_OSTRICH_WANDER;
  }
  if (job == JT_DANCER_H) {
    return JT_OSTRICH_ZIPSI;
  }
  if (job == JT_BARD_H) {
    return JT_OSTRICH_CROWN;
  }
  if (job == JT_HUNTER) {
    return JT_OSTRICH_HUNTER;
  }
  if (job == JT_TAEKWON) {
    return JT_PORING_TAEKWON;
  }
  if (job == JT_PRIEST) {
    return JT_SHEEP_PRIEST;
  }
  if (job == JT_PRIEST_H) {
    return JT_SHEEP_HPRIEST;
  }
  if (job == JT_NOVICE_B) {
    return JT_PORING_NOVICE_B;
  }
  if (job == JT_SWORDMAN_B) {
    return JT_PECO_SWORD_B;
  }
  if (job == JT_MAGICIAN_B) {
    return JT_FOX_MAGICIAN_B;
  }
  if (job == JT_ARCHER_B) {
    return JT_OSTRICH_ARCHER_B;
  }
  if (job == JT_ACOLYTE_B) {
    return JT_SHEEP_ACO_B;
  }
  if (job == JT_MERCHANT_B) {
    return JT_PIG_MERCHANT_B;
  }
  if (job == JT_HUNTER_B) {
    return JT_OSTRICH_HUNTER_B;
  }
  if (job == JT_ASSASSIN_B) {
    return JT_DOG_ASSASSIN_B;
  }
  if (job == JT_MONK_B) {
    return JT_SHEEP_MONK_B;
  }
  if (job == JT_SAGE_B) {
    return JT_FOX_SAGE_B;
  }
  if (job == JT_ROGUE_B) {
    return JT_DOG_ROGUE_B;
  }
  if (job == JT_ALCHEMIST_B) {
    return JT_PIG_ALCHE_B;
  }
  if (job == JT_BARD_B) {
    return JT_OSTRICH_BARD_B;
  }
  if (job == JT_DANCER_B) {
    return JT_OSTRICH_DANCER_B;
  }
  if (job == JT_SUPERNOVICE_B) {
    return JT_PORING_SNOVICE_B;
  }
  if (job == JT_WARLOCK_B) {
    return JT_FOX_WARLOCK_B;
  }
  if (job == JT_ARCHBISHOP_B) {
    return JT_SHEEP_ARCB_B;
  }
  if (job == JT_GUILLOTINE_CROSS_B) {
    return JT_DOG_G_CROSS_B;
  }
  if (job == JT_SORCERER_B) {
    return JT_FOX_SORCERER_B;
  }
  if (job == JT_MINSTREL_B) {
    return JT_OSTRICH_MINSTREL_B;
  }
  if (job == JT_WANDERER_B) {
    return JT_OSTRICH_WANDER_B;
  }
  if (job == JT_SURA_B) {
    return JT_SHEEP_SURA_B;
  }
  if (job == JT_GENETIC_B) {
    return JT_PIG_GENETIC_B;
  }
  if (job == JT_SHADOW_CHASER_B) {
    return JT_DOG_CHASER_B;
  }
  if (job == JT_NOVICE_H) {
    return JT_PORING_NOVICE_H;
  }
  if (job == JT_SWORDMAN_H) {
    return JT_PECO_SWORD_H;
  }
  if (job == JT_MAGICIAN_H) {
    return JT_FOX_MAGICIAN_H;
  }
  if (job == JT_ARCHER_H) {
    return JT_OSTRICH_ARCHER_H;
  }
  if (job == JT_ACOLYTE_H) {
    return JT_SHEEP_ACO_H;
  }
  if (job == JT_MERCHANT_H) {
    return JT_PIG_MERCHANT_H;
  }
  if (job == JT_THIEF_H) {
    return JT_DOG_THIEF_H;
  }
  if (job == JT_THIEF_B) {
    return JT_DOG_THIEF_B;
  }
  if (job == JT_SUPERNOVICE2) {
    return JT_PORING_SNOVICE2;
  }
  if (job == JT_SUPERNOVICE2_B) {
    return JT_PORING_SNOVICE2_B;
  }
  if (job == JT_PRIEST_B) {
    return JT_SHEEP_PRIEST_B;
  }
  if (job == JT_WIZARD_B) {
    return JT_FOX_WIZ_B;
  }
  if (job == JT_BLACKSMITH_B) {
    return JT_PIG_BLACKSMITH_B;
  }
  if (job == JT_MECHANIC || job == JT_MECHANIC_H) {
    return JT_PIG_MECHANIC;
  }
  if (job == JT_MECHANIC_B) {
    return JT_PIG_MECHANIC_B;
  }
  if (job == JT_RANGER || job == JT_RANGER_H) {
    return JT_OSTRICH_RANGER;
  }
  if (job == JT_RANGER_B) {
    return JT_OSTRICH_RANGER_B;
  }
  if (job == JT_KNIGHT) {
    return JT_LION_KNIGHT;
  }
  if (job == JT_KNIGHT_H) {
    return JT_LION_KNIGHT_H;
  }
  if (job == JT_KNIGHT_B) {
    return JT_LION_KNIGHT_B;
  }
  if (job == JT_ROYAL_GUARD || job == JT_ROYAL_GUARD_H) {
    return JT_LION_ROYAL_GUARD;
  }
  if (job == JT_ROYAL_GUARD_B) {
    return JT_LION_ROYAL_GUARD_B;
  }
  if (job == JT_RUNE_KNIGHT || job == JT_RUNE_KNIGHT_H) {
    return JT_LION_RUNE_KNIGHT;
  }
  if (job == JT_RUNE_KNIGHT_B) {
    return JT_LION_RUNE_KNIGHT_B;
  }
  if (job == JT_CRUSADER) {
    return JT_LION_CRUSADER;
  }
  if (job == JT_CRUSADER_H) {
    return JT_LION_CRUSADER_H;
  }
  if (job == JT_CRUSADER_B) {
    return JT_LION_CRUSADER_B;
  }
  if (job == JT_KAGEROU) {
    return JT_FROG_KAGEROU;
  }
  if (job == JT_OBORO) {
    return JT_FROG_OBORO;
  }
  if (job == JT_REBELLION) {
    return JT_PECO_REBELLION;
  }
  if (job == JT_DO_SUMMONER) {
    return JT_DO_CART_SUMMONER;
  }
  if (job == JT_DO_SUMMONER_B) {
    return JT_DO_CART_SUMMONER_B;
  }
  if (job == JT_NINJA_B) {
    return JT_FROG_NINJA_B;
  }
  if (job == JT_KAGEROU_B) {
    return JT_FROG_KAGEROU_B;
  }
  if (job == JT_OBORO_B) {
    return JT_FROG_OBORO_B;
  }
  if (job == JT_TAEKWON_B) {
    return JT_PORING_TAEKWON_B;
  }
  if (job == JT_STAR_B || job == JT_STAR2_B) {
    return JT_PORING_STAR_B;
  }
  if (job == JT_LINKER_B) {
    return JT_FROG_LINKER_B;
  }
  if (job == JT_GUNSLINGER_B) {
    return JT_PECO_GUNNER_B;
  }
  if (job == JT_REBELLION_B) {
    return JT_PECO_REBELLION_B;
  }
  if (job == JT_STAR_EMPEROR || job == JT_STAR2_EMPEROR) {
    return JT_HAETAE_STAR_EMPEROR;
  }
  if (job == JT_STAR_EMPEROR_B || job == JT_STAR2_EMPEROR_B) {
    return JT_HAETAE_STAR_EMPEROR_B;
  }
  if (job == JT_SOUL_REAPER) {
    return JT_HAETAE_SOUL_REAPER;
  }
  if (job == JT_SOUL_REAPER_B) {
    return JT_HAETAE_SOUL_REAPER_B;
  }
  if (job == JT_SHADOW_CROSS) {
    return JT_SHADOW_CROSS_RIDING;
  }
  if (job == JT_ABYSS_CHASER) {
    return JT_ABYSS_CHASER_RIDING;
  }
  if (job == JT_DRAGON_KNIGHT) {
    return JT_DRAGON_KNIGHT_RIDING;
  }
  if (job == JT_IMPERIAL_GUARD) {
    return JT_IMPERIAL_GUARD_RIDING;
  }
  if (job == JT_BIOLO) {
    return JT_BIOLO_RIDING;
  }
  if (job == JT_MEISTER) {
    return JT_MEISTER_RIDING;
  }
  if (job == JT_WINDHAWK) {
    return JT_WINDHAWK_RIDING;
  }
  if (job == JT_TROUBADOUR) {
    return JT_TROUBADOUR_RIDING;
  }
  if (job == JT_TROUVERE) {
    return JT_TROUVERE_RIDING;
  }
  if (job == JT_ARCH_MAGE) {
    return JT_ARCH_MAGE_RIDING;
  }
  if (job == JT_ELEMENTAL_MASTER) {
    return JT_ELEMENTAL_MASTER_RIDING;
  }
  if (job == JT_CARDINAL) {
    return JT_CARDINAL_RIDING;
  }
  if (job == JT_INQUISITOR) {
    return JT_INQUISITOR_RIDING;
  }
  if (job == JT_SKY_EMPEROR || job == JT_SKY_EMPEROR2) {
    return JT_SKY_EMPEROR_RIDING;
  }
  if (job == JT_SOUL_ASCETIC) {
    return JT_SOUL_ASCETIC_RIDING;
  }
  if (job == JT_SHINKIRO) {
    return JT_SHINKIRO_RIDING;
  }
  if (job == JT_SHIRANUI) {
    return JT_SHIRANUI_RIDING;
  }
  if (job == JT_NIGHT_WATCH) {
    return JT_NIGHT_WATCH_RIDING;
  }
  if (job == JT_HYPER_NOVICE) {
    return JT_HYPER_NOVICE_RIDING;
  }
  if (job == JT_SPIRIT_HANDLER) {
    return JT_SPIRIT_HANDLER_RIDING;
  }
  if (job == JT_DRUID || job == JT_DRUID_B) {
    return JT_DRUID_RIDING;
  }
  if (job == JT_KARNOS || job == JT_KARNOS_B) {
    return JT_KARNOS_RIDING;
  }
  if (job == JT_ALITEA) {
    return JT_ALITEA_RIDING;
  }
  if (job == JT_WEREWOLF || job == JT_WERERAPTOR) {
    return JT_DRUID_RIDING;
  }

  return job;
}

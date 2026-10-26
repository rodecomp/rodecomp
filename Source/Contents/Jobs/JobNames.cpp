#include "Contents/Jobs/JobNames.h"
#include "Contents/Jobs/JobId.h"

// OFFSET: 0x00E2FE80
// FUNCTION: fn_24165
unsigned JobNames::BaseJob(unsigned job) {
  switch (job) {
  case JT_CHICKEN:
  case JT_LION_KNIGHT:
    return JT_KNIGHT;
  case JT_CHICKEN2:
  case JT_LION_CRUSADER:
    return JT_CRUSADER;
  case JT_CHICKEN_H:
  case JT_LION_KNIGHT_H:
    return JT_KNIGHT_H;
  case JT_CHICKEN2_H:
  case JT_LION_CRUSADER_H:
    return JT_CRUSADER_H;
  case JT_CHICKEN_B:
  case JT_LION_KNIGHT_B:
    return JT_KNIGHT_B;
  case JT_CHICKEN2_B:
  case JT_LION_CRUSADER_B:
    return JT_CRUSADER_B;
  case JT_RUNE_CHICKEN:
  case JT_RUNE_CHICKEN2:
  case JT_RUNE_CHICKEN3:
  case JT_RUNE_CHICKEN4:
  case JT_RUNE_CHICKEN5:
  case JT_LION_RUNE_KNIGHT:
    return JT_RUNE_KNIGHT;
  case JT_RUNE_CHICKEN_H:
  case JT_RUNE_CHICKEN2_H:
  case JT_RUNE_CHICKEN3_H:
  case JT_RUNE_CHICKEN4_H:
  case JT_RUNE_CHICKEN5_H:
    return JT_RUNE_KNIGHT_H;
  case JT_ROYAL_CHICKEN:
  case JT_LION_ROYAL_GUARD:
    return JT_ROYAL_GUARD;
  case JT_ROYAL_CHICKEN_H:
    return JT_ROYAL_GUARD_H;
  case JT_WOLF_RANGER:
  case JT_OSTRICH_RANGER:
    return JT_RANGER;
  case JT_WOLF_RANGER_H:
    return JT_RANGER_H;
  case JT_MADOGEAR:
  case JT_PIG_MECHANIC:
    return JT_MECHANIC;
  case JT_MADOGEAR_H:
    return JT_MECHANIC_H;
  case JT_RUNE_CHICKEN_B:
  case JT_LION_RUNE_KNIGHT_B:
    return JT_RUNE_KNIGHT_B;
  case JT_ROYAL_CHICKEN_B:
  case JT_LION_ROYAL_GUARD_B:
    return JT_ROYAL_GUARD_B;
  case JT_WOLF_RANGER_B:
  case JT_OSTRICH_RANGER_B:
    return JT_RANGER_B;
  case JT_MADOGEAR_B:
  case JT_PIG_MECHANIC_B:
    return JT_MECHANIC_B;
  case JT_FROG_NINJA:
    return JT_NINJA;
  case JT_PECO_GUNNER:
    return JT_GUNSLINGER;
  case JT_PECO_SWORD:
    return JT_SWORDMAN;
  case JT_FROG_LINKER:
    return JT_LINKER;
  case JT_PIG_WHITESMITH:
    return JT_BLACKSMITH_H;
  case JT_PIG_MERCHANT:
    return JT_MERCHANT;
  case JT_PIG_GENETIC:
    return JT_GENETIC;
  case JT_PIG_CREATOR:
    return JT_ALCHEMIST_H;
  case JT_OSTRICH_ARCHER:
    return JT_ARCHER;
  case JT_PORING_STAR:
    return JT_STAR;
  case JT_PORING_NOVICE:
    return JT_NOVICE;
  case JT_SHEEP_MONK:
    return JT_MONK;
  case JT_SHEEP_ACO:
    return JT_ACOLYTE;
  case JT_SHEEP_SURA:
    return JT_SURA;
  case JT_PORING_SNOVICE:
    return JT_SUPERNOVICE;
  case JT_SHEEP_ARCB:
    return JT_ARCHBISHOP;
  case JT_FOX_MAGICIAN:
    return JT_MAGICIAN;
  case JT_FOX_SAGE:
    return JT_SAGE;
  case JT_FOX_SORCERER:
    return JT_SORCERER;
  case JT_FOX_WARLOCK:
    return JT_WARLOCK;
  case JT_FOX_WIZ:
    return JT_WIZARD;
  case JT_FOX_PROF:
    return JT_SAGE_H;
  case JT_FOX_HWIZ:
    return JT_WIZARD_H;
  case JT_PIG_ALCHE:
    return JT_ALCHEMIST;
  case JT_PIG_BLACKSMITH:
    return JT_BLACKSMITH;
  case JT_SHEEP_CHAMP:
    return JT_MONK_H;
  case JT_DOG_G_CROSS:
    return JT_GUILLOTINE_CROSS;
  case JT_DOG_THIEF:
    return JT_THIEF;
  case JT_DOG_ROGUE:
    return JT_ROGUE;
  case JT_DOG_CHASER:
    return JT_SHADOW_CHASER;
  case JT_DOG_STALKER:
    return JT_ROGUE_H;
  case JT_DOG_ASSASSIN:
    return JT_ASSASSIN;
  case JT_DOG_ASSA_X:
    return JT_ASSASSIN_H;
  case JT_OSTRICH_DANCER:
    return JT_DANCER;
  case JT_OSTRICH_MINSTREL:
    return JT_MINSTREL;
  case JT_OSTRICH_BARD:
    return JT_BARD;
  case JT_OSTRICH_SNIPER:
    return JT_HUNTER_H;
  case JT_OSTRICH_WANDER:
    return JT_WANDERER;
  case JT_OSTRICH_ZIPSI:
    return JT_DANCER_H;
  case JT_OSTRICH_CROWN:
    return JT_BARD_H;
  case JT_OSTRICH_HUNTER:
    return JT_HUNTER;
  case JT_PORING_TAEKWON:
    return JT_TAEKWON;
  case JT_SHEEP_PRIEST:
    return JT_PRIEST;
  case JT_SHEEP_HPRIEST:
    return JT_PRIEST_H;
  case JT_PORING_NOVICE_B:
    return JT_NOVICE_B;
  case JT_PECO_SWORD_B:
    return JT_SWORDMAN_B;
  case JT_FOX_MAGICIAN_B:
    return JT_MAGICIAN_B;
  case JT_OSTRICH_ARCHER_B:
    return JT_ARCHER_B;
  case JT_SHEEP_ACO_B:
    return JT_ACOLYTE_B;
  case JT_PIG_MERCHANT_B:
    return JT_MERCHANT_B;
  case JT_OSTRICH_HUNTER_B:
    return JT_HUNTER_B;
  case JT_DOG_ASSASSIN_B:
    return JT_ASSASSIN_B;
  case JT_SHEEP_MONK_B:
    return JT_MONK_B;
  case JT_FOX_SAGE_B:
    return JT_SAGE_B;
  case JT_DOG_ROGUE_B:
    return JT_ROGUE_B;
  case JT_PIG_ALCHE_B:
    return JT_ALCHEMIST_B;
  case JT_OSTRICH_BARD_B:
    return JT_BARD_B;
  case JT_OSTRICH_DANCER_B:
    return JT_DANCER_B;
  case JT_PORING_SNOVICE_B:
    return JT_SUPERNOVICE_B;
  case JT_FOX_WARLOCK_B:
    return JT_WARLOCK_B;
  case JT_SHEEP_ARCB_B:
    return JT_ARCHBISHOP_B;
  case JT_DOG_G_CROSS_B:
    return JT_GUILLOTINE_CROSS_B;
  case JT_FOX_SORCERER_B:
    return JT_SORCERER_B;
  case JT_OSTRICH_MINSTREL_B:
    return JT_MINSTREL_B;
  case JT_OSTRICH_WANDER_B:
    return JT_WANDERER_B;
  case JT_SHEEP_SURA_B:
    return JT_SURA_B;
  case JT_PIG_GENETIC_B:
    return JT_GENETIC_B;
  case JT_DOG_THIEF_B:
    return JT_THIEF_B;
  case JT_DOG_CHASER_B:
    return JT_SHADOW_CHASER_B;
  case JT_PORING_NOVICE_H:
    return JT_NOVICE_H;
  case JT_PECO_SWORD_H:
    return JT_SWORDMAN_H;
  case JT_FOX_MAGICIAN_H:
    return JT_MAGICIAN_H;
  case JT_OSTRICH_ARCHER_H:
    return JT_ARCHER_H;
  case JT_SHEEP_ACO_H:
    return JT_ACOLYTE_H;
  case JT_PIG_MERCHANT_H:
    return JT_MERCHANT_H;
  case JT_DOG_THIEF_H:
    return JT_THIEF_H;
  case JT_PORING_SNOVICE2:
    return JT_SUPERNOVICE2;
  case JT_PORING_SNOVICE2_B:
    return JT_SUPERNOVICE2_B;
  case JT_SHEEP_PRIEST_B:
    return JT_PRIEST_B;
  case JT_FOX_WIZ_B:
    return JT_WIZARD_B;
  case JT_PIG_BLACKSMITH_B:
    return JT_BLACKSMITH_B;
  case JT_FROG_KAGEROU:
    return JT_KAGEROU;
  case JT_FROG_OBORO:
    return JT_OBORO;
  case JT_PECO_REBELLION:
    return JT_REBELLION;
  case JT_DO_CART_SUMMONER:
    return JT_DO_SUMMONER;
  case JT_DO_CART_SUMMONER_B:
    return JT_DO_SUMMONER_B;
  case JT_FROG_NINJA_B:
    return JT_NINJA_B;
  case JT_FROG_KAGEROU_B:
    return JT_KAGEROU_B;
  case JT_FROG_OBORO_B:
    return JT_OBORO_B;
  case JT_PORING_TAEKWON_B:
    return JT_TAEKWON_B;
  case JT_PORING_STAR_B:
    return JT_STAR_B;
  case JT_FROG_LINKER_B:
    return JT_LINKER_B;
  case JT_PECO_GUNNER_B:
    return JT_GUNSLINGER_B;
  case JT_PECO_REBELLION_B:
    return JT_REBELLION_B;
  case JT_HAETAE_STAR_EMPEROR:
    return JT_STAR_EMPEROR;
  case JT_HAETAE_SOUL_REAPER:
    return JT_SOUL_REAPER;
  case JT_HAETAE_STAR_EMPEROR_B:
    return JT_STAR_EMPEROR_B;
  case JT_HAETAE_SOUL_REAPER_B:
    return JT_SOUL_REAPER_B;
  case JT_DRAGON_KNIGHT_RIDING:
  case JT_DRAGON_KNIGHT_CHICKEN:
    return JT_DRAGON_KNIGHT;
  case JT_MEISTER_RIDING:
  case JT_MEISTER_MADOGEAR:
    return JT_MEISTER;
  case JT_SHADOW_CROSS_RIDING:
    return JT_SHADOW_CROSS;
  case JT_ARCH_MAGE_RIDING:
    return JT_ARCH_MAGE;
  case JT_CARDINAL_RIDING:
    return JT_CARDINAL;
  case JT_WINDHAWK_RIDING:
  case JT_WOLF_WINDHAWK:
    return JT_WINDHAWK;
  case JT_IMPERIAL_GUARD_RIDING:
  case JT_IMPERIAL_GUARD_CHICKEN:
    return JT_IMPERIAL_GUARD;
  case JT_BIOLO_RIDING:
    return JT_BIOLO;
  case JT_ABYSS_CHASER_RIDING:
    return JT_ABYSS_CHASER;
  case JT_ELEMENTAL_MASTER_RIDING:
    return JT_ELEMENTAL_MASTER;
  case JT_INQUISITOR_RIDING:
    return JT_INQUISITOR;
  case JT_TROUBADOUR_RIDING:
    return JT_TROUBADOUR;
  case JT_TROUVERE_RIDING:
    return JT_TROUVERE;
  case JT_SKY_EMPEROR_RIDING:
    return JT_SKY_EMPEROR;
  case JT_SOUL_ASCETIC_RIDING:
    return JT_SOUL_ASCETIC;
  case JT_SHINKIRO_RIDING:
    return JT_SHINKIRO;
  case JT_SHIRANUI_RIDING:
    return JT_SHIRANUI;
  case JT_NIGHT_WATCH_RIDING:
    return JT_NIGHT_WATCH;
  case JT_HYPER_NOVICE_RIDING:
    return JT_HYPER_NOVICE;
  case JT_SPIRIT_HANDLER_RIDING:
    return JT_SPIRIT_HANDLER;
  case JT_DRUID_RIDING:
    return JT_DRUID;
  case JT_KARNOS_RIDING:
    return JT_KARNOS;
  case JT_ALITEA_RIDING:
    return JT_ALITEA;
  default:
    return job;
  }
}

// OFFSET: 0x00DDE5C0
// FUNCTION: fn_23396
const char* JobNames::Lookup(unsigned job, int sex) {
  if (this->names.size() <= job) {
    return "";
  }

  if (job - JT_NOVICE_H > unsigned(JT_MER_BEGIN - 1 - JT_NOVICE_H) && job > unsigned(JT_SUMMER2)) {
    return this->names[job];
  }

  const unsigned base = BaseJob(job);

  if (sex == -1) {
    if (this->cachedSex != this->currentSex) {
      this->cachedSex = this->currentSex;
    }

    sex = this->currentSex;
  }

  if (sex == 0) {
    const auto found = this->female.find(base);

    if (found != this->female.end()) {
      return found->second;
    }
  } else if (sex == 1) {
    const auto found = this->male.find(base);

    if (found != this->male.end()) {
      return found->second;
    }
  }

  return this->names[base];
}

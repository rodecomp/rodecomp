#include "Client/Session.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Jobs/JobNames.h"

// OFFSET: 0x00E2EE70
// FUNCTION: fn_24152
int CSession::GetSpriteJob(int job, int costumeJob, bool spriteIndex) {
  switch (job) {
  case JT_MARRIED:
  case JT_SANTA:
  case JT_SUMMER:
  case JT_HANBOK:
  case JT_OKTOBERFEST:
  case JT_SUMMER2:
    return job;
  }

  int mount;
  if (g_session.GetRidingJob(JobNames::BaseJob(job)) == job) {
    mount = 1;
  } else {
    switch (job) {
    case JT_CHICKEN:
    case JT_CHICKEN2:
    case JT_CHICKEN_H:
    case JT_CHICKEN2_H:
    case JT_CHICKEN_B:
    case JT_CHICKEN2_B:
    case JT_RUNE_CHICKEN:
    case JT_RUNE_CHICKEN_H:
    case JT_RUNE_CHICKEN_B:
    case JT_ROYAL_CHICKEN:
    case JT_ROYAL_CHICKEN_H:
    case JT_ROYAL_CHICKEN_B:
    case JT_WOLF_RANGER:
    case JT_WOLF_RANGER_H:
    case JT_WOLF_RANGER_B:
    case JT_WOLF_WINDHAWK:
    case JT_DRAGON_KNIGHT_CHICKEN:
    case JT_IMPERIAL_GUARD_CHICKEN:
      mount = 2;
      break;
    default:
      mount = 0;
      break;
    }
  }

  switch (costumeJob) {
  case JT_RUNE_KNIGHT_2ND:
    costumeJob = JT_RUNE_KNIGHT_H;
    break;
  case JT_MECHANIC_2ND:
    costumeJob = JT_MECHANIC_H;
    break;
  case JT_GUILLOTINE_CROSS_2ND:
    costumeJob = JT_GUILLOTINE_CROSS_H;
    break;
  case JT_WARLOCK_2ND:
    costumeJob = JT_WARLOCK_H;
    break;
  case JT_ARCHBISHOP_2ND:
    costumeJob = JT_ARCHBISHOP_H;
    break;
  case JT_RANGER_2ND:
    costumeJob = JT_RANGER_H;
    break;
  case JT_ROYAL_GUARD_2ND:
    costumeJob = JT_ROYAL_GUARD_H;
    break;
  case JT_GENETIC_2ND:
    costumeJob = JT_GENETIC_H;
    break;
  case JT_SHADOW_CHASER_2ND:
    costumeJob = JT_SHADOW_CHASER_H;
    break;
  case JT_SORCERER_2ND:
    costumeJob = JT_SORCERER_H;
    break;
  case JT_SURA_2ND:
    costumeJob = JT_SURA_H;
    break;
  case JT_MINSTREL_2ND:
    costumeJob = JT_MINSTREL_H;
    break;
  case JT_WANDERER_2ND:
    costumeJob = JT_WANDERER_H;
    break;
  }

  if (mount == 1) {
    costumeJob = g_session.GetRidingJob(costumeJob);
  } else if (mount == 2) {
    switch (costumeJob) {
    case JT_KNIGHT:
      costumeJob = JT_CHICKEN;
      break;
    case JT_CRUSADER:
      costumeJob = JT_CHICKEN2;
      break;
    case JT_KNIGHT_H:
      costumeJob = JT_CHICKEN_H;
      break;
    case JT_CRUSADER_H:
      costumeJob = JT_CHICKEN2_H;
      break;
    case JT_KNIGHT_B:
      costumeJob = JT_CHICKEN_B;
      break;
    case JT_CRUSADER_B:
      costumeJob = JT_CHICKEN2_B;
      break;
    case JT_RUNE_KNIGHT:
      costumeJob = JT_RUNE_CHICKEN;
      break;
    case JT_RUNE_KNIGHT_H:
      costumeJob = JT_RUNE_CHICKEN_H;
      break;
    case JT_RUNE_KNIGHT_B:
      costumeJob = JT_RUNE_CHICKEN_B;
      break;
    case JT_DRAGON_KNIGHT:
      costumeJob = JT_DRAGON_KNIGHT_CHICKEN;
      break;
    case JT_ROYAL_GUARD:
      costumeJob = JT_ROYAL_CHICKEN;
      break;
    case JT_ROYAL_GUARD_H:
      costumeJob = JT_ROYAL_CHICKEN_H;
      break;
    case JT_ROYAL_GUARD_B:
      costumeJob = JT_ROYAL_CHICKEN_B;
      break;
    case JT_IMPERIAL_GUARD:
      costumeJob = JT_IMPERIAL_GUARD_CHICKEN;
      break;
    case JT_RANGER:
      costumeJob = JT_WOLF_RANGER;
      break;
    case JT_RANGER_H:
      costumeJob = JT_WOLF_RANGER_H;
      break;
    case JT_RANGER_B:
      costumeJob = JT_WOLF_RANGER_B;
      break;
    case JT_WINDHAWK:
      costumeJob = JT_WOLF_WINDHAWK;
      break;
    }
  } else {
    if (job == JT_MADOGEAR || job == JT_MADOGEAR_H || job == JT_MADOGEAR_B || job == JT_MEISTER_MADOGEAR) {
      switch (costumeJob) {
      case JT_MECHANIC:
        costumeJob = JT_MADOGEAR;
        break;
      case JT_MECHANIC_H:
        costumeJob = JT_MADOGEAR_H;
        break;
      case JT_MECHANIC_B:
        costumeJob = JT_MADOGEAR_B;
        break;
      case JT_MEISTER:
        costumeJob = JT_MEISTER_MADOGEAR;
        break;
      }
    }

    if (job == JT_WEREWOLF || job == JT_WERERAPTOR) {
      costumeJob = job;
    }
  }

  if (spriteIndex == true && costumeJob > 3950) {
    return costumeJob - 3950;
  }

  return costumeJob;
}

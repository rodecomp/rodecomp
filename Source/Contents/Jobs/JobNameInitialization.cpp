#include "Contents/Jobs/JobNames.h"
#include "Contents/Jobs/JobId.h"
#include "Script/LuaScript.h"


// OFFSET: 0x00DE67E0
// FUNCTION: fn_23495
void JobNames::Initialize(JobNameInitialization& context) {
  this->names.resize(32003, "Poring");
  const char* name = nullptr;

  const auto player = [&](int first, int end) {
    for (int id = first; id < end; ++id) {
      if (context.playerScript.Call("ReqPCJobName", "d>s", id, &name) && *name != '\0') {
        this->names[id] = name;
      }

      if (context.playerScript.Call("ReqPCJobNameMan", "d>s", id, &name) && *name != '\0') {
        this->male[id] = name;
      }

      if (context.playerScript.Call("ReqPCJobNameWoman", "d>s", id, &name) && *name != '\0') {
        this->female[id] = name;
      }
    }
  };

  player(JT_NOVICE, JT_MAX_BASIC);
  player(JT_NOVICE_H, JT_UPPER_EXTENDED_JOB2_END);
  player(JT_RUNE_KNIGHT_2ND, JT_SECOND_JOB_END);
  player(JT_DRUID, JT_2004_JOB_LAST);

  const auto other = [&](int first, int end) {
    for (int id = first; id < end; ++id) {
      context.otherScript.Call("ReqJobName", "d>s", id, &this->names[id]);
    }
  };

  other(JT_WARPNPC, JT_SAFETYWALL);
  other(JT_NEW_NPC_START, JT_MON_BEGIN);
  other(JT_SCORPION, JT_MONSTER_LAST);
  other(JT_NPC_START, JT_NPC_NAME_END);
  other(JT_EXTENDED_JOB_NAME_START, JT_EXTENDED_JOB_NAME_END);

  const char* const homunculi[] = {"LIF",   "AMISTR",   "FILIR",   "VANILMIRTH",   "LIF2",   "AMISTR2",   "FILIR2",   "VANILMIRTH2",
                                   "LIF_H", "AMISTR_H", "FILIR_H", "VANILMIRTH_H", "LIF_H2", "AMISTR_H2", "FILIR_H2", "VANILMIRTH_H2"};

  for (unsigned index = 0; index < 16; ++index) {
    this->names[JT_MER_LIF + index] = homunculi[index];
  }

  this->names[JT_MER_EIRA] = "MER_EIRA";
  this->names[JT_MER_DIETER] = "MER_DIETER";
  this->names[JT_MER_BAYERI] = "MER_BAYERI";
  this->names[JT_MER_ELEANOR] = "MER_ELEANOR";
  this->names[JT_MER_SERA] = "MER_SERA";

  for (unsigned id = JT_MER_ARCHER01; id <= JT_MER_ARCHER10; ++id) {
    this->names[id] = "여\\활용병";
  }

  for (unsigned id = JT_MER_LANCER01; id <= JT_MER_LANCER10; ++id) {
    this->names[id] = "남\\창용병";
  }

  for (unsigned id = JT_MER_SWORDMAN01; id <= JT_MER_SWORDMAN10; ++id) {
    this->names[id] = "남\\검용병";
  }

  context.sexLabels.resize(2, "F");
  context.sexLabels[0] = "F";
  context.sexLabels[1] = "M";
  context.levels20.resize(20, 0);
  context.useLevels.resize(7999, 0);
  context.otherLevels.resize(258, 0);
  context.homunculusLevels.resize(61, 0);
}

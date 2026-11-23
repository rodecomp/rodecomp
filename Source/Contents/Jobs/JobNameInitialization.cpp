#include "Contents/Jobs/JobNames.h"
#include "Client/Session/Session.h"
#include "Contents/Jobs/JobId.h"
#include "Script/LuaScript.h"

#include <cstring>
#include <list>

// OFFSET: 0x00DE67E0
// FUNCTION: fn_23495
void CSession::InitJobNameTable() {
  this->m_jobNameTable.resize(32003, "Poring");

  const char* name;

  std::list<int> jobs;
  for (int id = JT_NOVICE; id < JT_MAX_BASIC; ++id) {
    jobs.push_back(id);
  }
  for (int id = JT_NOVICE_H; id < JT_UPPER_EXTENDED_JOB2_END; ++id) {
    jobs.push_back(id);
  }
  for (int id = JT_RUNE_KNIGHT_2ND; id < JT_SECOND_JOB_END; ++id) {
    jobs.push_back(id);
  }
  for (int id = JT_DRUID; id <= JT_ALITEA_RIDING; ++id) {
    jobs.push_back(id);
  }

  name = nullptr;
  for (std::list<int>::iterator job = jobs.begin(); job != jobs.end(); ++job) {
    int id = *job;
    LuaScript* script = this->m_hotkeyScript;
    if (script->Call("ReqPCJobName", "d>s", id, &name) && std::strlen(name) != 0) {
      this->m_jobNameTable[id] = name;
    }

    if (script->Call("ReqPCJobNameMan", "d>s", id, &name) && std::strlen(name) != 0) {
      this->m_jobNameTableMan[id] = name;
    }

    if (script->Call("ReqPCJobNameWoman", "d>s", id, &name) && std::strlen(name) != 0) {
      this->m_jobNameTableWoman[id] = name;
    }
  }

  for (int id = JT_WARPNPC; id <= JT_4_M_TELEPORTER; ++id) {
    g_session.m_hotkeyScript->Call("ReqJobName", "d>s", id, &this->m_jobNameTable[id]);
  }
  for (int id = JT_NEW_NPC_START; id < JT_MON_BEGIN; ++id) {
    g_session.m_hotkeyScript->Call("ReqJobName", "d>s", id, &this->m_jobNameTable[id]);
  }
  for (int id = JT_SCORPION; id < JT_MONSTER_LAST; ++id) {
    g_session.m_hotkeyScript->Call("ReqJobName", "d>s", id, &this->m_jobNameTable[id]);
  }
  for (int id = JT_NPC_START; id < JT_NPC_NAME_END; ++id) {
    g_session.m_hotkeyScript->Call("ReqJobName", "d>s", id, &this->m_jobNameTable[id]);
  }
  for (int id = JT_EXTENDED_JOB_NAME_START; id < JT_EXTENDED_JOB_NAME_END; ++id) {
    g_session.m_hotkeyScript->Call("ReqJobName", "d>s", id, &this->m_jobNameTable[id]);
  }

  this->m_jobNameTable[JT_MER_LIF + 0] = "LIF";
  this->m_jobNameTable[JT_MER_LIF + 1] = "AMISTR";
  this->m_jobNameTable[JT_MER_LIF + 2] = "FILIR";
  this->m_jobNameTable[JT_MER_LIF + 3] = "VANILMIRTH";
  this->m_jobNameTable[JT_MER_LIF + 4] = "LIF2";
  this->m_jobNameTable[JT_MER_LIF + 5] = "AMISTR2";
  this->m_jobNameTable[JT_MER_LIF + 6] = "FILIR2";
  this->m_jobNameTable[JT_MER_LIF + 7] = "VANILMIRTH2";
  this->m_jobNameTable[JT_MER_LIF + 8] = "LIF_H";
  this->m_jobNameTable[JT_MER_LIF + 9] = "AMISTR_H";
  this->m_jobNameTable[JT_MER_LIF + 10] = "FILIR_H";
  this->m_jobNameTable[JT_MER_LIF + 11] = "VANILMIRTH_H";
  this->m_jobNameTable[JT_MER_LIF + 12] = "LIF_H2";
  this->m_jobNameTable[JT_MER_LIF + 13] = "AMISTR_H2";
  this->m_jobNameTable[JT_MER_LIF + 14] = "FILIR_H2";
  this->m_jobNameTable[JT_MER_LIF + 15] = "VANILMIRTH_H2";

  this->m_jobNameTable[JT_MER_EIRA] = "MER_EIRA";
  this->m_jobNameTable[JT_MER_DIETER] = "MER_DIETER";
  this->m_jobNameTable[JT_MER_BAYERI] = "MER_BAYERI";
  this->m_jobNameTable[JT_MER_ELEANOR] = "MER_ELEANOR";
  this->m_jobNameTable[JT_MER_SERA] = "MER_SERA";

  for (unsigned id = JT_MER_ARCHER01; id <= JT_MER_ARCHER10; ++id) {
    this->m_jobNameTable[id] = "여\\활용병";
  }

  for (unsigned id = JT_MER_LANCER01; id <= JT_MER_LANCER10; ++id) {
    this->m_jobNameTable[id] = "남\\창용병";
  }

  for (unsigned id = JT_MER_SWORDMAN01; id <= JT_MER_SWORDMAN10; ++id) {
    this->m_jobNameTable[id] = "남\\검용병";
  }

  this->m_sexLabels.resize(2, "F");
  this->m_sexLabels[0] = "F";
  this->m_sexLabels[1] = "M";
  this->extendedSkillLevels.resize(20, 0);
  this->m_skillBundle.skillValues.resize(7999, 0);
  this->m_mercSkillLevels.resize(258, 0);
  this->m_homunSkillLevels.resize(61, 0);
}

#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Monster/EnemyMonsterTalk.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

// OFFSET: 0x00E09E60
// FUNCTION: fn_23791
void CSession::GetMonsterTalkText(int job, int event, int skill, char* text, std::uint32_t aid) {
  std::sprintf(text, "");

  char name[256];
  std::strcpy(name, "enemy_monster_talk_table");
  XmlNode* node = this->m_monsterTalkTable.FindChild(DecodeMessageRot(name));
  if (node == nullptr) {
    return;
  }

  char monster[256];
  std::strcpy(monster, g_session.m_jobNameTable[job]);
  _strlwr(monster);
  node = node->FindChild(DecodeMessageRot(monster));
  if (node == nullptr) {
    return;
  }

  const char* eventName = g_session.GetMonsterTalkEventName(event);
  if (eventName == nullptr) {
    return;
  }

  strcpy_s(name, sizeof(name), eventName);
  XmlNode* entry = node->FindChild(DecodeMessageRot(name));
  if (entry == nullptr) {
    return;
  }

  BOOL bySkill = FALSE;
  switch (event) {
  case MONSTER_TALK_CAST_SKILL:
  case MONSTER_TALK_USE_SKILL:
  case MONSTER_TALK_SKILL_ATTACKED:
    bySkill = TRUE;
    break;
  }
  std::vector<const char*> talks;
  if (bySkill) {
    char skillName[16];
    _itoa(skill, skillName, 10);
    entry = entry->FindChild(DecodeMessageRot(skillName));
    while (entry != nullptr) {
      talks.push_back(DecodeMessageRot(const_cast<char*>(entry->Text().c_str())));
      entry = entry->FindSibling(DecodeMessageRot(skillName));
    }
  } else {
    do {
      talks.push_back(DecodeMessageRot(const_cast<char*>(entry->Text().c_str())));
      strcpy_s(name, sizeof(name), eventName);
      entry = entry->FindSibling(DecodeMessageRot(name));
    } while (entry != nullptr);
  }

  const int count = static_cast<int>(talks.size());
  if (count != 0) {
    char line[256];
    std::sprintf(line, talks[std::rand() % count]);
    talks.clear();
    g_session.FormatMonsterSpeech(line, text, aid, 0);
  }
}

// OFFSET: 0x00E19090
// FUNCTION: fn_23834
const char* CSession::GetMonsterTalkEventName(int event) {
  switch (event) {
  case MONSTER_TALK_ATTACK:
    return "attack";
  case MONSTER_TALK_CAST_SKILL:
    return "castskill";
  case MONSTER_TALK_USE_SKILL:
    return "useskill";
  case MONSTER_TALK_SKILL_ATTACKED:
    return "skillattacked";
  case MONSTER_TALK_HP_50:
    return "hp50";
  case MONSTER_TALK_HP_25:
    return "hp25";
  case MONSTER_TALK_DEAD:
    return "dead";
  default:
    return nullptr;
  }
}

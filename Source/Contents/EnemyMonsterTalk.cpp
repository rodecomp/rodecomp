#include "Client/Session.h"
#include "Contents/MessageTable.h"

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

  const bool bySkill = event == 1 || event == 2 || event == 3;
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
    g_session.FormatMonsterSpeech(line, text, aid, 0);
  }
}

// OFFSET: 0x00E19090
// FUNCTION: fn_23834
const char* CSession::GetMonsterTalkEventName(int event) {
  switch (event) {
  case 0:
    return "attack";
  case 1:
    return "castskill";
  case 2:
    return "useskill";
  case 3:
    return "skillattacked";
  case 4:
    return "hp50";
  case 5:
    return "hp25";
  case 6:
    return "dead";
  default:
    return nullptr;
  }
}

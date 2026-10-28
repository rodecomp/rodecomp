#include "Client/Session.h"
#include "Contents/MessageTable.h"

#include <cstdlib>
#include <cstring>
#include <vector>

// OFFSET: 0x00E0AD10
// FUNCTION: fn_23805
const char* CSession::GetPetTalkActionName(int action) {
  switch (action) {
  case 0:
    return "feeding";
  case 1:
    return "hunting";
  case 2:
    return "danger";
  case 3:
    return "dead";
  case 4:
    return "stand";
  case 10:
    return "connect";
  case 6:
    return "levelup";
  case 7:
    return "perfor_1";
  case 8:
    return "perfor_2";
  case 9:
    return "perfor_3";
  case 5:
    return "perfor_s";
  default:
    return "normal";
  }
}

// OFFSET: 0x00E12470
// FUNCTION: fn_23815
void CSession::GetPetTalkText(int job, int hungerState, int action, char* text) {
  *text = '\0';
  char name[256];
  std::strcpy(name, "monster_talk_table");
  XmlNode* node = this->m_petTalkTable.FindChild(DecodeMessageRot(name));
  if (node == nullptr) {
    return;
  }

  std::strcpy(name, g_session.GetPetTalkName(job));
  node = node->FindChild(DecodeMessageRot(name));
  if (node == nullptr) {
    return;
  }

  const char* hunger;
  switch (hungerState) {
  case 1:
    hunger = "bit_hungry";
    break;
  case 2:
    hunger = "noting";
    break;
  case 3:
    hunger = "full";
    break;
  case 4:
    hunger = "so_full";
    break;
  default:
    hunger = "hungry";
    break;
  }
  std::strcpy(name, hunger);
  node = node->FindChild(DecodeMessageRot(name));
  if (node == nullptr) {
    return;
  }

  const char* actionName = g_session.GetPetTalkActionName(action);
  std::strcpy(name, actionName);
  XmlNode* entry = node->FindChild(DecodeMessageRot(name));
  if (entry == nullptr) {
    return;
  }

  std::vector<const char*> talks;
  do {
    talks.push_back(entry->Text().c_str());
    std::strcpy(name, actionName);
    entry = entry->FindSibling(DecodeMessageRot(name));
  } while (entry != nullptr);

  const int count = static_cast<int>(talks.size());
  if (count != 0) {
    std::strcpy(text, talks[std::rand() % count]);
    DecodeMessageRot(text);
  }
}

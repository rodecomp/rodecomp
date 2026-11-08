#include "Client/Session.h"
#include "Contents/Actors/PetEnums.h"
#include "Contents/MessageTable.h"

#include <cstdlib>
#include <cstring>
#include <vector>

// OFFSET: 0x00E0AD10
// FUNCTION: fn_23805
const char* CSession::GetPetTalkActionName(int action) {
  switch (action) {
  case PM_FEEDING:
    return "feeding";
  case PM_HUNTING:
    return "hunting";
  case PM_DANGER:
    return "danger";
  case PM_DEAD:
    return "dead";
  case PM_NORMAL:
    return "stand";
  case PM_CONNENCT:
    return "connect";
  case PM_LEVELUP:
    return "levelup";
  case PM_PERFORMANCE1:
    return "perfor_1";
  case PM_PERFORMANCE2:
    return "perfor_2";
  case PM_PERFORMANCE3:
    return "perfor_3";
  case PM_PERFORMANCE_S:
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
  case PET_HUNGRY:
    hunger = "bit_hungry";
    break;
  case PET_SATISFIED:
    hunger = "noting";
    break;
  case PET_ENOUGH:
    hunger = "full";
    break;
  case PET_FULL:
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

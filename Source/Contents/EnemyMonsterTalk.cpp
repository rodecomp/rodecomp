#include "Client/Session.h"

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

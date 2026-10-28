#include "Client/Session.h"

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

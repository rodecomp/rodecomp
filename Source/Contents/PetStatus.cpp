#include "Client/Session.h"

#include <cstring>

// OFFSET: 0x00E0ADC0
// FUNCTION: fn_23806
int CSession::GetPetActionNumber(int hungerLevel, int friendlyLevel, int action) {
  int number = this->m_petActionTable[hungerLevel][friendlyLevel][action];
  if (number == -1) {
    return -1;
  }

  return number * 10 + 2;
}

// OFFSET: 0x00E0AEB0
// FUNCTION: fn_23808
int CSession::GetPetFriendlyState(int friendly) {
  if (friendly <= 100) {
    return 0;
  }
  if (friendly <= 250) {
    return 1;
  }
  if (friendly <= 750) {
    return 2;
  }
  if (friendly <= 900) {
    return 3;
  }
  return 4;
}

// OFFSET: 0x00E0AF00
// FUNCTION: fn_23809
void CSession::GetPetFriendlyStateString(int friendly, char* buffer) {
  if (friendly >= 0) {
    if (friendly <= 100) {
      std::strcpy(buffer, "서먹서먹함");
      return;
    }
    if (friendly <= 250) {
      std::strcpy(buffer, "어색함");
      return;
    }
    if (friendly <= 750) {
      std::strcpy(buffer, "보통");
      return;
    }
    if (friendly <= 900) {
      std::strcpy(buffer, "친함");
      return;
    }
    if (friendly <= 1000) {
      std::strcpy(buffer, "절친함");
      return;
    }
  }

  std::strcpy(buffer, "저는 모릅니다아아아.");
}

// OFFSET: 0x00E0AFF0
// FUNCTION: fn_23810
int CSession::GetPetHungryState(int hunger) {
  if (hunger >= 91 && hunger <= 100) {
    return 4;
  }
  if (hunger >= 76 && hunger <= 90) {
    return 3;
  }
  if (hunger >= 26 && hunger <= 75) {
    return 2;
  }
  if (hunger >= 11 && hunger <= 25) {
    return 1;
  }
  return 0;
}

// OFFSET: 0x00E12830
// FUNCTION: fn_23817
int CSession::GetPetEmotionNumber(int job, int action, int hungerLevel) {
  if (g_session.unknown_0E98 != g_session.unknown_5558) {
    g_session.unknown_0E98 = g_session.unknown_5558;
  }

  return g_session.unknown_0E98 + ((job * 10 + action) * 10 + hungerLevel) * 10;
}

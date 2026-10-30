#include "Client/Session.h"
#include "Script/LuaScript.h"
#include "Contents/CashEmotionMgr.h"
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

// OFFSET: 0x00E0B040
// FUNCTION: fn_23811
void CSession::GetPetHungryStateString(int hunger, char* buffer) {
  if (hunger >= 0) {
    if (hunger <= 10) {
      std::strcpy(buffer, "배고픔");
      return;
    }
    if (hunger <= 25) {
      std::strcpy(buffer, "출출함");
      return;
    }
    if (hunger <= 75) {
      std::strcpy(buffer, "보통");
      return;
    }
    if (hunger <= 90) {
      std::strcpy(buffer, "배부름");
      return;
    }
    if (hunger <= 100) {
      std::strcpy(buffer, "아주 배부름");
      return;
    }
  }

  const char unknown[] = {'\xB8', '\xF4', '\xB6', '\xF3', '\xBF', '\xE4', '.', '\0'};
  std::memcpy(buffer, unknown, sizeof(unknown));
}

// OFFSET: 0x00E12830
// FUNCTION: fn_23817
int CSession::GetPetEmotionNumber(int job, int action, int hungerLevel) {
  if (g_session.unknown_0E98 != g_session.unknown_5558) {
    g_session.unknown_0E98 = g_session.unknown_5558;
  }

  return g_session.unknown_0E98 + ((job * 10 + action) * 10 + hungerLevel) * 10;
}

// OFFSET: 0x00E1DF60
// FUNCTION: fn_23888
int CSession::IsPetEgg(int itemId) const {
  return static_cast<unsigned>(itemId - 9001) <= 498;
}

// OFFSET: 0x00E27780
// FUNCTION: fn_23940
bool CSession::IsPetAccessory(int itemId) {
  bool isAccessory = false;
  if (this->m_hotkeyScript->Call("IsPetAccessory", "d>b", itemId, &isAccessory)) {
    return isAccessory;
  }

  return false;
}

// OFFSET: 0x00E19BB0
// FUNCTION: fn_23847
void CSession::InitPetActionTable() {
  const int hng = g_cashEmotionMgr->FindEmotionId("ET_HNG");
  const int kik = g_cashEmotionMgr->FindEmotionId("ET_KIK");
  const int fret = g_cashEmotionMgr->FindEmotionId("ET_FRET");
  const int cry = g_cashEmotionMgr->FindEmotionId("ET_CRY");
  const int sweat = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Sweat);
  const int scratch = g_cashEmotionMgr->FindEmotionId("ET_SCRATCH");
  const int ok = g_cashEmotionMgr->FindEmotionId("ET_OK");
  const int aha = g_cashEmotionMgr->FindEmotionId("ET_AHA");
  const int smile = g_cashEmotionMgr->FindEmotionId("ET_SMILE");
  const int delight = g_cashEmotionMgr->FindEmotionId("ET_DELIGHT");
  const int thanks = g_cashEmotionMgr->FindEmotionId("ET_THANKS");
  const int profuselySweat = g_cashEmotionMgr->FindEmotionId("ET_PROFUSELY_SWEAT");
  const int throb = g_cashEmotionMgr->FindEmotionId("ET_THROB");
  const int best = g_cashEmotionMgr->FindEmotionId("ET_BEST");
  const int bigthrob = g_cashEmotionMgr->FindEmotionId("ET_BIGTHROB");
  const int stareAbout = g_cashEmotionMgr->FindEmotionId("ET_STARE_ABOUT");
  const int think = g_cashEmotionMgr->FindEmotionId("ET_THINK");
  const int huk = g_cashEmotionMgr->FindEmotionId("ET_HUK");
  const int help = g_cashEmotionMgr->FindEmotionId("ET_HELP");
  const int chup = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Chup);
  g_cashEmotionMgr->FindEmotionId("ET_MONEY");
  const int surprise = g_cashEmotionMgr->FindEmotionId("ET_SURPRISE");

  this->m_petActionTable[0][0][0] = hng;
  this->m_petActionTable[0][0][1] = hng;
  this->m_petActionTable[0][0][2] = kik;
  this->m_petActionTable[0][0][3] = kik;
  this->m_petActionTable[0][0][4] = fret;
  this->m_petActionTable[0][0][5] = huk;
  this->m_petActionTable[0][1][0] = hng;
  this->m_petActionTable[0][1][1] = hng;
  this->m_petActionTable[0][1][2] = kik;
  this->m_petActionTable[0][1][3] = kik;
  this->m_petActionTable[0][1][4] = fret;
  this->m_petActionTable[0][1][5] = fret;
  this->m_petActionTable[0][1][6] = sweat;
  this->m_petActionTable[0][2][0] = scratch;
  this->m_petActionTable[0][2][1] = hng;
  this->m_petActionTable[0][2][2] = -1;
  this->m_petActionTable[0][2][3] = hng;
  this->m_petActionTable[0][2][4] = sweat;
  this->m_petActionTable[0][2][5] = scratch;
  this->m_petActionTable[0][2][6] = think;
  this->m_petActionTable[0][3][0] = ok;
  this->m_petActionTable[0][3][1] = -1;
  this->m_petActionTable[0][3][2] = -1;
  this->m_petActionTable[0][3][3] = think;
  this->m_petActionTable[0][3][4] = scratch;
  this->m_petActionTable[0][3][5] = think;
  this->m_petActionTable[0][3][6] = aha;
  this->m_petActionTable[0][4][0] = smile;
  this->m_petActionTable[0][4][1] = -1;
  this->m_petActionTable[0][4][2] = -1;
  this->m_petActionTable[0][4][3] = scratch;
  this->m_petActionTable[0][4][4] = ok;
  this->m_petActionTable[0][4][5] = delight;
  this->m_petActionTable[0][4][6] = aha;
  this->m_petActionTable[1][0][0] = scratch;
  this->m_petActionTable[1][0][1] = hng;
  this->m_petActionTable[1][0][2] = kik;
  this->m_petActionTable[1][0][3] = kik;
  this->m_petActionTable[1][0][4] = hng;
  this->m_petActionTable[1][0][5] = hng;
  this->m_petActionTable[1][1][0] = scratch;
  this->m_petActionTable[1][1][1] = hng;
  this->m_petActionTable[1][1][2] = -1;
  this->m_petActionTable[1][1][3] = hng;
  this->m_petActionTable[1][1][4] = kik;
  this->m_petActionTable[1][1][5] = hng;
  this->m_petActionTable[1][1][6] = hng;
  this->m_petActionTable[1][2][0] = ok;
  this->m_petActionTable[1][2][1] = -1;
  this->m_petActionTable[1][2][2] = -1;
  this->m_petActionTable[1][2][3] = think;
  this->m_petActionTable[1][2][4] = scratch;
  this->m_petActionTable[1][2][5] = think;
  this->m_petActionTable[1][2][6] = aha;
  this->m_petActionTable[1][3][0] = smile;
  this->m_petActionTable[1][3][1] = -1;
  this->m_petActionTable[1][3][2] = -1;
  this->m_petActionTable[1][3][3] = scratch;
  this->m_petActionTable[1][3][4] = ok;
  this->m_petActionTable[1][3][5] = delight;
  this->m_petActionTable[1][3][6] = aha;
  this->m_petActionTable[1][4][0] = thanks;
  this->m_petActionTable[1][4][1] = -1;
  this->m_petActionTable[1][4][2] = -1;
  this->m_petActionTable[1][4][3] = profuselySweat;
  this->m_petActionTable[1][4][4] = throb;
  this->m_petActionTable[1][4][5] = delight;
  this->m_petActionTable[1][4][6] = best;
  this->m_petActionTable[2][0][0] = hng;
  this->m_petActionTable[2][0][1] = hng;
  this->m_petActionTable[2][0][2] = -1;
  this->m_petActionTable[2][0][3] = hng;
  this->m_petActionTable[2][0][4] = kik;
  this->m_petActionTable[2][0][5] = sweat;
  this->m_petActionTable[2][1][0] = scratch;
  this->m_petActionTable[2][1][1] = -1;
  this->m_petActionTable[2][1][2] = -1;
  this->m_petActionTable[2][1][3] = think;
  this->m_petActionTable[2][1][4] = think;
  this->m_petActionTable[2][1][5] = think;
  this->m_petActionTable[2][1][6] = scratch;
  this->m_petActionTable[2][2][0] = ok;
  this->m_petActionTable[2][2][1] = -1;
  this->m_petActionTable[2][2][2] = -1;
  this->m_petActionTable[2][2][3] = scratch;
  this->m_petActionTable[2][2][4] = ok;
  this->m_petActionTable[2][2][5] = delight;
  this->m_petActionTable[2][2][6] = aha;
  this->m_petActionTable[2][3][0] = smile;
  this->m_petActionTable[2][3][1] = -1;
  this->m_petActionTable[2][3][2] = -1;
  this->m_petActionTable[2][3][3] = profuselySweat;
  this->m_petActionTable[2][3][4] = smile;
  this->m_petActionTable[2][3][5] = delight;
  this->m_petActionTable[2][3][6] = best;
  this->m_petActionTable[2][4][0] = thanks;
  this->m_petActionTable[2][4][1] = best;
  this->m_petActionTable[2][4][2] = -1;
  this->m_petActionTable[2][4][3] = huk;
  this->m_petActionTable[2][4][4] = bigthrob;
  this->m_petActionTable[2][4][5] = ok;
  this->m_petActionTable[2][4][6] = best;
  this->m_petActionTable[3][0][0] = hng;
  this->m_petActionTable[3][0][1] = -1;
  this->m_petActionTable[3][0][2] = -1;
  this->m_petActionTable[3][0][3] = think;
  this->m_petActionTable[3][0][4] = think;
  this->m_petActionTable[3][0][5] = think;
  this->m_petActionTable[3][1][0] = hng;
  this->m_petActionTable[3][1][1] = -1;
  this->m_petActionTable[3][1][2] = -1;
  this->m_petActionTable[3][1][3] = scratch;
  this->m_petActionTable[3][1][4] = scratch;
  this->m_petActionTable[3][1][5] = scratch;
  this->m_petActionTable[3][1][6] = think;
  this->m_petActionTable[3][2][0] = scratch;
  this->m_petActionTable[3][2][1] = -1;
  this->m_petActionTable[3][2][2] = -1;
  this->m_petActionTable[3][2][3] = profuselySweat;
  this->m_petActionTable[3][2][4] = best;
  this->m_petActionTable[3][2][5] = delight;
  this->m_petActionTable[3][2][6] = aha;
  this->m_petActionTable[3][3][0] = ok;
  this->m_petActionTable[3][3][1] = best;
  this->m_petActionTable[3][3][2] = profuselySweat;
  this->m_petActionTable[3][3][3] = huk;
  this->m_petActionTable[3][3][4] = throb;
  this->m_petActionTable[3][3][5] = ok;
  this->m_petActionTable[3][3][6] = best;
  this->m_petActionTable[3][4][0] = smile;
  this->m_petActionTable[3][4][1] = best;
  this->m_petActionTable[3][4][2] = help;
  this->m_petActionTable[3][4][3] = cry;
  this->m_petActionTable[3][4][4] = chup;
  this->m_petActionTable[3][4][5] = stareAbout;
  this->m_petActionTable[3][4][6] = smile;
  this->m_petActionTable[4][0][0] = huk;
  this->m_petActionTable[4][0][1] = -1;
  this->m_petActionTable[4][0][2] = -1;
  this->m_petActionTable[4][0][3] = scratch;
  this->m_petActionTable[4][0][4] = scratch;
  this->m_petActionTable[4][0][5] = scratch;
  this->m_petActionTable[4][1][0] = hng;
  this->m_petActionTable[4][1][1] = -1;
  this->m_petActionTable[4][1][2] = -1;
  this->m_petActionTable[4][1][3] = profuselySweat;
  this->m_petActionTable[4][1][4] = scratch;
  this->m_petActionTable[4][1][5] = surprise;
  this->m_petActionTable[4][1][6] = surprise;
  this->m_petActionTable[4][2][0] = hng;
  this->m_petActionTable[4][2][1] = best;
  this->m_petActionTable[4][2][2] = profuselySweat;
  this->m_petActionTable[4][2][3] = huk;
  this->m_petActionTable[4][2][4] = smile;
  this->m_petActionTable[4][2][5] = ok;
  this->m_petActionTable[4][2][6] = best;
  this->m_petActionTable[4][3][0] = scratch;
  this->m_petActionTable[4][3][1] = best;
  this->m_petActionTable[4][3][2] = help;
  this->m_petActionTable[4][3][3] = cry;
  this->m_petActionTable[4][3][4] = throb;
  this->m_petActionTable[4][3][5] = smile;
  this->m_petActionTable[4][3][6] = smile;
  this->m_petActionTable[4][4][0] = scratch;
  this->m_petActionTable[4][4][1] = best;
  this->m_petActionTable[4][4][2] = help;
  this->m_petActionTable[4][4][3] = cry;
  this->m_petActionTable[4][4][4] = chup;
  this->m_petActionTable[4][4][5] = smile;
  this->m_petActionTable[4][4][6] = chup;
}

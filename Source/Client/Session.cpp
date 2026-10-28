#include "Client/Session.h"
#include "Contents/Jobs/JobId.h"
#include "Network/TextEncoding.h"

#include <cstring>
#include <windows.h>

// STATIC: 0x01732638
// GLOBAL: gl_24474
CSession g_session;

// OFFSET: 0x00DDE860
// FUNCTION: fn_23401
int CSession::MercSkillLevel(int skillId) const {
  const int index = skillId - 8200;
  if (index > -1) {
    return this->m_mercSkillLevels[index];
  }

  return 0;
}

// OFFSET: 0x00E13B00
// FUNCTION: fn_23826
int CSession::SyncUnknown0E98() {
  if (this->unknown_0E98 != this->unknown_5558) {
    this->unknown_0E98 = this->unknown_5558;
  }

  return this->unknown_0E98;
}

// OFFSET: 0x00DFFCF0
// FUNCTION: fn_23633
void CSession::ClearItemSelection() {
  for (ItemInfo& item : this->m_inventoryItems) {
    if (item.unknown_0074[1] == 1) {
      item.unknown_0074[1] = 0;
    }
  }
}

// OFFSET: 0x00E3CFF0
// FUNCTION: fn_24179
bool CSession::IsDoramJob(int job) const {
  switch (job) {
  case JT_TRIBE_DORAM:
  case JT_DO_SUMMONER:
  case JT_DO_CART_SUMMONER:
  case JT_DO_SUMMONER_B:
  case JT_DO_CART_SUMMONER_B:
  case JT_SPIRIT_HANDLER:
  case JT_SPIRIT_HANDLER_RIDING:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00DFF1F0
// FUNCTION: fn_23614
char* CSession::ConvertTooltipText(const char* source, char* output) const {
  if (output != source) {
    std::strcpy(output, source);
  }

  for (char* current = output; *current != '\0'; current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0)) {
    if (*current == '_') {
      *current = ' ';
    }
  }

  return output;
}

#include "Client/Session.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <timeapi.h>
#include <windows.h>

// OFFSET: 0x00DE0A50
// FUNCTION: fn_23466
std::uint32_t CSession::GetServerTime() {
  return timeGetTime() - this->m_serverTimeOffset - 72;
}

// OFFSET: 0x00DFEF40
// FUNCTION: fn_23605
std::uint32_t CSession::AddPingSample(std::uint32_t sample) {
  this->m_pingSamples.push_back(sample);

  while (this->m_pingSamples.size() > 3) {
    this->m_pingSamples.pop_front();
  }

  std::uint32_t sum = 0;
  for (std::uint32_t value : this->m_pingSamples) {
    sum += value;
  }

  this->m_averagePing = sum / this->m_pingSamples.size();
  return this->m_averagePing;
}

// OFFSET: 0x00E00130
// FUNCTION: fn_23637
std::uint32_t CSession::SetServerTime(std::uint32_t serverTime) {
  const std::uint32_t offset = timeGetTime() - serverTime;
  this->m_serverTimeOffset = offset;
  this->m_serverTimeAdjustCount = 0;
  return offset;
}

// OFFSET: 0x00E00160
// FUNCTION: fn_23638
void CSession::SetHotkeyDescription(const char* description, int index) {
  for (int slot = 9 * this->m_hotkeyPage; slot < 9 * (this->m_hotkeyPage + 1); ++slot) {
    if (this->m_hotkeyDescriptions[slot] == description) {
      this->m_hotkeyDescriptions[slot] = "nothing";
    }
  }

  if (description[0] == '\0') {
    this->m_hotkeyDescriptions[index] = "nothing";
  } else {
    this->m_hotkeyDescriptions[index] = description;
  }
}

// OFFSET: 0x00E00320

// OFFSET: 0x00E003D0

// OFFSET: 0x00E004D0

// OFFSET: 0x00E004F0

// OFFSET: 0x00E006E0

// OFFSET: 0x00E010B0

// OFFSET: 0x00E01670

// OFFSET: 0x00E016A0

// OFFSET: 0x00E016D0

// OFFSET: 0x00E01700

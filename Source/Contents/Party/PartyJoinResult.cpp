#include "Client/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Party/PartyJoinResult.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <cstring>

// OFFSET: 0x00D3B9E0
// FUNCTION: fn_21788
void ReceivePartyJoinResult(const void* packet) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  std::uint32_t status;
  std::memcpy(&status, bytes + 54, sizeof(status));
  const auto message = [&](int id) { return LookupMessage(id); };
  const auto notify = [&](int id) { g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message(id)), 0xFF, 0, 0, 0); };
  const auto format = [&](int id, unsigned offset, unsigned color) {
    char text[500] = {};
    _snprintf_s(text, sizeof(text), _TRUNCATE, message(id), reinterpret_cast<const char*>(bytes + offset));
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), color, 0, 0, 0);
  };

  switch (status) {
  case 0:
    format(g_session.unknown_5464 == 1 ? 3501 : 3498, g_session.unknown_5464 == 1 ? 2 : 26, 0xFFFF64);
    break;
  case 1:
    format(g_session.unknown_5464 == 1 ? 3502 : 3499, g_session.unknown_5464 == 1 ? 2 : 26, 0xFF);
    break;
  case 2:
    notify(3497);
    break;
  case 3:
    notify(3442);
    break;
  case 4:
    notify(3496);
    break;
  case 5:
  case 6:
    notify(3492);
    break;
  case 7:
    format(3500, 2, 0xFF);
    break;
  case 8:
    format(3503, 2, 0xFF);
    break;
  case 9: {
    notify(3494);
    std::uint32_t master;
    std::memcpy(&master, bytes + 50, sizeof(master));
    g_adventurerAgencyMgr->RequestStop(master);
    break;
  }
  case 10:
    notify(3443);
    break;
  case 11:
    notify(3495);
    break;
  case 12:
    format(608, 2, 0xFF);
    break;
  case 13:
    notify(1816);
    break;
  default:
    break;
  }
}

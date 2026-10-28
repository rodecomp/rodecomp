#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/AgencyRequestContext.h"
#include "Contents/MessageTable.h"
#include "Contents/Party/PartyJoinResult.h"
#include "Contents/Party/StopRecruitingAsyncWork.h"
#include "UI/Window/WindowNotifications.h"

#include <cstdio>
#include <cstring>

// OFFSET: 0x00D3B9E0
// FUNCTION: fn_21788
void ReceivePartyJoinResult(const void* packet, PartyJoinResultContext& context) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  context.notifications.Notify(3, nullptr, 0);
  std::uint32_t status;
  std::memcpy(&status, bytes + 54, sizeof(status));
  const auto message = [&](int id) { return LookupMessage(id); };
  const auto notify = [&](int id) { context.notifications.Notify(1, message(id), 0xFF); };
  const auto format = [&](int id, unsigned offset, unsigned color) {
    char text[500] = {};
    _snprintf_s(text, sizeof(text), _TRUNCATE, message(id), reinterpret_cast<const char*>(bytes + offset));
    context.notifications.Notify(1, text, color);
  };

  switch (status) {
  case 0:
    format(context.partyMember == 1 ? 3501 : 3498, context.partyMember == 1 ? 2 : 26, 0xFFFF64);
    break;
  case 1:
    format(context.partyMember == 1 ? 3502 : 3499, context.partyMember == 1 ? 2 : 26, 0xFF);
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
    g_adventurerAgencyMgr->RequestStop(master, context.requests, context.stop);
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

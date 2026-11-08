#include "UI/Window/UIEnums.h"
#include "Client/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Party/PartyJoinResult.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstring>

// OFFSET: 0x00D3B9E0
// FUNCTION: fn_21788
void ReceivePartyJoinResult(const void* packet) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
  std::uint32_t status;
  std::memcpy(&status, bytes + 54, sizeof(status));

  switch (status) {
  case 5:
  case 6:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3492)), 0xFF, 0, 0, 0);
    break;
  case 9: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3494)), 0xFF, 0, 0, 0);
    std::uint32_t master;
    std::memcpy(&master, bytes + 50, sizeof(master));
    g_adventurerAgencyMgr->RequestStop(master);
    break;
  }
  case 10:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3443)), 0xFF, 0, 0, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3495)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3496)), 0xFF, 0, 0, 0);
    break;
  case 8: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3503), reinterpret_cast<const char*>(bytes + 2));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3442)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(3497)), 0xFF, 0, 0, 0);
    break;
  case 0: {
    char text[500] = {};

    if (g_session.unknown_5464 == 1) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3501), reinterpret_cast<const char*>(bytes + 2));
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3498), reinterpret_cast<const char*>(bytes + 26));
    }

    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF64, 0, 0, 0);
    break;
  }
  case 1: {
    char text[500] = {};

    if (g_session.unknown_5464 == 1) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3502), reinterpret_cast<const char*>(bytes + 2));
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3499), reinterpret_cast<const char*>(bytes + 26));
    }

    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 7: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(3500), reinterpret_cast<const char*>(bytes + 2));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 13:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(1816)), 0xFF, 0, 0, 0);
    break;
  case 12: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(608), reinterpret_cast<const char*>(bytes + 2));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  default:
    break;
  }
}

#include "UI/Window/UIEnums.h"
#include "Client/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Party/PartyJoinResult.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
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
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_JOIN_PARTY_BECAUSE_LOW_LEVEL)), 0xFF, 0, 0, 0);
    break;
  case 9: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXCEEDED_NUMBER_OF_PARTYMEMBERS)), 0xFF, 0, 0, 0);
    std::uint32_t master;
    std::memcpy(&master, bytes + 50, sizeof(master));
    g_adventurerAgencyMgr->RequestStop(master);
    break;
  }
  case 10:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_QUESTWND_HIGHER)), 0xFF, 0, 0, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_JOIN_ACCEPT)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_JOIN_REFUSE)), 0xFF, 0, 0, 0);
    break;
  case 8: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_NOT_PARTYMASTER_CANNOT_REGISTER_ADVENTURER_AGENCY), reinterpret_cast<const char*>(bytes + 2));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_WEB_ADDRESS_FOR_RESTART)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_ALREADY_JOINED_REQUEST_USER)), 0xFF, 0, 0, 0);
    break;
  case 0: {
    char text[500] = {};

    if (g_session.partyLeader == 1) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_ADVENTURER_AGENCY_LIST), reinterpret_cast<const char*>(bytes + 2));
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_JOIN_ACCEPT_REQUEST_USER), reinterpret_cast<const char*>(bytes + 26));
    }

    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF64, 0, 0, 0);
    break;
  }
  case 1: {
    char text[500] = {};

    if (g_session.partyLeader == 1) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_REQUEST_JOIN_PARTY), reinterpret_cast<const char*>(bytes + 2));
    } else {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_JOIN_REFUSE_REQUEST_USER), reinterpret_cast<const char*>(bytes + 26));
    }

    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 7: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_REQUEST_REJECTED_BECAUSE_INVALID_MAPPROPERTY), reinterpret_cast<const char*>(bytes + 2));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 13:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RESULT_FAIL_UNKNOWN)), 0xFF, 0, 0, 0);
    break;
  case 12: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_ALREADY_SAME_AID_JOINED), reinterpret_cast<const char*>(bytes + 2));
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  default:
    break;
  }
}

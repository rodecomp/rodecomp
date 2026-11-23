#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Party/PartyJoinResult.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)
struct PACKET_ZC_PARTY_JOIN_REQ_ACK_FROM_MASTER {
  /* 0000 */ std::uint16_t packetType;
  /* 0002 */ char partyName[24];
  /* 001A */ char playerName[24];
  /* 0032 */ std::uint32_t masterAid;
  /* 0036 */ std::uint32_t status;
};
#pragma pack(pop)

static_assert(sizeof(PACKET_ZC_PARTY_JOIN_REQ_ACK_FROM_MASTER) == 58);
static_assert(offsetof(PACKET_ZC_PARTY_JOIN_REQ_ACK_FROM_MASTER, masterAid) == 50);
static_assert(offsetof(PACKET_ZC_PARTY_JOIN_REQ_ACK_FROM_MASTER, status) == 54);

// OFFSET: 0x00D3B9E0
// FUNCTION: fn_21788
void ReceivePartyJoinResult(const void* packet) {
  const auto* ack = static_cast<const PACKET_ZC_PARTY_JOIN_REQ_ACK_FROM_MASTER*>(packet);
  g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);

  switch (ack->status) {
  case 5:
  case 6:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_FIND_REQUEST_USER)), 0xFF, 0, 0, 0);
    break;
  case 9: {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_FIND_PARTYMASTER)), 0xFF, 0, 0, 0);
    g_adventurerAgencyMgr->RequestStop(ack->masterAid);
    break;
  }
  case 10:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NO_PARTY_FOUND)), 0xFF, 0, 0, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_CANNOT_JOIN_PARTY_BECAUSE_LOW_LEVEL)), 0xFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INVALID_MAPPROPERTY_REQUEST_USER)), 0xFF, 0, 0, 0);
    break;
  case 8: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_REQUEST_REJECTED_BECAUSE_INVALID_MAPPROPERTY), ack->partyName);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 3:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_THE_PARTY_MASTER_CANNOT_BE_REQUESTED)), 0xFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EXCEEDED_NUMBER_OF_PARTYMEMBERS)), 0xFF, 0, 0, 0);
    break;
  case 0: {
    char text[500] = {};

    if (g_session.partyLeader == 1) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_JOIN_ACCEPT_REQUEST_USER), ack->partyName);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF64, 0, 0, 0);
      return;
    }

    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_JOIN_ACCEPT), ack->playerName);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFFFF64, 0, 0, 0);
    break;
  }
  case 1: {
    char text[500] = {};

    if (g_session.partyLeader == 1) {
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_JOIN_REFUSE_REQUEST_USER), ack->partyName);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      return;
    }

    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_JOIN_REFUSE), ack->playerName);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 7: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_ALREADY_JOINED_REQUEST_USER), ack->partyName);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  case 13:
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RESULT_FAIL_UNKNOWN)), 0xFF, 0, 0, 0);
    break;
  case 12: {
    char text[500] = {};
    StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_ALREADY_SAME_AID_JOINED), ack->partyName);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    break;
  }
  default:
    break;
  }
}

#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Contents/World/World.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

// OFFSET: 0x00D230E0
// FUNCTION: fn_21660
void CGameMode::ReceiveColoredNotice(const unsigned char* packet) {
  std::string text;
  text.assign(reinterpret_cast<const char*>(packet + 9), *reinterpret_cast<const short*>(packet + 2) - 9);
  if (!text.empty()) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), *reinterpret_cast<const std::uint32_t*>(packet + 5), packet[4], 0, 0);
  }
}

// OFFSET: 0x00D24430
// FUNCTION: fn_21666
void CGameMode::ReceiveActorText(const unsigned char* packet) {
  char text[81];
  std::strncpy(text, reinterpret_cast<const char*>(packet + 6), 80);
  text[80] = '\0';
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    const std::int64_t value = reinterpret_cast<std::intptr_t>(text);
    actor->SendMsg(nullptr, 141, 0, static_cast<int>(value), static_cast<int>(value >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D25920
// FUNCTION: fn_21674
void CGameMode::ReceiveRentalItemTime(const unsigned char* packet) {
  char number[12];
  char text[260];
  _itoa(*reinterpret_cast<const int*>(packet + 2), number, 10);
  const ITEM_DESC& description = FindItemDescription(std::atoi(number));
  const std::uint32_t seconds = *reinterpret_cast<const std::uint32_t*>(packet + 6);
  const char* name = description.identifiedName;
  const int minutes = static_cast<int>(seconds / 60);
  if (minutes > 0) {
    std::sprintf(text, LookupMessage(1256), name, minutes);
  } else {
    std::sprintf(text, LookupMessage(1270), name, seconds);
  }
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  if (minutes == 1) {
    std::sprintf(text, LookupMessage(1257), name);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
    g_windowMgr.SendNotice(19, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D26600
// FUNCTION: fn_21680
void CGameMode::ReceiveActorSequenceDeadline(const unsigned char* packet) {
  g_session.InsertPair(*reinterpret_cast<const int*>(packet + 2), *reinterpret_cast<const int*>(packet + 6));
  const std::uint32_t aid = *reinterpret_cast<const std::uint32_t*>(packet + 2);
  if (aid == g_session.m_aid) {
    this->scene->player->SetSequenceDeadline(*reinterpret_cast<const std::uint32_t*>(packet + 6));
    return;
  }
  if (CRenderObject* actor = this->scene->GetGameActorByAID(aid)) {
    actor->SetSequenceDeadline(*reinterpret_cast<const std::uint32_t*>(packet + 6));
  }
}

// OFFSET: 0x00D27060
// FUNCTION: fn_21686
void CGameMode::ReceiveActorSignal(const unsigned char* packet) {
  if (CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2))) {
    actor->SendMsg(nullptr, 142, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D2AF90
// FUNCTION: fn_21707
void CGameMode::IgnorePacket(const unsigned char*) {
}

// OFFSET: 0x00D2C2B0
// FUNCTION: fn_21715
void CGameMode::ReceiveHuntingList(const unsigned char* packet) {
  char text[256];
  int size = *reinterpret_cast<const short*>(packet + 2) - 4;
  if (size > 5000) {
    size = 5000;
  }
  char* list = new char[size + 1];
  std::memcpy(list, packet + 4, size);
  list[size] = '\0';
  std::sprintf(text, "=========== HUNTING LIST =============");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(list), 0xCDFFCD, 0, 0, 0);
  std::sprintf(text, "========================================");
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0x9BCD9B, 0, 0, 0);
}

// OFFSET: 0x00D2F150
// FUNCTION: fn_21728
bool CGameMode::NotifyItemResult(int result) {
  int message;
  switch (result) {
  case 1:
  case 6:
    message = 53;
    break;
  case 2:
    message = 52;
    break;
  case 4:
    message = 220;
    break;
  case 5:
    message = 279;
    break;
  case 7:
    message = 1418;
    break;
  case 9:
    message = 3984;
    break;
  default:
    return false;
  }
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0xFF, 0, 0, 0);
  return true;
}

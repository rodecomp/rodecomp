#include "Contents/EnchantMgr.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

namespace {
#pragma pack(push, 1)
struct CloseEnchantPacket {
  /* 0000 */ int16_t packetType;
};

struct RandomEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
};

struct PerfectEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
  /* 000C */ int32_t itemId;
};

struct UpgradeEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
  /* 000C */ int16_t slot;
};

struct UpgradePerfectEnchantPacket {
  /* 0000 */ int16_t packetType;
  /* 0002 */ uint64_t group;
  /* 000A */ int16_t index;
  /* 000C */ int16_t slot;
  /* 000E */ int32_t itemId;
};
#pragma pack(pop)
}

// OFFSET: 0x0067FF60
// FUNCTION: fn_05523
CEnchantMgr::CEnchantMgr() {
  m_request.packetType = 0;
  m_request.group = 0;
  m_request.index = -1;
  m_request.slot = -1;
  m_unknown_0030 = true;
}

// OFFSET: 0x00682960
// FUNCTION: fn_05540
void CEnchantMgr::SendCloseEnchant() {
  CloseEnchantPacket packet;
  packet.packetType = 0xBA0;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_unknown_0030 = true;
}

// OFFSET: 0x00683C20
// FUNCTION: fn_05545
void CEnchantMgr::RequestPerfectEnchant(uint64_t group, int16_t index, int16_t slot, int32_t itemId) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  PerfectEnchantPacket packet;
  packet.packetType = 0xB9C;
  packet.group = group;
  packet.index = index;
  packet.itemId = itemId;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9C, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683D10
// FUNCTION: fn_05546
void CEnchantMgr::RequestRandomEnchant(uint64_t group, int16_t index, int16_t slot) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  RandomEnchantPacket packet;
  packet.packetType = 0xB9B;
  packet.group = group;
  packet.index = index;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9B, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683E00
// FUNCTION: fn_05547
void CEnchantMgr::RequestResetEnchant(uint64_t group, int16_t index) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  RandomEnchantPacket packet;
  packet.packetType = 0xB9E;
  packet.group = group;
  packet.index = index;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9E, group, index, -1};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683EF0
// FUNCTION: fn_05548
void CEnchantMgr::RequestUpgradePerfectEnchant(uint64_t group, int16_t index, int16_t slot, int32_t itemId) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  UpgradePerfectEnchantPacket packet;
  packet.packetType = 0xBF1;
  packet.group = group;
  packet.index = index;
  packet.slot = slot;
  packet.itemId = itemId;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xBF1, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x00683FF0
// FUNCTION: fn_05549
void CEnchantMgr::RequestUpgradeEnchant(uint64_t group, int16_t index, int16_t slot) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  UpgradeEnchantPacket packet;
  packet.packetType = 0xB9D;
  packet.group = group;
  packet.index = index;
  packet.slot = slot;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xB9D, group, index, slot};
  m_unknown_0030 = true;
}

// OFFSET: 0x006840E0
// FUNCTION: fn_05550
void CEnchantMgr::RequestUpgradeRandomEnchant(uint64_t group, int16_t index, int16_t slot) {
  if (g_session.m_weight * 100 / g_session.m_maxWeight >= 70) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3837)), 0xFF, 0, 0, 0);
    return;
  }

  UpgradeEnchantPacket packet;
  packet.packetType = 0xBF0;
  packet.group = group;
  packet.index = index;
  packet.slot = slot;
  CRagConnection& connection = GetRagConnection();
  connection.Send(g_packetLengths.FixedLength(packet.packetType), &packet);
  m_request = EnchantRequest{0xBF0, group, index, slot};
  m_unknown_0030 = true;
}

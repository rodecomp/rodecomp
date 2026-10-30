#include "Contents/EntryQueueMgr.h"

#include "Contents/MessageTable.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"

#include <cstring>

struct EntryQueueAdmissionPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint8_t result;
  /* 0003 */ char queueName[24];
  /* 001B */ char characterName[24];
};

struct EntryQueueApplyPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ char queueName[24];
};

struct EntryQueueNamePacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ char queueName[24];
};

// GLOBAL: gl_24868
CEntryQueueMgr* g_entryQueueMgr;
// GLOBAL: gl_24869
std::map<int, EntryQueueInfo*> g_entryQueueList;

CEntryQueueMgr::~CEntryQueueMgr() {
  for (std::map<int, EntryQueueInfo*>::iterator it = g_entryQueueList.begin(); it != g_entryQueueList.end(); ++it) {
    if (it->second) {
      delete it->second;
    }
  }
}

// OFFSET: 0x00BA1F80
// FUNCTION: fn_19660
void CEntryQueueMgr::SendAdmissionReply(int result) {
  if (this->current) {
    EntryQueueAdmissionPacket packet;
    packet.id = 0x8E0;
    strcpy_s(packet.queueName, sizeof(packet.queueName), this->current->name);
    strcpy_s(packet.characterName, sizeof(packet.characterName), this->characterName.text);
    packet.result = static_cast<std::uint8_t>(result);
    if (result == 2) {
      this->ready = 0;
    }

    GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
  } else if (result == 2) {
    this->ready = 0;
  }
}

// OFFSET: 0x00BA2030
// FUNCTION: fn_19661
void CEntryQueueMgr::SendApply(std::uint16_t type, const char* name) {
  EntryQueueApplyPacket packet;
  packet.id = 0x8D7;
  packet.type = type;
  strncpy_s(packet.queueName, sizeof(packet.queueName), name, 23);
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00BA20A0
// FUNCTION: fn_19662
void CEntryQueueMgr::SendCancel() {
  if (this->current) {
    EntryQueueNamePacket packet;
    packet.id = 0x8DA;
    strcpy_s(packet.queueName, sizeof(packet.queueName), this->current->name);
    GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
  }
}

// OFFSET: 0x00BA2110
// FUNCTION: fn_19663
void CEntryQueueMgr::SendRankingRequest() {
  if (this->current) {
    EntryQueueNamePacket packet;
    packet.id = 0x90A;
    strcpy_s(packet.queueName, sizeof(packet.queueName), this->current->name);
    GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
  }
}

// OFFSET: 0x00BA2180
// FUNCTION: fn_19664
void DestroyEntryQueueMgr() {
  if (g_entryQueueMgr) {
    delete g_entryQueueMgr;
    g_entryQueueMgr = nullptr;
  }
}

// OFFSET: 0x00BA2250
// FUNCTION: fn_19665
void CEntryQueueMgr::SetQueueName(const EntryQueueName& name) {
  this->queueName = name;
}

// OFFSET: 0x00BA2270
// FUNCTION: fn_19666
void CEntryQueueMgr::SetCharacterName(const EntryQueueName& name) {
  this->characterName = name;
}

// OFFSET: 0x00BA28B0
// FUNCTION: fn_19669
void CEntryQueueMgr::ResetCurrent(int unused) {
  this->current = nullptr;
  this->ready = 0;
}

// OFFSET: 0x00BA28C0
// FUNCTION: fn_19670
void CEntryQueueMgr::IgnorePacket(const unsigned char* packet) {
}

// OFFSET: 0x00BA2E50
// FUNCTION: fn_19673
void CEntryQueueMgr::OnResultNotice(const EntryQueueNotice* notice) {
  if (notice->result == 3) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x83F), 0, false, false, 0, "Message", 0x118, 0x78, 0);
  } else if (notice->result == 11) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0x840), 0, false, false, 0, "Message", 0x118, 0x78, 0);
  }
}

// OFFSET: 0x00BA2EB0
// FUNCTION: fn_19674
void CEntryQueueMgr::OnNotify(const EntryQueueNotice* notice) {
  if (notice->result == 1) {
    g_soundMgr->PlayWave("se_btg_forward.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

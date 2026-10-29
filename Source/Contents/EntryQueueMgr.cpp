#include "Contents/EntryQueueMgr.h"

#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"

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

// OFFSET: 0x00BA2030

// OFFSET: 0x00BA20A0

// OFFSET: 0x00BA2110

// OFFSET: 0x00BA2180

// OFFSET: 0x00BA2250

// OFFSET: 0x00BA2270

// OFFSET: 0x00BA28B0
// FUNCTION: fn_19669
void CEntryQueueMgr::ResetCurrent(int unused) {
  this->current = nullptr;
  this->ready = 0;
}

// OFFSET: 0x00BA2EB0

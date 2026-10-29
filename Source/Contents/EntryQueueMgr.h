#ifndef RAGNAROK_SOURCE_CONTENTS_ENTRYQUEUEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ENTRYQUEUEMGR_H

#include <cstddef>
#include <cstdint>
#include <map>

struct EntryQueueName {
  /* 0000 */ char text[24];
};

struct EntryQueueInfo {
  /* 0000 */ int id;
  /* 0004 */ char name[24];
  /* 001C */ unsigned char unknown_001C[0xEC];
  /* 0108 */ char unknown_0108[0x630];
};

struct EntryQueueNotice {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint8_t result;
};

class CEntryQueueMgr {
public:
  virtual ~CEntryQueueMgr();

  void SendAdmissionReply(int result);
  void SendApply(std::uint16_t type, const char* name);
  void SendCancel();
  void SendRankingRequest();
  void SetQueueName(const EntryQueueName& name);
  void SetCharacterName(const EntryQueueName& name);
  void ResetCurrent(int unused);
  void OnNotify(const EntryQueueNotice* notice);

  /* 0004 */ EntryQueueName queueName;
  /* 001C */ EntryQueueName characterName;
  /* 0034 */ EntryQueueInfo* current;
  /* 0038 */ unsigned char ready;
};

extern CEntryQueueMgr* g_entryQueueMgr;
extern std::map<int, EntryQueueInfo*> g_entryQueueList;

void DestroyEntryQueueMgr();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EntryQueueInfo) == 0x738);
static_assert(offsetof(CEntryQueueMgr, characterName) == 0x1C);
static_assert(offsetof(CEntryQueueMgr, current) == 0x34);
static_assert(sizeof(CEntryQueueMgr) == 0x3C);
#endif

#endif

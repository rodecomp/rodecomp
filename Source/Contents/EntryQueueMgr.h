#ifndef RAGNAROK_SOURCE_CONTENTS_ENTRYQUEUEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ENTRYQUEUEMGR_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <map>

struct EntryQueueName {
  /* 0000 */ char text[24];
};

struct EntryQueueInfo {
  EntryQueueInfo() {
    std::memset(this, 0, sizeof(EntryQueueInfo));
  }

  /* 0000 */ int id;
  /* 0004 */ char name[0x104];
  /* 0108 */ char displayName[0x104];
  /* 020C */ int unknown_020C;
  /* 0210 */ int unknown_0210;
  /* 0214 */ unsigned char unknown_0214;
  /* 0215 */ unsigned char unknown_0215;
  /* 0216 */ unsigned char unknown_0216;
  /* 0217 */ unsigned char unknown_0217;
  /* 0218 */ unsigned char unknown_0218;
  /* 021C */ int unknown_021C;
  /* 0220 */ int unknown_0220;
  /* 0224 */ char unknown_0224[0x104];
  /* 0328 */ char unknown_0328[0x104];
  /* 042C */ char unknown_042C[0x104];
  /* 0530 */ char unknown_0530[0x104];
  /* 0634 */ char unknown_0634[0x104];
};

struct EntryQueueNotice {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint8_t result;
};

#pragma pack(push, 1)
struct EntryQueueApplyResult {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint8_t result;
  /* 0003 */ char queueName[24];
};

struct EntryQueueStatus {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ char queueName[24];
  /* 001A */ int ranking;
};

struct EntryQueueReady {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ char queueName[24];
  /* 001A */ char characterName[24];
};
#pragma pack(pop)

class CEntryQueueMgr {
public:
  virtual ~CEntryQueueMgr();

  void SendAdmissionReply(int result);
  void SendApply(std::uint16_t type, const char* name);
  void SendCancel();
  void SendRankingRequest();
  void SetQueueName(const EntryQueueName& name);
  void SetCharacterName(const EntryQueueName& name);
  void OnApplyResult(const EntryQueueApplyResult* packet);
  void OnCancelResult(const EntryQueueNotice* notice);
  void ResetCurrent(int unused);
  void IgnorePacket(const unsigned char* packet);
  void OnStatus(const EntryQueueStatus* packet);
  void OnReady(const EntryQueueReady* packet);
  void OnResultNotice(const EntryQueueNotice* notice);
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
static_assert(offsetof(EntryQueueStatus, ranking) == 0x1A);
static_assert(sizeof(EntryQueueReady) == 0x32);
static_assert(offsetof(EntryQueueInfo, unknown_020C) == 0x20C);
static_assert(offsetof(EntryQueueInfo, unknown_021C) == 0x21C);
static_assert(offsetof(EntryQueueInfo, unknown_0634) == 0x634);
static_assert(offsetof(CEntryQueueMgr, characterName) == 0x1C);
static_assert(offsetof(CEntryQueueMgr, current) == 0x34);
static_assert(sizeof(CEntryQueueMgr) == 0x3C);
#endif

#endif

#ifndef RAGNAROK_SOURCE_CONTENTS_TABLEMNG_H
#define RAGNAROK_SOURCE_CONTENTS_TABLEMNG_H

#include <map>

struct EntryQueueInfo {
  /* 0000 */ unsigned int id;
  /* 0004 */ char name[260];
  /* 0108 */ char description[260];
  /* 020C */ int minLevel;
  /* 0210 */ int maxLevel;
  /* 0214 */ unsigned char queueFlagA;
  /* 0215 */ unsigned char queueFlagB;
  /* 0216 */ unsigned char queueFlagC;
  /* 0217 */ unsigned char queueFlagD;
  /* 0218 */ unsigned char limitType;
  /* 021C */ int limitValue;
  /* 0220 */ int limitValueEnd;
  /* 0224 */ char detailsText[260];
  /* 0328 */ char rulesText[260];
  /* 042C */ char rewardText[260];
  /* 0530 */ char extraText[260];
  /* 0634 */ char bitmapName[260];
};

// VTABLE: vt_01823
class CTableMng {
public:
  CTableMng();
  virtual ~CTableMng();

  void ResetTables();
  EntryQueueInfo* GetEntryQueueInfo(unsigned int id);
  bool Init();
  bool ReadEntryQueueBinary();
  void Clear();
  bool AddEntryQueueInfo(unsigned int id, EntryQueueInfo* info);
  void Release();
  bool ReadEntryQueue();
  bool ReadEntryQueueFromXls();
  bool ReadEntryQueueXls(void* excel);
  bool WriteEntryQueueXls();

  /* 0004 */ std::map<unsigned int, EntryQueueInfo*> m_entryQueue;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EntryQueueInfo) == 0x738);
static_assert(sizeof(CTableMng) == 0xC);
#endif

#endif

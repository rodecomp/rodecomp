#ifndef RAGNAROK_SOURCE_CONTENTS_TABLEMNG_H
#define RAGNAROK_SOURCE_CONTENTS_TABLEMNG_H

#include <map>

struct EntryQueueInfo {
  /* 0000 */ unsigned int id;
  /* 0004 */ char name[260];
  /* 0108 */ char description[260];
  /* 020C */ int value020C;
  /* 0210 */ int value0210;
  /* 0214 */ unsigned char flag0214;
  /* 0215 */ unsigned char flag0215;
  /* 0216 */ unsigned char flag0216;
  /* 0217 */ unsigned char flag0217;
  /* 0218 */ unsigned char flag0218;
  /* 021C */ int value021C;
  /* 0220 */ int value0220;
  /* 0224 */ char text0224[260];
  /* 0328 */ char text0328[260];
  /* 042C */ char text042C[260];
  /* 0530 */ char text0530[260];
  /* 0634 */ char text0634[260];
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

  /* 0004 */ std::map<unsigned int, EntryQueueInfo*> m_entryQueue;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EntryQueueInfo) == 0x738);
static_assert(sizeof(CTableMng) == 0xC);
#endif

#endif

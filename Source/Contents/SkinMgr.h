#ifndef RAGNAROK_SOURCE_CONTENTS_SKINMGR_H
#define RAGNAROK_SOURCE_CONTENTS_SKINMGR_H

#include "Client/Application/Launcher.h"

#include <string>
#include <vector>

// VTABLE: vt_00808
class CSkinMgr {
public:
  /* 0004 */ std::string m_name;
  /* 001C */ int selected;
  /* 0020 */ std::vector<std::string> names;
  /* 002C */ std::vector<std::string> prefixes;
  /* 0038 */ std::vector<int> prepared;
  /* 0044 */ std::vector<int> archives;

  virtual ~CSkinMgr();

  CSkinMgr();

  bool Selected() const;
  const char* Prefix() const;
  const char* Name() const;
  const char* Name(int index) const;
  void Prepare();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSkinMgr) == 0x50);
#endif

extern std::string g_defaultSkinPrefix;
extern CSkinMgr g_skinInfo;

#endif

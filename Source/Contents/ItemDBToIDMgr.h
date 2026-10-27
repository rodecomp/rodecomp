#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMDBTOIDMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMDBTOIDMGR_H

#include "Script/LuaInstance.h"

#include <string>

class CItemDBToIDMgr {
public:
  CItemDBToIDMgr();
  virtual ~CItemDBToIDMgr();

  static CItemDBToIDMgr* GetInstance();

  void Init();
  bool Load();
  int GetItemID(std::string itemDBName);

protected:
  /* 0004 */ CLuaInstance m_lua;
};

extern CItemDBToIDMgr* g_itemDbToIdMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CItemDBToIDMgr) == 12);
#endif

#endif

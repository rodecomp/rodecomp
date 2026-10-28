#ifndef RAGNAROK_SOURCE_CONTENTS_MAPSKYMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MAPSKYMGR_H

#include "Script/LuaScript.h"

class CMapSkyMgr {
public:
  // INLINED
  virtual ~CMapSkyMgr() {
    if (this->m_script) {
      this->m_script->Close();
      delete this->m_script;
      this->m_script = nullptr;
    }
  }

  bool LoadData();

private:
  /* 0004 */ LuaScript* m_script;
};

#endif

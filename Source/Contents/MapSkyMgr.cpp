#include "Contents/MapSkyMgr.h"

// OFFSET: 0x005B8DD0
// FUNCTION: fn_03432
void CMapSkyMgr::Unused() {
}

// OFFSET: 0x005B8F00
// FUNCTION: fn_03435
bool CMapSkyMgr::LoadData() {
  return this->m_script->Load("Lua Files\\MapSkyData\\MapSkyData", true, false);
}

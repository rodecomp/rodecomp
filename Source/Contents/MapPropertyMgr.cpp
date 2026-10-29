#include "Contents/MapPropertyMgr.h"

#include <new>

// GLOBAL: gl_24816
CMapPropertyMgr* g_mapPropertyMgr;

// OFFSET: 0x00CA7640

// OFFSET: 0x00CA76C0

// OFFSET: 0x00CA7790

// OFFSET: 0x00CA7880

// OFFSET: 0x00CA7E70
// FUNCTION: fn_20979
void CMapPropertyMgr::Clear() {
  if (this != nullptr) {
    this->m_properties.clear();
  }
}

// OFFSET: 0x00CA7EA0

// OFFSET: 0x00CA7F90

// OFFSET: 0x00CA7FB0

#include "Contents/MapPropertyMgr.h"

#include <new>

// GLOBAL: gl_24816
CMapPropertyMgr* g_mapPropertyMgr;

// OFFSET: 0x00CA7640
// FUNCTION: fn_20970
CMapProperty::CMapProperty() {
}

// OFFSET: 0x00CA76C0
// FUNCTION: fn_20971
CMapProperty::~CMapProperty() {
}

// OFFSET: 0x00CA7790
// FUNCTION: fn_20973
void CMapProperty::SetValue(int key, int value) {
  this->m_values[key] = value;
}

// OFFSET: 0x00CA7880
// FUNCTION: fn_20974
int CMapProperty::GetValue(int key) {
  std::map<int, int>::iterator found = this->m_values.find(key);
  if (found == this->m_values.end()) {
    return 0;
  }
  return found->second;
}

// OFFSET: 0x00CA7E70
// FUNCTION: fn_20979
void CMapPropertyMgr::Clear() {
  if (this != nullptr) {
    this->m_properties.clear();
  }
}

// OFFSET: 0x00CA7EA0
// FUNCTION: fn_20980
bool CMapPropertyMgr::Create() {
  if (g_mapPropertyMgr == nullptr) {
    g_mapPropertyMgr = new (std::nothrow) CMapPropertyMgr;
    if (g_mapPropertyMgr != nullptr) {
      if (g_mapPropertyMgr->m_valid) {
        return true;
      }
      delete g_mapPropertyMgr;
      g_mapPropertyMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x00CA7F90
// FUNCTION: fn_20981
void CMapPropertyMgr::Destroy() {
  if (g_mapPropertyMgr != nullptr) {
    delete g_mapPropertyMgr;
    g_mapPropertyMgr = nullptr;
  }
}

// OFFSET: 0x00CA7FB0
// FUNCTION: fn_20982
int CMapPropertyMgr::GetValue(int mapId, int key) {
  std::map<int, CMapProperty>::iterator found = this->m_properties.find(mapId);
  if (found == this->m_properties.end()) {
    return 0;
  }
  return found->second.GetValue(key);
}

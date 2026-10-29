#include "Contents/SimpleStringMgr.h"

#include <cstring>

// GLOBAL: gl_24840
CSimpleStringMgr* g_simpleStringMgr = nullptr;

// OFFSET: 0x007A96A0
// FUNCTION: fn_09077
void CSimpleStringMgr::Destroy() {
  if (g_simpleStringMgr) {
    delete g_simpleStringMgr;
    g_simpleStringMgr = nullptr;
  }
}

// OFFSET: 0x007A9DA0
// FUNCTION: fn_09079
const char* CSimpleStringMgr::GetString(const char* key) {
  STR_TYPE type = STR_TYPE_EMOTION;
  if (key == nullptr || std::strlen(key) == 0) {
    return nullptr;
  }

  auto table = this->m_tables.find(type);
  if (table == this->m_tables.end()) {
    return nullptr;
  }

  auto entry = table->second.find(std::string(key));
  if (entry == table->second.end()) {
    return nullptr;
  }

  return entry->second.c_str();
}

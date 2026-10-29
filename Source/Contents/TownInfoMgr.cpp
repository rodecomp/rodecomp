#include "Contents/TownInfoMgr.h"

#include <new>

// GLOBAL: gl_24817
CTownInfoMgr* g_townInfoMgr;

// OFFSET: 0x00AE1A60
// FUNCTION: fn_18003
TownInfo::TownInfo(int type, std::string name, int x, int y, std::string image)
    : type(type), name(name), x(x), y(y), image(image) {
}

// OFFSET: 0x00AE1B00
// FUNCTION: fn_18004
TownInfo::TownInfo() : type(-1), name(""), x(-1), y(-1), image("") {
}

// OFFSET: 0x00AE2740
// FUNCTION: fn_18008
bool CTownInfoMgr::Create() {
  if (g_townInfoMgr == nullptr) {
    g_townInfoMgr = new (std::nothrow) CTownInfoMgr();
    if (g_townInfoMgr != nullptr) {
      if (g_townInfoMgr->m_isValid) {
        return true;
      }
      delete g_townInfoMgr;
      g_townInfoMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x00AE2840
// FUNCTION: fn_18009
void CTownInfoMgr::Destroy() {
  if (g_townInfoMgr != nullptr) {
    delete g_townInfoMgr;
    g_townInfoMgr = nullptr;
  }
}

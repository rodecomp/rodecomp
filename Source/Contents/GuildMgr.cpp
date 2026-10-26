#include "Contents/GuildMgr.h"
#include "Contents/Emblems/GuildEmblemReqBundle.h"

#include <new>

// GLOBAL: gl_24555
CGuildMgr* g_guildMgr = nullptr;

// INLINED
CGuildMgr::CGuildMgr() : m_reqBundle(nullptr), m_timeout(0xC800) {
  this->m_initialized = false;
  this->m_reqBundle = new (std::nothrow) CGuildEmblemReqBundle();
  if (this->m_reqBundle) {
    this->m_initialized = true;
  }
}

// INLINED
CGuildMgr::~CGuildMgr() {
  if (this->m_reqBundle) {
    delete this->m_reqBundle;
    this->m_reqBundle = nullptr;
  }
}

// OFFSET: 0x0065E290
// FUNCTION: fn_05172
bool CGuildMgr::Create() {
  if (g_guildMgr) {
    return false;
  }

  g_guildMgr = new (std::nothrow) CGuildMgr();
  if (!g_guildMgr) {
    return false;
  }

  if (!g_guildMgr->m_initialized) {
    delete g_guildMgr;
    g_guildMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x0065E430
// FUNCTION: fn_05173
void CGuildMgr::Destroy() {
  if (g_guildMgr) {
    delete g_guildMgr;
    g_guildMgr = nullptr;
  }
}

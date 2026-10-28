#include "Resource/ResourceFailures.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Threading/NSyncObject.h"

#include <cstdint>
#include <cstring>

// GLOBAL: gl_24582
ResourceFailures g_resourceFailures;

// INLINED
ResourceFailures::ResourceFailures() {
  InitializeCriticalSection(&this->m_lock);
}

// OFFSET: 0x00AEE310
// FUNCTION: fn_18171
void ResourceFailures::Report(std::string requested, std::string resolved) {
  const char* extension = std::strrchr(resolved.c_str(), '.');
  if (extension && _stricmp(extension, ".ebm") == 0) {
    return;
  }

  bool report;
  {
    NSyncObject lock(&this->m_lock);
    report = this->m_reported.insert(resolved).second;
  }
  if (report) {
    g_windowMgr.SendMsg(1, reinterpret_cast<std::intptr_t>("Resource File Loading fail"), 255, 0, 0, 0);
    g_windowMgr.SendMsg(1, reinterpret_cast<std::intptr_t>(requested.c_str()), 255, 0, 0, 0);
    g_windowMgr.SendMsg(1, reinterpret_cast<std::intptr_t>(resolved.c_str()), 255, 0, 0, 0);
  }
}

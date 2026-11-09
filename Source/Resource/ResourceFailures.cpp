#include "Resource/ResourceFailures.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Threading/NSyncObject.h"

#include <cstdint>
#include <cstring>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

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
  if (extension && _stricmp(".ebm", extension) == 0) {
    return;
  }

  {
    NSyncObject lock(&this->m_lock);
    if (this->m_reported.find(resolved) != this->m_reported.end()) {
      return;
    }
    this->m_reported.insert(resolved);
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("Resource File Loading fail"), 255, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(requested.c_str()), 255, 0, 0, 0);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(resolved.c_str()), 255, 0, 0, 0);
}

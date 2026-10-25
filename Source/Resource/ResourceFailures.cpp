#include "Resource/ResourceFailures.h"
#include "Utility/Threading/NSyncObject.h"

#include <cstring>

ResourceFailures::ResourceFailures(ResourceMessages& messages) : m_messages(&messages) {
  InitializeCriticalSection(&this->m_lock);
}

ResourceFailures::~ResourceFailures() {
  DeleteCriticalSection(&this->m_lock);
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
    this->m_messages->Message("Resource File Loading fail", 255);
    this->m_messages->Message(requested.c_str(), 255);
    this->m_messages->Message(resolved.c_str(), 255);
  }
}

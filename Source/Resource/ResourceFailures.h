#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEFAILURES_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEFAILURES_H

#include <cstddef>
#include <set>
#include <string>
#include <windows.h>

class ResourceMessages {
public:
  virtual void Message(const char* text, unsigned color) = 0;

protected:
  ~ResourceMessages() = default;
};

// VTABLE: vt_01619
class ResourceFailures {
public:
  ResourceFailures();
  virtual ~ResourceFailures() = default;
  void Report(std::string requested, std::string resolved);

  /* 0004 */ CRITICAL_SECTION m_lock;
  /* 001C */ std::set<std::string> m_reported;

private:
  ResourceFailures(const ResourceFailures&) = delete;
  ResourceFailures& operator=(const ResourceFailures&) = delete;
};

extern ResourceFailures g_resourceFailures;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ResourceFailures, m_lock) == 0x04);
static_assert(offsetof(ResourceFailures, m_reported) == 0x1C);
static_assert(sizeof(ResourceFailures) == 0x24);
#endif

#endif

#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEFAILURES_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEFAILURES_H

#include <set>
#include <string>
#include <windows.h>

class ResourceMessages {
public:
  virtual void Message(const char* text, unsigned color) = 0;

protected:
  ~ResourceMessages() = default;
};

// Original failure owner's critical section+4 and set+28. Actual manager
// message routing is explicit; missing-resource diagnostics are not discarded.
class ResourceFailures {
public:
  explicit ResourceFailures(ResourceMessages& messages);
  ~ResourceFailures();
  void Report(std::string requested, std::string resolved);

private:
  CRITICAL_SECTION m_lock;
  std::set<std::string> m_reported;
  ResourceMessages* m_messages;
};

#endif

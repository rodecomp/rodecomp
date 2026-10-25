#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEMGR_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEMGR_H

#include "Resource/Resource.h"
#include "Resource/ResourceAliases.h"

#include <map>
#include <vector>
#include <windows.h>

class ResourceFailures;

struct ResourceNameLess {
  bool operator()(const ResourceName* left, const ResourceName* right) const;
};

class ResourceMgr {
public:
  explicit ResourceMgr(ResourceFailures& failures);
  ~ResourceMgr();
  void Register(const char* extension, const char* prefix, CRes* prototype);
  int FindType(const char* extension) const;
  CRes* Find(int type, const char* name) const;
  CRes* Load(const char* name);
  void Remove(CRes* resource);
  bool Exists(const char* name);
  void ClearUnused(const char* extension);
  void ClearUnused();
  void ClearExpired(const unsigned& reuseDelay);
  void UpdateTransient();
  ResourceAliases aliases;

private:
  std::map<std::string, int> m_extensions;
  std::vector<const char*> m_prefixes;
  std::vector<CRes*> m_prototypes;
  std::vector<std::map<const ResourceName*, CRes*, ResourceNameLess>> m_resources;
  CRITICAL_SECTION m_lock;
  std::vector<CRes*> m_transient;
  ResourceFailures* m_failures;
  void EraseTransient(CRes* resource);
  ResourceMgr(const ResourceMgr&) = delete;
  ResourceMgr& operator=(const ResourceMgr&) = delete;
};

#endif

#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEMGR_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEMGR_H

#include "Resource/Resource.h"
#include "Resource/ResourceAliases.h"

#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <windows.h>

struct ResourceExtensionLess {
  bool operator()(const char* left, const char* right) const {
    while (*left == *right && *left != '\0') {
      ++left;
      ++right;
    }

    return static_cast<unsigned char>(*left) < static_cast<unsigned char>(*right);
  }
};

struct ResourceNameLess {
  bool operator()(const ResourceName* left, const ResourceName* right) const;
};

class ResourceMgr {
public:
  ResourceMgr();
  ~ResourceMgr();
  static ResourceMgr& Instance();
  void Register(const char* extension, const char* prefix, CRes* prototype);
  int FindType(const char* extension) const;
  CRes* Find(int type, const char* name) const;
  CRes* Load(const char* name);
  CRes* Load(std::string name);
  void Remove(CRes* resource);
  bool Exists(const char* name);
  bool Exists(std::string name);
  void UnloadAll();
  void ClearUnused(const char* extension);
  void ClearUnused();
  void ClearExpired();
  void UpdateTransient();
  const char* Resolve(const char* name) const;
  std::string ResolveMapName(const char* name) const;
  void LoadAliases(int option);

private:
  /* 0000 */ std::map<const char*, int, ResourceExtensionLess> m_extensions;
  /* 0008 */ std::vector<const char*> m_prefixes;
  /* 0014 */ std::vector<CRes*> m_prototypes;
  /* 0020 */ std::vector<std::map<const ResourceName*, CRes*, ResourceNameLess>> m_resources;
  /* 002C */ CRITICAL_SECTION m_lock;
  /* 0044 */ std::map<std::string, std::string> m_aliases;
  /* 004C */ std::uint32_t padding_004C[17];
  /* 0090 */ std::vector<CRes*> m_transient;
  void EraseTransient(CRes* resource);
  CRes* LoadNamed(const char* name);
  ResourceMgr(const ResourceMgr&) = delete;
  ResourceMgr& operator=(const ResourceMgr&) = delete;
};

extern unsigned g_resourceReuseDelay;
extern unsigned char g_lowercaseTable[256];

#endif

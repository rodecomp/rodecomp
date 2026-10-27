#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEALIASES_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEALIASES_H

#include <map>
#include <string>

struct ResourceAliases {
  /* 0044 */ std::map<std::string, std::string> names;

  const char* Resolve(const char* name) const;
  std::string ResolveMapName(const char* name) const;
  void Load(int option);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ResourceAliases) == 0x08);
#endif

std::string AddResourcePrefix(const char* name, const char* prefix);

#endif

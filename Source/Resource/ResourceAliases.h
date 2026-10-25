#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEALIASES_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEALIASES_H

#include <map>
#include <string>

struct ResourceAliases {
  std::map<std::string, std::string> names;

  const char* Resolve(const char* name) const;
};

std::string AddResourcePrefix(const char* name, const char* prefix);

#endif

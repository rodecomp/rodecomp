#ifndef RAGNAROK_SOURCE_RESOURCE_RESOURCEALIASES_H
#define RAGNAROK_SOURCE_RESOURCE_RESOURCEALIASES_H

#include <map>
#include <string>

// Resource manager's map at+68. Returned mapped string pointers remain borrowed
// from this map; misses retain the original input pointer, not a copied path.
struct ResourceAliases {
  std::map<std::string, std::string> names;

  const char* Resolve(const char* name) const;
};

std::string AddResourcePrefix(const char* name, const char* prefix);

#endif

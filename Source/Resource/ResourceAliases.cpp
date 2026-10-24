#include "Resource/ResourceAliases.h"

#include <cstring>

// OFFSET: 0x00AEF500
// FUNCTION: fn_18180
const char* ResourceAliases::Resolve(const char* name) const {
  std::string key(name);
  auto entry = this->names.find(key);
  if (entry != this->names.end()) {
    return entry->second.c_str();
  }
  const std::size_t marker = key.rfind('@');
  if (marker != std::string::npos && marker >= 4) {
    // Original removes three bytes starting FOUR bytes before the final '@'.
    key.erase(marker - 4, 3);
    entry = this->names.find(key);
    if (entry != this->names.end()) {
      return entry->second.c_str();
    }
  }
  return name;
}

// OFFSET: 0x00AEFAE0
// FUNCTION: fn_18184
std::string AddResourcePrefix(const char* name, const char* prefix) {
  if (std::strncmp(name, prefix, std::strlen(prefix)) == 0) {
    return name;
  }
  return std::string(prefix) + name;
}

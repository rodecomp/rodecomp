#include "Resource/ResourceAliases.h"
#include "Resource/ResourceMgr.h"
#include "File/File.h"

#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

using AliasTree = std::map<std::string, std::string>::_Mybase;

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char* const, const std::size_t, const char* const, const std::size_t) noexcept;
template __declspec(noinline) std::_Tree_find_result<AliasTree::_Nodeptr> AliasTree::_Find_lower_bound<std::string>(const std::string&) const;

// OFFSET: 0x00AEF500
// FUNCTION: fn_18180
const char* ResourceMgr::Resolve(const char* name) const {
  auto entry = this->m_aliases.find(name);
  if (entry == this->m_aliases.end()) {
    std::string key(name);
    const std::size_t marker = key.rfind("@");
    if (marker != std::string::npos && marker >= 4) {
      key.erase(marker - 4, 3);
      entry = this->m_aliases.find(key);
      if (entry != this->m_aliases.end()) {
        name = entry->second.c_str();
      }
    }

    return name;
  }

  return entry->second.c_str();
}

// OFFSET: 0x00AEFAE0
// FUNCTION: fn_18184
std::string AddResourcePrefix(const char* name, const char* prefix) {
  if (std::strncmp(name, prefix, std::strlen(prefix)) != 0) {
    return std::string(prefix) + std::string(name);
  }

  return name;
}

// OFFSET: 0x00AEF3A0
// FUNCTION: fn_18179
std::string ResourceMgr::ResolveMapName(const char* name) const {
  std::string filename(name);
  filename += ".gat";
  const char* resolved = this->Resolve(filename.c_str());
  const char* extension = std::strstr(resolved, ".gat");
  return std::string(resolved, extension);
}

// OFFSET: 0x00AEFE50
// FUNCTION: fn_18187
void ResourceMgr::LoadAliases(int) {
  this->m_aliases.clear();
  CFile file;
  if (!file.Open("ResNameTable.txt", 0, true, false)) {
    return;
  }

  const std::uint32_t bytes = file.GetSize();
  auto* contents = new char[bytes + 1];
  file.Read(contents, bytes);
  contents[bytes] = '\0';
  file.Close();

  std::string name;
  std::string replacement;
  const char* token = contents;
  for (int index = 0; index < static_cast<int>(bytes); ++index) {
    if (contents[index] == '\n' || contents[index] == '\r') {
      contents[index] = '\0';
      token = contents + index + 1;
    } else if (contents[index] == '#') {
      contents[index] = '\0';
      if (name.empty()) {
        name = token;
      } else {
        replacement = token;
        this->m_aliases.insert(std::make_pair(name, replacement));
        name.clear();
        replacement.clear();
      }
      token = contents + index + 1;
    }
  }

  if (contents) {
    delete[] contents;
  }
}

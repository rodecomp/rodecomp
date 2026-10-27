#include "Resource/ResourceAliases.h"
#include "File/File.h"

#include <cstdlib>
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

// OFFSET: 0x00AEF3A0
// FUNCTION: fn_18179
std::string ResourceAliases::ResolveMapName(const char* name) const {
  std::string filename(name);
  filename += ".gat";
  const char* resolved = this->Resolve(filename.c_str());
  const char* extension = std::strstr(resolved, ".gat");
  return std::string(resolved, extension);
}

// OFFSET: 0x00AEFE50
// FUNCTION: fn_18187
void ResourceAliases::Load(int) {
  this->names.clear();
  CFile file;
  if (!file.Open("ResNameTable.txt", 0, true, false)) {
    return;
  }

  const std::uint32_t bytes = file.GetSize();
  auto* contents = static_cast<char*>(std::malloc(bytes + 1));
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
        this->names.insert(std::make_pair(name, replacement));
        name.clear();
        replacement.clear();
      }
      token = contents + index + 1;
    }
  }

  std::free(contents);
}

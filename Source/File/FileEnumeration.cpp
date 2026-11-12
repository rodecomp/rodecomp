#include "File/FileEnumeration.h"

#include <windows.h>

// OFFSET: 0x00AF5080
// FUNCTION: fn_18246
void EnumerateFileNames(std::list<std::string>& names, const char* directory, const char* pattern) {
  std::string search(directory);
  search.append(pattern);
  WIN32_FIND_DATAA found{};
  HANDLE handle = FindFirstFileA(search.c_str(), &found);
  if (handle == INVALID_HANDLE_VALUE) {
    FindClose(handle);
    return;
  }

  const std::string current(".");
  const std::string parent("..");
  if (current != found.cFileName && parent != found.cFileName) {
    names.push_back(std::string(found.cFileName));
  }

  while (FindNextFileA(handle, &found)) {
    if (current != found.cFileName && parent != found.cFileName) {
      names.push_back(std::string(found.cFileName));
    }
  }
  FindClose(handle);
}

// OFFSET: 0x00AF5510
// FUNCTION: fn_18247
void EnumerateFileNames(std::vector<std::string>& names, const char* directory, const char* pattern) {
  std::string search(directory);
  search.append(pattern);
  WIN32_FIND_DATAA found{};
  HANDLE handle = FindFirstFileA(search.c_str(), &found);
  if (handle == INVALID_HANDLE_VALUE) {
    FindClose(handle);
    return;
  }

  const std::string current(".");
  const std::string parent("..");
  if (current != found.cFileName && parent != found.cFileName) {
    names.push_back(std::string(found.cFileName));
  }

  while (FindNextFileA(handle, &found)) {
    if (current != found.cFileName && parent != found.cFileName) {
      names.push_back(std::string(found.cFileName));
    }
  }
  FindClose(handle);
}

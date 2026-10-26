#include "File/FileEnumeration.h"

#include <windows.h>

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

  do {
    if (found.cFileName[0] == '.' && (found.cFileName[1] == 0 || (found.cFileName[1] == '.' && found.cFileName[2] == 0))) {
      continue;
    }
    names.push_back(std::string(found.cFileName));
  } while (FindNextFileA(handle, &found));
  FindClose(handle);
}

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

  do {
    if (found.cFileName[0] == '.' && (found.cFileName[1] == 0 || (found.cFileName[1] == '.' && found.cFileName[2] == 0))) {
      continue;
    }
    names.push_back(std::string(found.cFileName));
  } while (FindNextFileA(handle, &found));
  FindClose(handle);
}

#include "Contents/SkinMgr.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstring>
#include <string>
#include <windows.h>

// GLOBAL: gl_24533
const char* g_skinDirectoryPrefix = "..\\..\\skin\\";
// GLOBAL: gl_24534
const char* g_skinArchivePrefix = "..\\..\\_tmpskin_\\";

// OFFSET: 0x007E1120
// FUNCTION: fn_09689
void CSkinMgr::Refresh() {
  this->prefixes.clear();
  this->names.clear();
  this->archives.clear();
  this->selected = -1;

  WIN32_FIND_DATAA findData;
  std::memset(&findData, 0, sizeof(findData));
  HANDLE find = FindFirstFileA(".\\skin\\*.*", &findData);
  if (find == INVALID_HANDLE_VALUE) {
    FindClose(find);
    return;
  }

  const std::string current = ".";
  const std::string parent = "..";
  int index = 0;

  if (current != findData.cFileName && parent != findData.cFileName && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
    std::string path = g_skinDirectoryPrefix;
    path += findData.cFileName;
    path += "\\";
    this->prefixes.push_back(path.c_str());
    this->names.push_back(findData.cFileName);
    this->archives.push_back(0);
    this->prepared.push_back(0);

    if (this->m_name == findData.cFileName) {
      this->selected = index;
    }
    ++index;
  } else if (current != findData.cFileName && parent != findData.cFileName) {
    std::string extension = ReverseFindChar(findData.cFileName, '.');
    LowercaseText(extension);

    if (extension == ".zip") {
      std::string path = g_skinArchivePrefix;
      path += findData.cFileName;
      path.resize(path.size() - 4);
      path += "\\";
      this->prefixes.push_back(path.c_str());

      std::string name = findData.cFileName;
      name.resize(name.size() - 4);
      this->names.push_back(name.c_str());
      this->archives.push_back(1);
      this->prepared.push_back(0);

      if (this->m_name == name) {
        this->selected = index;
      }
      ++index;
    }
  }

  while (FindNextFileA(find, &findData)) {
    if (current != findData.cFileName && parent != findData.cFileName && (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      std::string path = g_skinDirectoryPrefix;
      path += findData.cFileName;
      path += "\\";
      this->prefixes.push_back(path.c_str());
      this->names.push_back(findData.cFileName);
      this->archives.push_back(0);
      this->prepared.push_back(0);

      if (this->m_name == findData.cFileName) {
        this->selected = index;
      }
      ++index;
    } else if (current != findData.cFileName && parent != findData.cFileName) {
      std::string extension = ReverseFindChar(findData.cFileName, '.');
      LowercaseText(extension);

      if (extension == ".zip") {
        std::string path = g_skinArchivePrefix;
        path += findData.cFileName;
        path.resize(path.size() - 4);
        path += "\\";
        this->prefixes.push_back(path.c_str());

        std::string name = findData.cFileName;
        name.resize(name.size() - 4);
        this->names.push_back(name.c_str());
        this->archives.push_back(1);
        this->prepared.push_back(0);

        if (this->m_name == name) {
          this->selected = index;
        }
        ++index;
      }
    }
  }

  FindClose(find);
}

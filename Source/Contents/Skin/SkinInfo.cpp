#include "Contents/Skin/SkinMgr.h"
#include "Resource/ResourceMgr.h"

#include <cstdio>
#include <cstring>
#include <direct.h>
#include <io.h>
#include <process.h>

// OFFSET: 0x007E2130
// FUNCTION: fn_09690
bool CSkinMgr::Selected() const {
  return this->selected > -1;
}

// OFFSET: 0x007E2140
// FUNCTION: fn_09691
void CSkinMgr::Select(int index) {
  const int previous = this->selected;
  this->selected = index;
  if (previous != index) {
    ResourceMgr::Instance().ClearUnused("bmp");
  }
}

// OFFSET: 0x007E1020
// FUNCTION: fn_09686
const char* CSkinMgr::Name() const {
  if (this->selected == -1) {
    return g_defaultSkinPrefix.c_str();
  }

  return this->names[this->selected].c_str();
}

// OFFSET: 0x007E1060
// FUNCTION: fn_09687
const char* CSkinMgr::Prefix() const {
  if (this->selected == -1) {
    return g_defaultSkinPrefix.c_str();
  }

  return this->prefixes[this->selected].c_str();
}

// OFFSET: 0x007E10E0
// FUNCTION: fn_09688
const char* CSkinMgr::Name(int index) const {
  if (index == -1) {
    return g_defaultSkinPrefix.c_str();
  }

  return this->names[index].c_str();
}

// OFFSET: 0x007E0C40
// FUNCTION: fn_09683
void CSkinMgr::Prepare() {
  if (this->selected == -1 || this->archives[this->selected] == 0 || this->prepared[this->selected] != 0) {
    return;
  }

  char path[2048];
  std::sprintf(path, "%s\\_tmpskin_", g_applicationDirectory);
  std::strcat(path, "\\");
  std::strcat(path, this->names[this->selected].c_str());
  if (_access(path, 0) == 0) {
    return;
  }

  std::sprintf(path, "%s\\_tmpskin_", g_applicationDirectory);
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  std::sprintf(path, "%s\\_tmpskin_", g_applicationDirectory);
  std::strcat(path, "\\");
  std::strcat(path, this->names[this->selected].c_str());
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  char noOverwrite[128];
  char input[128];
  char directoryOption[128];
  char output[128];
  std::strcpy(noOverwrite, "-n");
  std::strcpy(input, "skin/");
  std::strcat(input, this->names[this->selected].c_str());
  std::strcpy(directoryOption, "-d");
  std::strcpy(output, "_tmpskin_/");
  std::strcat(output, this->names[this->selected].c_str());
  _spawnl(_P_WAIT, "runzip.exe", "runzip.exe", noOverwrite, input, directoryOption, output, nullptr);

  this->prepared[this->selected] = 1;
}

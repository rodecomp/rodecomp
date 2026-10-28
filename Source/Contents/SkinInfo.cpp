#include "Contents/SkinMgr.h"
#include "Resource/ResourceMgr.h"

#include <cstdio>
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

  char directory[2048];
  std::sprintf(directory, "%s\\_tmpskin_", g_applicationDirectory);
  const std::string target = std::string(directory) + "\\" + this->names[this->selected];

  if (_access(target.c_str(), 0) == 0) {
    return;
  }

  if (_access(directory, 0) != 0 && _mkdir(directory) != 0) {
    return;
  }

  if (_access(target.c_str(), 0) != 0 && _mkdir(target.c_str()) != 0) {
    return;
  }

  const std::string input = std::string("skin/") + this->names[this->selected];
  const std::string output = std::string("_tmpskin_/") + this->names[this->selected];
  _spawnl(_P_WAIT, "runzip.exe", "runzip.exe", "-n", input.c_str(), "-d", output.c_str(), nullptr);

  this->prepared[this->selected] = 1;
}

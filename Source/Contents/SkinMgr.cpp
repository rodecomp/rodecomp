#include "Contents/SkinMgr.h"
#include "File/DirectoryRemoval.h"

#include <cstdio>
#include <io.h>

// GLOBAL: gl_24634
CSkinMgr g_skinInfo;

// GLOBAL: gl_24583
std::string g_defaultSkinPrefix = "<Basic Skin>";

// OFFSET: 0x007E09D0
// FUNCTION: fn_45445
CSkinMgr::CSkinMgr() {
  this->m_name = g_defaultSkinPrefix;
}

// OFFSET: 0x007E0AD0
// FUNCTION: fn_09681
CSkinMgr::~CSkinMgr() {
  char directory[2048];
  std::sprintf(directory, "%s\\_tmpskin_", g_applicationDirectory);

  if (_access(directory, 0) == 0) {
    RemoveDirectoryTree(directory);
  }
}

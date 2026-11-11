#include "Archive/PakRegistry.h"
#include "File/ResourcePath.h"
#include "Utility/Hash/GGHashMgr.h"
#include <cstring>
#include <shlwapi.h>

// STRING: st_14242
static char g_dataDirectory[] = "data\\";

// OFFSET: 0x0057BC20
// FUNCTION: fn_02596
std::string MakeResourcePath(std::string filename) {
  std::string path = std::string(g_dataDirectory) + filename;

  if (static_cast<int>(path.find(":")) >= 1) {
    path = path.substr(path.find(":") - 1);
  }

  std::size_t duplicate;
  while ((duplicate = path.find("//")) != std::string::npos) {
    path.replace(duplicate, 2, "/", 1);
  }

  return path;
}

// OFFSET: 0x0057BB50
// FUNCTION: fn_02595
bool ResourceExists(const char* filename, bool looseOnly) {
  std::string path = MakeResourcePath(std::string(filename));
  return g_pakRegistry.Exists(path.c_str(), looseOnly);
}

// OFFSET: 0x00AE9EE0
// FUNCTION: fn_18113
void* PakRegistry::ReadLooseResource(const char* filename, std::uint32_t* size) {
  HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file != INVALID_HANDLE_VALUE) {
    LARGE_INTEGER fileSize;
    GetFileSizeEx(file, &fileSize);
    *size = fileSize.LowPart;
    if (size) {
      void* data = VirtualAlloc(nullptr, *size, MEM_COMMIT, PAGE_READWRITE);
      if (data) {
        DWORD read;
        ReadFile(file, data, *size, &read, nullptr);
        CloseHandle(file);

        const char* extension = PathFindExtensionA(filename);
        if (!extension || std::strcmp(extension, ".ebm") != 0) {
          g_ggHashMgr->VerifyFile(filename);
        }

        return data;
      }
      VirtualFree(nullptr, 0, MEM_RELEASE);
    }
    CloseHandle(file);
  }

  return nullptr;
}

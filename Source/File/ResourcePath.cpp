#include "Archive/PakRegistry.h"
#include "File/ResourcePath.h"

// OFFSET: 0x0057BC20
// FUNCTION: fn_02596
std::string MakeResourcePath(std::string filename) {
  std::string path = "data\\";
  path.append(filename);

  std::size_t colon = path.find(':');
  if (colon != std::string::npos && colon >= 1) {
    path = path.substr(colon - 1);
  }

  std::size_t duplicate;

  while ((duplicate = path.find("//")) != std::string::npos) {
    path.replace(duplicate, 2, "/", 1);
  }

  return path;
}

// OFFSET: 0x0057BB50
// FUNCTION: fn_02595
bool ResourceExists(const char* filename, int looseOnly) {
  std::string path = MakeResourcePath(std::string(filename));
  return g_pakRegistry.Exists(path.c_str(), static_cast<char>(looseOnly) != 0);
}

// OFFSET: 0x00AE9EE0
// FUNCTION: fn_18113
void* ReadLooseResource(const char* filename, std::uint32_t* size) {
  HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    return nullptr;
  }

  LARGE_INTEGER fileSize;
  GetFileSizeEx(file, &fileSize);
  *size = fileSize.LowPart;

  void* data = VirtualAlloc(nullptr, *size, MEM_COMMIT, PAGE_READWRITE);
  if (!data) {
    VirtualFree(nullptr, 0, MEM_RELEASE);
    CloseHandle(file);
    return nullptr;
  }

  DWORD read;
  ReadFile(file, data, *size, &read, nullptr);
  CloseHandle(file);

  return data;
}

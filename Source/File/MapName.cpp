#include "File/MapName.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <map>
#include <windows.h>

// OFFSET: 0x00CD4E40
// FUNCTION: fn_21276
const char* ScriptMapName(std::string name) {
  // STATIC: 0x017203E0
  // GLOBAL: gl_24517
  static std::map<std::intptr_t, std::array<char, 16>> buffers;
  auto& output = buffers[reinterpret_cast<std::intptr_t>(GetCurrentThread())];
  const std::size_t marker = name.rfind('@');

  if (marker != std::string::npos && marker >= 4) {
    name.erase(marker - 4, 3);
  }

  _snprintf_s(output.data(), output.size(), _TRUNCATE, "%s", name.c_str());
  return output.data();
}

#include "Network/TextEncoding.h"

#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <oleauto.h>

// STATIC: 0x016D3A78
// GLOBAL: gl_24375
UINT g_textCodePage;

// OFFSET: 0x005E8FE0
// FUNCTION: fn_03951
std::string ToUtf8(std::string text) {
  std::string result;
  int wideLength = MultiByteToWideChar(g_textCodePage, 0, text.c_str(), lstrlenA(text.c_str()), nullptr, 0);
  BSTR wide = SysAllocStringLen(nullptr, wideLength);
  if (wide) {
    MultiByteToWideChar(g_textCodePage, 0, text.c_str(), lstrlenA(text.c_str()), wide, wideLength);
    int length = WideCharToMultiByte(CP_UTF8, 0, wide, -1, nullptr, 0, nullptr, nullptr);
    char* converted = static_cast<char*>(_malloca(length + 1));
    if (converted) {
      WideCharToMultiByte(CP_UTF8, 0, wide, -1, converted, length, nullptr, nullptr);
      result.assign(converted, std::strlen(converted));
      _freea(converted);
    } else {
      // Original failure path reaches an unchecked null string assignment.
      result.assign(converted, std::strlen(converted));
    }
    SysFreeString(wide);
  }
  return result;
}

// OFFSET: 0x005E9440
// FUNCTION: fn_03954
std::string FromUtf8(std::string text) {
  std::string result;
  int wideLength = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), lstrlenA(text.c_str()) + 1, nullptr, 0);
  BSTR wide = SysAllocStringLen(nullptr, wideLength);
  MultiByteToWideChar(CP_UTF8, 0, text.c_str(), lstrlenA(text.c_str()) + 1, wide, wideLength);
  int length = WideCharToMultiByte(g_textCodePage, 0, wide, -1, nullptr, 0, nullptr, nullptr);
  char* converted = static_cast<char*>(_malloca(length + 1));
  if (converted) {
    WideCharToMultiByte(g_textCodePage, 0, wide, -1, converted, length, nullptr, nullptr);
    result.assign(converted, std::strlen(converted));
    _freea(converted);
  }
  if (wide) {
    SysFreeString(wide);
  }
  return result;
}

// OFFSET: 0x00AF87B0
// FUNCTION: fn_18264
std::string FromUtf8(const std::string& text, UINT codePage) {
  int wideLength = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
  if (!wideLength) {
    return {};
  }
  BSTR wide = SysAllocStringLen(nullptr, wideLength);
  if (!wide) {
    return {};
  }
  if (!MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), wide, wideLength)) {
    SysFreeString(wide);
    return {};
  }
  int length = WideCharToMultiByte(codePage, 0, wide, -1, nullptr, 0, nullptr, nullptr);
  if (!length) {
    SysFreeString(wide);
    return {};
  }
  char* converted = static_cast<char*>(std::malloc(length));
  if (!converted) {
    SysFreeString(wide);
    return {};
  }
  std::string result;
  if (WideCharToMultiByte(codePage, 0, wide, -1, converted, length, nullptr, nullptr)) {
    result.assign(converted);
  }
  std::free(converted);
  SysFreeString(wide);
  return result;
}

// OFFSET: 0x00AF4050
// FUNCTION: fn_18243
std::string ToUtf8(const std::string& text, UINT codePage) {
  int wideLength = MultiByteToWideChar(codePage, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
  if (!wideLength) {
    return {};
  }
  BSTR wide = SysAllocStringLen(nullptr, wideLength);
  if (!wide) {
    return {};
  }
  if (!MultiByteToWideChar(codePage, 0, text.data(), static_cast<int>(text.size()), wide, wideLength)) {
    SysFreeString(wide);
    return {};
  }
  int length = WideCharToMultiByte(CP_UTF8, 0, wide, -1, nullptr, 0, nullptr, nullptr);
  if (!length) {
    SysFreeString(wide);
    return {};
  }
  char* converted = static_cast<char*>(std::malloc(length));
  if (!converted) {
    SysFreeString(wide);
    return {};
  }
  std::string result;
  if (WideCharToMultiByte(CP_UTF8, 0, wide, -1, converted, length, nullptr, nullptr)) {
    result.assign(converted);
  }
  std::free(converted);
  SysFreeString(wide);
  return result;
}

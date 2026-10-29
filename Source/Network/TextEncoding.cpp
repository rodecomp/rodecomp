#include "Network/TextEncoding.h"
#include "Utility/StackAlloc.h"

#include <algorithm>
#include <atlconv.h>
#include <cctype>

#include <cstdlib>
#include <cstring>
#include <cwctype>
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
    char* converted = static_cast<char*>(STACK_ALLOC(length + 1));

    if (converted) {
      WideCharToMultiByte(CP_UTF8, 0, wide, -1, converted, length, nullptr, nullptr);
      result.assign(converted, std::strlen(converted));
      _freea(converted);
    } else {
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
  char* converted = static_cast<char*>(STACK_ALLOC(length + 1));

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

// FUNCTION: fn_18257
void LowercaseText(std::string& text) {
  for (unsigned index = 0; index < text.size(); ++index) {
    if (static_cast<signed char>(text[index]) <= 0) {
      ++index;
    } else if (std::isupper(text[index]) != 0) {
      text[index] += 32;
    }
  }
}

// FUNCTION: fn_18258
std::wstring DecodeText(UINT codePage, const std::string& text) {
  const int length = MultiByteToWideChar(codePage, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
  if (length == 0) {
    return {};
  }
  BSTR converted = SysAllocStringLen(nullptr, length);
  if (converted == nullptr) {
    return {};
  }
  if (MultiByteToWideChar(codePage, 0, text.data(), static_cast<int>(text.size()), converted, length) == 0) {
    SysFreeString(converted);
    return {};
  }
  std::wstring result(converted);
  SysFreeString(converted);
  return result;
}

// FUNCTION: fn_18265
std::string EncodeText(UINT codePage, const std::wstring& text) {
  const int length = WideCharToMultiByte(codePage, 0, text.c_str(), -1, nullptr, 0, nullptr, nullptr);
  if (length == 0) {
    return {};
  }
  char* converted = static_cast<char*>(std::malloc(length));
  if (converted == nullptr) {
    return {};
  }
  if (WideCharToMultiByte(codePage, 0, text.c_str(), -1, converted, length, nullptr, nullptr) == 0) {
    std::free(converted);
    return {};
  }
  std::string result(converted, std::strlen(converted));
  std::free(converted);
  return result;
}

// OFFSET: 0x00590EB0
// FUNCTION: fn_02832
std::wstring LowercaseWide(const std::string& text) {
  std::wstring result(ATL::CA2WEX<128>(text.c_str(), CP_THREAD_ACP));
  std::transform(result.begin(), result.end(), result.begin(), towlower);
  return result;
}

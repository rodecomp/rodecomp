#ifndef RAGNAROK_SOURCE_UTILITY_JSONCPPLOADER_H
#define RAGNAROK_SOURCE_UTILITY_JSONCPPLOADER_H

#include <cstddef>
#include <string>

class JsonCppLoader {
public:
  void Fail(const char* format, ...);

  /* 0000 */ unsigned char unknown_0000[0x18];
  /* 0018 */ std::string error;
  /* 0030 */ bool loaded;
  /* 0031 */ bool succeeded;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(JsonCppLoader, error) == 0x18);
static_assert(offsetof(JsonCppLoader, loaded) == 0x30);
static_assert(offsetof(JsonCppLoader, succeeded) == 0x31);
#endif

#endif

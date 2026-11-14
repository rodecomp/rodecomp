#ifndef RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADERPARSING_H
#define RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADERPARSING_H

#include "Resource/Json/JsonCppLoader.h"
#include "Resource/Json/JsonCppLoaderExternal.h"

#include <fstream>
#include <vector>

struct JsonCppLoaderBsonOptions {
  void* storage[4];
};

struct JsonCppLoaderCommonJsonOptions {
  /* 0000 */ unsigned char storage[240];
};

struct JsonCppLoaderJsonOptions {
  /* 0000 */ unsigned char padding_0000[8];
  /* 0008 */ JsonCppLoaderCommonJsonOptions common;
};

static_assert(sizeof(JsonCppLoaderJsonOptions) == 248);
static_assert(offsetof(JsonCppLoaderJsonOptions, common) == 8);

struct JsonCppLoaderJsonHandler {
  unsigned char storage[40];

  // INLINED
  JsonCppLoaderJsonHandler() {
    reinterpret_cast<void(__thiscall*)(JsonCppLoaderJsonHandler*, JsonCppLoaderJsonHandler*)>(JsonDefaultParsingHandler)(this, this);
  }

  JsonCppLoaderJsonHandler(const JsonCppLoaderJsonHandler&) = delete;

  // INLINED
  ~JsonCppLoaderJsonHandler() {
  }
};

extern "C" {
JsonCppLoaderParsedValue __cdecl JsonDecodeBsonStream(std::ifstream* file, JsonCppLoaderBsonOptions* options);
JsonCppLoaderParsedValue __cdecl JsonDecodeBsonBuffer(const std::vector<unsigned char>* buffer, JsonCppLoaderBsonOptions* options);
JsonCppLoaderParsedValue __cdecl JsonParseStream(std::ifstream* file, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
JsonCppLoaderParsedValue __cdecl JsonParseString(const unsigned char* data, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
}

#endif

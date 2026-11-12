#ifndef RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADERPARSING_H
#define RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADERPARSING_H

#include "Resource/Json/JsonCppLoader.h"
#include "Resource/Json/JsonCppLoaderExternal.h"

#include <fstream>
#include <vector>

struct JsonCppLoaderBsonOptions {
  void* storage[4];
};

struct JsonCppLoaderJsonOptions {
  unsigned char storage[248];
};

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

struct JsonCppLoaderParsedValue {
  JsonValue value;

  // INLINED
  JsonCppLoaderParsedValue& operator=(JsonCppLoaderParsedValue&& other) {
    if (this != &other) {
      reinterpret_cast<void(__thiscall*)(JsonValue*, JsonValue*)>(JsonSwap)(&this->value, &other.value);
    }

    return *this;
  }

  // INLINED
  ~JsonCppLoaderParsedValue() {
    reinterpret_cast<void(__thiscall*)(JsonValue*)>(JsonDestroy)(&this->value);
  }
};

extern "C" {
JsonCppLoaderParsedValue __cdecl JsonDecodeBsonStream(std::ifstream* file, JsonCppLoaderBsonOptions* options);
JsonCppLoaderParsedValue __cdecl JsonDecodeBsonBuffer(const std::vector<unsigned char>* buffer, JsonCppLoaderBsonOptions* options);
JsonCppLoaderParsedValue __cdecl JsonParseStream(std::ifstream* file, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
JsonCppLoaderParsedValue __cdecl JsonParseString(const unsigned char* data, JsonCppLoaderJsonOptions* options, JsonCppLoaderJsonHandler handler);
}

#endif

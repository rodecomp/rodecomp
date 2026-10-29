#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_OPTIONWRITER_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_OPTIONWRITER_H

#include <windows.h>

class CFile;

struct OptionWriter {
  CFile& file;

  void Float(const char* key, float value);
  void Integer(const char* key, int value);
  void String(const char* key, const char* value);
  void Guid(const char* key, const GUID& value);
};

#endif

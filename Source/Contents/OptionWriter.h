#ifndef RAGNAROK_SOURCE_CONTENTS_OPTIONWRITER_H
#define RAGNAROK_SOURCE_CONTENTS_OPTIONWRITER_H

#include <windows.h>

class CFile;

// Borrowed original output-file owner. These routines preserve CRLF and the
// application's distinct integer/string/GUID formatting policies.
struct OptionWriter {
  CFile& file;

  void Integer(const char* key, int value);
  void String(const char* key, const char* value);
  void Guid(const char* key, const GUID& value);
};

#endif

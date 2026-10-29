#include "Contents/Configuration/OptionWriter.h"
#include "File/File.h"

#include <cstdio>
#include <cstring>
#include <string>

// OFFSET: 0x00E26B10

// OFFSET: 0x00E26C00
// FUNCTION: fn_23919
void OptionWriter::Integer(const char* key, int value) {
  char line[204];
  std::sprintf(line, "OptionInfoList[\"%s\"] = %d\r\n", key, value);
  this->file.Write(line, static_cast<unsigned>(std::strlen(line)));
}

// OFFSET: 0x00E26C70
// FUNCTION: fn_23920
void OptionWriter::String(const char* key, const char* value) {
  std::string escaped(value);
  std::size_t offset = 0;

  while ((offset = escaped.find('\\', offset)) != std::string::npos) {
    escaped.replace(offset, 1, "\\\\");
    offset += 2;
  }

  char line[200];
  const int count = _snprintf_s(line, sizeof(line), _TRUNCATE, "OptionInfoList[\"%s\"] = \"%s\"\r\n", key, escaped.c_str());

  if (count > 0) {
    this->file.Write(line, count);
  }
}

// OFFSET: 0x00E26B90
// FUNCTION: fn_23918
void OptionWriter::Guid(const char* key, const GUID& value) {
  char text[40];
  _snprintf_s(
    text, sizeof(text), _TRUNCATE, "{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}", value.Data1, value.Data2, value.Data3,
    value.Data4[0], value.Data4[1], value.Data4[2], value.Data4[3], value.Data4[4], value.Data4[5], value.Data4[6], value.Data4[7]
  );
  char line[200];
  const int count = _snprintf_s(line, sizeof(line), _TRUNCATE, "OptionInfoList[\"%s\"] = \"%s\"\r\n", key, text);

  if (count > 0) {
    this->file.Write(line, count);
  }
}

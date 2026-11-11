#include "Client/Session/Session.h"
#include "File/File.h"
#include "Utility/Text/GuidParse.h"
#include "Utility/Text/StringUtil.h"

#include <cstdio>

// OFFSET: 0x00E26B10
// FUNCTION: fn_23917
void CSession::WriteOptionFloat(CFile& file, const char* key, float value) {
  char line[200];
  std::sprintf(line, "OptionInfoList[\"%s\"] = %f\r\n", key, value);
  file.Write(line, static_cast<unsigned>(std::strlen(line)));
}

// OFFSET: 0x00E26C00
// FUNCTION: fn_23919
void CSession::WriteOptionInteger(CFile& file, const char* key, int value) {
  char line[200];
  std::sprintf(line, "OptionInfoList[\"%s\"] = %d\r\n", key, value);
  file.Write(line, static_cast<unsigned>(std::strlen(line)));
}

// OFFSET: 0x00E26C70
// FUNCTION: fn_23920
void CSession::WriteOptionString(CFile& file, const char* key, const char* value) {
  std::string escaped(value);
  ReplaceAll(escaped, "\\", "\\\\");

  char line[200];
  const int count = _snprintf_s(line, _TRUNCATE, "OptionInfoList[\"%s\"] = \"%s\"\r\n", key, escaped.c_str());

  if (count > 0) {
    file.Write(line, count);
  }
}

// OFFSET: 0x00E26B90
// FUNCTION: fn_23918
void CSession::WriteOptionGuid(CFile& file, const char* key, const GUID& value) {
  char text[40];
  FormatGuid(text, sizeof(text), value, true);

  char line[200];
  const int count = _snprintf_s(line, _TRUNCATE, "OptionInfoList[\"%s\"] = \"%s\"\r\n", key, text);

  if (count > 0) {
    file.Write(line, count);
  }
}

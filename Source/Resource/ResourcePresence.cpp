#include "File/ResourcePath.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/GroupedInteger.h"

#include <string>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00AEF940
// FUNCTION: fn_18182
bool ResourceMgr::Exists(std::string name) {
  const char* text = name.c_str();
  if (!text) {
    return false;
  }

  char normalized[260] = {};
  strncpy_s(normalized, sizeof(normalized), text, _TRUNCATE);

  for (unsigned char* byte = reinterpret_cast<unsigned char*>(normalized); *byte; ++byte) {
    if (*byte >= 'A' && *byte <= 'Z') {
      *byte += 'a' - 'A';
    }
  }

  const char* dot = ReverseFindChar(normalized, '.');
  const int type = this->FindType(dot ? dot + 1 : nullptr);
  if (type < 0) {
    return false;
  }

  const std::string resolved = AddResourcePrefix(normalized, this->m_prefixes[type]);
  if (CRes* existing = this->Find(type, resolved.c_str())) {
    existing->lastUse = timeGetTime();
    return true;
  }

  return ResourceExists(resolved.c_str(), 0);
}

#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/Base64.h"
#include "Utility/Text/LanguageCharset.h"

#include <cstdio>
#include <cstring>
#include <vector>

// STATIC: 0x016D5780
// GLOBAL: gl_24382
std::unordered_map<int, std::string> g_messageStrings;

namespace {
  const char* const messageNames[] = {
#include "Contents/MessageNames.inc"
};
  static_assert(sizeof(messageNames) / sizeof(messageNames[0]) == 4698);

  // OFFSET: 0x005B8500
  // FUNCTION: fn_03421
  void DecodeField(std::string& field, std::vector<std::string>& row) {
    if (field.empty()) {
      return;
    }

    std::string decoded = DecodeBase64(field);
    if (decoded.size() > 2) {
      if (decoded.front() == '"' && decoded.back() == '"') {
        decoded = decoded.substr(1, decoded.size() - 2);
      }

      for (std::size_t pos = 0; (pos = decoded.find("\\\"\"", pos)) != std::string::npos; ++pos) {
        decoded.replace(pos, 3, "\"");
      }
    }

    row.push_back(FromUtf8(decoded, g_textCodePage));
  }

  // OFFSET: 0x00B009D0
  // FUNCTION: fn_18373
  void TransformFields(std::vector<std::string>& row) {
    const char* escapes[] = {"\\r", "\\n", "\\t", "\\'", "\\\"", "\\\\"};
    const char replacements[] = {'\r', '\n', '\t', '\'', '"', '\\'};

    for (std::string& text : row) {
      if (text.empty()) {
        text = "NO MSG";
      } else {
        for (unsigned i = 0; i < 6; ++i) {
          for (std::size_t pos = 0; (pos = text.find(escapes[i], pos)) != std::string::npos; ++pos) {
            text.replace(pos, 2, 1, replacements[i]);
          }
        }
      }
    }
  }
}

// OFFSET: 0x00B00360

struct MessageBucketVector {
  void* begin;
  void* end;
  void* capacity;

  void Clear();
};

// OFFSET: 0x00B00520


// OFFSET: 0x00B00130
// FUNCTION: fn_18367
void InsertMessage(int id, const char* text) {
  g_messageStrings.emplace(id, text);
}

// OFFSET: 0x00B00F00
// FUNCTION: fn_18376
const char* LookupMessage(int id, bool languagePrefix, int serviceType, WORD language) {
  // STATIC: 0x016D57A0
  static char missing[20];
  if (id >= 4698) {
    _snprintf_s(missing, sizeof(missing), _TRUNCATE, "NO MSG : %d", id);
    return missing;
  }

  auto found = g_messageStrings.find(id);
  if (found == g_messageStrings.end()) {
    return "NO MSG";
  }

  const std::string& text = found->second;
  if (!languagePrefix || text[0] == '/' || text == "NO MSG") {
    return text.c_str();
  }

  // STATIC: 0x016D57B4
  // GLOBAL: gl_24383
  static std::vector<std::string> cached(4698);
  std::string& result = cached[id];
  if (result.empty() || serviceType == 19) {
    // STATIC: 0x016D57C8
    // GLOBAL: gl_24384
    static char buffer[260];
    char prefix[8];
    std::sprintf(prefix, "|%02x", LanguageCharset(language));
    _snprintf_s(buffer, sizeof(buffer), _TRUNCATE, "%s%s", prefix, text.c_str());
    result = buffer;
  }

  return result.c_str();
}

void InsertMessageRow(std::vector<std::string>& row);

bool ImportMessageCsv(std::string_view bytes) {
  g_messageStrings.clear();
  if (bytes.size() >= 2 && ((static_cast<unsigned char>(bytes[0]) == 0xFF && static_cast<unsigned char>(bytes[1]) == 0xFE) ||
                            (static_cast<unsigned char>(bytes[0]) == 0xFE && static_cast<unsigned char>(bytes[1]) == 0xFF))) {
    return false;
  }

  if (bytes.substr(0, 3) == "\xEF\xBB\xBF") {
    bytes.remove_prefix(3);
  }

  std::size_t cursor = 0;

  while (cursor < bytes.size()) {
    std::vector<std::string> row;
    std::string field;

    while (cursor < bytes.size()) {
      char value = bytes[cursor++];
      if (value == ',' || value == '\r' || value == '\n') {
        DecodeField(field, row);
        field.clear();
        if (value == '\n') {
          break;
        }

        if (value == '\r' && cursor < bytes.size() && bytes[cursor] == '\n') {
          ++cursor;
          break;
        }
      } else {
        field.push_back(value);
      }
    }

    DecodeField(field, row);
    TransformFields(row);
    InsertMessageRow(row);
  }

  return true;
}

// OFFSET: 0x00B00810
// FUNCTION: fn_18370
int FindMessageId(const char* name) {
  if (!name || *name == '\0') {
    return 4698;
  }

  for (int id = 0; id < 4699; ++id) {
    if (messageNames[id] && std::strcmp(messageNames[id], name) == 0) {
      return id;
    }
  }

  return 4698;
}

// OFFSET: 0x00B00890

// OFFSET: 0x00B00910

// OFFSET: 0x00B00E40

// OFFSET: 0x00B00EC0

// OFFSET: 0x00B011B0

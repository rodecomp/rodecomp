#include "Contents/SimpleStringMgr.h"
#include "File/CommonObject.h"
#include "Utility/Text/StringUtil.h"

#include <cstring>
#include <utility>

// GLOBAL: gl_24840
CSimpleStringMgr* g_simpleStringMgr = nullptr;

// GLOBAL: gl_24863
static CCommonObject g_simpleStringObject;

// OFFSET: 0x007A96A0
// FUNCTION: fn_09077
void CSimpleStringMgr::Destroy() {
  if (g_simpleStringMgr) {
    delete g_simpleStringMgr;
    g_simpleStringMgr = nullptr;
  }
}

// OFFSET: 0x007A9DA0
// FUNCTION: fn_09079
const char* CSimpleStringMgr::GetString(const char* key) {
  STR_TYPE type = STR_TYPE_EMOTION;
  if (key == nullptr || std::strlen(key) == 0) {
    return nullptr;
  }

  auto table = this->m_tables.find(type);
  if (table == this->m_tables.end()) {
    return nullptr;
  }

  auto entry = table->second.find(std::string(key, std::strlen(key)));
  if (entry == table->second.end()) {
    return nullptr;
  }

  return entry->second.c_str();
}

// OFFSET: 0x007A9780
std::vector<std::string> CSimpleStringMgr::TransformFields(const std::vector<std::string>& fields) {
  std::vector<std::string> result;
  result.reserve(fields.size());
  for (std::string field : fields) {
    if (field.empty()) {
      field = "NO MSG";
    } else {
      ReplaceAll(field, "\\r", "\r");
      ReplaceAll(field, "\\n", "\n");
      ReplaceAll(field, "\\t", "\t");
      ReplaceAll(field, "\\'", "'");
      ReplaceAll(field, "\\\"", "\"");
      ReplaceAll(field, "\\\\", "\\");
    }
    result.push_back(std::move(field));
  }
  return result;
}

// OFFSET: 0x007AA270
void CSimpleStringMgr::Init() {
  this->LoadCsv("simplemsg\\msg_emotion.csv",
                [this](const std::vector<std::string>& fields) { return this->TransformFields(fields); },
                [this](const std::vector<const char*>& fields, std::any context) {
                  if (static_cast<int>(fields.size()) >= 2) {
                    this->AddString(std::any_cast<STR_TYPE>(context), fields.front(), fields.back());
                  }
                },
                STR_TYPE_EMOTION);
}

// OFFSET: 0x007AA360
void CSimpleStringMgr::LoadCsv(const char* name, std::function<std::vector<std::string>(std::vector<std::string>&)> transform,
                              std::function<void(std::vector<const char*>&, std::any)> insert, std::any context) {
  g_simpleStringObject.LoadCsv(name, transform, insert, context);
}

// OFFSET: 0x007AA4A0
// FUNCTION: fn_09082
void CSimpleStringMgr::AddString(STR_TYPE type, const char* key, const char* value) {
  if (IsNullOrEmpty(key) || IsNullOrEmpty(value)) {
    return;
  }

  auto table = this->m_tables.find(type);
  if (table == this->m_tables.end()) {
    std::unordered_map<std::string, std::string> entries;
    entries.emplace(key, value);
    this->m_tables.emplace(type, std::move(entries));
  } else {
    table->second.emplace(std::string(key), value);
  }
}

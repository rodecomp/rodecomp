#ifndef RAGNAROK_SOURCE_CONTENTS_SIMPLESTRINGMGR_H
#define RAGNAROK_SOURCE_CONTENTS_SIMPLESTRINGMGR_H

#include <any>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// VTABLE: vt_00755
class CSimpleStringMgr {
public:
  enum STR_TYPE {
    STR_TYPE_EMOTION = 0,
  };

  struct StrTypeHash {
    std::size_t operator()(STR_TYPE type) const {
      return static_cast<std::size_t>(type);
    }
  };

  virtual ~CSimpleStringMgr() {
  }

  static void Destroy();
  void Init();
  void LoadCsv(const char* name, std::function<std::vector<std::string>(std::vector<std::string>&)> transform,
               std::function<void(std::vector<const char*>&, std::any)> insert, std::any context);
  std::vector<std::string> TransformFields(const std::vector<std::string>& fields);
  void AddString(STR_TYPE type, const char* key, const char* value);
  const char* GetString(const char* key);

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::unordered_map<STR_TYPE, std::unordered_map<std::string, std::string>, StrTypeHash> m_tables;
};

extern CSimpleStringMgr* g_simpleStringMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSimpleStringMgr) == 0x28);
#endif

#endif

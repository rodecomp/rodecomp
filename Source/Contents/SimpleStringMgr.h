#ifndef RAGNAROK_SOURCE_CONTENTS_SIMPLESTRINGMGR_H
#define RAGNAROK_SOURCE_CONTENTS_SIMPLESTRINGMGR_H

#include <cstddef>
#include <string>
#include <unordered_map>

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
  const char* GetString(const char* key);

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::unordered_map<STR_TYPE, std::unordered_map<std::string, std::string>, StrTypeHash> m_tables;
};

extern CSimpleStringMgr* g_simpleStringMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSimpleStringMgr) == 0x28);
#endif

#endif

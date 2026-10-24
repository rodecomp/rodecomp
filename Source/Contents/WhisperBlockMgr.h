#ifndef RAGNAROK_SOURCE_CONTENTS_WHISPERBLOCKMGR_H
#define RAGNAROK_SOURCE_CONTENTS_WHISPERBLOCKMGR_H

#include <cstdint>
#include <json/value.h>
#include <set>
#include <string>

class CWhisperBlockMgr {
public:
  CWhisperBlockMgr();
  virtual ~CWhisperBlockMgr();
  static bool Initialize();
  static void Shutdown();
  void Clear();
  void Add(std::string name);
  bool Contains(std::string name) const;
  void Remove(std::string name);
  void Reset();
  void Export(Json::Value& values) const;

protected:
  friend class CAccountLinkedUserDataMgr;
  // Original +4 is the compiler's polymorphic alignment gap, not a proven field.
  /* 0008 */ bool m_enabled;
  /* 0009 */ bool m_tracking;
  /* 0010 */ std::uint64_t m_changes;
  /* 0018 */ std::set<std::string> m_names;
};

// STATIC: 0x01389940
// GLOBAL: gl_24357
extern CWhisperBlockMgr* g_whisperBlockMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CWhisperBlockMgr) == 32);
#endif

#endif

#ifndef RAGNAROK_SOURCE_CONTENTS_MESSENGER_WHISPERBLOCKMGR_H
#define RAGNAROK_SOURCE_CONTENTS_MESSENGER_WHISPERBLOCKMGR_H

#include <cstdint>
#include <set>
#include <string>

#if __has_include(<json/value.h>)
#include <json/value.h>
#else
namespace Json {
class Value;
}
#endif

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

  /* 0008 */ bool m_enabled;
  /* 0009 */ bool m_tracking;
  /* 0010 */ std::uint64_t m_changes;
  /* 0018 */ std::set<std::string> m_names;
};

extern CWhisperBlockMgr* g_whisperBlockMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CWhisperBlockMgr) == 32);
#endif

#endif

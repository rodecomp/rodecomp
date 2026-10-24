#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMCACHE_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMCACHE_H

#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <vector>

class UIWindow;
class WindowManagerWindows;

// Borrowed CGuildMgr receive fields and invalid-resource set. Resource lookup,
// guild metadata, and CGuildMgr's singleton lifetime remain separate operations.
struct EmblemCache {
  std::uint32_t& guildId;           // CGuildMgr +0x1C
  std::uint32_t& version;           // CGuildMgr +0x20
  std::vector<unsigned char>& data; // CGuildMgr +0x24
  std::set<std::string>& invalid;   // CGuildMgr +0x14
  const std::string& accountName;
  const char* rootDirectory;
  const std::list<UIWindow*>* const& modeWindows;
  WindowManagerWindows& windows;

  std::string Filename(std::uint32_t guild, std::uint32_t revision) const;
  bool IsInvalid(std::string path) const;
  void MarkInvalid(std::string path);
  void Store(const void* bytes, std::uint32_t size, std::uint32_t guild, std::uint32_t revision);
  void Receive(std::uint16_t state, std::uint32_t guild, std::uint32_t revision, const unsigned char* bytes, int size);
  void Refresh(std::uint32_t guild);
};

#endif

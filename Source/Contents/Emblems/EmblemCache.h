#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMCACHE_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMCACHE_H

#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <vector>

class UIWindow;
class WindowManagerWindows;

struct EmblemCache {
  std::uint32_t& guildId;
  std::uint32_t& version;
  std::vector<unsigned char>& data;
  std::set<std::string>& invalid;
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

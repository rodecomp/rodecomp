#ifndef RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H

#include <cstdint>
#include <windows.h>

struct BinkHandle {
  std::uint32_t width;
  std::uint32_t height;
  std::uint32_t frames;
  std::uint32_t frame;
};

class BinkRuntime {
public:
  BinkRuntime();
  ~BinkRuntime();
  BinkRuntime(const BinkRuntime&) = delete;
  BinkRuntime& operator=(const BinkRuntime&) = delete;

  BinkHandle*(WINAPI* open)(const void*, unsigned);
  void(WINAPI* close)(BinkHandle*);
  const char*(WINAPI* error)();
  int(WINAPI* wait)(BinkHandle*);
  int(WINAPI* doFrame)(BinkHandle*);
  int(WINAPI* shouldSkip)(BinkHandle*);
  void(WINAPI* nextFrame)(BinkHandle*);
  void(WINAPI* copyToBuffer)(BinkHandle*, void*, int, unsigned, unsigned, unsigned, unsigned);
  void(WINAPI* setVolume)(BinkHandle*, unsigned, int);

private:
  HMODULE m_module;
};

#endif

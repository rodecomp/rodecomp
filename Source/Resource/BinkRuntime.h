#ifndef RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H

#include <cstdint>
#include <windows.h>

// Public prefix read/written by the original wrapper, not the decoder's
// complete private allocation. Supplied binkw32.dll is x86-only.
struct BinkHandle {
  std::uint32_t width;
  std::uint32_t height;
  std::uint32_t frames;
  std::uint32_t frame;
};

// Host loader for original imported APIs; no fabricated original entry.
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

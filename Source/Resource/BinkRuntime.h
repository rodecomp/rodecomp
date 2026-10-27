#ifndef RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H

#include <cstdint>
#include <system_error>
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

// INLINED
inline BinkRuntime::BinkRuntime() : m_module(LoadLibraryA("binkw32.dll")) {
  if (!this->m_module) {
    throw std::system_error(GetLastError(), std::system_category(), "binkw32.dll");
  }

  this->open = reinterpret_cast<decltype(this->open)>(GetProcAddress(this->m_module, "_BinkOpen@8"));
  this->close = reinterpret_cast<decltype(this->close)>(GetProcAddress(this->m_module, "_BinkClose@4"));
  this->error = reinterpret_cast<decltype(this->error)>(GetProcAddress(this->m_module, "_BinkGetError@0"));
  this->wait = reinterpret_cast<decltype(this->wait)>(GetProcAddress(this->m_module, "_BinkWait@4"));
  this->doFrame = reinterpret_cast<decltype(this->doFrame)>(GetProcAddress(this->m_module, "_BinkDoFrame@4"));
  this->shouldSkip = reinterpret_cast<decltype(this->shouldSkip)>(GetProcAddress(this->m_module, "_BinkShouldSkip@4"));
  this->nextFrame = reinterpret_cast<decltype(this->nextFrame)>(GetProcAddress(this->m_module, "_BinkNextFrame@4"));
  this->copyToBuffer = reinterpret_cast<decltype(this->copyToBuffer)>(GetProcAddress(this->m_module, "_BinkCopyToBuffer@28"));
  this->setVolume = reinterpret_cast<decltype(this->setVolume)>(GetProcAddress(this->m_module, "_BinkSetVolume@12"));

  if (!this->open || !this->close || !this->error || !this->wait || !this->doFrame || !this->shouldSkip || !this->nextFrame || !this->copyToBuffer ||
      !this->setVolume) {
    FreeLibrary(this->m_module);
    throw std::system_error(ERROR_PROC_NOT_FOUND, std::system_category(), "binkw32 exports");
  }
}

// INLINED
inline BinkRuntime::~BinkRuntime() {
  FreeLibrary(this->m_module);
}

#endif

#ifndef RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_BINKRUNTIME_H

#include <cstddef>
#include <cstdint>
#include <windows.h>

struct BinkHandle {
  /* 0000 */ std::uint32_t width;
  /* 0004 */ std::uint32_t height;
  /* 0008 */ std::uint32_t frames;
  /* 000C */ std::uint32_t frame;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(BinkHandle, frames) == 0x08);
static_assert(offsetof(BinkHandle, frame) == 0x0C);
#endif

extern "C" {
__declspec(dllimport) BinkHandle* WINAPI BinkOpen(const char* name, std::uint32_t flags);
__declspec(dllimport) void WINAPI BinkClose(BinkHandle* handle);
__declspec(dllimport) char* WINAPI BinkGetError();
__declspec(dllimport) std::int32_t WINAPI BinkWait(BinkHandle* handle);
__declspec(dllimport) std::int32_t WINAPI BinkDoFrame(BinkHandle* handle);
__declspec(dllimport) std::int32_t WINAPI BinkShouldSkip(BinkHandle* handle);
__declspec(dllimport) void WINAPI BinkNextFrame(BinkHandle* handle);
__declspec(dllimport) std::int32_t WINAPI BinkCopyToBuffer(
  BinkHandle* handle, void* destination, std::int32_t pitch, std::uint32_t height, std::uint32_t x, std::uint32_t y, std::uint32_t flags
);
__declspec(dllimport) void WINAPI BinkSetVolume(BinkHandle* handle, std::uint32_t track, std::int32_t volume);
}

#endif

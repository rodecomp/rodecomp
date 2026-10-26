#ifndef RAGNAROK_SOURCE_UTILITY_NFILELOG_H
#define RAGNAROK_SOURCE_UTILITY_NFILELOG_H

#include <cstdint>
#include <windows.h>

class NFileLog {
public:
  // INLINED
  virtual ~NFileLog() {
    DeleteCriticalSection(&this->criticalSection);
  }

  bool Initialize(const char* directoryName, const char* prefix);

  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ CRITICAL_SECTION criticalSection;
  /* 0020 */ char directory[1024];
  /* 0420 */ char prefix[1024];
  /* 0820 */ int year;
  /* 0824 */ int month;
  /* 0828 */ int day;
  /* 082C */ int fileIndex;
  /* 0830 */ int fileSize;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NFileLog) == 0x834);
#endif

#endif

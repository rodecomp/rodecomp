#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKREGISTRY_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKREGISTRY_H

#include "Archive/GPak.h"
#include "Archive/MemMapPakFile.h"

#include <list>
#include <utility>

class PakRegistry {
public:
  ~PakRegistry();
  bool Add(const char* filename);
  bool Exists(const char* filename, bool looseOnly) const;
  void* Read(const char* filename, std::uint32_t* size, int option = 0);
  __declspec(noinline) void* ReadArchive(const char* filename, std::uint32_t* size);
  BOOL Free(void* buffer);
  void ApplyAuxiliary(const void* data, std::uint32_t size);

private:
  std::list<std::pair<CMemMapPakFile*, CGPak*>> m_archives;
};

extern PakRegistry g_pakRegistry;

#endif

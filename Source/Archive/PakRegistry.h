#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKREGISTRY_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKREGISTRY_H

#include "Archive/GPak.h"
#include "Archive/MemMapPakFile.h"

#include <list>
#include <utility>

// Reconstructed owner name; original state is a list of file/archive pointers.
class PakRegistry {
public:
  ~PakRegistry();
  bool Add(const char* filename);
  bool Exists(const char* filename, bool looseOnly) const;
  void* Read(const char* filename, std::uint32_t* size, int option = 0);
  __declspec(noinline) void* ReadArchive(const char* filename, std::uint32_t* size);
  static void __stdcall Free(void* buffer);

private:
  std::list<std::pair<CMemMapPakFile*, CGPak*>> m_archives;
};

extern PakRegistry g_pakRegistry;

#endif

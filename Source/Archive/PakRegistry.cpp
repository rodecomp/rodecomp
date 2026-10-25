#include "Archive/PakRegistry.h"
#include "File/ResourcePath.h"

// STATIC: 0x016D5688
// GLOBAL: gl_24380
PakRegistry g_pakRegistry;

// INLINED
PakRegistry::~PakRegistry() {
  for (auto& archive : this->m_archives) {
    delete archive.first;
    delete archive.second;
  }

  this->m_archives.clear();
}

// OFFSET: 0x00AE99D0
// FUNCTION: fn_18111
bool PakRegistry::Add(const char* filename) {
  CGPak* archive = new CGPak;
  CMemMapPakFile* file = new CMemMapPakFile;
  if (file) {
    file->Open(filename);
  }

  this->m_archives.emplace_front(file, archive);
  return this->m_archives.front().second->Open(this->m_archives.front().first);
}

// OFFSET: 0x00AE9BF0
// FUNCTION: fn_18112
void* PakRegistry::Read(const char* filename, std::uint32_t* size, int) {
  void* result = this->ReadArchive(filename, size);
  if (result) {
    return result;
  }

  return ReadLooseResource(filename, size);
}

// OFFSET: 0x00AE9FE0
// FUNCTION: fn_18114
void* PakRegistry::ReadArchive(const char* filename, std::uint32_t* size) {
  for (const auto& archive : this->m_archives) {
    myGPack::PakEntry entry;
    myGPack::PakName name;
    name.Assign(filename);

    if (archive.second->Find(name, &entry)) {
      *size = entry.originalSize;

      void* buffer = VirtualAlloc(nullptr, entry.originalSize + 8, MEM_COMMIT, PAGE_READWRITE);
      if (archive.second->Extract(&entry, buffer)) {
        return buffer;
      }

      VirtualFree(buffer, 0, MEM_RELEASE);
    }
  }

  return nullptr;
}

// OFFSET: 0x00AEA0E0
// FUNCTION: fn_18115
bool PakRegistry::Exists(const char* filename, bool looseOnly) const {
  if (!looseOnly) {
    for (const auto& archive : this->m_archives) {
      myGPack::PakName name;
      name.Assign(filename);
      if (archive.second->Find(name, nullptr)) {
        return true;
      }
    }
  }

  WIN32_FIND_DATAA information;
  HANDLE search = FindFirstFileA(filename, &information);
  if (search == INVALID_HANDLE_VALUE) {
    return false;
  }

  FindClose(search);
  return true;
}

// FUNCTION: fn_18116
void PakRegistry::ApplyAuxiliary(const void*, std::uint32_t) {
}

// OFFSET: 0x00AEA400
// FUNCTION: fn_18118
BOOL PakRegistry::Free(void* buffer) {
  return VirtualFree(buffer, 0, MEM_RELEASE);
}

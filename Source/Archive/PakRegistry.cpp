#include "Archive/PakRegistry.h"
#include "File/ResourcePath.h"

// STATIC: 0x016D5688
// GLOBAL: gl_24380
// Initializer constructs the list and registers teardown.
PakRegistry g_pakRegistry;

// OFFSET: 0x01095B80
// FUNCTION: fn_41403
PakRegistry::~PakRegistry() {
  for (auto& archive : this->m_archives) {
    // Preserve original file-before-parser destruction order.
    delete archive.first;
    delete archive.second;
  }
}

// OFFSET: 0x00AE99D0
bool PakRegistry::Add(const char* filename) {
  CGPak* archive = new CGPak;
  CMemMapPakFile* file = new CMemMapPakFile;
  // The original retains the list item even if file opening or parsing fails.
  file->Open(filename);
  this->m_archives.emplace_back(file, archive);
  return archive->Open(file);
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
    myGPack::PakName name;
    name.Assign(filename);

    myGPack::PakEntry entry;
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

// OFFSET: 0x00AEA400
// FUNCTION: fn_18118
void __stdcall PakRegistry::Free(void* buffer) {
  VirtualFree(buffer, 0, MEM_RELEASE);
}

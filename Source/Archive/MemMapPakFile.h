#ifndef RAGNAROK_SOURCE_ARCHIVE_MEMMAPPAKFILE_H
#define RAGNAROK_SOURCE_ARCHIVE_MEMMAPPAKFILE_H

#include "Archive/PakFile.h"

class CMemMapPakFile : public CPakFile {
public:
  // INLINED
  explicit CMemMapPakFile(const char* filename) {
    this->Initialize();
    this->Open(filename);
  }
  ~CMemMapPakFile() override;

  void Initialize();
  bool Open(const char* filename);
  __declspec(noinline) void Close();
  std::uint64_t GetSize() const override;
  std::uint8_t* Read(std::uint64_t offset, std::uint32_t length) override;

protected:
  /* 0004 */ HANDLE m_handle = nullptr;
  /* 0008 */ HANDLE m_mapping = nullptr;
  /* 000C */ std::uint32_t m_viewSize = 0;
  /* 0010 */ std::uint32_t m_windowSize = 0;
  /* 0018 */ std::uint64_t m_size = 0;
  /* 0020 */ std::uint64_t m_viewOffset = 0;
  /* 0028 */ std::uint64_t m_offsetMask = 0;
  /* 0030 */ std::uint8_t* m_view = nullptr;
  /* 0034 */ std::vector<std::uint8_t> m_buffer;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMemMapPakFile) == 64);
#endif

#endif

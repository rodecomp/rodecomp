#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKFILE_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKFILE_H

#include <cstdint>
#include <vector>
#include <windows.h>

// Class names and inheritance are established by original RTTI.
// Method and member names are reconstructed from their behavior.
class CPakFile {
public:
  virtual ~CPakFile();
  virtual std::uint64_t GetSize() const = 0;
  virtual std::uint8_t* Read(std::uint64_t offset, std::uint32_t length) = 0;
};

namespace myGPack {

  class CReadPakFile : public CPakFile {
  public:
    CReadPakFile();
    ~CReadPakFile() override;

    std::uint64_t GetSize() const override;
    std::uint8_t* Read(std::uint64_t offset, std::uint32_t length) override;
    void Close();

  protected:
    /* 0004 */ HANDLE m_handle;
    /* 0008 */ std::uint64_t m_size;
    /* 0010 */ std::vector<std::uint8_t> m_buffer;
  };

  class CWritePakFile : public CReadPakFile {
  public:
    ~CWritePakFile() override;

    bool Open(const char* filename);
    bool Write(std::uint64_t offset, const void* bytes, std::uint32_t length);
  };

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CPakFile) == 4);
static_assert(sizeof(std::vector<std::uint8_t>) == 12);
static_assert(sizeof(myGPack::CReadPakFile) == 32);
static_assert(sizeof(myGPack::CWritePakFile) == 32);
#endif

#endif

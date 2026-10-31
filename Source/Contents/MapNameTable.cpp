#include "Client/Session.h"
#include "Contents/MapInfoMgr.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

class TableBytes : public ResourceBytes {
public:
  CFile file;

  // INLINED
  bool Open(const char* name) override {
    return this->file.Open(name, 0, true, false);
  }

  // INLINED
  const void* Data() const override {
    return this->file.GetData();
  }

  // INLINED
  int Size() const override {
    return static_cast<int>(this->file.GetSize());
  }

  // INLINED
  void Close() override {
    this->file.Close();
  }
};

}

// OFFSET: 0x00DE8740
// FUNCTION: fn_23498
bool CSession::LoadMapNameTable() {
  std::vector<char*> tokens;
  CCommonObject& storage = reinterpret_cast<CCommonObject&>(this->unknown_5400[0x30]);
  const std::uint8_t source[] = {0x52, 0x88, 0x4B, 0xB1, 0x0A, 0x62, 0x9E, 0x43, 0x5E,
                                 0x8B, 0x57, 0x9A, 0x45, 0x7B, 0x83, 0x63, 0x3F};
  thread_local EncryptedName<sizeof(source), 0x17FB0F6BFF3BE93FULL> name(source);
  TableBytes file;
  const bool loaded = storage.LoadTokens(tokens, name.Text(), 0, file);

  for (auto token = tokens.begin(); token != tokens.end(); token += 2) {
    g_mapInfoMgr->AddMapDisplayName(token[0], token[1], false);
  }

  return loaded;
}

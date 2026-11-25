#ifndef RAGNAROK_SOURCE_ARCHIVE_GPAKEX_H
#define RAGNAROK_SOURCE_ARCHIVE_GPAKEX_H

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace GPakEx {

  enum CipherDir {
    ENCRYPTION = 0,
    DECRYPTION = 1,
  };

  // INLINED
  template <class T> inline T* PtrAdd(T* pointer, std::ptrdiff_t offset) {
    return pointer ? pointer + offset : nullptr;
  }

  __declspec(noinline) void XorBuffers(std::uint8_t* output, const std::uint8_t* input, const std::uint8_t* mask, unsigned int length);
  void XorBuffers(std::uint8_t* buffer, const std::uint8_t* mask, unsigned int length);
  void ByteReverse(std::uint32_t* output, const std::uint32_t* input, unsigned int byteCount);

  class SecureByteBuffer {
  public:
    /* 0000 */ std::vector<std::uint8_t> m_bytes;

    ~SecureByteBuffer();

    // INLINED
    std::uint8_t* data() { return this->m_bytes.data(); }
    // INLINED
    const std::uint8_t* data() const { return this->m_bytes.data(); }
    // INLINED
    unsigned int size() const { return this->m_bytes.size(); }
    // INLINED
    void resize(unsigned int size) { this->m_bytes.resize(size); }
    // INLINED
    void swap(SecureByteBuffer& other) { this->m_bytes.swap(other.m_bytes); }
    // INLINED
    std::uint8_t& operator[](unsigned int index) { return this->m_bytes[index]; }
  };

  static_assert(sizeof(SecureByteBuffer) == 12);

  class SecureWordBuffer {
  public:
    /* 0000 */ std::vector<std::uint32_t> m_words;

    // INLINED
    ~SecureWordBuffer() { SecureZeroMemory(this->m_words.data(), this->m_words.size() * sizeof(std::uint32_t)); }

    // INLINED
    std::uint32_t* data() { return this->m_words.data(); }
    // INLINED
    const std::uint32_t* data() const { return this->m_words.data(); }
    // INLINED
    unsigned int size() const { return this->m_words.size(); }
    // INLINED
    void resize(unsigned int size) { this->m_words.resize(size); }
    // INLINED
    std::uint32_t& operator[](unsigned int index) { return this->m_words[index]; }
    // INLINED
    std::uint32_t operator[](unsigned int index) const { return this->m_words[index]; }
  };

  static_assert(sizeof(SecureWordBuffer) == 12);

  template <class T, unsigned int N> class FixedSecureBlock {
  public:
    /* 0000 */ T m_data[N];

    // INLINED
    ~FixedSecureBlock() { SecureZeroMemory(this->m_data, sizeof(this->m_data)); }

    // INLINED
    T& operator[](unsigned int index) { return this->m_data[index]; }
    // INLINED
    const T& operator[](unsigned int index) const { return this->m_data[index]; }
  };

  class BlockCipher {
  public:
    enum Flags {
      XorInput = 1,
      ReverseDirection = 2,
      InBlockIsCounter = 4,
    };

    __declspec(noinline) virtual ~BlockCipher();
    virtual bool IsForwardTransformation() const = 0;
    virtual unsigned int BlockSize() const = 0;
    virtual unsigned int KeyLength() const = 0;
    virtual void SetKey(std::span<const std::uint8_t> key) = 0;
    virtual void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const = 0;
    virtual unsigned int AdvancedProcessBlocks(const std::uint8_t* inBlocks, const std::uint8_t* xorBlocks, std::uint8_t* outBlocks, unsigned int length, unsigned int flags) const;
  };

  static_assert(sizeof(BlockCipher) == 4);

  template <CipherDir DIR, class BASE> class BlockCipherFinal : public BASE {
  public:
    ~BlockCipherFinal() override = default;
    bool IsForwardTransformation() const override;
  };

  class CipherModeBase {
  public:
    /* 0004 */ BlockCipher* m_cipher;
    /* 0008 */ SecureByteBuffer m_register;
    /* 0014 */ SecureByteBuffer m_buffer;

    // INLINED
    CipherModeBase() : m_cipher(nullptr) {}
    virtual ~CipherModeBase();
    virtual bool IsForwardTransformation() const = 0;
    virtual void ResizeBuffers();
    virtual void ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) = 0;

    unsigned int CiphertextLength(unsigned int length, bool pad) const;
    unsigned int Process(std::span<const std::uint8_t> input, std::span<std::uint8_t> output, bool pad);
    void SetKey(std::span<const std::uint8_t> key);
    void SetKeyWithIV(std::span<const std::uint8_t> key, std::span<const std::uint8_t> iv);
  };

  static_assert(sizeof(CipherModeBase) == 0x20);

  class CBC_Encryption : public CipherModeBase {
  public:
    ~CBC_Encryption() override = default;
    void ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) override;
  };

  static_assert(sizeof(CBC_Encryption) == 0x20);

  class CBC_Decryption : public CipherModeBase {
  public:
    /* 0020 */ SecureByteBuffer m_temp;

    CBC_Decryption();
    ~CBC_Decryption() override = default;
    void ResizeBuffers() override;
    void ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) override;
  };

  static_assert(sizeof(CBC_Decryption) == 0x2C);

  class CFB_Decryption : public CipherModeBase {
  public:
    /* 0020 */ SecureByteBuffer m_temp;

    CFB_Decryption();
    ~CFB_Decryption() override = default;
    void ResizeBuffers() override;
    void ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) override;
  };

  static_assert(sizeof(CFB_Decryption) == 0x2C);

  class OFB_Base : public CipherModeBase {
  public:
    OFB_Base();
    ~OFB_Base() override = default;
    void ProcessData(const std::uint8_t* input, std::uint8_t* output, unsigned int length) override;
  };

  static_assert(sizeof(OFB_Base) == 0x20);

  template <class CIPHER, CipherDir DIR, class BLOCKCIPHER, class MODE> class CipherModeFinal : public MODE {
  public:
    BLOCKCIPHER m_object;

    CipherModeFinal();
    ~CipherModeFinal() override;
    bool IsForwardTransformation() const override;
  };

  class GOST {
  public:
    class Base : public BlockCipher {
    public:
      static const std::uint8_t s_sbox[8][16];
      static bool s_sTableCalculated;
      static unsigned int s_sTable[4][256];

      static void PrecalculateSTable();

      /* 0004 */ FixedSecureBlock<std::uint32_t, 8> m_key{};

      ~Base() override = default;
      unsigned int BlockSize() const override;
      unsigned int KeyLength() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
    };

    class Enc : public Base {
    public:
      ~Enc() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };

    class Dec : public Base {
    public:
      ~Dec() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };
  };

  static_assert(sizeof(GOST::Base) == 0x24);

  class IDEA {
  public:
    enum {
      ROUNDS = 8,
    };

    class Base : public BlockCipher {
    public:
      /* 0004 */ FixedSecureBlock<std::uint32_t, 6 * ROUNDS + 4> m_key{};

      ~Base() override = default;
      unsigned int BlockSize() const override;
      unsigned int KeyLength() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;

      void DeKey();
      void EnKey(const std::uint8_t* userKey);
    };
  };

  static_assert(sizeof(IDEA::Base) == 0xD4);

  class RC6 {
  public:
    enum {
      ROUNDS = 20,
    };

    class Base : public BlockCipher {
    public:
      /* 0004 */ SecureWordBuffer m_sTable;

      __declspec(noinline) ~Base() override;
      unsigned int BlockSize() const override;
      unsigned int KeyLength() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
    };

    class Enc : public Base {
    public:
      ~Enc() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };
  };

  static_assert(sizeof(RC6::Base) == 0x10);

  class RC5 {
  public:
    enum {
      ROUNDS = 20,
    };

    class Base : public BlockCipher {
    public:
      /* 0004 */ SecureWordBuffer m_sTable;

      __declspec(noinline) ~Base() override;
      unsigned int BlockSize() const override;
      unsigned int KeyLength() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
    };

    class Enc : public Base {
    public:
      ~Enc() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };
  };

  static_assert(sizeof(RC5::Base) == 0x10);

  class TEA {
  public:
    enum {
      ROUNDS = 16,
    };

    class Base : public BlockCipher {
    public:
      /* 0004 */ FixedSecureBlock<std::uint32_t, 4> m_k{};

      ~Base() override = default;
      unsigned int BlockSize() const override;
      unsigned int KeyLength() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
    };

    class Enc : public Base {
    public:
      ~Enc() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };
  };

  static_assert(sizeof(TEA::Base) == 0x14);

  class XTEA {
  public:
    enum {
      ROUNDS = 16,
    };

    class Base : public BlockCipher {
    public:
      /* 0004 */ FixedSecureBlock<std::uint32_t, 4> m_k{};

      ~Base() override = default;
      unsigned int BlockSize() const override;
      unsigned int KeyLength() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
    };

    class Enc : public Base {
    public:
      ~Enc() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };
  };

  static_assert(sizeof(XTEA::Base) == 0x14);

  class ThreeWay {
  public:
    enum {
      ROUNDS = 11,
    };

    class Base : public BlockCipher {
    public:
      /* 0004 */ FixedSecureBlock<std::uint32_t, 3> m_k{};

      ~Base() override = default;
      unsigned int BlockSize() const override;
      unsigned int KeyLength() const override;
      void SetKey(std::span<const std::uint8_t> key) override;
    };

    class Enc : public Base {
    public:
      ~Enc() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };

    class Dec : public Base {
    public:
      ~Dec() override = default;
      void ProcessAndXorBlock(const std::uint8_t* inBlock, const std::uint8_t* xorBlock, std::uint8_t* outBlock) const override;
    };
  };

  static_assert(sizeof(ThreeWay::Base) == 0x10);

}

#endif

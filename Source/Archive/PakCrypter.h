#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKCRYPTER_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKCRYPTER_H

#include "Archive/GPakEx.h"

#include <cstddef>
#include <cstdint>
#include <span>

namespace GPakEx {

  // INLINED
  template <std::uint32_t KEY1, std::uint32_t KEY2, class T> inline T Obfuscated(T value) {
    volatile T masked = value ^ KEY1 ^ KEY2;
    masked = masked ^ KEY1;
    masked = masked ^ KEY2;
    return masked;
  }

  template <class CIPHER> struct CipherSizes;

  template <> struct CipherSizes<IDEA> {
    enum {
      BLOCKSIZE = 8,
      DEFAULT_KEYLENGTH = 16,
    };
  };

  template <> struct CipherSizes<RC5> {
    enum {
      BLOCKSIZE = 8,
      DEFAULT_KEYLENGTH = 16,
    };
  };

  template <> struct CipherSizes<RC6> {
    enum {
      BLOCKSIZE = 16,
      DEFAULT_KEYLENGTH = 16,
    };
  };

  template <> struct CipherSizes<TEA> {
    enum {
      BLOCKSIZE = 8,
      DEFAULT_KEYLENGTH = 16,
    };
  };

  template <> struct CipherSizes<XTEA> {
    enum {
      BLOCKSIZE = 8,
      DEFAULT_KEYLENGTH = 16,
    };
  };

  template <> struct CipherSizes<ThreeWay> {
    enum {
      BLOCKSIZE = 12,
      DEFAULT_KEYLENGTH = 12,
    };
  };

  struct CBC_Mode {
    template <class CIPHER> using Decryption = CipherModeFinal<CIPHER, DECRYPTION, BlockCipherFinal<DECRYPTION, typename CIPHER::Dec>, CBC_Decryption>;
  };

  struct CFB_Mode {
    template <class CIPHER> using Decryption = CipherModeFinal<CIPHER, DECRYPTION, BlockCipherFinal<ENCRYPTION, typename CIPHER::Enc>, CFB_Decryption>;
  };

  struct OFB_Mode {
    template <class CIPHER> using Decryption = CipherModeFinal<CIPHER, DECRYPTION, BlockCipherFinal<ENCRYPTION, typename CIPHER::Enc>, OFB_Base>;
  };

  class CrypterBase {
  public:
    virtual int Id() const = 0;
    virtual bool Decrypt(
      std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
      std::span<std::uint8_t> output
    ) const = 0;
  };

  static_assert(sizeof(CrypterBase) == 4);

  class Crypter_SUB_XOR : public CrypterBase {
  };

  template <
    int ID, int AES_MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET, std::size_t PLAIN_SIZE,
    std::size_t BLOCK_SIZE>
  class Crypter_SUB_XOR_AES_A final : public Crypter_SUB_XOR {
  public:
    int Id() const override;
    bool Decrypt(
      std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
      std::span<std::uint8_t> output
    ) const override;
  };

  template <
    int ID, class CIPHER, class MODE, std::size_t KEY_OFFSET, std::size_t IV_OFFSET, class MASK_CIPHER, bool MASK_BY_NAME, std::size_t MASK_IV_OFFSET,
    std::size_t PLAIN_SIZE, std::size_t BLOCK_SIZE>
  class Crypter_SUB_XOR_BlockCipher_A final : public Crypter_SUB_XOR {
  public:
    int Id() const override;
    bool Decrypt(
      std::span<const std::uint8_t> name, std::span<const std::uint8_t, 60> parameters, std::span<const std::uint8_t, 16> hash, std::span<const std::uint8_t> input,
      std::span<std::uint8_t> output
    ) const override;
  };

}

#endif

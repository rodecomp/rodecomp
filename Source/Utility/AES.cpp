#include "Utility/AES.h"

#include <cstring>

// OFFSET: 0x0061AE00
// FUNCTION: fn_04417
AES::~AES() {
  if (m_key != nullptr) {
    BCryptDestroyKey(m_key);
    m_key = nullptr;
  }
  if (m_algorithm != nullptr) {
    BCryptCloseAlgorithmProvider(m_algorithm, 0);
    m_algorithm = nullptr;
  }
}

// OFFSET: 0x0061AF30
// FUNCTION: fn_04418
ULONG AES::Decrypt(PUCHAR input, ULONG inputSize, PUCHAR output, ULONG outputSize, UCHAR flags) {
  PUCHAR iv;
  ULONG ivSize;
  if (m_mode != ECB) {
    iv = m_iv.data();
    ivSize = m_iv.size();
  } else {
    iv = nullptr;
    ivSize = 0;
  }

  ULONG result = 0;
  UCHAR scratch[16]{};
  const NTSTATUS status = BCryptDecrypt(m_key, input, inputSize, nullptr, iv, ivSize, outputSize != 0 ? output : scratch, outputSize, &result, flags);
  return status < 0 ? static_cast<ULONG>(-1) : result;
}

// OFFSET: 0x0061B1A0
// FUNCTION: fn_04419
bool AES::Initialize(PUCHAR secret, ULONG secretSize, const UCHAR* iv, ULONG ivSize, int mode) {
  if (m_key != nullptr) {
    BCryptDestroyKey(m_key);
    m_key = nullptr;
  }
  if (m_algorithm != nullptr) {
    BCryptCloseAlgorithmProvider(m_algorithm, 0);
    m_algorithm = nullptr;
  }

  if (!SetIV(iv, ivSize)) {
    return false;
  }
  if (BCryptOpenAlgorithmProvider(&m_algorithm, BCRYPT_AES_ALGORITHM, nullptr, 0) < 0) {
    return false;
  }

  ULONG objectLength = 0;
  ULONG result = 0;
  if (BCryptGetProperty(m_algorithm, BCRYPT_OBJECT_LENGTH, reinterpret_cast<PUCHAR>(&objectLength), sizeof(objectLength), &result, 0) < 0) {
    return false;
  }
  m_keyObject.resize(objectLength);
  if (BCryptGenerateSymmetricKey(m_algorithm, &m_key, m_keyObject.data(), m_keyObject.size(), secret, secretSize, 0) < 0) {
    return false;
  }
  return SetMode(mode);
}

// OFFSET: 0x0061B3C0
// FUNCTION: fn_04420
bool AES::Initialize(PUCHAR secret, ULONG secretSize, int mode) {
  UCHAR iv[16]{};
  return Initialize(secret, secretSize, iv, sizeof(iv), mode);
}

// OFFSET: 0x0061B410
// FUNCTION: fn_04421
bool AES::SetIV(const UCHAR* iv, ULONG ivSize) {
  if (ivSize != 16) {
    return false;
  }
  m_iv.resize(16);
  std::memcpy(m_iv.data(), iv, 16);
  return true;
}

// OFFSET: 0x0061B440
// FUNCTION: fn_04422
bool AES::SetMode(int mode) {
  if (!IsWindows8OrGreater()) {
    if (mode == CBC) {
      m_mode = CBC;
      return true;
    }
    return false;
  }

  const wchar_t* chainingMode;
  switch (mode) {
    case ECB:
      chainingMode = BCRYPT_CHAIN_MODE_ECB;
      break;
    case CBC:
      chainingMode = BCRYPT_CHAIN_MODE_CBC;
      break;
    case CFB:
      chainingMode = BCRYPT_CHAIN_MODE_CFB;
      break;
    default:
      return false;
  }
  if (BCryptSetProperty(m_key, BCRYPT_CHAINING_MODE, reinterpret_cast<PUCHAR>(const_cast<wchar_t*>(chainingMode)), 32, 0) < 0) {
    return false;
  }
  m_mode = mode;
  return true;
}

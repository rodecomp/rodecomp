#include "Archive/PakCrypto.h"

#include <cstring>
#include <wincrypt.h>

namespace myGPack {
  // FUNCTION: fn_18121
  int DecryptPakData(const BYTE* secret, DWORD secretSize, const BYTE* input, BYTE* output, DWORD dataSize) {
    HCRYPTPROV provider = 0;
    HCRYPTKEY key = 0;
    HCRYPTHASH hash = 0;
    DWORD result = 0;
    std::memcpy(output, input, dataSize);
    try {
      if (!CryptAcquireContextA(&provider, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        throw GetLastError();
      }
      if (!CryptCreateHash(provider, CALG_MD5, 0, 0, &hash)) {
        throw GetLastError();
      }
      if (!CryptHashData(hash, secret, secretSize, 0)) {
        throw GetLastError();
      }
      if (!CryptDeriveKey(provider, CALG_RC4, hash, CRYPT_EXPORTABLE, &key)) {
        throw GetLastError();
      }
      if (!CryptDecrypt(key, 0, TRUE, 0, output, &dataSize)) {
        throw GetLastError();
      }
    } catch (DWORD error) {
      result = error;
    }
    if (key != 0) {
      CryptDestroyKey(key);
    }
    if (hash != 0) {
      CryptDestroyHash(hash);
    }
    if (provider != 0) {
      CryptReleaseContext(provider, 0);
    }
    return result;
  }
}

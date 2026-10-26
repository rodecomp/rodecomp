#include "Utility/FileSignatureVerifier.h"

#include <cstdlib>
#include <cstring>

// STRING: st_12997
const WCHAR aMicrosoftBaseC[] = L"Microsoft Base Cryptographic Provider v1.0";

// GLOBAL: gl_24467
BYTE g_rsaPublicKeyBlob[] = {
  0x06, 0x02, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x52, 0x53, 0x41, 0x31, 0x00, 0x08, 0x00, 0x00,
  0x01, 0x00, 0x01, 0x00, 0x73, 0x29, 0xE3, 0x44, 0x8C, 0xA4, 0x24, 0xDF, 0x50, 0xE9, 0xFC, 0x71,
  0x13, 0x64, 0xE8, 0xC8, 0x64, 0xE2, 0x5C, 0xCB, 0xBB, 0x8B, 0xA1, 0x7F, 0x55, 0xD6, 0xE7, 0x47,
  0xAA, 0xBB, 0x16, 0x80, 0xDA, 0x20, 0xD4, 0x97, 0xEE, 0x61, 0xA4, 0x50, 0xE5, 0x55, 0xDC, 0x98,
  0x77, 0xFB, 0x93, 0xBE, 0x33, 0x9F, 0x55, 0xC2, 0xF9, 0xFE, 0x1C, 0x42, 0x5A, 0x3F, 0xF9, 0x8F,
  0x3F, 0xEC, 0x3C, 0xE0, 0xDB, 0xD1, 0x7E, 0x49, 0x6C, 0xDD, 0x12, 0xB9, 0x00, 0x3F, 0x3A, 0x48,
  0x86, 0x47, 0x95, 0xFC, 0x6F, 0xA0, 0xDA, 0x2D, 0x06, 0x70, 0x08, 0xCF, 0x89, 0x43, 0xD2, 0xFF,
  0xAC, 0xB8, 0xD3, 0x5F, 0xF2, 0x8A, 0x1C, 0x74, 0xE3, 0x94, 0x18, 0xCE, 0x9E, 0x76, 0xBE, 0xB2,
  0x8C, 0x53, 0xA0, 0xDF, 0xEF, 0x83, 0x8D, 0x5B, 0x7C, 0xD4, 0x2B, 0x7B, 0x26, 0x09, 0xD3, 0x60,
  0x9D, 0x53, 0x3D, 0x5C, 0xF1, 0x5E, 0xC3, 0x9E, 0xAE, 0x53, 0x1F, 0x11, 0x36, 0xB1, 0xDD, 0xF9,
  0xB9, 0xE9, 0xAA, 0xDC, 0x23, 0x15, 0x34, 0x8C, 0x1E, 0xBB, 0x0F, 0x1B, 0xCA, 0x5A, 0xB1, 0x7C,
  0xDE, 0x0F, 0x51, 0x60, 0x7E, 0xDF, 0x71, 0x2E, 0x17, 0xC8, 0x68, 0x57, 0x00, 0x2B, 0xDF, 0xCF,
  0x0E, 0x04, 0xA3, 0x35, 0x57, 0x0B, 0xEF, 0xEB, 0xC4, 0x2F, 0x13, 0x15, 0xF4, 0xEB, 0xA9, 0x24,
  0xA4, 0x2D, 0x9C, 0xEA, 0x32, 0xF7, 0xB8, 0xCA, 0x81, 0x9E, 0x8C, 0xEA, 0x27, 0xF8, 0x54, 0x30,
  0xC7, 0xE3, 0x66, 0x30, 0x10, 0xDB, 0xC5, 0xAE, 0xC3, 0xF5, 0x89, 0x65, 0xAF, 0x81, 0x6F, 0x8A,
  0xF0, 0xE0, 0xDE, 0x8A, 0x02, 0x2A, 0xC4, 0x20, 0x86, 0x56, 0xC2, 0x00, 0x93, 0x79, 0xFC, 0x56,
  0x1C, 0x58, 0xD6, 0x20, 0x12, 0x4C, 0x62, 0xF4, 0xF1, 0xD7, 0xFB, 0xB2, 0x8D, 0x7D, 0x55, 0xEC,
  0x92, 0x4E, 0xAA, 0x90,
};

// GLOBAL: gl_24468
DWORD g_rsaPublicKeyBlobLen = 276;

// FUNCTION: fn_00136
FileSignatureVerifier::FileSignatureVerifier() {
  signature = nullptr;
  hash = 0;
  provider = 0;
  fileData = nullptr;
  contentSize = 0;
  fileSize = 0;
  fileNameSize = 0;
  secondaryHash = 0;
  secondaryKey = 0;
  signatureSize = 0;
  hasSignature = false;
  signatureBlockSize = 0;
  error = 0;
  publicKey = 0;
  if (!CryptAcquireContextW(&provider, nullptr, aMicrosoftBaseC, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
    provider = 0;
  }
}

// FUNCTION: fn_45444
FileSignatureVerifier::~FileSignatureVerifier() {
  if (fileData != nullptr) {
    std::free(fileData);
  }
  if (signature != nullptr) {
    std::free(signature);
  }
  if (hash != 0) {
    CryptDestroyHash(hash);
  }
  if (publicKey != 0) {
    CryptDestroyKey(publicKey);
  }
  if (secondaryKey != 0) {
    CryptDestroyKey(secondaryKey);
  }
  if (secondaryHash != 0) {
    CryptDestroyHash(secondaryHash);
  }
  if (provider != 0) {
    CryptReleaseContext(provider, 0);
  }
}

// FUNCTION: fn_00138
bool FileSignatureVerifier::VerifyBuffer(const WCHAR* fileName, const BYTE* data, DWORD dataSize, bool verifyFileName) {
  std::memcpy(&footer, data + dataSize - sizeof(footer), sizeof(footer));
  if (footer.startMarker == 0x32812622 && footer.endMarker == 0x32812621) {
    signatureBlockSize = footer.signatureSize + footer.fileNameSize + sizeof(footer);
    contentSize = dataSize - signatureBlockSize;
    hasSignature = true;
    fileNameSize = footer.fileNameSize;
  } else {
    footer = {};
    hasSignature = false;
    signatureBlockSize = 0;
    contentSize = dataSize;
    fileNameSize = 0;
  }
  if (dataSize < signatureBlockSize) {
    hasSignature = false;
    signatureBlockSize = 0;
    contentSize = 0;
    fileNameSize = 0;
  }
  if (!hasSignature) {
    error = 6;
    return false;
  }

  signatureSize = footer.signatureSize;
  if (signature != nullptr) {
    std::free(signature);
    signature = nullptr;
  }
  signature = static_cast<BYTE*>(std::malloc(signatureSize));
  if (signature == nullptr) {
    error = 7;
    return false;
  }
  std::memcpy(signature, data + contentSize + fileNameSize, signatureSize);
  if (provider == 0 && !CryptAcquireContextW(&provider, nullptr, aMicrosoftBaseC, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
    error = 8;
    return false;
  }

  if (publicKey != 0) {
    CryptDestroyKey(publicKey);
    publicKey = 0;
  }
  if (!CryptImportKey(provider, g_rsaPublicKeyBlob, g_rsaPublicKeyBlobLen, 0, 0, &publicKey)) {
    if (GetLastError() != ERROR_INVALID_PARAMETER) {
      error = 9;
      return false;
    }
    if (!CryptAcquireContextW(&provider, nullptr, aMicrosoftBaseC, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
      error = 98;
      return false;
    }
    if (!CryptImportKey(provider, g_rsaPublicKeyBlob, g_rsaPublicKeyBlobLen, 0, 0, &publicKey)) {
      error = 9;
      return false;
    }
  }
  if (hash != 0) {
    CryptDestroyHash(hash);
    hash = 0;
  }
  if (!CryptCreateHash(provider, CALG_SHA1, 0, 0, &hash)) {
    error = 10;
    return false;
  }
  if (!CryptHashData(hash, data, contentSize + fileNameSize, 0)) {
    error = 11;
    return false;
  }
  if (!CryptVerifySignatureW(hash, signature, signatureSize, publicKey, nullptr, 0)) {
    if (GetLastError() != ERROR_INVALID_PARAMETER) {
      error = 12;
      return false;
    }
    if (!CryptAcquireContextW(&provider, nullptr, aMicrosoftBaseC, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
      error = 128;
      return false;
    }
    if (!CryptVerifySignatureW(hash, signature, signatureSize, publicKey, nullptr, 0)) {
      error = 12;
      return false;
    }
  }

  char convertedFileName[MAX_PATH];
  WideCharToMultiByte(CP_ACP, 0, fileName, -1, convertedFileName, MAX_PATH, nullptr, nullptr);
  if (verifyFileName) {
    const char* baseName = std::strrchr(convertedFileName, '\\');
    baseName = baseName != nullptr ? baseName + 1 : convertedFileName;
    if (_strnicmp(baseName, reinterpret_cast<const char*>(data + contentSize), fileNameSize) != 0) {
      error = 13;
      return false;
    }
  }
  return true;
}

// FUNCTION: fn_00139
bool FileSignatureVerifier::VerifyFile(const WCHAR* fileName, bool verifyFileName) {
  if (fileName == nullptr) {
    error = 1;
    return false;
  }
  if (*fileName == 0) {
    error = 2;
    return false;
  }
  HANDLE file = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    error = 3;
    return false;
  }
  fileSize = GetFileSize(file, nullptr);
  if (fileSize == INVALID_FILE_SIZE) {
    CloseHandle(file);
    error = 33;
    return false;
  }

  if (fileData != nullptr) {
    std::free(fileData);
    fileData = nullptr;
  }
  fileData = static_cast<BYTE*>(std::malloc(fileSize));
  if (fileData == nullptr) {
    CloseHandle(file);
    error = 4;
    return false;
  }
  DWORD bytesRead;
  ReadFile(file, fileData, fileSize, &bytesRead, nullptr);
  if (bytesRead != fileSize) {
    CloseHandle(file);
    error = 5;
    return false;
  }
  CloseHandle(file);

  bool result = VerifyBuffer(fileName, fileData, fileSize, verifyFileName);
  if (fileData != nullptr) {
    std::free(fileData);
    fileData = nullptr;
  }
  return result;
}

// FUNCTION: fn_00140
bool FileSignatureVerifier::VerifyBuffer(const char* fileName, const BYTE* data, DWORD dataSize, bool verifyFileName) {
  WCHAR wideFileName[MAX_PATH];
  MultiByteToWideChar(CP_ACP, 0, fileName, -1, wideFileName, MAX_PATH);
  return VerifyBuffer(wideFileName, data, dataSize, verifyFileName);
}

// FUNCTION: fn_00141
bool FileSignatureVerifier::VerifyFile(const char* fileName, bool verifyFileName) {
  WCHAR wideFileName[MAX_PATH];
  MultiByteToWideChar(CP_ACP, 0, fileName, -1, wideFileName, MAX_PATH);
  return VerifyFile(wideFileName, verifyFileName);
}

#ifndef RAGNAROK_SOURCE_UTILITY_FILESIGNATUREVERIFIER_H
#define RAGNAROK_SOURCE_UTILITY_FILESIGNATUREVERIFIER_H

#include <cstddef>
#include <windows.h>
#include <wincrypt.h>

struct FileSignatureFooter {
  /* 0000 */ DWORD startMarker;
  /* 0004 */ DWORD fileNameSize;
  /* 0008 */ DWORD signatureSize;
  /* 000C */ DWORD endMarker;
};

class FileSignatureVerifier {
public:
  /* 0000 */ HCRYPTPROV provider;
  /* 0004 */ BYTE* fileData;
  /* 0008 */ DWORD contentSize;
  /* 000C */ HCRYPTHASH hash;
  /* 0010 */ HCRYPTKEY publicKey;
  /* 0014 */ BYTE* signature;
  /* 0018 */ DWORD signatureSize;
  /* 001C */ bool hasSignature;
  /* 0020 */ DWORD signatureBlockSize;
  /* 0024 */ DWORD fileSize;
  /* 0028 */ FileSignatureFooter footer;
  /* 0038 */ DWORD fileNameSize;
  /* 003C */ HCRYPTKEY secondaryKey;
  /* 0040 */ HCRYPTHASH secondaryHash;
  /* 0044 */ DWORD error;

  FileSignatureVerifier();
  ~FileSignatureVerifier();

  bool VerifyBuffer(const WCHAR* fileName, const BYTE* data, DWORD dataSize, bool verifyFileName);
  bool VerifyFile(const WCHAR* fileName, bool verifyFileName);
  bool VerifyBuffer(const char* fileName, const BYTE* data, DWORD dataSize, bool verifyFileName);
  bool VerifyFile(const char* fileName, bool verifyFileName);
};

static_assert(sizeof(FileSignatureFooter) == 0x10);
static_assert(offsetof(FileSignatureFooter, fileNameSize) == 0x04);
static_assert(offsetof(FileSignatureFooter, signatureSize) == 0x08);
static_assert(offsetof(FileSignatureFooter, endMarker) == 0x0C);
static_assert(sizeof(FileSignatureVerifier) == 0x48);
static_assert(offsetof(FileSignatureVerifier, provider) == 0x00);
static_assert(offsetof(FileSignatureVerifier, fileData) == 0x04);
static_assert(offsetof(FileSignatureVerifier, contentSize) == 0x08);
static_assert(offsetof(FileSignatureVerifier, hash) == 0x0C);
static_assert(offsetof(FileSignatureVerifier, publicKey) == 0x10);
static_assert(offsetof(FileSignatureVerifier, signature) == 0x14);
static_assert(offsetof(FileSignatureVerifier, signatureSize) == 0x18);
static_assert(offsetof(FileSignatureVerifier, hasSignature) == 0x1C);
static_assert(offsetof(FileSignatureVerifier, signatureBlockSize) == 0x20);
static_assert(offsetof(FileSignatureVerifier, fileSize) == 0x24);
static_assert(offsetof(FileSignatureVerifier, footer) == 0x28);
static_assert(offsetof(FileSignatureVerifier, fileNameSize) == 0x38);
static_assert(offsetof(FileSignatureVerifier, secondaryKey) == 0x3C);
static_assert(offsetof(FileSignatureVerifier, secondaryHash) == 0x40);
static_assert(offsetof(FileSignatureVerifier, error) == 0x44);

#endif

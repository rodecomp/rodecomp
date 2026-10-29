#include "Render/Text/EmbedFont.h"

#include "Client/Application/ErrorDialog.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <t2embapi.h>
#include <vector>

// GLOBAL: gl_24812
CEmbedFont* g_embedFont;

// INLINED
CEmbedFont::CEmbedFont() : m_module(nullptr), m_unknownC(0) {
  std::memset(this->m_rootUrl, 0, sizeof(this->m_rootUrl));
}

CEmbedFont::~CEmbedFont() {
  this->UnloadModule();
}

// OFFSET: 0x00ADCE70
// FUNCTION: fn_17938
void FreeEmbedFont() {
  if (g_embedFont != nullptr) {
    g_embedFont->UnloadModule();
    delete g_embedFont;
    g_embedFont = nullptr;
  }
}

// OFFSET: 0x00ADCEB0
// FUNCTION: fn_17939
void InitEmbedFont() {
  g_embedFont = new CEmbedFont();
  g_embedFont->LoadModule();
}

// OFFSET: 0x00ADCF60
// FUNCTION: fn_17940
int LoadEmbedFont(HANDLE* fontReference, const char* fileName) {
  char message[256];
  char path[256];
  GetCurrentDirectoryA(sizeof(path), path);
  std::strcat(path, "\\System\\Font\\");
  std::strcat(path, fileName);
  if (std::fopen(path, "r") == nullptr) {
    std::sprintf(message, "%s \xc6\xc4\xc0\xcf \xbe\xf8\xc0\xbd!", path);
    ShowErrorDialog(nullptr, message, 0);
    std::exit(0);
  }
  if (g_embedFont == nullptr) {
    return 0;
  }
  return g_embedFont->Load(fontReference, fileName, path);
}

// OFFSET: 0x00ADD090
// FUNCTION: fn_17941
void SetEmbedFontRootUrl(const char* url) {
  if (g_embedFont != nullptr && url != nullptr) {
    std::strcpy(g_embedFont->m_rootUrl, url);
  }
}

// OFFSET: 0x00ADD710
// FUNCTION: fn_17943
static unsigned long ReadEmbedFontStream(void* stream, void* buffer, const unsigned long size) {
  return static_cast<unsigned long>(std::fread(buffer, 1, size, static_cast<FILE*>(stream)));
}

// OFFSET: 0x00ADD810
// FUNCTION: fn_17945
static std::wstring ToWideString(const std::string& text) {
  WORD length = static_cast<WORD>(MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text.c_str(), -1, nullptr, 0));
  std::vector<wchar_t> buffer(length);
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text.c_str(), -1, buffer.data(), static_cast<int>(buffer.size()));
  return std::wstring(buffer.data(), buffer.size());
}

// OFFSET: 0x00ADD1B0
// FUNCTION: fn_17942
int CEmbedFont::Load(HANDLE* fontReference, const char* familyName, const char* path) {
  int result = -2;
  *fontReference = nullptr;
  if (this->m_module == nullptr) {
    return result;
  }
  const char* rootUrl = this->m_rootUrl;
  if (rootUrl == nullptr) {
    return -3;
  }
  if (std::strcmp(rootUrl, "http://222.122.45.46") != 0 && std::strcmp(rootUrl, "http://fontrix.co.kr") != 0 &&
      std::strcmp(rootUrl, "http://www.puccaracing.co.kr") != 0 && std::strcmp(rootUrl, "http://www.ragnarok.co.kr") != 0) {
    return -3;
  }
  std::string familyNameText = familyName;
  std::string pathText = path == nullptr ? familyNameText : std::string(path);
  std::wstring wideFamilyName = ToWideString(familyNameText);
  std::wstring widePath = ToWideString(pathText);
  std::wstring wideRootUrl = ToWideString(std::string(rootUrl));
  TTLOADINFO loadInfo;
  loadInfo.usStructSize = sizeof(TTLOADINFO);
  loadInfo.usRefStrSize = static_cast<unsigned short>(wideRootUrl.size());
  loadInfo.pusRefStr = reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(wideRootUrl.c_str()));
  FILE* file = _wfopen(widePath.c_str(), L"rb");
  if (file == nullptr) {
    return -2;
  }
  ULONG privStatus = 2;
  ULONG status = 1;
  typedef LONG(WINAPI * TTLoadEmbeddedFontProc)(HANDLE*, ULONG, ULONG*, ULONG, ULONG*, READEMBEDPROC, LPVOID, LPWSTR,
                                                 LPSTR, TTLOADINFO*);
  TTLoadEmbeddedFontProc loadEmbeddedFont;
  if (this->m_module == nullptr ||
      (loadEmbeddedFont = reinterpret_cast<TTLoadEmbeddedFontProc>(GetProcAddress(this->m_module, "TTLoadEmbeddedFont"))) ==
          nullptr) {
    result = 0;
  } else {
    LONG error = loadEmbeddedFont(fontReference, TTLOAD_PRIVATE, &privStatus, LICENSE_EDITABLE, &status,
                                  ReadEmbedFontStream, file, const_cast<wchar_t*>(wideFamilyName.c_str()),
                                  const_cast<char*>(familyNameText.c_str()), &loadInfo);
    if (error < 0) {
      result = -2;
    } else if (error == 0) {
      result = 1;
    } else if (error == 0x10f) {
      result = 2;
    } else if (error == 0x10e) {
      result = 2;
    } else if (error == 0x100) {
      result = -5;
    } else if (error == 0x102) {
      result = -4;
    } else if (error == 0x11c) {
      result = -3;
    } else if (error == 0x11a) {
      result = -3;
    } else {
      result = -1;
    }
  }
  fclose(file);
  return result;
}

#include "Render/Text/EmbedFont.h"

#include "Client/Application/ErrorDialog.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// GLOBAL: gl_24812
CEmbedFont* g_embedFont;

// OFFSET: 0x00ADCC50
// FUNCTION: fn_45885
CEmbedFont::CEmbedFont() {
  this->Reset();
}

CEmbedFont::~CEmbedFont() {
  this->UnloadModule();
}

// OFFSET: 0x00ADCD70
// FUNCTION: fn_45886
void CEmbedFont::Reset() {
  this->m_module = nullptr;
  this->unknown_000C = 0;
  std::memset(this->m_rootUrl, 0, sizeof(this->m_rootUrl));
}

// OFFSET: 0x00ADCDA0
// FUNCTION: fn_45887
int CEmbedFont::UnloadModule() {
  if (this->m_module != nullptr) {
    FreeLibrary(this->m_module);
    this->m_module = nullptr;
  }
  return 1;
}

// OFFSET: 0x00ADCDC0
// FUNCTION: fn_45888
int CEmbedFont::DeleteEmbeddedFont(HANDLE fontReference, ULONG flags, ULONG* status) {
  if (this->m_module == nullptr) {
    return 0;
  }
  auto deleteFont = reinterpret_cast<decltype(&TTDeleteEmbeddedFont)>(GetProcAddress(this->m_module, "TTDeleteEmbeddedFont"));
  if (deleteFont == nullptr) {
    return 0;
  }
  LONG error = deleteFont(fontReference, flags, status);
  if (error < 0) {
    return 0;
  }
  if (error == 0) {
    return 1;
  }
  return 0;
}

// OFFSET: 0x00ADCE00
// FUNCTION: fn_45889
void CEmbedFont::Unload(HANDLE fontReference) {
  if (this->m_module != nullptr && fontReference != nullptr) {
    this->DeleteEmbeddedFont(fontReference, TTDELETE_DONTREMOVEFONT, nullptr);
  }
}

// OFFSET: 0x00ADCE30
// FUNCTION: fn_45890
void UnloadEmbedFont(HANDLE fontReference) {
  if (g_embedFont != nullptr) {
    g_embedFont->Unload(fontReference);
  }
}

// OFFSET: 0x00ADD0C0
// FUNCTION: fn_45891
int CEmbedFont::LoadModule() {
  if (this->m_module == nullptr) {
    HMODULE module = LoadLibraryA("t2embed.dll");
    if (reinterpret_cast<UINT_PTR>(module) > 32) {
      this->m_module = module;
      return 1;
    }
  }
  return 0;
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
int LoadEmbedFont(HANDLE* fontReference, const char* fileName, int option) {
  char message[256];
  char path[256];
  GetCurrentDirectoryA(sizeof(path), path);
  std::strcat(path, "\\System\\Font\\");
  std::strcat(path, fileName);
  if (std::fopen(path, "r") == nullptr) {
    std::sprintf(message, "%s \xc6\xc4\xc0\xcf \xbe\xf8\xc0\xbd!", path);
    ShowErrorDialog(message, 0);
    std::exit(0);
  }
  if (g_embedFont != nullptr) {
    return g_embedFont->Load(fontReference, fileName, path);
  }
  return 0;
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

// OFFSET: 0x00ADD760
// FUNCTION: fn_45929
unsigned long WINAPIV WriteEmbedFontStream(void* stream, const void* buffer, const unsigned long size) {
  return static_cast<unsigned long>(std::fwrite(buffer, 1, size, static_cast<FILE*>(stream)));
}

// OFFSET: 0x00ADD810
// FUNCTION: fn_17945
static std::wstring ToWideString(const std::string& text) {
  WORD length = static_cast<WORD>(MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text.c_str(), -1, nullptr, 0));
  std::vector<wchar_t> buffer(length, L'\0');
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text.c_str(), -1, buffer.data(), static_cast<int>(buffer.size()));
  return std::wstring(buffer.data(), buffer.size());
}

// OFFSET: 0x00ADD1B0
// FUNCTION: fn_17942
int CEmbedFont::Load(HANDLE* fontReference, const char* familyName, const char* path) {
  int result = -2;
  *fontReference = nullptr;
  if (this->m_module != nullptr) {
    const char* rootUrl = this->m_rootUrl;
    if (rootUrl != nullptr &&
        (std::strcmp(rootUrl, "http://222.122.45.46") == 0 || std::strcmp(rootUrl, "http://fontrix.co.kr") == 0 ||
         std::strcmp(rootUrl, "http://www.puccaracing.co.kr") == 0 || std::strcmp(rootUrl, "http://www.ragnarok.co.kr") == 0)) {
      const std::string familyNameText = familyName;
      std::string pathText = path == nullptr ? familyNameText : path;
      std::wstring wideFamilyName = ToWideString(familyNameText);
      std::wstring widePath = ToWideString(pathText);
      std::wstring wideRootUrl = ToWideString(std::string(rootUrl));
      TTLOADINFO loadInfo;
      loadInfo.usStructSize = sizeof(TTLOADINFO);
      loadInfo.pusRefStr = reinterpret_cast<unsigned short*>(const_cast<wchar_t*>(wideRootUrl.c_str()));
      loadInfo.usRefStrSize = static_cast<unsigned short>(wideRootUrl.size());
      FILE* file = _wfopen(widePath.c_str(), L"rb");
      if (file == nullptr) {
        return -2;
      }
      LPWSTR windowsFamily = const_cast<wchar_t*>(wideFamilyName.c_str());
      ULONG privStatus = 2;
      LPSTR macFamily = const_cast<char*>(familyNameText.c_str());
      ULONG status = 1;
      result = this->LoadEmbeddedFont(fontReference, TTLOAD_PRIVATE, &privStatus, LICENSE_EDITABLE, &status,
                                      ReadEmbedFontStream, file, windowsFamily, macFamily, &loadInfo);
      fclose(file);
    } else {
      return -3;
    }
  }
  return result;
}

// OFFSET: 0x00ADD0F0
// FUNCTION: fn_45892
int CEmbedFont::LoadEmbeddedFont(HANDLE* fontReference, ULONG flags, ULONG* privStatus, ULONG privileges, ULONG* status,
                                 READEMBEDPROC readStream, LPVOID stream, LPWSTR windowsFamily, LPSTR macFamily,
                                 TTLOADINFO* info) {
  if (this->m_module != nullptr) {
    auto loadFont = reinterpret_cast<decltype(&TTLoadEmbeddedFont)>(GetProcAddress(this->m_module, "TTLoadEmbeddedFont"));
    if (loadFont != nullptr) {
      LONG error = loadFont(fontReference, flags, privStatus, privileges, status, readStream, stream, windowsFamily,
                            macFamily, info);
      if (error < 0) {
        return -2;
      }
      if (error == 0) {
        return 1;
      }
      if (error == E_FONTNAMEALREADYEXISTS) {
        return 2;
      }
      if (error == E_FONTALREADYEXISTS) {
        return 2;
      }
      if (error == E_ERRORCOMPRESSINGFONTDATA) {
        return -5;
      }
      if (error == E_FONTDATAINVALID) {
        return -4;
      }
      if (error == 0x11c) {
        return -3;
      }
      if (error == E_SUBSTRING_TEST_FAIL) {
        return -3;
      }
      return -1;
    }
  }
  return 0;
}

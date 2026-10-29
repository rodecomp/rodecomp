#ifndef RAGNAROK_SOURCE_RENDER_TEXT_EMBEDFONT_H
#define RAGNAROK_SOURCE_RENDER_TEXT_EMBEDFONT_H

#include <windows.h>

// VTABLE: vt_01611
class CEmbedFont {
public:
  CEmbedFont();
  virtual ~CEmbedFont();

  // INLINED
  void LoadModule() {
    if (this->m_module == nullptr) {
      HMODULE module = LoadLibraryA("t2embed.dll");
      if (reinterpret_cast<UINT_PTR>(module) > 32) {
        this->m_module = module;
      }
    }
  }

  // INLINED
  void UnloadModule() {
    if (this->m_module != nullptr) {
      FreeLibrary(this->m_module);
      this->m_module = nullptr;
    }
  }

  int Load(HANDLE* fontReference, const char* familyName, const char* path);

  /* 0004 */ HMODULE m_module;
  /* 0008 */ int m_unknown8;
  /* 000C */ int m_unknownC;
  /* 0010 */ char m_rootUrl[256];
};

extern CEmbedFont* g_embedFont;

void FreeEmbedFont();
void InitEmbedFont();
int LoadEmbedFont(HANDLE* fontReference, const char* fileName);
void SetEmbedFontRootUrl(const char* url);

#endif

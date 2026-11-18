#ifndef RAGNAROK_SOURCE_RENDER_TEXT_EMBEDFONT_H
#define RAGNAROK_SOURCE_RENDER_TEXT_EMBEDFONT_H

#include <windows.h>
#include <t2embapi.h>

// VTABLE: vt_01611
class CEmbedFont {
public:
  CEmbedFont();
  virtual ~CEmbedFont();

  void Reset();
  int LoadModule();
  int UnloadModule();
  int DeleteEmbeddedFont(HANDLE fontReference, ULONG flags, ULONG* status);
  void Unload(HANDLE fontReference);
  int LoadEmbeddedFont(HANDLE* fontReference, ULONG flags, ULONG* privStatus, ULONG privileges, ULONG* status,
                       READEMBEDPROC readStream, LPVOID stream, LPWSTR windowsFamily, LPSTR macFamily, TTLOADINFO* info);
  int Load(HANDLE* fontReference, const char* familyName, const char* path);

  /* 0004 */ HMODULE m_module;
  /* 0008 */ int padding_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ char m_rootUrl[256];
};

extern CEmbedFont* g_embedFont;

void UnloadEmbedFont(HANDLE fontReference);
void FreeEmbedFont();
void InitEmbedFont();
int LoadEmbedFont(HANDLE* fontReference, const char* fileName, int option);
void SetEmbedFontRootUrl(const char* url);

#endif

#ifndef RAGNAROK_SOURCE_CONTENTS_SIGNBOARD_SIGNBOARD_H
#define RAGNAROK_SOURCE_CONTENTS_SIGNBOARD_SIGNBOARD_H

#include "Contents/Actors/RenderObject.h"
#include "UI/Window/UIWindow.h"

#include <cstddef>
#include <string>
#include <vector>

class UIStaticText;
struct UIButtonContext;

class CBmpObjWnd : public UIWindow {
public:
  // INLINED
  explicit CBmpObjWnd(UIWindowContext& windows) : UIWindow(windows, 0) {}
  ~CBmpObjWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 007C */ std::string path;
};

class CSignBoardWnd : public UIWindow {
public:
  // INLINED
  CSignBoardWnd(UIWindowContext& windows, UIButtonContext& controls) : UIWindow(windows, 0), text(nullptr), m_controls(&controls) {}
  ~CSignBoardWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void ApplyColor();

  /* 007C */ UIStaticText* text;
  /* 0080 */ std::string unknown_0080;
  /* 0098 */ std::string iconPath;
  /* 00B0 */ std::string unknown_00B0;

protected:
  UIButtonContext* m_controls;
};

class CIcon : public CRenderObject {
public:
  ~CIcon() override;
  bool OnProcess() override;
  void SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m) override;
  void Render(int argument) override;
  void SetRenderInfo(const float* bounds, float x, float y) override;
  void Load(const std::string& name);
};

class C3DCooRendering : public CRenderObject {
public:
  ~C3DCooRendering() override;
  bool OnProcess() override;
  void Render(int argument) override;
  void Create(UIWindowContext& windows, UIButtonContext& controls);
  void SetOverrideColor();
  void SetBoard(const std::string& text, const std::string& color);
  void SetWindowImage(const std::string& name);

  /* 0138 */ int type;
  /* 013C */ int unknown_013C;
  /* 0140 */ UIWindow* window;
  /* 0144 */ int unknown_0144;
};

struct SignBoardInfo {
  /* 0000 */ int column;
  /* 0004 */ int row;
  /* 0008 */ int offset;
  /* 000C */ const char* map;
  /* 0010 */ int type;
  /* 0014 */ const char* unknown_0014;
  /* 0018 */ const char* image;
  /* 001C */ const char* unknown_001C;
};

class CSignBoardMgr {
public:
  void Initialize();
  void LoadList();

  /* 0000 */ bool unknown_0000;
  /* 0004 */ std::vector<SignBoardInfo> m_entries;
};

extern CSignBoardMgr* s_signBoardMgr;
bool CreateSignBoardMgr();
void DestroySignBoardMgr();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CIcon) == 0x138);
static_assert(sizeof(SignBoardInfo) == 0x20);
static_assert(sizeof(CSignBoardMgr) == 0x10);
static_assert(offsetof(C3DCooRendering, window) == 0x140);
static_assert(sizeof(C3DCooRendering) == 0x148);
#endif

#endif

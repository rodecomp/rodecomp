#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMESSAGEBOX_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMESSAGEBOX_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UITextViewer;
struct UITextViewerContext;
struct UIButtonContext;
class UIWindowMgr;

struct WindowResponseRecorder;

struct UIMessageBoxContext {
  UITextViewerContext& text;
  UIWindowMgr& retirement;
  WindowResponseRecorder& recorder;
  int& response;
  const int& screenWidth;
  const int& screenHeight;
};

class UIMessageBox : public UIFrameWnd {
public:
  UIMessageBox(UIWindowContext& windows, UIFrameContext& frames, UIMessageBoxContext& dialog);
  ~UIMessageBox() override;
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Configure(const char* text, int type, bool defaultAccept, int timeout, const char* caption, int width, int height, int modal, const char* icon);

  /* 00B4 */ UITextViewer* viewer;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int counting;
  /* 00C0 */ std::uint32_t started;
  /* 00C4 */ std::uint32_t deadline;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ std::uint32_t remaining;
  /* 00D0 */ std::string format;
  /* 00E8 */ std::string messageText;
  /* 0100 */ std::string caption;
  /* 0118 */ int modal;
  /* 011C */ unsigned char protectedMessage;

private:
  UIMessageBoxContext* m_dialog;
  void AddButton(const char* name, int id, int x, int y);
  void FormatCountdown();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIMessageBox) == 300);
#endif

#endif

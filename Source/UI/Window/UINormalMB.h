#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINORMALMB_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINORMALMB_H

#include "UI/Window/UIFrameWnd.h"

#include <map>
#include <string>

class UITextViewer;
struct UITextViewerContext;

class UINormalMB : public UIFrameWnd {
public:
  UINormalMB(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Create(UIWindow* owner, const char* text, int type, bool defaultAccept, const char* title, int width, int height, const char* icon);

protected:
  /* 00B4 */ UIWindow* m_owner;
  /* 00B8 */ UITextViewer* m_viewer;
  /* 00BC */ std::string m_message;
  /* 00D4 */ std::string m_title;
  /* 00EC */ unsigned char m_keepOpen;
  /* 00F0 */ std::map<int, void (*)()> m_actions;

private:
  UITextViewerContext* m_textContext;
  void AddButton(const char* normal, const char* hover, const char* pressed, int x, int y, int id);
};

#endif

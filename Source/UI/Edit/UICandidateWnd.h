#ifndef RAGNAROK_SOURCE_UI_EDIT_UICANDIDATEWND_H
#define RAGNAROK_SOURCE_UI_EDIT_UICANDIDATEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;

class UICandidateWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ std::string m_candidates[9];
  /* 018C */ int m_selection;
  /* 0190 */ int m_page;
  /* 0194 */ int m_lastPage;
  /* 0198 */ int m_count;
  UIButtonContext* m_controls;
};

class UICompositionWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ std::string m_text;
  UIButtonContext* m_controls;
};

class UIKeyStrokeWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

#endif

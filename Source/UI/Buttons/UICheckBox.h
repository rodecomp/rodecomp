#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UICHECKBOX_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UICHECKBOX_H

#include "UI/Buttons/UIButton.h"

class UICheckBox : public UIWindow {
  friend class CMergeItemWnd;
  friend class UIFrameWnd;
  friend class UIRodexInboxWnd;
  friend class UISeekPartyMBWnd;

public:
  UICheckBox();
  UICheckBox(UIWindowContext& windowContext, UIButtonContext& controls);
  ~UICheckBox() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void SetChecked(int value);
  void SetImages(const char* checked, const char* unchecked, const char* text);
  void SetBitmaps(const char* checked, const char* unchecked);
  void FitToText();
  void SetTooltip(const char* text);
  void SetHidden(bool hidden);

protected:
  /* 007C */ int m_disabled;
  /* 0080 */ std::string m_checked;
  /* 0098 */ std::string m_unchecked;
  /* 00B0 */ std::string m_text;
  /* 00C8 */ std::string m_tooltip;
  /* 00E0 */ unsigned char m_hidden;
  /* 00E4 */ std::uint32_t m_color;
  /* 00E8 */ int m_fontHeight;
  /* 00EC */ int m_message;
  /* 00F0 */ int m_notify;
  /* 00F4 */ std::string m_uncheckedTooltip;
  UIButtonContext* m_controls;

private:
  void Notify();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UICheckBox) == 276);
#endif

#endif

#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICONTROL_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICONTROL_H

#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Window/UIWindow.h"

#include <functional>
#include <map>
#include <string>
#include <string_view>

template <class T> class CResPtr {
public:
  CResPtr() : m_res(nullptr) {
  }

  CResPtr(T* res) : m_res(res) {
    if (this->m_res) {
      this->m_res->Lock();
    }
  }

  ~CResPtr() {
    if (this->m_res) {
      this->m_res->Unlock();
    }
  }

  CResPtr& operator=(CResPtr&& other) {
    T* res = other.m_res;
    other.m_res = nullptr;
    T* old = this->m_res;
    this->m_res = res;
    if (old) {
      old->Unlock();
    }
    return *this;
  }

  operator T*() const {
    return this->m_res;
  }

  T* operator->() const {
    return this->m_res;
  }

  /* 0000 */ T* m_res;

private:
  CResPtr(const CResPtr&) = delete;
  CResPtr& operator=(const CResPtr&) = delete;
};

class CUIControl : public UIWindow {
public:
  __declspec(noinline) CBitmapRes* LoadBitmapResource(const std::string& name, bool resolveSkin);

  explicit CUIControl(UIWindowContext& context);
  ~CUIControl() override;
  bool CanHitTest() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot34(int x, int y) override;
  void Slot35(int value) override;
  void Invalidate() override;
  void Slot52(int value) override;
  virtual void Slot53(int value);

  bool IsPointInside(int x, int y);

  /* 007C */ unsigned char hitEnabled;
};

class CUITab : public CUIControl {
public:
  void Paint() override;
};

class CUIButton : public CUIControl {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot47() override;
  void Slot48() override;

  void SetState(int value);

  /* 0080 */ int state;
  /* 0084 */ unsigned char disabled;
  /* 0085 */ unsigned char unknown_0085[0x6B];
};

class CUITextButton : public CUIButton {};

class CUIImageTextButton : public CUITextButton {};

class CUITabButton : public CUIImageTextButton {
public:
  void Slot31(int x, int y) override;
  void Slot47() override;
  void Slot48() override;
};

class CUIImageButton : public CUIButton {
public:
  void Paint() override;
  void Slot53(int resolveSkin) override;

  void SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& inactive);

  /* 00F0 */ std::map<int, CResPtr<CBitmapRes>> images;
  /* 00F8 */ CResPtr<CBitmapRes> disabledImage;
  /* 00FC */ unsigned char transparent;
  /* 0100 */ std::map<int, std::string> imageNames;
  /* 0108 */ std::string disabledName;
};

class CUIStaticImage : public CUIControl {
public:
  void Paint() override;
  void Slot53(int resolveSkin) override;

  void SetImage(const std::string& name);

  /* 0080 */ CResPtr<CBitmapRes> image;
  /* 0084 */ std::string tooltip;
  /* 009C */ unsigned char transparent;
  /* 009E */ unsigned short scale;
  /* 00A0 */ std::string imageName;
};

class CUITitleBar : public CUIControl {
public:
  void Resize(int width, int height) override;
  void Paint() override;
  void Slot53(int resolveSkin) override;

  /* 0080 */ unsigned char resizable;
  /* 0084 */ CResPtr<CBitmapRes> left;
  /* 0088 */ CResPtr<CBitmapRes> middle;
  /* 008C */ CResPtr<CBitmapRes> right;
  /* 0090 */ std::string leftName;
  /* 00A8 */ std::string middleName;
  /* 00C0 */ std::string rightName;
  /* 00D8 */ unsigned char unknown_00D8[0x08];
  /* 00E0 */ UIWindow* firstButton;
  /* 00E4 */ UIWindow* secondButton;
};

class CUIEditCtrl;

class CUIEditBox : public CUIControl {
public:
  void Resize(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SetBackgroundColor(std::uint32_t color);
  void SetBorderColor(std::uint32_t color);
  void SetBorderWidth(int width);
  void SetMaximum(int value);
  void SetEditBounds(int left, int top, int right, int bottom);
  void SetText(const std::string& text);

  /* 0080 */ std::uint32_t backgroundColor;
  /* 0084 */ std::uint32_t borderColor;
  /* 0088 */ int borderWidth;
  /* 008C */ int editLeft;
  /* 0090 */ int editTop;
  /* 0094 */ int editRight;
  /* 0098 */ int editBottom;
  /* 009C */ CUIEditCtrl* edit;
  /* 00A0 */ std::function<void()> onSubmit;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIControl) == 132);
#endif

#endif

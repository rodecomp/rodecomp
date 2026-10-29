#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICONTROL_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICONTROL_H

#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Window/UIWindow.h"

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
  CBitmapRes* LoadBitmapResource(const std::string& name, bool resolveSkin);

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

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIControl) == 132);
#endif

#endif

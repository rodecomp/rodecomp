#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICONTROL_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICONTROL_H

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/SkinPalette.h"
#include "UI/Window/UIWindow.h"

#include <cstddef>
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
  UIWindow* FindChildById(int id);

  /* 007C */ unsigned char hitEnabled;
};

class CUITabButton;

class CUITab : public CUIControl {
public:
  void Paint() override;

  CUITabButton* FindButton(UIWindow* page);
  __declspec(noinline) void Select(CUITabButton* button);
  void SelectPage(UIWindow* page);
  bool SelectIndex(unsigned int index);

  /* 0080 */ CUITabButton* current;
  /* 0084 */ unsigned char unknown_0084[0x04];
  /* 0088 */ std::function<void(CUITabButton*)> onChange;
  /* 00B0 */ std::function<bool(CUITabButton*, CUITabButton*)> onChanging;
};

class CUIButton : public CUIControl {
public:
  explicit CUIButton(UIWindowContext& context);
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot26(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int argument1, int argument2) override;
  void Slot47() override;
  void Slot48() override;

  void SetState(int value);
  void SetOnClick(std::function<void()> handler);

  /* 0080 */ int state;
  /* 0084 */ unsigned char disabled;
  /* 0085 */ unsigned char unknown_0085[0x03];
  /* 0088 */ std::string tooltip;
  /* 00A0 */ unsigned char unknown_00A0[0x24];
  /* 00C4 */ UIWindow* unknown_00C4;
  /* 00C8 */ unsigned char unknown_00C8[0x24];
  /* 00EC */ UIWindow* unknown_00EC;
};

class CUITextButton : public CUIButton {
public:
  void Slot28(int x, int y) override;
  void Paint() override;
  virtual void Slot54(int value);
  virtual void Slot55();

  void SetShadow(bool enabled, std::uint32_t color);
  void SetOutline(bool enabled, std::uint32_t color);

  /* 00F0 */ unsigned char unknown_00F0[0x30];
  /* 0120 */ std::string fullText;
  /* 0138 */ unsigned char unknown_0138[0x28];
  /* 0160 */ unsigned char shadow;
  /* 0164 */ std::uint32_t shadowColor;
  /* 0168 */ unsigned char outline;
  /* 016C */ std::uint32_t outlineColor;
  /* 0170 */ unsigned char unknown_0170[0x08];
  /* 0178 */ std::string tooltipText;
};

class CUIImageTextButton : public CUITextButton {
public:
  void Slot53(int resolveSkin) override;

  void SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& inactive);

  /* 0190 */ std::map<int, CResPtr<CBitmapRes>> images;
  /* 0198 */ CResPtr<CBitmapRes> disabledImage;
  /* 019C */ std::map<int, std::string> imageNames;
  /* 01A4 */ std::string disabledName;
  /* 01BC */ unsigned char unknown_01BC[0x04];
};

class CUIBasicButton : public CUITextButton {
public:
  void Resize(int width, int height) override;
  void Paint() override;
  void Slot53(int resolveSkin) override;

  /* 0190 */ unsigned char unknown_0190;
  /* 0194 */ int unknown_0194;
  /* 0198 */ std::map<int, CResPtr<CBitmapRes>> leftImages;
  /* 01A0 */ CResPtr<CBitmapRes> disabledLeft;
  /* 01A4 */ std::map<int, CResPtr<CBitmapRes>> middleImages;
  /* 01AC */ CResPtr<CBitmapRes> disabledMiddle;
  /* 01B0 */ std::map<int, CResPtr<CBitmapRes>> rightImages;
  /* 01B8 */ CResPtr<CBitmapRes> disabledRight;
};

class CUITabButton : public CUIImageTextButton {
public:
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot47() override;
  void Slot48() override;
  void Slot53(int resolveSkin) override;

  __declspec(noinline) void SetSelected(bool selected);
  void SetSkinColors(int normalColumn, int normalRow, int selectedColumn, int selectedRow, int borderColumn, int borderRow);

  /* 01C0 */ UIWindow* page;
  /* 01C4 */ unsigned char useSkinColors;
  /* 01C8 */ std::uint32_t normalColor;
  /* 01CC */ std::uint32_t selectedColor;
  /* 01D0 */ std::uint32_t borderColor;
  /* 01D4 */ SkinPaletteCell normalCell;
  /* 01DC */ SkinPaletteCell selectedCell;
  /* 01E4 */ SkinPaletteCell borderCell;
};

class CUIImageButton : public CUIButton {
public:
  explicit CUIImageButton(UIWindowContext& context);
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

  void SetImages(const std::string& left, const std::string& middle, const std::string& right);
  void SetSecondButton(CUIButton* button);

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
  void SetTextColor(std::uint32_t color);

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

class CUIResizer : public CUIControl {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot53(int resolveSkin) override;

  void SetImage(const std::string& name);

  /* 0084 */ int minWidth;
  /* 0088 */ int minHeight;
  /* 008C */ int maxWidth;
  /* 0090 */ int maxHeight;
  /* 0094 */ int stepX;
  /* 0098 */ int stepY;
  /* 009C */ int startX;
  /* 00A0 */ int startY;
  /* 00A4 */ int startWidth;
  /* 00A8 */ int startHeight;
  /* 00AC */ int deltaX;
  /* 00B0 */ int deltaY;
  /* 00B4 */ std::string imageName;
  /* 00CC */ unsigned char unknown_00CC[0x04];
  /* 00D0 */ std::function<void(int, int)> onResize;
  /* 0080 */ CResPtr<CBitmapRes> image;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIControl) == 132);
static_assert(offsetof(CUIResizer, minWidth) == 0x84);
static_assert(offsetof(CUIResizer, imageName) == 0xB4);
static_assert(offsetof(CUIResizer, onResize) == 0xD0);
#endif

#endif

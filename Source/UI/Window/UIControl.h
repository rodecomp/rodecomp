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

  // INLINED
  CUIControl() : UIWindow(0), hitEnabled(1) {
  }
  ~CUIControl() override;
  bool CanHitTest() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  __declspec(noinline) void OnMouseHover(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnRBtnDblClk(int x, int y) override;
  void OnWheel(int value) override;
  void Invalidate() override;
  void ApplySkin(int value) override;
  virtual void LoadSkin(int value);

  bool IsPointInside(int x, int y);
  UIWindow* FindChildById(int id);

  /* 007C */ bool hitEnabled;
};

class CUITabButton;

class CUITab : public CUIControl {
public:
  __declspec(noinline) CUITab();

  void Paint() override;

  CUITabButton* FindButton(UIWindow* page);
  __declspec(noinline) void Select(CUITabButton* button);
  void SelectPage(UIWindow* page);
  bool SelectIndex(unsigned int index);
  void SetOnChange(std::function<void(CUITabButton*)> handler);

  /* 0080 */ CUITabButton* current;
  /* 0084 */ unsigned char unknown_0084[0x04];
  /* 0088 */ std::function<void(CUITabButton*)> onChange;
  /* 00B0 */ std::function<bool(CUITabButton*, CUITabButton*)> onChanging;
};

class CUIButton : public CUIControl {
public:
  CUIButton();
  ~CUIButton() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int argument1, int argument2) override;
  void OnMouseEnter() override;
  void OnMouseLeave() override;

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
  CUITextButton();
  ~CUITextButton() override;
  void OnMouseHover(int x, int y) override;
  void Paint() override;
  virtual void ResizeToText(int value);
  virtual void DrawLabel();

  void SetShadow(bool enabled, std::uint32_t color);
  void SetOutline(bool enabled, std::uint32_t color);
  void UpdateText();
  void DrawVerticalLabel();
  __declspec(noinline) void SetAlign(int horizontal, int vertical);
  void SetOrientation(int value);
  __declspec(noinline) void SetFitText(bool enabled);
  void SetBold(unsigned char enabled);
  void SetFontHeight(int height);
  void SetFont(const std::string& name);
  void SetText(const std::string& value);
  void SetTextColors(std::uint32_t normal, std::uint32_t hover, std::uint32_t pressed, std::uint32_t disabled);

  /* 00F0 */ std::string text;
  /* 0108 */ std::string displayText;
  /* 0120 */ std::string fullText;
  /* 0138 */ bool fitText;
  /* 013C */ int orientation;
  /* 0140 */ int horizontalAlign;
  /* 0144 */ int verticalAlign;
  /* 0148 */ int fontIndex;
  /* 014C */ int fontHeight;
  /* 0150 */ unsigned char bold;
  /* 0151 */ unsigned char unknown_0151[0x03];
  /* 0154 */ std::map<int, std::uint32_t> textColors;
  /* 015C */ std::uint32_t disabledTextColor;
  /* 0160 */ unsigned char shadow;
  /* 0164 */ std::uint32_t shadowColor;
  /* 0168 */ unsigned char outline;
  /* 016C */ std::uint32_t outlineColor;
  /* 0170 */ int textX;
  /* 0174 */ int textY;
  /* 0178 */ std::string tooltipText;
};

class CUIImageTextButton : public CUITextButton {
public:
  CUIImageTextButton();
  ~CUIImageTextButton() override;
  void Paint() override;
  void LoadSkin(int resolveSkin) override;

  void SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& inactive);

  /* 0190 */ std::map<int, CResPtr<CBitmapRes>> images;
  /* 0198 */ CResPtr<CBitmapRes> disabledImage;
  /* 019C */ std::map<int, std::string> imageNames;
  /* 01A4 */ std::string disabledName;
  /* 01BC */ unsigned char unknown_01BC[0x04];
};

class CUIBasicButton : public CUITextButton {
public:
  CUIBasicButton();
  ~CUIBasicButton() override;
  void Resize(int width, int height) override;
  void Paint() override;
  void LoadSkin(int resolveSkin) override;
  void DrawLabel() override;

  void Create(int width, int height);
  void SetAutoWidth(bool enabled);

  /* 0190 */ unsigned char unknown_0190;
  /* 0194 */ int defaultWidth;
  /* 0198 */ std::map<int, CResPtr<CBitmapRes>> leftImages;
  /* 01A0 */ CResPtr<CBitmapRes> disabledLeft;
  /* 01A4 */ std::map<int, CResPtr<CBitmapRes>> middleImages;
  /* 01AC */ CResPtr<CBitmapRes> disabledMiddle;
  /* 01B0 */ std::map<int, CResPtr<CBitmapRes>> rightImages;
  /* 01B8 */ CResPtr<CBitmapRes> disabledRight;
};

class CUITabButton : public CUIImageTextButton {
public:
  CUITabButton();
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnMouseEnter() override;
  void OnMouseLeave() override;
  void Paint() override;
  void LoadSkin(int resolveSkin) override;
  void ResizeToText(int padding) override;

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
  /* 01EC */ unsigned char unknown_01EC[4];
};

class CUIImageButton : public CUIButton {
public:
  CUIImageButton();
  ~CUIImageButton() override;
  void Paint() override;
  void LoadSkin(int resolveSkin) override;

  void SetImages(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& inactive);

  /* 00F0 */ std::map<int, CResPtr<CBitmapRes>> images;
  /* 00F8 */ CResPtr<CBitmapRes> disabledImage;
  /* 00FC */ unsigned char transparent;
  /* 0100 */ std::map<int, std::string> imageNames;
  /* 0108 */ std::string disabledName;
};

class CUIStaticImage : public CUIControl {
public:
  // OFFSET: 0x009BE0C0
  // FUNCTION: fn_13945
  explicit CUIStaticImage() : CUIControl(), image(), tooltip(), transparent(0), scale(100), imageName() {
  }

  void Paint() override;
  void OnMouseHover(int x, int y) override;
  void LoadSkin(int resolveSkin) override;

  void SetImage(const std::string& name);

  /* 0080 */ CResPtr<CBitmapRes> image;
  /* 0084 */ std::string tooltip;
  /* 009C */ unsigned char transparent;
  /* 009E */ unsigned short scale;
  /* 00A0 */ std::string imageName;
};

class CUITitleBar : public CUIControl {
public:
  // INLINED
  explicit CUITitleBar()
      : CUIControl(), resizable(1), left(), middle(), right(), leftName(), middleName(), rightName(), unknown_00D8(), titleText(nullptr),
        firstButton(nullptr), secondButton(nullptr) {
  }

  void Resize(int width, int height) override;
  void Paint() override;
  void LoadSkin(int resolveSkin) override;

  void SetImages(const std::string& left, const std::string& middle, const std::string& right);
  void SetSecondButton(CUIButton* button);
  void SetTitle(std::string_view title);
  // INLINED
  void SetTitle(const char* title, std::size_t length) {
    this->SetTitle(std::string_view(title, length));
  }

  /* 0080 */ unsigned char resizable;
  /* 0084 */ CResPtr<CBitmapRes> left;
  /* 0088 */ CResPtr<CBitmapRes> middle;
  /* 008C */ CResPtr<CBitmapRes> right;
  /* 0090 */ std::string leftName;
  /* 00A8 */ std::string middleName;
  /* 00C0 */ std::string rightName;
  /* 00D8 */ unsigned char unknown_00D8[0x04];
  /* 00DC */ class CUIStaticText* titleText;
  /* 00E0 */ UIWindow* firstButton;
  /* 00E4 */ UIWindow* secondButton;
};

class CUIEditCtrl;

class CUIEditBox : public CUIControl {
public:
  // INLINED
  explicit CUIEditBox()
      : CUIControl(), backgroundColor(0xF7F7F7), borderColor(0xC6C6C6), borderWidth(1), editLeft(0), editTop(0), editRight(0), editBottom(0),
        edit(nullptr), onSubmit() {
  }

  void OnCreate(int width, int height) override;
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
  // INLINED
  explicit CUIResizer()
      : CUIControl(), minWidth(0), minHeight(0), maxWidth(0), maxHeight(0), stepX(1), stepY(1), startX(0), startY(0), startWidth(0), startHeight(0),
        deltaX(0), deltaY(0), imageName(), onResize(), image() {
  }

  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void LoadSkin(int resolveSkin) override;

  void SetImage(const std::string& name);

  // INLINED
  void SetOnResize(std::function<void(int, int)> handler) {
    this->onResize = handler;
  }


  /* 0080 */ CResPtr<CBitmapRes> image;
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
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIControl) == 0x80);
static_assert(offsetof(CUIResizer, minWidth) == 0x84);
static_assert(offsetof(CUIResizer, imageName) == 0xB4);
static_assert(offsetof(CUIResizer, onResize) == 0xD0);
#endif

#endif

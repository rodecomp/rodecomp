#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIWINDOW_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIWINDOW_H

#include "Render/Primitives/UIRPData.h"
#include "UI/Layout/WindowGeometry.h"

#include <cstddef>
#include <list>
#include <string>
#include <unordered_set>
#include <vector>

class CRenderSystemBase;
class ModeEvents;
class RenderPrimitivePool;
class CRenderer;
struct ModeCursor;
struct WindowRegistration;
class UIWindow;
class UIWindowMgr;
struct RenderQueues;
struct BitmapImage;
class FontCache;
struct TextLocale;
struct tagRECT;
struct tagSIZE;
struct ActionFrame;
class CSprRes;
struct SpriteRleImage;
struct SpriteFrameCell;
class CBitmapRes;
class CActRes;
class TextureCache;
struct WindowReplayRecorder;

struct UIWindowContext {
  RenderPrimitivePool& primitives;
  CRenderSystemBase& renderSystem;
  CRenderer& renderer;
  ModeEvents*& mode;
  ModeCursor& cursor;
  std::uint32_t& cursorTick;
  WindowRegistration& registration;
  UIWindowMgr& windowMgr;
  std::unordered_set<UIWindow*>& deferredDeletion;

  void DestroyDeferredChildren();
};

class UIWindow : public UIRPData {
  friend struct WindowDialogs;
  friend struct InputFocus;
  friend class CUIDropDownBox;

public:
  explicit UIWindow(UIWindowContext& context, unsigned char cached);
  ~UIWindow() override;
  virtual void Resize(int width, int height);
  virtual bool Slot02();
  virtual bool Slot03();
  virtual void Move(int x, int y);
  virtual bool Slot05();
  virtual UIWindow* Slot06();
  virtual bool Slot07(void* output);
  virtual bool Slot08();
  virtual bool CanHitTest();
  virtual void Slot10(int argument1, int argument2, std::intptr_t argument3);
  virtual void Slot11();
  virtual void Slot12();
  virtual void Slot13(int argument1, int argument2);
  virtual void Show(int visible);
  virtual void Slot15(int width, int height);
  virtual void Slot16();
  virtual void Grow(int width, int height);
  virtual void Slot18();
  virtual void Slot19();
  virtual void Paint();
  virtual void Slot21();
  virtual void Slot22(int width, int height);
  virtual void Slot23();
  virtual void Slot24(int argument);
  virtual void Slot25(int argument1, int argument2);
  virtual void Slot26(int argument1, int argument2);
  virtual void Slot27(int argument1, int argument2);
  virtual void Slot28(int argument1, int argument2);
  virtual void Slot29(int argument1, int argument2);
  virtual void ResetCursor(int argument1, int argument2);
  virtual void Slot31(int argument1, int argument2);
  virtual void Slot32(int argument1, int argument2);
  virtual void Slot33(int argument1, int argument2);
  virtual void Slot34(int argument1, int argument2);
  virtual void Slot35(int argument);
  virtual void Slot36();
  virtual int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4);
  virtual void Invalidate();
  virtual void Slot39();
  virtual std::uint32_t Color();
  virtual bool Update(int redraw);
  virtual bool Slot42(void* output);
  virtual void Slot43();
  virtual std::vector<std::uint32_t> Slot44();
  virtual void SetId(int value);
  virtual int Slot46();
  virtual void Slot47();
  virtual void Slot48();
  virtual RenderPrimitive* DrawPrimitive();
  virtual UIWindow* HitTest(int x, int y);
  virtual void VisitChildren();
  virtual void Slot52(int argument);

  void AddChild(UIWindow* child);
  void AddChildFront(UIWindow* child);
  void DeleteChildren();
  void ClearChildren(bool destroy);
  void RemoveChild(UIWindow* child, unsigned char destroy);

  void InvalidateChildren();
  bool Contains(const UIWindow* window) const;
  bool IsDescendantOf(const UIWindow* ancestor) const;
  void BroadcastMsg4();
  void BroadcastMsg3();
  void PropagateMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4);
  void ToScreen(int& x, int& y) const;
  UIWindow* FindUid(std::uint32_t uid);
  // INLINED
  std::uint32_t Uid() const {
    return this->m_uid;
  }
  UIWindow* Root();
  UIWindow* NextFocusable(UIWindow* child);
  UIWindow* PreviousFocusable(UIWindow* child);
  void ResetStates();
  void ClearSurface(std::uint32_t color);
  void SetSize(int width, int height);
  void DrawBitmap(int x, int y, const BitmapImage* image, bool transparent);
  void FillRegion(int x, int y, int width, int height, std::uint32_t color);
  int TextWidth(FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic);
  void DrawTextAt(
    FontCache& fonts, const TextLocale& locale, int x, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold,
    unsigned char italic
  );
  void DrawVerticalText(int x, int y, const char* text, unsigned bytes, int family, int height, std::uint32_t color, unsigned char bold);
  void DrawCenteredText(int x, int y, const char* text, int width, int family, int height, std::uint32_t color, unsigned char bold);
  void DrawRightOutlinedText(
    int right, int y, const char* text, int bytes, std::uint32_t color, std::uint32_t outline, int family,
    int height, unsigned char bold
  );
  void DrawCenteredOutlinedText(
    int x, int y, const char* text, int width, int bytes, std::uint32_t color, std::uint32_t outline, int family, int height, unsigned char bold
  );
  void DrawVerticalOutlinedText(
    int x, int y, const char* text, int bytes, std::uint32_t color, std::uint32_t outline, int family, int height, unsigned char bold
  );
  void DrawRightAlignedText(int right, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold);
  void DrawShadowedText(
    int x, int y, const char* text, int bytes, std::uint32_t color, std::uint32_t shadow, int family, int height, unsigned char bold
  );
  void DrawFormattedText(int x, int y, const std::string& text, int width, int align, int family, int height, std::uint32_t color, unsigned char bold);
  void DrawEmboldenedText(int x, int y, const char* text, std::uint32_t color, std::uint32_t secondColor, int bytes, int family, int height);
  int sub_A84400(
    int x, int y, const std::string& text, int width, int lineSpacing, int align, int family, int height,
    std::uint32_t color, unsigned char bold
  );
  void FlushPrimitives(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd);
  void FillSurface(std::uint32_t color, const tagRECT* rectangle);
  void DrawBitmapScaled(int x, int y, int width, int height, const BitmapImage* image, int option);
  void DrawBitmapRegion(int x, int y, const BitmapImage* image, bool transparent, bool whole, int left, int top, int right, int bottom);
  void DrawAuxiliaryScaled(int mode, int x, int y, int width, int height, BitmapImage* image, int option, bool rebuild);
  void DrawAuxiliary(int mode, int x, int y, BitmapImage* image, bool transparent, bool rebuild);
  void DrawAmount(FontCache& fonts, const TextLocale& locale, int amount, std::string text, int x, int y);
  int TextHeight(FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic);
  int VerticalTextHeight(FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold);
  tagSIZE TextSize(FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic);
  tagSIZE VerticalTextSize(
    FontCache& fonts, const TextLocale& locale, const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
  );
  int FormattedTextWidth(std::string text, int family, int height, unsigned char bold);
  std::list<int> WrapFormattedText(std::string text, std::list<std::string>* lines, int maxWidth, int family, int height, unsigned char bold);
  int RecordEditTexts(WindowReplayRecorder& recorder);
  int RestoreEditTexts(const unsigned char* data);
  void DrawActionSprite(
    const CSprRes* sprite, const CActRes* action, int x, int y, int width, int height, int interval, bool scaled, const CBitmapRes* background
  );
  bool FitSprite(const ActionFrame* frame, const CSprRes* sprite, int width, int height, int* x, int* y, float* scale);
  void AddSpriteCellOverlay(
    int x, int y, int, int, const SpriteRleImage* image, const SpriteFrameCell* cell, float scale, float angle, std::uint32_t color,
    const std::uint32_t* palette, float extentScale
  );
  int DrawTitleBar(FontCache& fonts, const TextLocale& locale, bool closeButton, const char* title, int width);
  int DrawTitleBarAt(FontCache& fonts, const TextLocale& locale, int x, int width, bool closeButton, const char* title);
  void DrawPanelBackground(int y, int width, const char* bitmapName);
  void DrawPanelBackgroundAt(int x, int y, int width, const char* bitmapName);
  int DrawButtonBar(int style, int width, int rows);
  int DrawButtonBarAt(int style, int x, int width, int rows);
  CBitmapRes* LoadStyledBitmap(int style, const char* format, const char* name);

  // INLINED
  bool HasTextBackground() const {
    return this->unknown_0044 != 0;
  }
  // INLINED
  std::uint32_t TextBackground() const {
    return this->unknown_0048;
  }

  /* 0010 */ UIWindow* parent;
  /* 0014 */ WindowGeometry geometry;

protected:
  /* 0044 */ unsigned char unknown_0044;
  /* 0048 */ std::uint32_t unknown_0048;
  /* 004C */ std::uint32_t unknown_004C;
  /* 0050 */ std::list<UIWindow*> m_children;
  /* 0058 */ int m_dirty;
  /* 005C */ std::vector<RenderPrimitive*> m_overlayPrimitives;
  /* 0068 */ std::vector<RenderPrimitive*> unknown_0068;
  /* 0074 */ std::uint32_t m_uid;
  /* 0078 */ unsigned char m_cached;
  /* 0079 */ unsigned char m_clip;

  UIWindowContext* m_context;

private:
  UIWindow* Parent() const;
  void DeferDeletion(UIWindow* window);
  UIWindow(const UIWindow&) = delete;
  UIWindow& operator=(const UIWindow&) = delete;
};

extern std::uint32_t g_windowUid;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIWindow) == 128);
static_assert(offsetof(UIWindow, parent) == 0x10);
#endif

#endif

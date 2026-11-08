#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIWINDOW_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIWINDOW_H

#include "Render/Primitives/UIRPData.h"
#include "UI/Layout/WindowGeometry.h"

#include <cstddef>
#include <list>
#include <string>
#include <unordered_set>
#include <vector>

class UIWindow;
class UIWindowMgr;
class CUIGameSettingsUI;
struct RenderQueues;
struct BitmapImage;
class FontCache;
struct tagRECT;
struct tagSIZE;
struct ActionFrame;
class CSprRes;
struct SpriteRleImage;
struct SpriteFrameCell;
class CBitmapRes;
class CActRes;
class TextureCache;
class UITextViewer;
struct ModeDragState;
struct ITEM_INFO;
class UIItemTagButton;

class UIWindow : public UIRPData {
  friend class UIWindowMgr;
  friend class CUIDropDownBox;
  friend class CUIGameSettingsUI;
  friend class CUIRenewQuestUI;
  friend class UIItemTagOnChat;
  friend class UIScrollWnd;

public:
  explicit UIWindow(unsigned char cached);
  ~UIWindow() override;
  virtual void Resize(int width, int height);
  virtual bool IsFrameWnd();
  virtual bool NeedsFrameRefresh();
  virtual void Move(int x, int y);
  virtual bool IsEditable();
  virtual UIWindow* GetEditWindow();
  virtual bool GetTransBoxInfo(void* output);
  virtual bool IsTransmitMouseInput();
  virtual bool CanHitTest();
  virtual void DragAndDrop(int argument1, int argument2, std::intptr_t argument3);
  virtual void StoreInfo();
  virtual void StoreMoveOrigin();
  virtual void MoveFromOrigin(int argument1, int argument2);
  virtual void Show(int visible);
  virtual void OnCreate(int width, int height);
  virtual void OnInit();
  virtual void Grow(int width, int height);
  virtual void OnDestroy();
  virtual void OnProcess();
  virtual void Paint();
  virtual void OnRun();
  virtual void OnSize(int width, int height);
  virtual void OnBeginEdit();
  virtual void OnFinishEdit(int argument);
  virtual void OnLBtnDown(int argument1, int argument2);
  virtual void OnLBtnDblClk(int argument1, int argument2);
  virtual void OnMouseMove(int argument1, int argument2);
  virtual void OnMouseHover(int argument1, int argument2);
  virtual void OnMouseCheck(int argument1, int argument2);
  virtual void ResetCursor(int argument1, int argument2);
  virtual void OnLBtnUp(int argument1, int argument2);
  virtual void OnRBtnDown(int argument1, int argument2);
  virtual void OnRBtnUp(int argument1, int argument2);
  virtual void OnRBtnDblClk(int argument1, int argument2);
  virtual void OnWheel(int argument);
  virtual void RefreshSnap();
  virtual int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4);
  virtual void Invalidate();
  virtual void OnInvalidate();
  virtual std::uint32_t Color();
  virtual bool Update(int redraw);
  virtual bool GetCustomTransBoxInfo(void* output);
  virtual void DrawTransBox();
  virtual std::vector<unsigned char> GetReplayData();
  virtual void SetId(int value);
  virtual int GetWindowType();
  virtual void OnMouseEnter();
  virtual void OnMouseLeave();
  virtual RenderPrimitive* DrawPrimitive();
  virtual UIWindow* HitTest(int x, int y);
  virtual void VisitChildren();
  virtual void ApplySkin(int argument);

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
  __declspec(noinline) void ToScreen(int& x, int& y) const;
  void PlaceBeside(UIWindow* anchor);
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
  void DrawBitmap(int x, int y, CBitmapRes* bitmap, bool transparent);
  void DrawAlignedText(int x, int y, std::string text, int width, int align, std::uint32_t color, int arg7, int arg8, int arg9, int arg10, unsigned char bold);
  void FillRegion(int x, int y, int width, int height, std::uint32_t color);
  int TextWidth(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic);
  void WrapPixelText(const char* text, std::vector<std::string>& lines, int width);
  int ColoredTextWidth(const char* text, unsigned bytes, int family, int height, unsigned char bold);
  void DrawTextAt(int x, int y, const char* text, int bytes, int family, int height, std::uint32_t color, unsigned char bold,
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
  int DrawWrappedFormattedText(
    int x, int y, const std::string& text, int width, int lineSpacing, int align, int family, int height,
    std::uint32_t color, unsigned char bold
  );
  void FlushPrimitives(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd);
  void FillSurface(std::uint32_t color, const tagRECT* rectangle);
  void WrapLinkText(int* kind, std::vector<std::string>* lines, const std::string& text);
  void WrapTextWithBreaks(const char* text, std::vector<std::string>* lines, std::vector<bool>* breaks, int width);
  int DrawItemNameSegments(
    int x, int y, int width, const std::vector<char*>& breaks, const char* text, const char* highlight, std::uint32_t highlightColor,
    std::uint32_t color, int unused, int lineHeight
  );
  void DrawBitmapScaled(int x, int y, int width, int height, const BitmapImage* image, int option);
  void DrawBitmapRegion(int x, int y, const BitmapImage* image, bool transparent, bool whole, int left, int top, int right, int bottom);
  void DrawAuxiliaryScaled(int mode, int x, int y, int width, int height, BitmapImage* image, int option, bool rebuild);
  void DrawAuxiliary(int mode, int x, int y, BitmapImage* image, bool transparent, bool rebuild);
  void DrawAmount(int amount, std::string text, int x, int y);
  int TextHeight(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic);
  int VerticalTextHeight(const char* text, unsigned bytes, int family, int height, unsigned char bold);
  tagSIZE TextSize(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic);
  tagSIZE VerticalTextSize(const char* text, unsigned bytes, int family, int height, unsigned char bold, unsigned char italic
  );
  int FormattedTextWidth(std::string text, int family, int height, unsigned char bold);
  std::string FitDisplayText(int width, const char* text, int family, int height, unsigned char bold);
  std::string FitVerticalDisplayText(int height, const char* text, int family, int fontHeight, unsigned char bold);
  std::list<int> WrapFormattedText(std::string text, std::list<std::string>* lines, int maxWidth, int family, int height, unsigned char bold);
  UIItemTagButton* CreateItemTagButton(const std::string& title, const std::string& label, int tagId, UIWindow* parent, std::uint32_t color);
  std::list<int> WrapText(std::string text, std::list<std::string>* lines, int maxWidth, int family, int height, unsigned char bold);
  const char* TruncateText(const char* text, int maxBytes);
  const char* ClipTextToWidth(const char* text, int maxWidth, int family, int height, unsigned char bold);
  void RecordResponse(int response, std::intptr_t b, std::intptr_t c, std::intptr_t d);
  int RecordEditTexts();
  void RecordDrop(int x, int y, const ModeDragState& drag, int target);
  int RecordData(std::uint16_t type, std::vector<unsigned char> data);
  void RecordValues(int value, int argument1, int argument2, int argument3, int argument4);
  void RecordWindowFlag(bool value);
  void RecordText(int event, const char* text);
  int RestoreEditTexts(const unsigned char* data);
  void DrawActionSprite(
    const CSprRes* sprite, const CActRes* action, int x, int y, int width, int height, int interval, bool scaled, const CBitmapRes* background
  );
  bool FitSprite(const ActionFrame* frame, const CSprRes* sprite, int width, int height, int* x, int* y, float* scale);
  void AddSpriteCellOverlay(
    int x, int y, int, int, const SpriteRleImage* image, const SpriteFrameCell* cell, float scale, float angle, std::uint32_t color,
    const std::uint32_t* palette, float extentScale
  );
  void AddRotatedSpriteCellOverlay(int x, int y, int width, int height, const SpriteRleImage* image, const SpriteFrameCell* cell, const std::uint32_t* palette);
  int DrawTitleBar(bool closeButton, const char* title, int width);
  int DrawTitleBarAt(int x, int width, bool closeButton, const char* title);
  void DrawPanelBackground(int y, int width, const char* bitmapName);
  void DrawPanelBackgroundAt(int x, int y, int width, const char* bitmapName);
  void DrawRobeSprite(int job, int sex, int robe, int x, int y, bool overlay, int animation, int frame);
  int DrawButtonBar(int style, int width, int rows);
  int DrawButtonBarAt(int style, int x, int width, int rows);
  CBitmapRes* LoadStyledBitmap(int style, const char* format, const char* name);

  // INLINED
  bool HasTextBackground() const {
    return this->hasTextBackground != 0;
  }
  // INLINED
  std::uint32_t TextBackground() const {
    return this->textBackgroundColor;
  }

  bool IsCached() const {
    return this->m_cached != 0;
  }

  /* 0010 */ UIWindow* parent;
  /* 0014 */ WindowGeometry geometry;

protected:
  /* 0044 */ unsigned char hasTextBackground;
  /* 0048 */ std::uint32_t textBackgroundColor;
  /* 004C */ std::uint32_t renderLayer;
  /* 0050 */ std::list<UIWindow*> m_children;
  /* 0058 */ int m_dirty;
  /* 005C */ std::vector<RenderPrimitive*> m_overlayPrimitives;
  /* 0068 */ std::vector<RenderPrimitive*> unknown_0068;
  /* 0074 */ std::uint32_t m_uid;
  /* 0078 */ unsigned char m_cached;
  /* 0079 */ unsigned char m_clip;

private:
  UIWindow* Parent() const;
  UIWindow(const UIWindow&) = delete;
  UIWindow& operator=(const UIWindow&) = delete;
};

extern std::uint32_t g_windowUid;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIWindow) == 0x7C);
static_assert(offsetof(UIWindow, parent) == 0x10);
#endif

#endif

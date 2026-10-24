#ifndef RAGNAROK_SOURCE_UI_UIWINDOW_H
#define RAGNAROK_SOURCE_UI_UIWINDOW_H

#include "Render/UIRPData.h"
#include "UI/WindowAncestry.h"
#include "UI/WindowGeometry.h"

#include <unordered_set>
#include <vector>

class CRenderSystemBase;
class ModeEvents;
class RenderPrimitivePool;
class CRenderer;
struct ModeCursor;
struct WindowRegistration;
class UIWindow;
struct RenderQueues;
struct BitmapImage;
class FontCache;
struct TextLocale;

// Borrowed original owners and noncontiguous manager fields; no second manager.
struct UIWindowContext {
  RenderPrimitivePool& primitives;
  CRenderSystemBase& renderSystem;
  CRenderer& renderer;
  ModeEvents*& mode;
  ModeCursor& cursor;
  std::uint32_t& cursorTick;
  WindowRegistration& registration;
  WindowAncestry*& reference_019C;
  WindowAncestry*& reference_01A0;
  WindowAncestry*& reference_01B8;
  WindowAncestry*& reference_0A30;
  WindowAncestry*& reference_4F1D4;
  std::unordered_set<UIWindow*>& deferredDeletion;

  // Mouse-dispatch deferred children are independent of modal-gated retirees.
  // Keep set membership visible until every virtual destructor has run.
  void DestroyDeferredChildren();
};

// UIWindow vtable, 53 slots. Names of unknown slots remain
// positional; their bodies/argument extents come from the original entries.
class UIWindow : public UIRPData, public WindowAncestry {
  friend struct WindowDialogs;
  friend struct InputFocus;
  friend class CUIDropDownBox;

public:
  explicit UIWindow(UIWindowContext& context, unsigned char cached);
  ~UIWindow() override;                       // 0
  virtual void Resize(int width, int height); // 1
  virtual bool Slot02();
  virtual bool Slot03();
  virtual void Move(int x, int y); // 4
  virtual bool Slot05();
  virtual UIWindow* Slot06();
  virtual bool Slot07(void* output);
  virtual bool Slot08();
  virtual bool CanHitTest(); // 9
  virtual void Slot10(int argument1, int argument2, std::intptr_t argument3);
  virtual void Slot11();
  virtual void Slot12();
  virtual void Slot13(int argument1, int argument2);
  virtual void Show(int visible); // 14
  virtual void Slot15(int width, int height);
  virtual void Slot16();
  virtual void Grow(int width, int height); // 17
  virtual void Slot18();
  virtual void Slot19();
  virtual void Paint(); // 20
  virtual void Slot21();
  virtual void Slot22(int width, int height);
  virtual void Slot23();
  virtual void Slot24(int argument);
  virtual void Slot25(int argument1, int argument2);
  virtual void Slot26(int argument1, int argument2);
  virtual void Slot27(int argument1, int argument2);
  virtual void Slot28(int argument1, int argument2);
  virtual void Slot29(int argument1, int argument2);
  virtual void ResetCursor(int argument1, int argument2); // 30
  virtual void Slot31(int argument1, int argument2);
  virtual void Slot32(int argument1, int argument2);
  virtual void Slot33(int argument1, int argument2);
  virtual void Slot34(int argument1, int argument2);
  virtual void Slot35(int argument);
  virtual void Slot36();
  virtual int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t argument1,
    std::intptr_t argument2,
    std::intptr_t argument3,
    std::intptr_t argument4
  );                         // 37
  virtual void Invalidate(); // 38
  virtual void Slot39();
  virtual std::uint32_t Color();   // 40
  virtual bool Update(int redraw); // 41
  virtual bool Slot42(void* output);
  virtual void Slot43();
  virtual std::vector<std::uint32_t> Slot44();
  virtual void SetId(int value); // 45
  virtual int Slot46();
  virtual void Slot47();
  virtual void Slot48();
  virtual RenderPrimitive* DrawPrimitive(); // 49
  virtual UIWindow* HitTest(int x, int y);  // 50
  virtual void VisitChildren();             // 51
  virtual void Slot52(int argument);

  void AddChild(UIWindow* child);
  void AddChildFront(UIWindow* child);
  void DeleteChildren();
  void ClearChildren(bool destroy);
  void RemoveChild(UIWindow* child, unsigned char destroy);
  // Dirty this window, invoke Slot39, then recurse into children with nonzero
  // visibility (not cached-parent bubbling).
  void InvalidateChildren();
  bool Contains(const UIWindow* window) const;
  UIWindow* FindUid(std::uint32_t uid);
  std::uint32_t Uid() const {
    return this->m_uid;
  }
  UIWindow* Root();
  void ClearSurface(std::uint32_t color);
  void SetSize(int width, int height);
  void DrawBitmap(int x, int y, const BitmapImage* image, bool transparent);
  void FillRegion(int x, int y, int width, int height, std::uint32_t color);
  int TextWidth(
    FontCache& fonts,
    const TextLocale& locale,
    const char* text,
    unsigned bytes,
    int family,
    int height,
    unsigned char bold,
    unsigned char italic
  );
  void DrawTextAt(
    FontCache& fonts,
    const TextLocale& locale,
    int x,
    int y,
    const char* text,
    int bytes,
    int family,
    int height,
    std::uint32_t color,
    unsigned char bold,
    unsigned char italic
  );
  void FlushPrimitives(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd);

  // Source accessors for original direct reads by the colored text operation.
  bool HasTextBackground() const {
    return this->unknown_0044 != 0;
  }
  std::uint32_t TextBackground() const {
    return this->unknown_0048;
  }

  WindowGeometry geometry;

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
  // Source-only dependency view after all original fields. Original size124;
  // reconstructed x86 size128 includes this explicit owner pointer.
  UIWindowContext* m_context;

private:
  UIWindow* Parent() const;
  void DeferDeletion(UIWindow* window);
  UIWindow(const UIWindow&) = delete;
  UIWindow& operator=(const UIWindow&) = delete;
};

// STATIC: 0x0145723C
// GLOBAL: gl_24364
extern std::uint32_t g_windowUid;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIWindow) == 128);
#endif

#endif

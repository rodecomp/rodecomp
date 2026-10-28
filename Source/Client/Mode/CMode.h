#ifndef RAGNAROK_SOURCE_CLIENT_MODE_CMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_CMODE_H

#include <cstddef>
#include <cstdint>
#include <string>

class CTexture;
class UIWindow;
struct TextureRegion;
struct tagRECT;

class CMode {
public:
  /* 0004 */ int cursorType;
  /* 0008 */ std::uint32_t cursorFrame;
  /* 000C */ int pendingCursorType;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int running;
  /* 0018 */ int unknown_0018;
  /* 001C */ UIWindow* tooltip;
  /* 0020 */ std::uint32_t tooltipTick;
  /* 0024 */ std::uint32_t cursorTick;
  /* 0028 */ int unknown_0028;
  /* 002C */ int screenshotRequested;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ std::string screenshotName;
  /* 0050 */ int unknown_0050;
  /* 0054 */ int unknown_0054;

  virtual ~CMode();
  virtual int Run();
  virtual void Initialize(const char* name);
  virtual void End();
  virtual void Update();
  virtual void Slot5(int value);
  virtual int SendMsg(int event, int argument1, int argument2, int argument3, int argument4);
  virtual void Slot7(int value);

  CMode();
  void TakeScreenshot();
  void DrawTexture(const tagRECT& rect, int angle, float rhw, std::uint32_t color, CTexture* texture, TextureRegion region);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CMode, cursorType) == 0x4);
static_assert(offsetof(CMode, screenshotName) == 0x38);
static_assert(sizeof(CMode) == 0x58);
#endif

#endif

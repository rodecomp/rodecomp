#ifndef RAGNAROK_SOURCE_UI_ENTRYQUEUE_UIENTRYQUEUEWND_H
#define RAGNAROK_SOURCE_UI_ENTRYQUEUE_UIENTRYQUEUEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>

class UIEntryQueueWnd : public UIFrameWnd {
public:
  void Slot11() override;

  /* 00B4 */ unsigned char unknown_00B4[0x44];
  /* 0100 */ WindowPlacement* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIEntryQueueWnd, saved) == 0x100);
#endif

#endif

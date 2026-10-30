#ifndef RAGNAROK_SOURCE_UI_ENTRYQUEUE_UIENTRYQUEUEWND_H
#define RAGNAROK_SOURCE_UI_ENTRYQUEUE_UIENTRYQUEUEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <cstdint>
#include <string>

class UIBitmapButton;
class UIStaticText;
class UITextViewer;
struct UITextViewerContext;
struct WindowResponseRecorder;

struct UIEntryQueueContext {
  UITextViewerContext& text;
  WindowResponseRecorder& recorder;
};

class UIEntryQueueListBox : public UIListBox {
};

class UIEntryQueueRequestWnd : public UIFrameWnd {
public:
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIEntryQueueContext* m_entryQueue;

public:
  /* 00B4 */ unsigned char unknown_00B4[0xC];
  /* 00C0 */ unsigned m_interval;
  /* 00C4 */ unsigned m_elapsed;
  /* 00C8 */ unsigned m_delta;
  /* 00CC */ unsigned m_lastTick;
  /* 00D0 */ UIStaticText* m_countdown;
};

class UIEntryQueueStandByWnd : public UIFrameWnd {
public:
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  bool Create();
  void SetInfo(const char* queueName, int waiting, int ranking);

protected:
  UIEntryQueueContext* m_entryQueue;

public:
  /* 00B4 */ std::string m_queueName;
  /* 00CC */ int m_waiting;
  /* 00D0 */ int m_ranking;
  /* 00D4 */ UITextViewer* m_viewer;
  /* 00D8 */ UIBitmapButton* m_cancel;
  /* 00DC */ UIBitmapButton* m_close;
  /* 00E0 */ char m_queueText[0x104];
  /* 01E4 */ char m_waitingText[0x104];
  /* 02E8 */ char m_rankingText[0x104];
};

class UIEntryQueueWnd : public UIFrameWnd {
public:
  void Slot11() override;
  int SelectedIndex();

  /* 00B4 */ unsigned char unknown_00B4[0x40];
  /* 00FC */ UIEntryQueueListBox* m_list;
  /* 0100 */ WindowPlacement* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIEntryQueueWnd, m_list) == 0xFC);
static_assert(offsetof(UIEntryQueueWnd, saved) == 0x100);
#endif

#endif

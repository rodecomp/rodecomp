#ifndef RAGNAROK_SOURCE_UI_ENTRYQUEUE_UIENTRYQUEUEWND_H
#define RAGNAROK_SOURCE_UI_ENTRYQUEUE_UIENTRYQUEUEWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <cstdint>
#include <map>
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

class UIEntryQueueHelpWnd : public UIFrameWnd {
public:
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIEntryQueueContext* m_entryQueue;
};

class UIEntryQueueListBox : public UIListBox {
public:
  UIEntryQueueListBox(UIWindowContext& windows, UIListBoxContext& list);
  ~UIEntryQueueListBox() override;
  void Slot31(int x, int y) override;
  virtual void AddEntry(const char* name, int value);
  virtual int EntryValue(int index);

  /* 00D4 */ std::map<std::string, int> m_entries;
};

class UIEntryQueueRequestWnd : public UIFrameWnd {
public:
  ~UIEntryQueueRequestWnd() override;
  void Slot18() override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  bool Create();

protected:
  UIEntryQueueContext* m_entryQueue;

public:
  /* 00B4 */ UITextViewer* m_viewer;
  /* 00B8 */ UIBitmapButton* m_ok;
  /* 00BC */ UIBitmapButton* m_cancel;
  /* 00C0 */ unsigned m_interval;
  /* 00C4 */ unsigned m_elapsed;
  /* 00C8 */ unsigned m_delta;
  /* 00CC */ unsigned m_lastTick;
  /* 00D0 */ UIStaticText* m_countdown;
};

class UIEntryQueueStandByWnd : public UIFrameWnd {
public:
  ~UIEntryQueueStandByWnd() override;
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
  ~UIEntryQueueWnd() override;
  void Slot11() override;
  int SelectedIndex();
  bool ConfirmApply(const char* type);

  /* 00B4 */ unsigned char unknown_00B4[0x40];
  /* 00FC */ UIEntryQueueListBox* m_list;
  /* 0100 */ WindowPlacement* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIEntryQueueWnd, m_list) == 0xFC);
static_assert(offsetof(UIEntryQueueWnd, saved) == 0x100);
#endif

#endif

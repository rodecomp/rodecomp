#ifndef RAGNAROK_SOURCE_UI_PARTY_UIREQUESTJOINPARTYWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIREQUESTJOINPARTYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UITextViewer;
class UICheckBox;
class UIStaticText;
class UIBitmapButton;
struct UITextViewerContext;
struct JobNames;
struct PartyJoinRequest;
class UIRequestJoinPartyMgr;

struct UIRequestJoinPartyContext {
  UITextViewerContext& text;
  JobNames& jobs;
  UIRequestJoinPartyMgr& requests;
};

class UIRequestJoinPartyWnd : public UIFrameWnd {
public:
  UIRequestJoinPartyWnd(UIWindowContext& windows, UIFrameContext& frames, UIRequestJoinPartyContext& request);
  ~UIRequestJoinPartyWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UITextViewer* details;
  /* 00B8 */ UITextViewer* blockText;
  /* 00BC */ UICheckBox* block;
  /* 00C0 */ std::uint32_t unknown_00C0;
  /* 00C4 */ std::uint32_t unknown_00C4;
  /* 00C8 */ std::uint32_t unknown_00C8;
  /* 00CC */ std::uint32_t unknown_00CC;
  /* 00D0 */ std::uint32_t unknown_00D0;
  /* 00D4 */ std::uint32_t unknown_00D4;
  /* 00D8 */ std::uint32_t characterId;
  /* 00DC */ std::uint32_t accountId;
  /* 00E0 */ std::int16_t job;
  /* 00E2 */ std::int16_t level;
  /* 00E4 */ UIBitmapButton* icon;
  /* 00E8 */ UIStaticText* levelText;

private:
  UIRequestJoinPartyContext* m_request;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRequestJoinPartyWnd) == 248);
#endif

#endif

#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIRODEXCONTENTSWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIRODEXCONTENTSWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>
#include <string>

struct UIButtonContext;
class UIRodexEditCtrl3;
class UIBitmapButton;
class UIBasicButton;

class UIRodexContentsWnd : public UIFrameWnd {
public:
  UIRodexContentsWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);

  void Paint() override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void SetContentsText(const char* text);
  void SetMailContents(std::string title, std::string sender, std::string body, unsigned char hasItem, std::int64_t zeny, unsigned char flags);
  ITEM_INFO GetItemAtPoint(int x, int y);
  void DisableItemButton();
  void DisableZenyButton();
  void SetItemButtonDisabled(unsigned char disabled);
  void SetZenyButtonDisabled(unsigned char disabled);

  /* 00B4 */ UIRodexEditCtrl3* m_contents;
  /* 00B8 */ std::string m_title;
  /* 00D0 */ std::string m_sender;
  /* 00E8 */ std::int64_t m_zeny;
  /* 00F0 */ std::list<ITEM_INFO> m_items;
  /* 00F8 */ int m_itemIndex;
  /* 00FC */ UIBitmapButton* m_itemButton;
  /* 0100 */ UIBitmapButton* m_zenyButton;
  /* 0104 */ UIBasicButton* unknown_0104;
  /* 0108 */ UIBasicButton* unknown_0108;
  /* 010C */ bool unknown_010C;

protected:
  UIButtonContext* m_controls;
};

#endif

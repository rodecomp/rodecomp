#ifndef RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H

#include "UI/Lists/UIListBox.h"

#include <list>
#include <string>
#include <vector>

class UINaviLinkButton;

class UIChatHisBox : public UIListBox {
public:
  UIChatHisBox(UIWindowContext& windows, UIListBoxContext& list);
  bool Slot08() override;
  void Resize(int width, int height) override;
  void Show(int visible) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Clear() override;
  const char* HistoryText(unsigned int index);
  bool SaveChatLog(const char* path);
  void PlaceNaviLinks(int line);
  void DropOldestLines();
  void ShiftNaviLinks();
  void ClearNaviLinks();

  /* 00DC */ std::list<std::vector<UINaviLinkButton*>> naviLinks;
  /* 00E4 */ std::vector<int> unknown_00E4;
  /* 00F0 */ unsigned char unknown_00F0[0x4];
  /* 00F4 */ std::vector<std::string> history;
  /* 0100 */ std::vector<std::string> unknown_0100;
  /* 010C */ std::vector<int> unknown_010C;
  /* 0118 */ std::vector<std::string> unknown_0118;
};

#endif

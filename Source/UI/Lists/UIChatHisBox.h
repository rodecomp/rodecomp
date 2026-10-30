#ifndef RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H

#include "UI/Lists/UIListBox.h"

#include <string>
#include <vector>

class UIChatHisBox : public UIListBox {
public:
  void Show(int visible) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;
  const char* HistoryText(unsigned int index);
  bool SaveChatLog(const char* path);

  /* 00DC */ unsigned char unknown_00DC[0x18];
  /* 00F4 */ std::vector<std::string> history;
};

#endif

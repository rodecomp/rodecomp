#ifndef RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H

#include "UI/Lists/UIListBox.h"

#include <string>
#include <vector>

class UIChatHisBox : public UIListBox {
public:
  const char* HistoryText(unsigned int index);

  /* 00DC */ unsigned char unknown_00DC[0x18];
  /* 00F4 */ std::vector<std::string> history;
};

#endif

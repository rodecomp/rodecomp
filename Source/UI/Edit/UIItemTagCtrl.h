#ifndef RAGNAROK_SOURCE_UI_EDIT_UIITEMTAGCTRL_H
#define RAGNAROK_SOURCE_UI_EDIT_UIITEMTAGCTRL_H

#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIEditCtrl.h"

#include <list>
#include <string>

class UIItemTagCtrl : public UIEditCtrl {
public:
  virtual bool CanInsertTag(const std::string& name, int width);

  std::string AppendTag(const std::string& name);

  /* 011C */ int unknown_011C;
  /* 0120 */ std::list<ItemTagRecord> tags;
  /* 0128 */ std::string taggedText;
  /* 0140 */ unsigned char unknown_0140[0x18];
  /* 0158 */ int tagWidth;
};

#endif

#ifndef RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H

#include "UI/Lists/UIListBox.h"

#include <vector>

class UITagListBox : public UIListBox {
public:
  void AddLine(const char* text) override;
  void Clear() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;

protected:
  /* 00D4 */ std::vector<UIWindow*> m_tagWindows;
};

#endif

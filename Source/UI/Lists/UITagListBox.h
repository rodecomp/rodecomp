#ifndef RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H

#include "UI/Lists/UIListBox.h"

#include <string>
#include <vector>

struct ITEM_INFO;

class UITagListBox : public UIListBox {
public:
  void AddLine(const char* text) override;
  void Clear() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;

  int DecodeNumber(std::string text);
  bool ItemFromFields(const std::vector<int>& fields, ITEM_INFO& item);
  void DecodeValues(std::vector<int>& values, const char* text, int& position, const int& end, const char& separator, const char& alternate);
  int DecodeCards(std::vector<int>& fields, const char* text, int& position, const char& separator, const int& end, int index);

protected:
  /* 00D4 */ std::vector<UIWindow*> m_tagWindows;
};

#endif

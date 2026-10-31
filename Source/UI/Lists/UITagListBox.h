#ifndef RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H

#include "UI/Lists/UIListBox.h"

#include <string>
#include <vector>

struct ITEM_INFO;
class UITagButton;

class UITagListBox : public UIListBox {
public:
  struct STagInfoV2 {
    /* 0004 */ int unknown_0004;
    /* 0008 */ int unknown_0008;
    /* 000C */ int unknown_000C;
    /* 0010 */ std::string unknown_0010;
    /* 0028 */ std::string unknown_0028;

    virtual ~STagInfoV2();
  };

  void AddLine(const char* text) override;
  void Clear() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void sub_9B9EA0(std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4);
  void sub_9BC0A0(std::intptr_t type, UIWindow* sender);
  std::string sub_9B8AA0(std::string text);
  void sub_9BB8E0(std::string text, std::vector<STagInfoV2>& tags, int width);
  int DecodeNumber(std::string text);
  void DecodeItemFields(const std::string& text, std::vector<int>& fields);
  bool ItemFromFields(const std::vector<int>& fields, ITEM_INFO& item);
  void DecodeValues(std::vector<int>& values, const char* text, int& position, const int& end, const char& separator, const char& alternate);
  int DecodeCards(std::vector<int>& fields, const char* text, int& position, const char& separator, const int& end, int index);

protected:
  /* 00D4 */ std::vector<UITagButton*> m_tagWindows;
};

#endif

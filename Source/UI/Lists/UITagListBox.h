#ifndef RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H

#include "UI/Lists/UIListBox.h"
#include "UI/TextTags.h"

#include <string>
#include <utility>
#include <vector>

struct ITEM_INFO;
class UITagButton;

class UITagListBox : public UIListBox {
public:
  struct STagInfoV2 {
    /* 0004 */ int type;
    /* 0008 */ int startOffset;
    /* 000C */ int endOffset;
    /* 0010 */ std::string label;
    /* 0028 */ std::string data;

    virtual ~STagInfoV2();
  };

  void AddLine(const char* text) override;
  void Clear() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void AppendText(std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4);
  void OnTagClick(std::intptr_t type, UIWindow* sender);
  std::string ParseTaggedText(std::string text, std::vector<STagInfoV2>& tags);
  std::pair<std::string, std::string> ParseTag(std::string text, int tagIndex);
  void AppendTaggedLines(std::string text, std::vector<STagInfoV2>& tags, int width);
  int DecodeNumber(std::string text);
  void DecodeItemFields(const std::string& text, std::vector<int>& fields);
  bool ItemFromFields(const std::vector<int>& fields, ITEM_INFO& item);
  void DecodeValues(std::vector<int>& values, const char* text, int& position, const int& end, const char& separator, const char& alternate);
  int DecodeCards(std::vector<int>& fields, const char* text, int& position, const char& separator, const int& end, int index);
  void AddTagButton(STagInfoV2 tag, int lineCount);

protected:
  /* 00D4 */ std::vector<UITagButton*> m_tagWindows;
};

#endif

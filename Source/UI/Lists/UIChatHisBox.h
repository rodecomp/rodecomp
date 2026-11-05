#ifndef RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UICHATHISBOX_H

#include "UI/Lists/UIListBox.h"

#include <list>
#include <string>
#include <vector>

class UIItemTagButton;
class UINaviLinkButton;

class UIChatHisBox : public UIListBox {
public:
  UIChatHisBox();
  ~UIChatHisBox() override;
  bool Slot08() override;
  void Resize(int width, int height) override;
  void Show(int visible) override;
  void Paint() override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Clear() override;
  virtual void AddText(const char* text, int color, const char* style);
  void AddTextWrapped(const char* text, int color, const char* style);
  void sub_89DC30(const char* text, int& color, int& mode);
  void sub_89E0F0(const char* text, int& color, int& mode);
  std::string RenderItemLinks(const char* lineText, std::string& source);
  std::string RenderNavigationLinks(const char* text, std::string& source);
  void RewrapLinkLines(std::vector<std::string>& wrapped, int& mode);
  __declspec(noinline) const char* HistoryText(unsigned int index);
  bool SaveChatLog(const char* path);
  __declspec(noinline) void PlaceNaviLinks(int line);
  void DropOldestLines();
  void ShiftNaviLinks();
  void ClearNaviLinks();
  void DropOldestRecords();
  void AddLinkRecord(const char* name, int value, const char* detail);
  virtual void ResetContents();
  void ShiftItemTags();
  __declspec(noinline) void PlaceItemTags(int line);
  void CopyHistory(const UIChatHisBox& other);
  void Resize(int width, int height, bool flag);
  void sub_89C080();
  std::string ResolvePlayerTags(const char* name);
  bool Name(const std::string& encoded, std::string& output);
  bool Next(std::string& remaining, std::string& encoded, unsigned& position, const char* original);
  std::string Convert(const std::string& text);
  const std::string BracketName(const std::string& name);

  /* 00D4 */ std::list<std::vector<UIItemTagButton*>> itemTags;

  /* 00DC */ std::list<std::vector<UINaviLinkButton*>> naviLinks;
  /* 00E4 */ std::vector<int> unknown_00E4;
  /* 00F0 */ int maxLines;
  /* 00F4 */ std::vector<std::string> history;
  /* 0100 */ std::vector<std::string> unknown_0100;
  /* 010C */ std::vector<int> unknown_010C;
  /* 0118 */ std::vector<std::string> unknown_0118;
};

#endif

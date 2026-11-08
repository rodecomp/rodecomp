#ifndef RAGNAROK_SOURCE_UI_EDIT_UIITEMTAGCTRL_H
#define RAGNAROK_SOURCE_UI_EDIT_UIITEMTAGCTRL_H

#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/TextTags.h"

#include <list>
#include <string>
#include <vector>

struct ITEM_INFO;

class UIItemTagCtrl : public UIEditCtrl {
public:
  UIItemTagCtrl();
  ~UIItemTagCtrl() override;
  virtual bool CanInsertTag(const std::string& name, int width);
  virtual void AddTagButton(UIWindow* button);
  virtual void RemoveTagChild(UIWindow* child);
  virtual void PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int unused) = 0;

  void ClearTags();
  std::string AppendTag(const std::string& name);
  int MeasureText(const std::string& text, const unsigned& bytes);
  bool SyncTagsWithText();
  bool RemoveInvalidTag(std::list<ItemTagRecord>::iterator& entry);
  void SetTaggedText(const std::string& text, int width);
  void UpdateItemTags();
  void AddItemTag(const ITEM_INFO& item, unsigned int color);
  std::string AddSkillTag(int tagId, unsigned int color);

  /* 011C */ int previousTagCount;
  /* 0120 */ std::list<ItemTagRecord> tags;
  /* 0128 */ std::string taggedText;
  /* 0140 */ std::string linkedText;
  /* 0158 */ int tagWidth;
};

class UIItemTagOnChat : public UIItemTagCtrl {
public:
  UIItemTagOnChat();
  bool CanInsertTag(const std::string& name, int width) override;
  void RemoveTagChild(UIWindow* child) override;
  void PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int unused) override;
};

class UIItemTagOnMail : public UIItemTagCtrl {
public:
  UIItemTagOnMail();
  bool CanInsertTag(const std::string& name, int width) override;
  void AddTagButton(UIWindow* button) override;
  void SetLines(const std::vector<std::string>& lines);
  void RemoveTagChild(UIWindow* child) override;
  void PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int unused) override;

  /* 015C */ std::vector<std::string> m_lines;
};

class UIItemTagOnMegaPhone : public UIItemTagCtrl {
public:
  UIItemTagOnMegaPhone();
  bool CanInsertTag(const std::string& name, int width) override;
  void AddTagButton(UIWindow* button) override;
  void RemoveTagChild(UIWindow* child) override;
  void PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int unused) override;

  /* 015C */ int textWidth;
};

#endif

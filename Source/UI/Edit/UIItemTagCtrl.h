#ifndef RAGNAROK_SOURCE_UI_EDIT_UIITEMTAGCTRL_H
#define RAGNAROK_SOURCE_UI_EDIT_UIITEMTAGCTRL_H

#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Edit/UIEditCtrl.h"

#include <list>
#include <string>
#include <vector>

struct ITEM_INFO;

class UIItemTagCtrl : public UIEditCtrl {
public:
  virtual bool CanInsertTag(const std::string& name, int width);
  virtual void AddTagButton(UIWindow* button);

  std::string AppendTag(const std::string& name);
  int MeasureText(const std::string& text, const unsigned& bytes);
  bool SyncTagsWithText();
  void SetTaggedText(const std::string& text, int width);
  void sub_89CFE0(const ITEM_INFO& item, unsigned int color);
  std::string sub_874A40(int tagId, unsigned int color);

  /* 011C */ int unknown_011C;
  /* 0120 */ std::list<ItemTagRecord> tags;
  /* 0128 */ std::string taggedText;
  /* 0140 */ std::string unknown_0140;
  /* 0158 */ int tagWidth;
};

class UIItemTagOnChat : public UIItemTagCtrl {
public:
  UIItemTagOnChat();
  bool CanInsertTag(const std::string& name, int width) override;
  void PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int unused);
};

class UIItemTagOnMail : public UIItemTagCtrl {
public:
  bool CanInsertTag(const std::string& name, int width) override;
  void AddTagButton(UIWindow* button) override;
  void SetLines(const std::vector<std::string>& lines);
  void PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int unused);

  /* 015C */ std::vector<std::string> m_lines;
};

class UIItemTagOnMegaPhone : public UIItemTagCtrl {
public:
  void AddTagButton(UIWindow* button) override;
  void PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int unused);

  /* 015C */ int textWidth;
};

#endif

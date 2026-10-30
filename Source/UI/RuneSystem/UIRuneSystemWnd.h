#ifndef RAGNAROK_SOURCE_UI_RUNESYSTEM_UIRUNESYSTEMWND_H
#define RAGNAROK_SOURCE_UI_RUNESYSTEM_UIRUNESYSTEMWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>
#include <vector>

class Decom_ItemInfo;
class RuneSetData;

class UIRuneSetReWard : public UIBitmapButton {
public:
  void Paint() override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0120 */ int m_itemId;
  /* 0124 */ int m_state;
  /* 0128 */ std::string m_texts[3];
};

class UIRuneSystemTab : public UIWindow {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ int m_count;
  /* 0080 */ std::string m_label;
};

class UIEquipRuneSetWnd : public UIWindow {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
};

class UIRuneSystem_tagListWnd : public UIWindow {
public:
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ int unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int m_firstTag;
};

class UIRuneSetList : public UIScrollBar {
public:
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void DrawScrollBar(bool drawLine);
  void ScrollBy(int delta);
  void Select(int index);

  /* 00B4 */ int m_itemHeight;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int m_selected;
};

class UIRuneList : public UIRuneSetList {
public:
  void Slot28(int x, int y) override;
};

class UIDecomlist : public UIRuneSetList {
public:
  void Slot28(int x, int y) override;
};

class UIGage_Circle : public UIWindow {
};

class UIRuneSystem_ActiveWnd : public UIWindow {
public:
  int HitTestItem(int x, int y);

  /* 007C */ int m_columns;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int m_listId;
};

class UIRuneSystem_Decom_RadioBtn : public UIWindow {
};

class UIRuneSet_Info : public UIWindow {
public:
  void Slot19() override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void UpdateActivation();

  /* 007C */ RuneSetData* m_runeSet;
  /* 0080 */ UIRuneSystem_ActiveWnd* m_activeWnd;
  /* 0084 */ UIBitmapButton* unknown_0084;
  /* 0088 */ UIBitmapButton* unknown_0088;
  /* 008C */ UIBitmapButton* unknown_008C;
  /* 0090 */ UIRuneSystemTab* m_tab;
  /* 0094 */ UIGage_Circle* m_gauge;
};

class UIRune_Info : public UIWindow {
};

class UIRune_Decom : public UIWindow {
public:
  void Reset();
  int HitTestItem(int x, int y);
  void SelectDecomType(unsigned int type);

  /* 007C */ std::string unknown_007C;
  /* 0094 */ std::string unknown_0094;
  /* 00AC */ unsigned int m_ownedCount;
  /* 00B0 */ std::vector<Decom_ItemInfo>* m_decomList;
  /* 00B4 */ int m_itemId;
  /* 00B8 */ UIBitmapButton* m_decomButton;
  /* 00BC */ unsigned int m_decomType;
  /* 00C0 */ UIRuneSystem_Decom_RadioBtn* m_radioButtons[2];
};

class UIRuneSystemWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Slot25(int x, int y) override;

  /* 00B4 */ unsigned char unknown_00B4[0x0C];
  /* 00C0 */ UIRuneSystemTab* m_tab;
  /* 00C4 */ UIEquipRuneSetWnd* m_equipRuneSet;
  /* 00C8 */ UIRuneSystem_tagListWnd* m_tagList;
  /* 00CC */ UIRuneSetList* m_runeSetList;
  /* 00D0 */ UIRuneList* m_runeList;
  /* 00D4 */ UIDecomlist* m_decomList;
  /* 00D8 */ UIRuneSet_Info* m_runeSetInfo;
  /* 00DC */ UIRune_Info* m_runeInfo;
  /* 00E0 */ UIRune_Decom* m_runeDecom;
};

#endif

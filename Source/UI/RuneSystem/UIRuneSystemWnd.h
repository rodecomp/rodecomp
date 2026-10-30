#ifndef RAGNAROK_SOURCE_UI_RUNESYSTEM_UIRUNESYSTEMWND_H
#define RAGNAROK_SOURCE_UI_RUNESYSTEM_UIRUNESYSTEMWND_H

#include "Client/Input/InputCharset.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowDialogs.h"

#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>

class Decom_ItemInfo;
class RuneData;
class RuneSetData;

class UIRuneSetReWard : public UIBitmapButton {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0120 */ int m_itemId;
  /* 0124 */ int m_state;
  /* 0128 */ std::string m_texts[3];
};

class UIRuneSystemTab : public UIWindow {
public:
  explicit UIRuneSystemTab(UIWindowContext& windows);
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ int m_count;
  /* 0080 */ std::string m_label;
};

class UIEquipRuneSetWnd : public UIWindow {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
};

class UIRuneSystem_tagListWnd : public UIWindow {
public:
  UIRuneSystem_tagListWnd(UIWindowContext& windows, UIButtonContext& controls);
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ UIBitmapButton* m_leftButton;
  /* 0080 */ UIBitmapButton* m_rightButton;
  /* 0084 */ int m_firstTag;
  /* 0088 */ int m_visibleCount;
};

class UIRuneSetList : public UIScrollBar {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void DrawScrollBar(bool drawLine);
  void ScrollBy(int delta);
  void Select(int index);
  __declspec(noinline) void DrawRuneIcon(int x, int y, const char* name, bool active);

  /* 00B4 */ int m_itemHeight;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int m_selected;
};

class UIRuneList : public UIRuneSetList {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
};

class UIDecomlist : public UIRuneSetList {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
};

struct RuneSetInfoLine {
  // INLINED
  RuneSetInfoLine(std::string value, std::uint32_t lineColor, bool lineBold, int lineX) {
    this->text = value;
    this->color = lineColor;
    this->bold = lineBold;
    this->x = lineX;
  }

  /* 0000 */ std::string text;
  /* 0018 */ std::uint32_t color;
  /* 001C */ bool bold;
  /* 0020 */ int x;
};

class UIRuneSetinfoList : public UIRuneSetList {
public:
  void Paint() override;
  __declspec(noinline) void AddLine(std::string text, std::uint32_t color, bool bold);
  __declspec(noinline) void ClearLines();

  /* 00C0 */ std::vector<RuneSetInfoLine> m_lines;
};

class UIGage_Circle : public UIWindow {
public:
  void Paint() override;

  /* 007C */ int m_textY;
  /* 0080 */ int m_value;
  /* 0084 */ bool m_max;
  /* 0088 */ std::string m_emptyBitmap;
  /* 00A0 */ std::string m_fullBitmap;
  /* 00B8 */ std::string m_maxBitmap;
};

class UIRuneSystem_ActiveWnd : public UIWindow {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  int HitTestItem(int x, int y);
  __declspec(noinline) void DrawCount(int x, int y, int count, int total, std::uint32_t color, int fontHeight);

  /* 007C */ int m_columns;
  /* 0080 */ int m_rows;
  /* 0084 */ int m_listId;
};

class UIRuneSystem_DecomItemWnd : public UIRuneSystem_ActiveWnd {
public:
  void Paint() override;
};

class UIRuneSystem_Decom_RadioBtn : public UIRadioBtn {
public:
  void Paint() override;

  /* 00F0 */ std::string m_disabledBitmap;
};

class UIRuneSet_Info : public UIWindow {
public:
  void Slot19() override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  __declspec(noinline) void UpdateActivation();
  __declspec(noinline) void UpdateRewards();
  __declspec(noinline) void Refresh();
  __declspec(noinline) void RefreshDescription();

  /* 007C */ RuneSetData* m_runeSet;
  /* 0080 */ UIRuneSystem_ActiveWnd* m_activeWnd;
  /* 0084 */ UIBitmapButton* unknown_0084;
  /* 0088 */ UIBitmapButton* unknown_0088;
  /* 008C */ UIBitmapButton* unknown_008C;
  /* 0090 */ UIRuneSystemTab* m_tab;
  /* 0094 */ UIGage_Circle* m_gauge;
  /* 0098 */ UIStaticText* m_gradeText;
  /* 009C */ UIRuneSystem_ActiveWnd* m_gradeItems;
  /* 00A0 */ UIBitmapButton* m_gradeButton;
  /* 00A4 */ UIRuneSetinfoList* m_infoList;
  /* 00A8 */ UIRuneSetReWard* m_rewards[7];
};

class UIRune_Info : public UIWindow {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ RuneData* m_rune;
  /* 0080 */ UIRuneSystem_ActiveWnd* m_activeWnd;
  /* 0084 */ UIBitmapButton* m_activateButton;
};

class UIRune_Decom : public UIWindow {
public:
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void Reset();
  int HitTestItem(int x, int y);
  void SelectDecomType(unsigned int type);
  __declspec(noinline) void SetItem(int itemId);

  /* 007C */ std::string unknown_007C;
  /* 0094 */ std::string unknown_0094;
  /* 00AC */ unsigned int m_ownedCount;
  /* 00B0 */ std::vector<Decom_ItemInfo>* m_decomList;
  /* 00B4 */ int m_itemId;
  /* 00B8 */ UIBitmapButton* m_decomButton;
  /* 00BC */ unsigned int m_decomType;
  /* 00C0 */ UIRuneSystem_Decom_RadioBtn* m_radioButtons[2];
};

class UIRune_ViewInfo : public UIWindow {
public:
  void Paint() override;

  /* 007C */ UIGage_Circle* m_gauge;
};

#pragma pack(push, 1)
struct RuneSystemOpenPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ unsigned char open;
};

struct RuneDecomResultPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ int itemIds[8];
  /* 0022 */ std::int16_t counts[8];
};
#pragma pack(pop)

class UIRuneSystem_DecomResultWnd : public UIFrameWnd {
public:
  UIRuneSystem_DecomResultWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ int m_total;
  /* 00B8 */ int m_itemIds[8];
  /* 00D8 */ int m_counts[8];
};

class UIRuneSystemWnd : public UIFrameWnd {
public:
  UIRuneSystemWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  __declspec(noinline) void SelectTab();

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ std::int64_t m_nextTick;
  /* 00C0 */ UIRuneSystemTab* m_tab;
  /* 00C4 */ UIEquipRuneSetWnd* m_equipRuneSet;
  /* 00C8 */ UIRuneSystem_tagListWnd* m_tagList;
  /* 00CC */ UIRuneSetList* m_lists[3];
  /* 00D8 */ UIRuneSet_Info* m_runeSetInfo;
  /* 00DC */ UIRune_Info* m_runeInfo;
  /* 00E0 */ UIRune_Decom* m_runeDecom;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RuneSetInfoLine) == 0x24);
static_assert(sizeof(RuneSystemOpenPacket) == 3);
static_assert(sizeof(RuneDecomResultPacket) == 0x32);
#endif

#endif

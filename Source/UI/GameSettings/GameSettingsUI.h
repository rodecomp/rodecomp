#ifndef RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H
#define RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H

#include "Render/DisplayMode.h"
#include "Render/RenderAdapter.h"
#include "Render/RenderBackends.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIFrameWnd.h"

#include <functional>
#include <string>

enum TALKTYPE : int;

class CUICheckBox;
class CUIRadioButton;
class CUIStaticText;
class CUIImageButton;

extern int g_textureDetailLevel;
extern int g_textureFiltering;
extern bool g_gameOptionFlag;

template <class T, class D> class CUIComboBox : public CUIControl {
public:
  explicit CUIComboBox(UIWindowContext& context)
      : CUIControl(context), unknown_0084(), unknown_0088(), listBox(nullptr), unknown_0090(), itemHeight(0), visibleItems(1), onSelect(), onChange() {
  }

  void Paint() override;
  int AddItem(const D& value, bool refresh);
  unsigned int Selected() const;
  __declspec(noinline) void UpdateListHeight();

  /* 0084 */ int unknown_0084;
  /* 0088 */ CUIImageButton* unknown_0088;
  /* 008C */ CUIListBoxEx<T, D>* listBox;
  /* 0090 */ int unknown_0090;
  /* 0094 */ int itemHeight;
  /* 0098 */ int visibleItems;
  /* 009C */ int unknown_009C;
  /* 00A0 */ std::function<void(T*, const D&)> onSelect;
  /* 00C8 */ std::function<void(T*, const D&)> onChange;
};

namespace GameSettingsUI {
class CUIComboItem;
class CUIOptionItem;

class CUIGroup : public CUIControl {
public:
  virtual void Apply();
  virtual void Refresh() = 0;

  /* 0080 */ CUIStaticText* m_title;
};

class CUIGroupEmblem : public CUIGroup {
public:
  void Slot16() override;
  void Apply() override;
  void Refresh() override;
  __declspec(noinline) void SetEnabled(bool enabled);
  void OnSelect(CUIRadioButton* button);

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIGroupFullscreen : public CUIGroup {
public:
  void Slot16() override;
  void Refresh() override;

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIGroupGraphics : public CUIGroup {
public:
  void Refresh() override;
  int GetPendingBackend();
  void LoadAdapters();
  void LoadModes();
  void OnAdapterSelect(CUIComboItem* item, const RenderAdapter& adapter);
  void OnModeSelect(CUIComboItem* item, const DisplayMode& mode);

  /* 0084 */ CUIComboBox<CUIComboItem, RenderAdapter>* m_adapterCombo;
  /* 0088 */ CUIStaticText* m_modeText;
  /* 008C */ CUIComboBox<CUIComboItem, DisplayMode>* m_modeCombo;
  /* 0090 */ RenderAdapter m_adapter;
  /* 00F8 */ RenderAdapter m_pendingAdapter;
  /* 0160 */ DisplayMode m_mode;
  /* 0184 */ DisplayMode m_pendingMode;
};

class CUIGroupGraphicsAPI : public CUIGroup {
public:
  void Slot16() override;
  void Apply() override;
  void Refresh() override;
  void LoadBackends();
  void OnSelect(CUIComboItem* item, const RenderBackend& backend);

  /* 0084 */ CUIComboBox<CUIComboItem, RenderBackend>* m_combo;
  /* 0088 */ int m_backend;
  /* 008C */ int m_pendingBackend;
};

class CUIGroupLogInOut : public CUIGroup {
public:
  void Slot16() override;
  void Apply() override;
  void Refresh() override;
  __declspec(noinline) void SetEnabled(bool enabled);
  void OnSelect(CUIRadioButton* button);

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIGroupProcessPriority : public CUIGroup {
public:
  void Slot16() override;
  void Refresh() override;
  void OnSelect(CUIRadioButton* button);

  /* 0084 */ CUIRadioButton* m_highButton;
  /* 0088 */ CUIStaticText* m_highText;
  /* 008C */ CUIRadioButton* m_normalButton;
  /* 0090 */ CUIStaticText* m_normalText;
  /* 0094 */ CUIRadioButton* m_idleButton;
  /* 0098 */ CUIStaticText* m_idleText;
};

class CUIGroupRodexSpam : public CUIGroup {
public:
  void Slot16() override;
  void Apply() override;
  void Refresh() override;
  void OnSelect(CUIRadioButton* button);

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIGroupSkin : public CUIGroup {
public:
  void Slot15(int width, int height) override;
  void Slot16() override;
  void Apply() override;
  void Refresh() override;

  /* 0084 */ CUIComboBox<CUIComboItem, std::string>* m_combo;
};

class CUIGroupSound : public CUIGroup {
public:
  void Slot16() override;
  void Apply() override;
  void Paint() override;
  void Refresh() override;
  __declspec(noinline) void SetBgmEnabled(bool enabled);
  __declspec(noinline) void SetBgmVolume(int volume);
  __declspec(noinline) void SetEffectVolume(int volume);
  void OnBgmMute(bool muted);
  void OnEffectMute(bool muted);
  void OnBgmScroll(int position);
  void OnEffectScroll(int position);

  /* 0084 */ CUIStaticText* m_bgmText;
  /* 0088 */ CUIVScrollBar* m_bgmSlider;
  /* 008C */ CUICheckBox* m_bgmMute;
  /* 0090 */ CUIStaticText* unknown_0090;
  /* 0094 */ CUIStaticText* m_effectText;
  /* 0098 */ CUIVScrollBar* m_effectSlider;
  /* 009C */ CUICheckBox* m_effectMute;
  /* 00A0 */ CUIStaticText* unknown_00A0;
};

class CUIGroupSpriteDetailLevel : public CUIGroup {
public:
  void Slot16() override;
  void Paint() override;
  void Refresh() override;

  /* 0084 */ CUIVScrollBar* m_slider;
};

class CUIGroupTextureDetailLevel : public CUIGroup {
public:
  void Slot16() override;
  void Paint() override;
  void Refresh() override;

  /* 0084 */ CUIVScrollBar* m_slider;
};

class CUIGroupTrilinear : public CUIGroup {
public:
  void Slot16() override;
  void Refresh() override;

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIPage : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;
  virtual void Apply();
  virtual void Refresh();
};

class CUIGraphicsPage : public CUIPage {
public:
  __declspec(noinline) bool IsModified();
  int GetPendingBackend();
  void LoadAdapters();

  /* 0080 */ CUIGroupGraphicsAPI* m_graphicsApi;
  /* 0084 */ CUIGroupGraphics* m_graphics;
  /* 0088 */ CUIGroupSpriteDetailLevel* m_spriteDetailLevel;
  /* 008C */ CUIGroupTextureDetailLevel* m_textureDetailLevel;
  /* 0090 */ CUIGroupFullscreen* m_fullscreen;
  /* 0094 */ CUIGroupTrilinear* m_trilinear;
};

class CUIListPage : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;
  virtual void Slot54(CUIOptionItem* item, const TALKTYPE& type);
  bool SetOptionChecked(int id);

  /* 0080 */ CUIListBoxEx<CUIOptionItem, TALKTYPE>* m_optionList;
};

class CUIComboItem : public CUIListItem {
public:
  void Slot15(int width, int height) override;
  void Slot16() override;

  /* 00E8 */ CUIStaticText* m_text;
};

class CUIOptionItem : public CUIListItem {
public:
  void Slot16() override;
  void Paint() override;

  /* 00E8 */ CUICheckBox* m_check;
  /* 00EC */ unsigned char unknown_00EC[0x0C];
  /* 00F8 */ int m_optionId;
};

std::string GetHotkeyText(int tab, int index);
}

class CUIGameSettingsUI : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;

  void sub_A3F0F0();
  void sub_A421E0();
  void sub_A433D0(UIWindow* window);
  bool IsTab4Selected();
  void SelectTab4();
  void sub_A43380();
  void sub_A43390();
  void sub_A433A0();
  void sub_A433B0();
  void sub_A433C0();
  void sub_A43590(int id);
  void sub_A43BA0();
  void sub_A43C70();

  /* 00B8 */ unsigned char unknown_00B8[0x1C];
  /* 00D8 */ CUIControl* m_titleBar;
  /* 00DC */ GameSettingsUI::CUIPage* m_currentPage;
  /* 00E0 */ GameSettingsUI::CUIGraphicsPage* m_graphicsPage;
  /* 00E4 */ std::vector<GameSettingsUI::CUIListPage*> m_pages;
  /* 00F0 */ CUITab* m_tab;
  /* 00F4 */ CUIControl* m_pageContainer;
  /* 00F8 */ CUIControl* m_applyButton;
};

#endif

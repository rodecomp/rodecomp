#ifndef RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H
#define RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H

#include "Render/DisplayMode.h"
#include "Render/RenderAdapter.h"
#include "Render/RenderBackends.h"
#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIFrameWnd.h"

#include <functional>
#include <map>
#include <new>
#include <optional>
#include <string>

void ToggleGameSettingsTab4();

enum TALKTYPE : int;

class CUICheckBox;
class CUIRadioButton;
class CUIStaticText;
class CUIImageButton;
class CUITextButton;

extern int g_textureDetailLevel;
extern int g_textureFiltering;
extern int g_configuredRenderBackend;
extern bool g_restartClient;
extern bool g_gameOptionFlag;
extern std::map<int, int> g_optionHotkeys;

template <class T, class D> class CUIComboBox : public CUIControl {
public:
  explicit CUIComboBox()
      : CUIControl(), display(), button(), dropDownButton(), listBox(nullptr), width(), itemHeight(0), visibleItems(1), onSelect(), onChange() {
  }

  void Paint() override;
  int AddItem(const D& value, bool refresh);
  unsigned int Selected() const;
  __declspec(noinline) void UpdateListHeight();
  void Create(int width, int itemHeight, int visibleItems);
  void SetItemBinder(std::function<void(T*, const D&)> binder);
  // INLINED
  void SetOnSelect(std::function<void(T*, const D&)> handler) {
    this->onSelect = handler;
  }
  void SetButton(const std::string& normal, const std::string& hover, const std::string& pressed, const std::string& disabled);

  /* 0080 */ T* display;
  /* 0084 */ CUIButton* button;
  /* 0088 */ CUIImageButton* dropDownButton;
  /* 008C */ CUIListBoxEx<T, D>* listBox;
  /* 0090 */ int width;
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
  // INLINED
  explicit CUIGroup() : CUIControl(), m_title(nullptr) {
  }

  // INLINED
  explicit CUIGroup(int) : CUIControl() {
  }

  virtual void Apply();
  virtual void Refresh() = 0;

  /* 0080 */ CUIStaticText* m_title;
};

class CUIGroupEmblem : public CUIGroup {
public:
  __declspec(noinline) CUIGroupEmblem();
  void OnCreate(int width, int height) override;
  void OnInit() override;
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
  __declspec(noinline) CUIGroupFullscreen();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Refresh() override;

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIGroupGraphics : public CUIGroup {
public:
  __declspec(noinline) CUIGroupGraphics();
  void OnCreate(int width, int height) override;
  void OnInit() override;
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
  __declspec(noinline) CUIGroupGraphicsAPI();
  void OnCreate(int width, int height) override;
  void OnInit() override;
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
  __declspec(noinline) CUIGroupLogInOut();
  void OnCreate(int width, int height) override;
  void OnInit() override;
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
  __declspec(noinline) CUIGroupProcessPriority();
  void OnCreate(int width, int height) override;
  void OnInit() override;
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
  __declspec(noinline) CUIGroupRodexSpam();
  void OnCreate(int width, int height) override;
  void OnInit() override;
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
  __declspec(noinline) CUIGroupSkin();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Apply() override;
  void Refresh() override;
  void OnSelect(CUIComboItem* item, const std::string& name);

  /* 0084 */ CUIComboBox<CUIComboItem, std::string>* m_combo;
};

class CUIGroupSound : public CUIGroup {
public:
  __declspec(noinline) CUIGroupSound();
  void OnCreate(int width, int height) override;
  void OnInit() override;
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
  /* 0090 */ CUIStaticText* m_bgmMuteText;
  /* 0094 */ CUIStaticText* m_effectText;
  /* 0098 */ CUIVScrollBar* m_effectSlider;
  /* 009C */ CUICheckBox* m_effectMute;
  /* 00A0 */ CUIStaticText* m_effectMuteText;
};

class CUIGroupSpriteDetailLevel : public CUIGroup {
public:
  __declspec(noinline) CUIGroupSpriteDetailLevel();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void Refresh() override;

  /* 0084 */ CUIVScrollBar* m_slider;
};

class CUIGroupTextureDetailLevel : public CUIGroup {
public:
  __declspec(noinline) CUIGroupTextureDetailLevel();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void Refresh() override;

  /* 0084 */ CUIVScrollBar* m_slider;
};

class CUIGroupTrilinear : public CUIGroup {
public:
  __declspec(noinline) CUIGroupTrilinear();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Refresh() override;

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIPage : public CUIControl {
public:
  // INLINED
  explicit CUIPage() : CUIControl() {
  }

  void OnInit() override;
  void Paint() override;
  virtual void Apply();
  virtual void Refresh();
};

class CUIBasicPage : public CUIPage {
public:
  __declspec(noinline) CUIBasicPage();
  void OnCreate(int width, int height) override;

  /* 0080 */ CUIGroupSkin* m_skin;
  /* 0084 */ CUIGroupSound* m_sound;
  /* 0088 */ CUIGroupEmblem* m_emblem;
  /* 008C */ CUIGroupRodexSpam* m_rodexSpam;
  /* 0090 */ CUIGroupProcessPriority* m_processPriority;
  /* 0094 */ CUIGroupLogInOut* m_logInOut;
};

class CUIGraphicsPage : public CUIPage {
public:
  __declspec(noinline) CUIGraphicsPage();
  void OnCreate(int width, int height) override;
  __declspec(noinline) bool IsModified();
  void ApplySettings();
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
  __declspec(noinline) CUIListPage();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  virtual void OnSelectOption(CUIOptionItem* item, const TALKTYPE& type);
  bool SetOptionChecked(int id);
  void BindOption(CUIOptionItem* item, const TALKTYPE& id);
  void LoadOptions(int tab);
  __declspec(noinline) void RefreshOptions();
  __declspec(noinline) void RefreshHotkeyLabels();
  std::string GetHotkeyText(int tab, int index);

  /* 0080 */ CUIListBoxEx<CUIOptionItem, TALKTYPE>* m_optionList;
};

class CUIComboItem : public CUIListItem {
public:
  CUIComboItem();
  void OnCreate(int width, int height) override;
  void OnInit() override;

  /* 00E8 */ CUIStaticText* m_text;
};

class CUIOptionItem : public CUIListItem {
public:
  CUIOptionItem();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void RunFunction();
  void OnCheck(bool checked);
  void OnClickLabel();

  /* 00E8 */ CUICheckBox* m_check;
  /* 00EC */ UIWindow* m_runButton;
  /* 00F0 */ CUITextButton* m_label;
  /* 00F4 */ CUIStaticText* m_description;
  /* 00F8 */ int m_optionId;
};

}

class CUIGameSettingsUI : public CUIFrameWnd {
public:
  __declspec(noinline) CUIGameSettingsUI();

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void OnLBtnDown(int x, int y) override;
  void Initialize() override;

  void OnApply();
  void OnCloseButton();
  void OnTitleClose();
  void OnTitleIconClick();
  void UpdateApplyButton();
  void OnTabChanged(UIWindow* window);
  bool IsTab4Selected();
  void SelectTab4();
  void OnBasicTabClick();
  void OnControlTabClick();
  void OnEffectTabClick();
  void OnEtcTabClick();
  void OnGraphicsTabClick();
  void RefreshOption(int id);
  __declspec(noinline) void RefreshOptions();
  void RefreshHotkeyLabels();
  void ResetToDefaults();

  /* 00D8 */ CUITitleBar* m_titleBar;
  /* 00DC */ GameSettingsUI::CUIPage* m_currentPage;
  /* 00E0 */ GameSettingsUI::CUIGraphicsPage* m_graphicsPage;
  /* 00E4 */ std::vector<GameSettingsUI::CUIListPage*> m_pages;
  /* 00F0 */ CUITab* m_tab;
  /* 00F4 */ CUIImageButton* m_pageContainer;
  /* 00F8 */ CUIImageButton* m_applyButton;
  /* 00FC */ CUIImageButton* m_closeButton;
};

#endif

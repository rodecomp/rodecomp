#ifndef RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H
#define RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H

#include "Render/DisplayMode.h"
#include "Render/RenderAdapter.h"
#include "Render/RenderBackends.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

enum TALKTYPE : int;

class CUICheckBox;
class CUIRadioButton;
class CUIStaticText;

template <class T, class D> class CUIComboBox : public CUIControl {
public:
  void Paint() override;
  int AddItem(const D& value, bool refresh);
  unsigned int Selected() const;
  __declspec(noinline) void UpdateListHeight();

  /* 0084 */ unsigned char unknown_0084[0x08];
  /* 008C */ CUIListBoxEx<T, D>* listBox;
  /* 0090 */ unsigned char unknown_0090[0x04];
  /* 0094 */ int itemHeight;
  /* 0098 */ int visibleItems;
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
  void Refresh() override;
  void LoadBackends();
  void OnSelect(CUIComboItem* item, const RenderBackend& backend);

  /* 0084 */ CUIComboBox<CUIComboItem, RenderBackend>* m_combo;
  /* 0088 */ int m_backend;
  /* 008C */ int m_pendingBackend;
};

class CUIGroupLogInOut : public CUIGroup {
public:
  void Apply() override;
  void Refresh() override;
  __declspec(noinline) void SetEnabled(bool enabled);
  void OnSelect(CUIRadioButton* button);

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIGroupRodexSpam : public CUIGroup {
public:
  void Apply() override;
  void OnSelect(CUIRadioButton* button);

  /* 0084 */ CUIRadioButton* m_onButton;
  /* 0088 */ CUIStaticText* m_onText;
  /* 008C */ CUIRadioButton* m_offButton;
  /* 0090 */ CUIStaticText* m_offText;
};

class CUIGroupSkin : public CUIGroup {
public:
  void Apply() override;
  void Refresh() override;

  /* 0084 */ CUIComboBox<CUIComboItem, std::string>* m_combo;
};

class CUIGroupSound : public CUIGroup {
public:
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
};

class CUIGroupSpriteDetailLevel : public CUIGroup {
public:
  void Paint() override;
  void Refresh() override;

  /* 0084 */ CUIVScrollBar* m_slider;
};

class CUIGroupTextureDetailLevel : public CUIGroup {
public:
  void Paint() override;

  /* 0084 */ CUIVScrollBar* m_slider;
};

class CUIGroupTrilinear : public CUIGroup {
public:
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
  bool IsModified();
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
};

class CUIComboItem : public CUIListItem {
public:
  void Slot16() override;
};

class CUIOptionItem : public CUIListItem {
public:
  void Slot16() override;
  void Paint() override;
};

std::string GetHotkeyText(int tab, int index);
}

class CUIGameSettingsUI : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;

  void sub_A3F0F0();
  bool IsTab4Selected();
  void sub_A43380();
  void sub_A43390();
  void sub_A433A0();
  void sub_A433B0();
  void sub_A433C0();

  /* 00B8 */ unsigned char unknown_00B8[0x1C];
  /* 00D8 */ CUIControl* m_titleBar;
  /* 00DC */ unsigned char unknown_00DC[0x14];
  /* 00F0 */ CUITab* m_tab;
};

#endif

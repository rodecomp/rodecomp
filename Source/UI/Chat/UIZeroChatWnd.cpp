#include "UI/Buttons/UIBalloonOpenBtn.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Guild/UIGuild_Storage_Log.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Skill/UIDebuffRemoveWnd.h"

#include "Resource/MSGSTRINGID.h"
#include <string>

// OFFSET: 0x0094B410
// FUNCTION: fn_12891
void UIBalloonOpenBtn::OnCreate(int width, int height) {
  this->m_button = new UIBitmapButton();
  this->m_button->SetBitmap("유저인터페이스\\basic_interface\\combo_btn_a.bmp", 0, 0);
  this->m_button->SetBitmap("유저인터페이스\\basic_interface\\combo_btn_b.bmp", 1, 0);
  this->m_button->SetBitmap("유저인터페이스\\basic_interface\\combo_btn_c.bmp", 2, 0);
  this->m_button->SetSize(this->m_button->BitmapWidth(), this->m_button->BitmapHeight());
  this->m_button->Move(0, 0);
  this->m_button->SetId(0x1FE);
  this->AddChild(this->m_button);
}

// OFFSET: 0x0094B500
// FUNCTION: fn_12892
void UIDebuffRemoveWnd::OnCreate(int width, int height) {
  this->m_button = new UIBasicButton();
  this->m_button->Configure(0x96, LookupMessage(0x10E6));
  this->m_button->Move(0xF, 0x16);
  this->m_button->SetId(0xB8);
  this->AddChild(this->m_button);
}

// OFFSET: 0x0094B5C0
// FUNCTION: fn_12893
void UIGuild_Storage_Log::OnCreate(int width, int height) {
  const int ids[3] = {246, 218, 184};
  const char* const bitmaps[3][3] = {
    {"stickon", "stickon_a", "stickon_b"},
    {"stickoff", "stickoff_a", "stickoff_b"},
    {"btn_close", "btn_close_a", "btn_close_b"},
  };
  const int positions[3][3] = {{160, 229}, {215, 229}, {350, 225}};

  int y = 60;
  for (int row = 0; row < 10; ++row) {
    this->m_dateTexts[row] = new UIStaticText();
    this->m_dateTexts[row]->SetSize(100, 15);
    this->m_dateTexts[row]->SetTextFitted("");
    this->m_dateTexts[row]->Move(50, y);
    this->AddChild(this->m_dateTexts[row]);

    this->m_nameTexts[row] = new UIStaticText();
    this->m_nameTexts[row]->SetSize(20, 15);
    this->m_nameTexts[row]->SetTextFitted("");
    this->m_nameTexts[row]->Move(150, y);
    this->AddChild(this->m_nameTexts[row]);

    this->m_itemTexts[row] = new UIStaticText();
    this->m_itemTexts[row]->SetSize(80, 15);
    this->m_itemTexts[row]->SetTextFitted("");
    this->m_itemTexts[row]->Move(190, y);
    this->AddChild(this->m_itemTexts[row]);

    this->m_countTexts[row] = new UIStaticText();
    this->m_countTexts[row]->SetSize(130, 15);
    this->m_countTexts[row]->SetTextFitted("");
    this->m_countTexts[row]->Move(280, y);
    this->AddChild(this->m_countTexts[row]);

    y += 15;
  }

  const std::string prefix = "유저인터페이스\\basic_interface\\";
  for (int index = 0; index < 3; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
    button->SetBitmap((prefix + bitmaps[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + bitmaps[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + bitmaps[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_tabs = new UITabControl();
  this->m_tabs->Configure(1, 20, 50, 1, 0, 0, 0, 0, 0, 0, 0, 0);
  this->m_tabs->Move(0, 17);
  this->AddChild(this->m_tabs);
  this->m_tabs->AddTab(LookupMessage(MSI_GUILD_STORAGE_LOG_WND_NAME), "");
  this->m_tabs->AddTab(LookupMessage(MSI_GUILD_STORAGE_LOG_CAUTION), "");
  this->m_tabs->AddTab(LookupMessage(MSI_GUILD_STORAGE_CLOSE_TIMEOUT), "");
}

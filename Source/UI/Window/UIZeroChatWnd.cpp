#include "UI/Window/UIBalloonOpenBtn.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Guild/UIGuild_Storage_Log.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIDebuffRemoveWnd.h"

#include <string>

// OFFSET: 0x0094B410
// FUNCTION: fn_12891
void UIBalloonOpenBtn::Slot15(int width, int height) {
  this->m_button = new UIBitmapButton(*this->m_context, *this->m_controls);
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
void UIDebuffRemoveWnd::Slot15(int width, int height) {
  this->m_button = new UIBasicButton(*this->m_context, *this->m_controls);
  this->m_button->Configure(0x96, LookupMessage(0x10E6));
  this->m_button->Move(0xF, 0x16);
  this->m_button->SetId(0xB8);
  this->AddChild(this->m_button);
}

// OFFSET: 0x0094B5C0

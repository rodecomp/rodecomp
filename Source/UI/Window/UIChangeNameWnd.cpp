#include "UI/Window/UIChangeNameWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

#include <string>

// OFFSET: 0x0092BCA0
// FUNCTION: fn_12660
void UIChangeNameWnd::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\login_interface\\change_name.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
}

// OFFSET: 0x0091EDA0
// FUNCTION: fn_12637
void UIChangeNameWnd::Slot15(int, int) {
  {
    const std::string prefix("유저인터페이스\\");
    this->m_okButton = new UIBitmapButton();
    this->m_okButton->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
    this->m_okButton->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    this->m_okButton->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    this->m_okButton->SetSize(this->m_okButton->BitmapWidth(), this->m_okButton->BitmapHeight());
    this->m_okButton->Move(this->geometry.width - 93, this->geometry.height - 24);
    this->m_okButton->SetId(184);
    this->AddChild(this->m_okButton);
  }
  {
    const std::string prefix("유저인터페이스\\");
    this->m_cancelButton = new UIBitmapButton();
    this->m_cancelButton->SetBitmap((prefix + "btn_cancel" + ".bmp").c_str(), 0, 0);
    this->m_cancelButton->SetBitmap((prefix + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
    this->m_cancelButton->SetBitmap((prefix + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
    this->m_cancelButton->SetSize(this->m_cancelButton->BitmapWidth(), this->m_cancelButton->BitmapHeight());
    this->m_cancelButton->Move(this->geometry.width - 46, this->geometry.height - 24);
    this->m_cancelButton->SetId(185);
    this->AddChild(this->m_cancelButton);
  }

  g_modeMgr.gameMode->SendMsg(10014, 0, 0, 0, 0);
  this->m_edit = new UIBitmapEditCtrl();
  this->m_edit->SetBitmapName("유저인터페이스\\login_interface\\name-edit.bmp");
  this->m_edit->SetSize(this->m_edit->geometry.width, this->m_edit->geometry.height);
  this->m_edit->Move(60, 44);
  this->m_edit->maximum = 24;
  this->m_edit->SetId(184);
  this->AddChild(this->m_edit);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_edit);
  this->m_message0 = 184;
}

// OFFSET: 0x00911190

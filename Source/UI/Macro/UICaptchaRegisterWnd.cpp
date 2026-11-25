#include "UI/Macro/UICaptchaRegisterWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Contents/Tables/MessageTable.h"
#include "File/File.h"
#include "File/FileEnumeration.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Buttons/UIStaticBitmap.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <zlib.h>

namespace {
struct CaptchaRegisterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char text[16];
  /* 0012 */ std::int16_t size;
};
static_assert(sizeof(CaptchaRegisterPacket) == 20);
}

// OFFSET: 0x007EA1C0
// FUNCTION: fn_09786
UICaptchaRegisterWnd::UICaptchaRegisterWnd()
    : UIFrameWnd(0), m_pathText(nullptr), m_fileComboBox(nullptr),
      m_captchaBitmap(nullptr), m_editCtrl(nullptr), registerButton(nullptr), m_selectedFile(), m_decompressedData(nullptr), m_decompressedSize(0) {}

// OFFSET: 0x007EA3B0
// FUNCTION: fn_09788
void UICaptchaRegisterWnd::OnCreate(int width, int height) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  std::string directory;
  directory = static_cast<const std::string&>(FormatOwnedString("%s\\captcha\\", g_applicationDirectory));

  this->m_pathText = new UIStaticText();
  this->m_pathText->SetSize(240, 16);
  this->m_pathText->Move(25, 27);
  this->m_pathText->SetTextFitted(directory.c_str());
  this->AddChild(this->m_pathText);

  std::list<std::string> files;
  EnumerateFileNames(files, directory.c_str(), "*.bmp");

  this->m_captchaBitmap = new UIStaticBitmap();
  this->m_captchaBitmap->SetSize(220, 90);
  this->m_captchaBitmap->Move(35, 81);
  this->AddChild(this->m_captchaBitmap);

  this->m_fileComboBox = new UIComboBoxDefWnd();
  this->m_fileComboBox->dropdownHeight = 80;
  this->m_fileComboBox->SetSize(240, 18);
  this->m_fileComboBox->Move(25, 53);
  this->m_fileComboBox->SetBorderColor(100, 100, 100);
  this->m_fileComboBox->SetColor(232, 232, 232);
  this->AddChild(this->m_fileComboBox);

  for (auto fileIt = files.begin(); fileIt != files.end(); ++fileIt) {
    const std::string& file = *fileIt;
    this->m_fileComboBox->AddOption(file.c_str(), 0);
  }

  this->m_editCtrl = new UIEditCtrl();
  this->m_editCtrl->SetSize(200, 16);
  this->m_editCtrl->Move(30, 183);
  this->m_editCtrl->maximum = 16;
  this->m_editCtrl->Background(255, 255, 255);
  this->m_editCtrl->flat = 0;
  this->m_editCtrl->Border(0, 0, 0);
  this->m_editCtrl->TextColor(0, 0, 0, 0xFFFFFFFF);
  this->m_editCtrl->preserveInput = 1;
  this->AddChild(this->m_editCtrl);

  this->registerButton = new UIBitmapButton();
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok_dis.bmp", 3, 0);
  this->registerButton->SetSize(this->registerButton->BitmapWidth(), this->registerButton->BitmapHeight());
  this->registerButton->Move(238, 181);
  this->registerButton->SetId(ID_SEND);
  this->AddChild(this->registerButton);
}

// OFFSET: 0x007EA8D0
// FUNCTION: fn_09789
void UICaptchaRegisterWnd::Paint() {
  this->DrawFilledRoundedOutline(0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  this->DrawTitleBar(true, "", 0);
  this->DrawRectOutline(34, 80, 222, 92, 0);
}

// OFFSET: 0x007EA910
// FUNCTION: fn_09790
int UICaptchaRegisterWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4
) {
  switch (message) {
  case UM_COMMAND:
    switch (a1) {
    case ID_SEND:
      if (this->m_editCtrl->Length() < 4) {
        g_windowMgr.Confirm(LookupMessage(MSI_MACRO_INPUT_ANSWER), 0, 1, 0, 0, "메시지", 280, 120, 0);
        break;
      }

      if (this->m_selectedFile.empty()) {
        g_windowMgr.Confirm(LookupMessage(MSI_MACRO_SELECT_CAPTCHA), 0, 1, 0, 0, "메시지", 280, 120, 0);
      } else if (this->m_decompressedSize <= 0) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("compress data size error"), 0xFF, 0, 0, 0);
      } else {
        const int result =
          g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_MACRO_CHECK_ANSWER), this->m_editCtrl->Text()).c_str(), 2, 0, 0, 0, "메시지", 280, 120, 0);
        if (result != ID_NO) {
          UIBitmapButton* button = this->registerButton;
          button->SetDisabled(1);
          button->Invalidate();

          CaptchaRegisterPacket packet;
          packet.id = ID_CZ_REQ_UPLOAD_MACRO_DETECTOR;
          memcpy_s(packet.text, sizeof(packet.text), this->m_editCtrl->Text(), sizeof(packet.text));
          packet.size = static_cast<std::int16_t>(this->m_decompressedSize);

          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);

          g_serverLanguage->ResetInput();
        }
      }
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_CAPTCHAREGISTERWND);
      break;
    }
    break;
  case UM_COMBOBOX_SELECTED:
    if (sender == this->m_fileComboBox) {
      this->m_selectedFile = this->m_fileComboBox->GetSelectedText();
      if (!this->m_selectedFile.empty()) {
        std::string fullPath = this->m_pathText->tooltip.c_str() + this->m_selectedFile;
        CFile file;
        if (!file.Open(fullPath.c_str(), 0, 1, 0)) {
          break;
        }

        CBitmapRes bitmap;
        const unsigned int size = file.GetSize();
        if (!bitmap.Decode(fullPath.c_str(), file.GetData(), size, false)) {
          break;
        }

        if (bitmap.image.width != 220 || bitmap.image.height != 90) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("image size error"), 0xFF, 0, 0, 0);
          break;
        }

        if (this->m_decompressedData != nullptr) {
          delete[] this->m_decompressedData;
          this->m_decompressedData = nullptr;
          this->m_decompressedSize = 0;
        }

        this->m_decompressedSize = compressBound(size);
        this->m_decompressedData = new unsigned char[this->m_decompressedSize];
        compress(this->m_decompressedData, &this->m_decompressedSize, static_cast<const Bytef*>(file.GetData()), size);

        this->m_captchaBitmap->SetBitmap(fullPath.c_str());
        this->m_captchaBitmap->Invalidate();
        this->Invalidate();
      }
    }
    break;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
}

// OFFSET: 0x007EAD90
// FUNCTION: fn_09791
void UICaptchaRegisterWnd::SetRegisterDisabled(unsigned char disabled) {
  UIBitmapButton* button = this->registerButton;
  button->SetDisabled(disabled);
  button->Invalidate();
  this->Invalidate();
}

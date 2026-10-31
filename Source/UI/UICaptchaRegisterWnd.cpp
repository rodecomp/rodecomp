#include "UI/UICaptchaRegisterWnd.h"

#include "Client/Application/Launcher.h"
#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Contents/MessageTable.h"
#include "File/File.h"
#include "File/FileEnumeration.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Text/UIStaticText.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <zlib.h>

namespace {
struct UIWindowMgrHelper : public UIWindowMgr {
  int sub_A901E0(const char* text, int type, bool defaultAccept, bool replace, int timeout, const char* caption, int width, int height, unsigned char protect);
};

struct UIComboBoxDefWndHelper : public UIComboBoxDefWnd {
  std::string sub_8A15A0();
};

struct UIEditCtrlHelper : public UIEditCtrl {
  void sub_865E00(int red, int green, int blue);
  void sub_868570(int red, int green, int blue, std::uint32_t secondary);
};

struct UIStaticTextHelper : public UIStaticText {
  void sub_868880(const char* text);
};

struct CaptchaRegisterPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char text[16];
  /* 0012 */ std::int16_t size;
};
static_assert(sizeof(CaptchaRegisterPacket) == 20);
}

// OFFSET: 0x007EA1C0

// OFFSET: 0x007EA3B0
// FUNCTION: fn_09788
void UICaptchaRegisterWnd::Slot15(int width, int height) {
  this->sub_8CEF10();
  this->sub_8CF0C0("", 0);

  std::string directory = FormatOwnedString("%s\\captcha\\", g_applicationDirectory);

  this->m_pathText = new UIStaticText();
  this->m_pathText->SetSize(240, 16);
  this->m_pathText->Move(25, 27);
  static_cast<UIStaticTextHelper*>(this->m_pathText)->sub_868880(directory.c_str());
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
  this->m_fileComboBox->borderRed = 100;
  this->m_fileComboBox->borderGreen = 100;
  this->m_fileComboBox->borderBlue = 100;
  this->m_fileComboBox->red = 232;
  this->m_fileComboBox->green = 232;
  this->m_fileComboBox->blue = 232;
  this->AddChild(this->m_fileComboBox);

  for (const std::string& file : files) {
    this->m_fileComboBox->AddOption(file, 0);
  }

  this->m_editCtrl = new UIEditCtrl();
  this->m_editCtrl->SetSize(200, 16);
  this->m_editCtrl->Move(30, 183);
  this->m_editCtrl->maximum = 16;
  this->m_editCtrl->Background(255, 255, 255);
  this->m_editCtrl->flat = 0;
  static_cast<UIEditCtrlHelper*>(this->m_editCtrl)->sub_865E00(0, 0, 0);
  static_cast<UIEditCtrlHelper*>(this->m_editCtrl)->sub_868570(0, 0, 0, 0xFFFFFFFF);
  this->m_editCtrl->preserveInput = 1;
  this->AddChild(this->m_editCtrl);

  this->registerButton = new UIBitmapButton();
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  this->registerButton->SetBitmap("유저인터페이스\\btn_ok_dis.bmp", 3, 0);
  this->registerButton->SetSize(this->registerButton->BitmapWidth(), this->registerButton->BitmapHeight());
  this->registerButton->Move(238, 181);
  this->registerButton->SetId(345);
  this->AddChild(this->registerButton);
}

// OFFSET: 0x007EA8D0
// FUNCTION: fn_09789
void UICaptchaRegisterWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  this->DrawTitleBar(controls.fonts, controls.locale, true, "", 0);
  DrawRectangleBorder(*this, 34, 80, 222, 92, 0);
}

// OFFSET: 0x007EA910
// FUNCTION: fn_09790
int UICaptchaRegisterWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4
) {
  if (message == 6) {
    if (a1 == 201) {
      this->m_frameContext->windows.CloseWindow(284);
    } else if (a1 == 345) {
      if (this->m_editCtrl->Length() < 4) {
        static_cast<UIWindowMgrHelper*>(&g_windowMgr)->sub_A901E0(LookupMessage(2872), 0, 1, 0, 0, "Message", 280, 120, 0);
      } else if (this->m_selectedFile.empty()) {
        static_cast<UIWindowMgrHelper*>(&g_windowMgr)->sub_A901E0(LookupMessage(2874), 0, 1, 0, 0, "Message", 280, 120, 0);
      } else if (this->m_decompressedSize == 0) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("compress data size error"), 0xFF, 0, 0, 0);
      } else {
        std::string str = FormatOwnedString(LookupMessage(2873), this->m_editCtrl->Text());
        int result = static_cast<UIWindowMgrHelper*>(&g_windowMgr)->sub_A901E0(str.c_str(), 2, 0, 0, 0, "Message", 280, 120, 0);
        if (result != 188) {
          this->registerButton->SetDisabled(1);
          this->registerButton->Invalidate();

          CaptchaRegisterPacket packet;
          packet.id = 0x0A52;
          memcpy_s(packet.text, sizeof(packet.text), this->m_editCtrl->Text(), sizeof(packet.text));
          packet.size = static_cast<std::int16_t>(this->m_decompressedSize);

          CRagConnection& connection = GetRagConnection();
          connection.Send(g_packetLengths.FixedLength(packet.id), &packet);

          reinterpret_cast<CLanguage*>(g_serverLanguage)->ResetInput();
        }
      }
    }
  } else if (message == 39 && sender == this->m_fileComboBox) {
    this->m_selectedFile = static_cast<UIComboBoxDefWndHelper*>(this->m_fileComboBox)->sub_8A15A0();
    if (!this->m_selectedFile.empty()) {
      std::string fullPath = this->m_pathText->tooltip + this->m_selectedFile;
      CFile file;
      if (file.Open(fullPath.c_str(), 0, 1, 0)) {
        CBitmapRes bitmap;
        if (bitmap.Decode(fullPath.c_str(), file.GetData(), file.GetSize(), false)) {
          if (bitmap.image.width == 220 && bitmap.image.height == 90) {
            if (this->m_decompressedData != nullptr) {
              std::free(this->m_decompressedData);
              this->m_decompressedData = nullptr;
              this->m_decompressedSize = 0;
            }

            this->m_decompressedSize = compressBound(file.GetSize());
            this->m_decompressedData = std::malloc(this->m_decompressedSize);
            compress(
              static_cast<Bytef*>(this->m_decompressedData),
              reinterpret_cast<uLongf*>(&this->m_decompressedSize),
              static_cast<const Bytef*>(file.GetData()),
              file.GetSize()
            );

            this->m_captchaBitmap->SetBitmap(fullPath.c_str());
            this->m_captchaBitmap->Invalidate();
            this->Invalidate();
          } else {
            g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>("image size error"), 0xFF, 0, 0, 0);
          }
        }
      }
    }
  }

  return this->UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
}

// OFFSET: 0x007EAD90
// FUNCTION: fn_09791
void UICaptchaRegisterWnd::SetRegisterDisabled(unsigned char disabled) {
  this->registerButton->SetDisabled(disabled);
  this->registerButton->Invalidate();
  this->Invalidate();
}

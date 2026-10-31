#include "UI/UICaptchaPreviewWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/UIMacroDetectorWnd.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Utility/Text/FormatString.h"

#include <cstdlib>
#include <cstring>
#include <new>
#include <windows.h>
#include <timeapi.h>
#include <zlib.h>

// OFFSET: 0x007ED2F0
// FUNCTION: fn_09808
void UICaptchaPreviewWnd::AddImageData(const void* data, int size) {
  memcpy_s(this->m_imageData + this->m_imageReceived, 0x400, data, size);
  this->m_imageReceived += size;

  if (this->m_imageSize == this->m_imageReceived && this->m_imageData && this->m_imageReceived) {
    uLongf length = 0x9AB0;
    Bytef* image = new (std::nothrow) Bytef[0x9AB0];
    uncompress(image, &length, this->m_imageData, this->m_imageReceived);
    this->m_image->m_bitmap.image.DecodeBitmap(image, length, false);
    if (image) {
      delete[] image;
    }
    this->Show(1);
  }
}

// OFFSET: 0x007ED400
// FUNCTION: fn_09809
void UIMacroDetectorWnd::AddImageData(const void* data, int size) {
  memcpy_s(this->m_imageData + this->m_imageReceived, 0x400, data, size);
  this->m_imageReceived += size;

  if (this->m_imageSize == this->m_imageReceived) {
    if (this->m_imageData && this->m_imageReceived) {
      uLongf length = 0x9AB0;
      Bytef* image = new (std::nothrow) Bytef[0x9AB0];
      uncompress(image, &length, this->m_imageData, this->m_imageReceived);
      this->m_image->m_bitmap.image.DecodeBitmap(image, length, false);
      if (image) {
        delete[] image;
      }
      this->m_image->Invalidate();
      this->Invalidate();
    }

    short packetType = 0xA5A;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packetType), &packetType);
  }
}

// OFFSET: 0x007ED660
// FUNCTION: fn_09810
void UICaptchaPreviewWnd::Slot15(int, int) {
  this->m_image = new UICaptchaImage();
  this->m_image->SetSize(220, 90);
  this->m_image->Move(0, 0);
  this->AddChild(this->m_image);
  this->AddCloseButton(*this->m_controls, "", 0);
}

// OFFSET: 0x007ED710
// FUNCTION: fn_09811
void UIMacroDetectorWnd::Slot15(int, int) {
  this->geometry.currentAlpha = 0xBB;
  this->geometry.targetAlpha = 0xBB;

  this->m_image = new UICaptchaImage();
  this->m_image->SetSize(220, 90);
  this->m_image->Move(60, 159);
  this->AddChild(this->m_image);

  this->m_editCtrl = new UIEditCtrl();
  this->m_editCtrl->SetSize(200, 16);
  this->m_editCtrl->Move(45, 262);
  this->m_editCtrl->maximum = 16;
  this->m_editCtrl->Background(255, 255, 255);
  this->m_editCtrl->flat = 0;
  this->m_editCtrl->Border(125, 125, 125);
  this->m_editCtrl->TextColor(0, 0xFFFFFFFF);
  this->m_editCtrl->preserveInput = 1;
  this->AddChild(this->m_editCtrl);

  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->m_editCtrl);

  UIBitmapButton* button = new UIBitmapButton();
  this->m_button = button;
  button->SetBitmap("유저인터페이스\\btn_get.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\btn_get_a.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\btn_get_b.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(253, 261);
  button->SetId(345);
  this->AddChild(button);
}

// OFFSET: 0x007ED950
// FUNCTION: fn_09812
void UICaptchaPreviewWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x007ED960
// FUNCTION: fn_09813
void UIMacroDetectorWnd::Paint() {
  this->sub_8D2CE0(false, "", 0);
  this->DrawButtonBar(0, 0, 1);
  this->FillRegion(0, 16, this->geometry.width, this->geometry.height - 36, 0xFFFFFFFF);
  DrawRectangleBorder(*this, 0, 16, this->geometry.width, this->geometry.height - 36, 0xC6C6C6);

  this->sub_A84360(25, 28, LookupMessage(2883), 0, 0, 12, 0, 1, 0);
  this->sub_A84360(25, 45, LookupMessage(2884), 0, 0, 12, 0, 1, 0);
  this->sub_A84360(25, 62, LookupMessage(2885), 0, 0, 12, 0, 1, 0);

  this->sub_A84360(25, 82, this->message.c_str(), 0, 0, 12, 0x2323FF, 1, 0);

  this->FillRegion(59, 158, 222, 92, 0xC6C6C6);
  this->FillRegion(60, 97, 220, 30, 0xC6C6C6);

  DWORD now = timeGetTime();
  std::uint32_t remaining = (this->messageDuration + this->startTick) - now;
  if (remaining != 0) {
    float width = (static_cast<float>(remaining) / static_cast<float>(this->totalDuration)) * 220.0f;
    this->FillRegion(60, 97, static_cast<int>(width), 30, 0xFF);

    unsigned int seconds = remaining / 1000;
    {
      std::string str = FormatOwnedString("%2d : %2d", seconds / 60, seconds);
      this->sub_A86440(280, 138, str.c_str(), 0, 0, 13, 0xFF2323, 1);
    }
    {
      std::string str = FormatOwnedString(LookupMessage(2886), this->unknown_00D0);
      this->sub_A84360(60, 138, str.c_str(), 0, 0, 13, 0x2323FF, 1, 0);
    }
  }
}

// OFFSET: 0x007EDC70
// FUNCTION: fn_09818
void UICaptchaPreviewWnd::ClearImageData() {
  if (this->m_imageData) {
    std::free(this->m_imageData);
    this->m_imageData = nullptr;
  }
  this->m_imageSize = 0;
  this->m_imageReceived = 0;
  std::memset(this->m_captchaKey, 0, sizeof(this->m_captchaKey));
}

// OFFSET: 0x007EDFC0
// FUNCTION: fn_09823
void UICaptchaPreviewWnd::InitImageData(unsigned short size) {
  if (this->m_imageData) {
    std::free(this->m_imageData);
    this->m_imageData = nullptr;
  }
  this->m_imageSize = size;
  this->m_imageData = static_cast<unsigned char*>(std::malloc(size));
  std::memset(this->m_imageData, 0, size);
}

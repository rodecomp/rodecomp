#include "UI/Macro/UIMacroBlackListCheckWnd.h"
#include "UI/Window/UIEnums.h"

#include "Contents/Tables/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>

#include <windows.h>
#include <timeapi.h>

namespace {
  struct MacroBlackListCheckPacket {
    /* 0000 */ std::int16_t id;
    /* 0002 */ char text[16];
  };
  static_assert(sizeof(MacroBlackListCheckPacket) == 18);
}

// OFFSET: 0x007EB0A0
// FUNCTION: fn_09792
UIMacroBlackListCheckWnd::UIMacroBlackListCheckWnd()
    : UIFrameWnd(0) {
  this->resultState = 0;
  this->inputLength = 0;
  this->duration = 0;
  this->startTick = 0;
  std::memset(this->input, 0, sizeof(this->input));
  std::memset(this->code, 0, sizeof(this->code));
  std::memset(this->digitButtons, 0, sizeof(this->digitButtons));
  this->resetButton = nullptr;
  std::memset(this->charSizes, 0, sizeof(this->charSizes) + sizeof(this->charFlags) + sizeof(this->charColors));
}

// OFFSET: 0x007EB230
// FUNCTION: fn_09794
void UIMacroBlackListCheckWnd::OnCreate(int width, int height) {
  for (int i = 0; i < 10; ++i) {
    this->digits[i] = static_cast<char>(i);
  }

  std::mt19937 engine(std::random_device{}());
  std::shuffle(this->digits, this->digits + 10, engine);

  std::string directory("");

  this->okButton = new UIBitmapButton();
  this->okButton->SetBitmap("유저인터페이스\\btn_big_ok.bmp", 0, 0);
  this->okButton->SetBitmap("유저인터페이스\\btn_big_ok_a.bmp", 1, 0);
  this->okButton->SetBitmap("유저인터페이스\\btn_big_ok_b.bmp", 2, 0);
  this->okButton->SetBitmap("유저인터페이스\\btn_big_ok_b.bmp", 3, 0);
  this->okButton->SetSize(this->okButton->BitmapWidth(), this->okButton->BitmapHeight());
  this->okButton->Move(110, 284);
  this->okButton->SetId(184);
  this->okButton->Enable(1);
  this->AddChild(this->okButton);

  directory = "유저인터페이스\\basic_interface\\";

  const int ids[11] = {293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303};
  const char* const names[11][3] = {
    {"0-a", "0-b", "0-c"}, {"1-a", "1-b", "1-c"}, {"2-a", "2-b", "2-c"}, {"3-a", "3-b", "3-c"},
    {"4-a", "4-b", "4-c"}, {"5-a", "5-b", "5-c"}, {"6-a", "6-b", "6-c"}, {"7-a", "7-b", "7-c"},
    {"8-a", "8-b", "8-c"}, {"9-a", "9-b", "9-c"}, {"reset-a", "reset-b", "reset-c"},
  };
  const int positions[11][2] = {
    {303, 75}, {363, 75}, {423, 75}, {303, 135}, {363, 135}, {423, 135}, {303, 195}, {363, 195}, {423, 195}, {303, 255}, {363, 255},
  };

  for (int i = 0; i < 10; ++i) {
    const int digit = this->digits[i];
    UIBitmapButton*& button = this->digitButtons[i];
    button = new UIBitmapButton();
    button->SetBitmap((directory + names[digit][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((directory + names[digit][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((directory + names[digit][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[digit]);
    this->AddChild(button);
  }

  this->resetButton = new UIBitmapButton();
  this->resetButton->SetBitmap((directory + names[10][0] + ".bmp").c_str(), 0, 0);
  this->resetButton->SetBitmap((directory + names[10][1] + ".bmp").c_str(), 1, 0);
  this->resetButton->SetBitmap((directory + names[10][2] + ".bmp").c_str(), 2, 0);
  this->resetButton->SetSize(this->resetButton->BitmapWidth(), this->resetButton->BitmapHeight());
  this->resetButton->Move(positions[10][0], positions[10][1]);
  this->resetButton->SetId(ids[10]);
  this->AddChild(this->resetButton);

  this->geometry.targetAlpha = 176;
}

// OFFSET: 0x007EC1E0
// FUNCTION: fn_09795
void UIMacroBlackListCheckWnd::Paint() {
  const int titleHeight = this->UIWindow::DrawTitleBar(true, LookupMessage(MSI_MENU_UAEURLWND), this->geometry.width + 1);
  this->DrawPanelBackground(titleHeight, 0x21C, "유저인터페이스\\basic_interface\\blacklist_check_bg.bmp");
  this->DrawButtonBar(2, this->geometry.width + 1, 1);

  this->DrawAlignedText(7, 0x24, FormatOwnedString(LookupMessage(MSI_WEDDING_RING_ITEM_NAME_FORMAT)).c_str(), 0x20E, 6, 0, 0, 0, 0xC, 1, 0);
  this->DrawAlignedText(7, 0x159, FormatOwnedString(LookupMessage(MSI_ACCOUNT_LIMITED_TOOL_RESULT_NEWFORMAT), this->resultState).c_str(), 0x20E, 6, 0x3939FF, 0, 0, 0xC, 1, 0);
  this->DrawAlignedText(0x4F, 0xF6, FormatOwnedString(LookupMessage(MSI_ACCOUNT_LIMITED_SALE_ITEM_NEXT_NEWDATE), this->resultState).c_str(), 0xA2, 6, 0, 0, 0, 0xC, 0, 0);

  std::string stars;
  for (int i = 0; i < this->inputLength; ++i) {
    stars += "*";
  }
  this->DrawAlignedText(0x86, 0x109, stars.c_str(), 0x32, 6, 0x4949FF, 0, 0, 0xC, 0, 0);

  int x = 0;
  for (int i = 0; i < this->codeLength; ++i) {
    int base = 0x46;
    if (this->codeLength == 4) {
      base = 0x74;
    } else if (this->codeLength == 5 || this->codeLength == 6) {
      base = 0x5D;
    }

    this->DrawAlignedText(x + base, this->charColors[i], this->code + i, 0, 0, 0, 0, 0, this->charSizes[i], this->charFlags[i], 0);
    x += 0x17;
  }


  const unsigned remainingMs = this->startTick + static_cast<unsigned>(this->duration) - timeGetTime();
  const unsigned remainingSec = remainingMs / 1000u;
  const unsigned remainingMin = remainingSec / 60u;
  if (remainingSec < 60u) {
    this->DrawAlignedText(
      0x4F, 0xA4, FormatOwnedString(LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_ANSWER_INPUT), 0, static_cast<int>(remainingMin), static_cast<int>(remainingSec)).c_str(), 0xA2, 6, 0xFF, 0, 0,
      0xD, 1, 0
    );
  } else {
    this->DrawAlignedText(0x4F, 0xA4, FormatOwnedString(LookupMessage(MSI_MACRO_BLACKLIST_USER_CHCEK_ANSWER_INPUT), 0, 0, 0).c_str(), 0xA2, 6, 0xFF, 0, 0, 0xD, 1, 0);
  }
}

// OFFSET: 0x007EC770
// FUNCTION: fn_09796
void UIMacroBlackListCheckWnd::OnLBtnDown(int, int) {
}

// OFFSET: 0x007EC780
// FUNCTION: fn_09797
void UIMacroBlackListCheckWnd::OnLBtnUp(int, int) {
}

// OFFSET: 0x007EC790
// FUNCTION: fn_09798
void UIMacroBlackListCheckWnd::OnMouseMove(int, int) {
}

// OFFSET: 0x007EC7A0
// FUNCTION: fn_09799
void UIMacroBlackListCheckWnd::OnProcess() {
  this->Invalidate();
}

// OFFSET: 0x007EC7B0
// FUNCTION: fn_09800
void UIMacroBlackListCheckWnd::ShuffleDigitButtons() {
  unsigned char order[10];
  for (int i = 0; i < 10; ++i) {
    order[i] = static_cast<unsigned char>(i);
  }

  std::mt19937 engine(std::random_device{}());
  std::shuffle(order, order + 10, engine);

  const int positions[11][2] = {
    {303, 75}, {363, 75}, {423, 75}, {303, 135}, {363, 135}, {423, 135}, {303, 195}, {363, 195}, {423, 195}, {303, 255}, {363, 255},
  };

  for (int i = 0; i < 10; ++i) {
    this->digitButtons[i]->Move(positions[order[i]][0], positions[order[i]][1]);
  }

  this->Invalidate();
}

// OFFSET: 0x007EC910
// FUNCTION: fn_09801
int UIMacroBlackListCheckWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case ID_OK:
    if (static_cast<unsigned>(this->inputLength - 4) <= 4) {
      MacroBlackListCheckPacket packet;
      packet.id = 0x0BDC;
      strncpy_s(packet.text, sizeof(packet.text), this->input, _TRUNCATE);
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
      this->inputLength = 0;
      std::memset(this->input, 0, sizeof(this->input));
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM0:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '0';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM1:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '1';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM2:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '2';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM3:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '3';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM4:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '4';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM5:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '5';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM6:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '6';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM7:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '7';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM8:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '8';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM9:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = '9';
      ++this->inputLength;
      return 0;
    }
    break;
  case ID_STORE_PASSWORD_NUM_RESET:
    this->inputLength = 0;
    std::memset(this->input, 0, sizeof(this->input));
    this->ShuffleDigitButtons();
    break;
  }

  return 0;
}

// OFFSET: 0x007ECD00
// FUNCTION: fn_09802
void UIMacroBlackListCheckWnd::SetCode(unsigned char state, const char* code) {
  strncpy_s(this->code, sizeof(this->code), code, _TRUNCATE);
  this->resultState = state;
  std::string text(code);
  this->codeLength = static_cast<int>(text.size());
  for (int i = 0; i < this->codeLength; ++i) {
    this->charSizes[i] = std::rand() % 11 + 14;
    this->charFlags[i] = std::rand() % 2;
    this->charColors[i] = std::rand() % 31 + 185;
  }
}

// OFFSET: 0x007ECE80
// FUNCTION: fn_09803
void UIMacroBlackListCheckWnd::Activate(unsigned char state, int duration) {
  const std::uint32_t now = timeGetTime();
  this->resultState = state;
  this->startTick = now;
  this->duration = duration;
  this->Invalidate();
}

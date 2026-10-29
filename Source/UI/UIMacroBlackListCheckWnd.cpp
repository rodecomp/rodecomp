#include "UI/UIMacroBlackListCheckWnd.h"

#include "UI/Buttons/UIBitmapButton.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"

#include <algorithm>
#include <cstdint>
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

// OFFSET: 0x007EB230
// FUNCTION: fn_09794
void UIMacroBlackListCheckWnd::Slot15(int width, int height) {
  for (int i = 0; i < 10; ++i) {
    this->digits[i] = static_cast<char>(i);
  }

  std::mt19937 engine(std::random_device{}());
  std::shuffle(this->digits, this->digits + 10, engine);

  UIButtonContext& controls = *this->m_controls;
  std::string directory;

  this->okButton = new UIBitmapButton(*this->m_context, controls);
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
    button = new UIBitmapButton(*this->m_context, controls);
    button->SetBitmap((directory + names[digit][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((directory + names[digit][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((directory + names[digit][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[digit]);
    this->AddChild(button);
  }

  this->resetButton = new UIBitmapButton(*this->m_context, controls);
  this->resetButton->SetBitmap((directory + names[10][0] + ".bmp").c_str(), 0, 0);
  this->resetButton->SetBitmap((directory + names[10][1] + ".bmp").c_str(), 1, 0);
  this->resetButton->SetBitmap((directory + names[10][2] + ".bmp").c_str(), 2, 0);
  this->resetButton->SetSize(this->resetButton->BitmapWidth(), this->resetButton->BitmapHeight());
  this->resetButton->Move(positions[10][0], positions[10][1]);
  this->resetButton->SetId(ids[10]);
  this->AddChild(this->resetButton);

  this->geometry.targetAlpha = 176;
}

// OFFSET: 0x007EC770
// FUNCTION: fn_09796
void UIMacroBlackListCheckWnd::Slot25(int, int) {
}

// OFFSET: 0x007EC780
// FUNCTION: fn_09797
void UIMacroBlackListCheckWnd::Slot31(int, int) {
}

// OFFSET: 0x007EC790
// FUNCTION: fn_09798
void UIMacroBlackListCheckWnd::Slot27(int, int) {
}

// OFFSET: 0x007EC7A0
// FUNCTION: fn_09799
void UIMacroBlackListCheckWnd::Slot19() {
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
  case 184:
    if (static_cast<unsigned>(this->inputLength - 4) <= 4) {
      MacroBlackListCheckPacket packet;
      packet.id = 0x0BDC;
      strncpy_s(packet.text, sizeof(packet.text), this->input, _TRUNCATE);
      CRagConnection& connection = GetRagConnection();
      connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
      this->inputLength = 0;
      std::memset(this->input, 0, sizeof(this->input));
      return 0;
    }
    break;
  case 293:
  case 294:
  case 295:
  case 296:
  case 297:
  case 298:
  case 299:
  case 300:
  case 301:
  case 302:
    this->ShuffleDigitButtons();
    if (this->inputLength < 8) {
      this->input[this->inputLength] = static_cast<char>('0' + (argument1 - 293));
      ++this->inputLength;
      return 0;
    }
    break;
  case 303:
    this->inputLength = 0;
    std::memset(this->input, 0, sizeof(this->input));
    this->ShuffleDigitButtons();
    break;
  }

  return 0;
}

// OFFSET: 0x007ECE80
// FUNCTION: fn_09803
void UIMacroBlackListCheckWnd::Activate(unsigned char state, int duration) {
  this->startTick = timeGetTime();
  this->duration = duration;
  this->unknown_00E4 = state;
  this->Invalidate();
}

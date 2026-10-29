#include "UI/Guild/UIGuildWnd.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIButton.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00957D30
// FUNCTION: fn_13044
int UIGuildBanishedMemberWnd::Tab() {
  return 4;
}

// OFFSET: 0x00957D40
// FUNCTION: fn_13045
int UIGuildInfoManageWnd::Tab() {
  return 0;
}

// OFFSET: 0x00957D50
// FUNCTION: fn_13046
int UIGuildMemberManageWnd::Tab() {
  return 1;
}

// OFFSET: 0x00957D80
// FUNCTION: fn_13049
int UIGuildSkillWnd::Tab() {
  return 3;
}

// OFFSET: 0x00957D90
// FUNCTION: fn_13050
int UIGuildTotalInfoWnd::Tab() {
  return 6;
}

// OFFSET: 0x0095F860

// OFFSET: 0x00960020

// OFFSET: 0x0095FE10

// OFFSET: 0x009602E0

// OFFSET: 0x00960330

// OFFSET: 0x009603C0

// OFFSET: 0x00960620
// FUNCTION: fn_13108
void UIGuildMemberManageWnd::Slot33(int, int) {
  this->SendMsg(nullptr, 49, 0, 0, 0, 0);
}

// OFFSET: 0x00960640
// FUNCTION: fn_13109
void UIGuildSkillWnd::Slot33(int, int) {
  this->SendMsg(nullptr, 49, 0, 0, 0, 0);
}

// OFFSET: 0x00960960

// OFFSET: 0x009609B0

// OFFSET: 0x00960A00

// OFFSET: 0x00960A50

// OFFSET: 0x00960AA0

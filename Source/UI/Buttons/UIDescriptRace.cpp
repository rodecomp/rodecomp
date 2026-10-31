#include "UI/Buttons/UIDescriptRace.h"

#include <windows.h>
#include <mmsystem.h>

// GLOBAL: gl_24988
unsigned char g_doramCreationEnabled;

// OFFSET: 0x0086D060
// FUNCTION: fn_11041
UIDescriptRace::UIDescriptRace(int race) : m_race(race), unknown_0284(0) {
  this->m_createTime = timeGetTime();
  this->m_noRace = race == 0;
}

// OFFSET: 0x008861C0
// FUNCTION: fn_11323
void UIDescriptRace::Slot15(int, int) {
  this->SendMsg(nullptr, 13, this->m_noRace, 0, 0, 0);
}

// OFFSET: 0x008985D0
// FUNCTION: fn_11548
int UIDescriptRace::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 13 && (this->m_race != 1 || g_doramCreationEnabled)) {
    if (argument1) {
      this->m_noRace = 1;
      this->SetBitmap(this->m_race == 1 ? "유저인터페이스\\make_character_ver2\\img_doram_on.bmp" : "유저인터페이스\\make_character_ver2\\img_human_on.bmp", 0, 0);
      return 0;
    }

    this->m_noRace = 0;
    if (this->m_race == 1) {
      this->SetBitmap("유저인터페이스\\make_character_ver2\\img_doram_off.bmp", 0, 0);
      return 0;
    }

    this->SetBitmap("유저인터페이스\\make_character_ver2\\img_human_off.bmp", 0, 0);
  }

  return 0;
}

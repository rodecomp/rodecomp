#include "UI/Buttons/UIDescriptRace.h"

#include <windows.h>
#include <mmsystem.h>

// OFFSET: 0x0086D060
// FUNCTION: fn_11041
UIDescriptRace::UIDescriptRace(int race) : m_race(race), unknown_0284(0) {
  this->m_createTime = timeGetTime();
  this->m_noRace = race == 0;
}

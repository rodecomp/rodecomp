#include "UI/Window/UIRoulletteWnd.h"
#include "Contents/RouletteMgr.h"

// OFFSET: 0x0080BAF0
// FUNCTION: fn_10084
UIRoulletteWnd::~UIRoulletteWnd() = default;

// OFFSET: 0x0080C490
// FUNCTION: fn_10088
void UIRoulletteWnd::Slot25(int, int) {
}

// OFFSET: 0x0080CE40
// FUNCTION: fn_10094
int UIRoulletteWnd::CoinType() const {
  const RouletteMgr* roulette = g_rouletteMgr;

  if (roulette->m_resultShown == 1) {
    if (roulette->m_stage + 1 < 7 && roulette->m_bonus == 0) {
      return 1;
    }
  } else if (roulette->m_bronzePoint <= 0) {
    if (roulette->m_silverPoint >= 10) {
      return 2;
    }
    if (roulette->m_goldPoint >= 10) {
      return 4;
    }
  }

  return 0;
}

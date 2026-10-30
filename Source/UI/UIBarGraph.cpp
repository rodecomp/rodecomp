#include "UI/UIBarGraph.h"

// OFFSET: 0x0086C310
// FUNCTION: fn_11024
UIBarGraphHomun::UIBarGraphHomun(UIWindowContext& context) : UIWindow(context, 0) {
}

// OFFSET: 0x0086C390
// FUNCTION: fn_11025
UIBarGraphPlayer::UIBarGraphPlayer(UIWindowContext& context) : UIWindow(context, 0) {
}

// OFFSET: 0x0086FFA0
// FUNCTION: fn_11097
UIBarGraph3::~UIBarGraph3() = default;

// OFFSET: 0x008700F0
// FUNCTION: fn_11098
UIBarGraphPlayer::~UIBarGraphPlayer() = default;

// OFFSET: 0x0089CDB0
// FUNCTION: fn_11593
void UIBarGraphHomun::SetValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}

// OFFSET: 0x0089CE60
// FUNCTION: fn_11594
void UIBarGraphPlayer::SetValues(std::function<std::int64_t()> current, std::function<std::int64_t()> maximum) {
  this->m_current = current;
  this->m_maximum = maximum;
}

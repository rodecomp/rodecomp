#include "UI/Text/UICharInfoBalloonText.h"

// OFFSET: 0x0092BCE0
// FUNCTION: fn_12661
void UICharInfoBalloonText::Paint() {
  this->ClearSurface(0xFFFF00FF);

  int y = 6;
  for (unsigned int i = 0; i < this->m_lines.size(); ++i) {
    const bool rightColumn = (i & 1) != 0;
    this->DrawShadowedText(rightColumn ? 50 : 6, y, this->m_lines[i].c_str(), 0, this->m_textColor, this->m_outlineColor, 0, this->m_fontHeight, 0);
    if (rightColumn) {
      y += this->TextHeight(this->m_textContext->fonts, this->m_textContext->locale, this->m_lines[i].c_str(), 0, 0, this->m_fontHeight, 0, 0) + 4;
    }
  }
}

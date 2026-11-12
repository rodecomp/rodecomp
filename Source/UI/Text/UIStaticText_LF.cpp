#include "UI/Text/UIStaticText_LF.h"

#include <cstdint>
#include <string>

template __declspec(noinline) std::string::basic_string(const std::string&);

// OFFSET: 0x0094ECF0
// FUNCTION: fn_12915
void UIStaticText_LF::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawWrappedFormattedText(
    0, 0,
    *reinterpret_cast<const std::string*>(reinterpret_cast<const char*>(this) + 0x7C),
    this->geometry.width,
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0x94),
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0x98),
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0x9C),
    *reinterpret_cast<const int*>(reinterpret_cast<const char*>(this) + 0xA0),
    *reinterpret_cast<const std::uint32_t*>(reinterpret_cast<const char*>(this) + 0xA4),
    *reinterpret_cast<const unsigned char*>(reinterpret_cast<const char*>(this) + 0xA8)
  );
}

// OFFSET: 0x0094EEA0
// FUNCTION: fn_12920
void UIStaticText_LF::OnLBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDown(x, y);
  }
}

// OFFSET: 0x0094EFA0
// FUNCTION: fn_12925
void UIStaticText_LF::OnLBtnUp(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnUp(x, y);
  }
}

// OFFSET: 0x0094F300
// FUNCTION: fn_12936
void UIStaticText_LF::OnWheel(int delta) {
  if (this->parent) {
    this->parent->OnWheel(delta);
  }
}

// OFFSET: 0x00951BF0
// FUNCTION: fn_12966
void UIStaticText_LF::SetText(std::string text) {
  this->m_text = text;
  std::list<std::string> lines;
  this->WrapFormattedText(text, &lines, this->geometry.width, this->m_fontFamily, this->m_fontHeight, this->m_bold);
  const int count = static_cast<int>(lines.size());
  const int height = (count - 1) * this->m_lineSpacing + this->m_fontHeight * count;
  this->Resize(this->geometry.width, height);
  this->Invalidate();
}

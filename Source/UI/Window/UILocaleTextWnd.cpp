#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

namespace AdventureGuide {

// OFFSET: 0x009D1830
// FUNCTION: fn_14008
void CUISubTab::Slot15(int width, int height) {
  this->unknown_00E8 = new CUIStaticText(*this->m_context);
  this->unknown_00E8->SetSize(98, 26);
  this->unknown_00E8->Move(0, 0);
  this->unknown_00E8->SetText("", 0);
  this->unknown_00E8->SetFitText(0);
  this->unknown_00E8->SetAlign(6, 6);
  {
    CUIStaticText* text = this->unknown_00E8;
    const std::uint8_t fontSource[] = {0x16, 0x82, 0x37, 0x1E, 0xF0, 0xA1};
    thread_local EncryptedName<sizeof(fontSource), 0xB127A19D775BF751ULL> fontName(fontSource);
    text->SetFont(fontName.Text());
  }
  this->unknown_00E8->SetFontHeight(12);
  this->unknown_00E8->SetBold(1);
  this->unknown_00E8->SetColor(0x5F3C2C);
  this->unknown_00E8->SetShadow(0, 0xFFFFFF);
  this->unknown_00E8->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00E8);

  this->unknown_00EC = new CUIStaticImage(*this->m_context);
  this->unknown_00EC->SetSize(40, 26);
  this->unknown_00EC->Move(58, 0);
  {
    CUIStaticImage* stamp = this->unknown_00EC;
    const std::uint8_t stampSource[] = {0x9D, 0x44, 0xB3, 0xB3, 0x5B, 0x87, 0xA4, 0xC2, 0x9B, 0x0F, 0xB3, 0x85, 0x26, 0xF3, 0x3D, 0x4E, 0x39, 0x9D,
                                        0x16, 0x27, 0xEF, 0x3C, 0x13, 0x6A, 0x1A, 0x9E, 0x1A, 0x2D, 0xFE, 0x15, 0x12, 0x7B, 0x3C, 0x86, 0x03, 0x16,
                                        0xF8, 0x26, 0x0C, 0x7F, 0x31, 0x8E, 0x07, 0x2C, 0xFF, 0x16, 0x12, 0x7A, 0x3F, 0xC5, 0x11, 0x24, 0xEB, 0x49};
    thread_local EncryptedName<sizeof(stampSource), 0x0F61499B4973EB5DULL> stampName(stampSource);
    stamp->SetImage(stampName.Text());
  }
  this->AddChild(this->unknown_00EC);
}

// OFFSET: 0x009D1C80

// OFFSET: 0x009D1F00

}

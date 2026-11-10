#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  template <typename Byte>
  explicit EncryptedName(const Byte (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  // INLINED
  template <typename Byte>
  explicit EncryptedName(const std::array<Byte, Length>& source) : m_encrypted(true) {
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
void CUISubTab::OnCreate(int width, int height) {
  this->labelText = new CUIStaticText();
  this->labelText->SetSize(98, 26);
  this->labelText->Move(0, 0);
  this->labelText->SetText("");
  this->labelText->SetFitText(0);
  this->labelText->SetAlign(6, 6);
  {
    CUIStaticText* text = this->labelText;
    const std::uint8_t fontSource[] = {0x16, 0x82, 0x37, 0x1E, 0xF0, 0xA1};
    thread_local EncryptedName<sizeof(fontSource), 0xB127A19D775BF751ULL> fontName(fontSource);
    text->SetFont(fontName.Text());
  }
  this->labelText->SetFontHeight(12);
  this->labelText->SetBold(1);
  this->labelText->SetColor(0x5F3C2C);
  this->labelText->SetShadow(0, 0xFFFFFF);
  this->labelText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labelText);

  this->stampImage = new CUIStaticImage();
  this->stampImage->SetSize(40, 26);
  this->stampImage->Move(58, 0);
  {
    CUIStaticImage* stamp = this->stampImage;
    const std::array<char, 54> stampSource = {'\x9D', '\x44', '\xB3', '\xB3', '\x5B', '\x87', '\xA4', '\xC2', '\x9B', '\x0F', '\xB3', '\x85', '\x26', '\xF3', '\x3D', '\x4E', '\x39', '\x9D',
                                        '\x16', '\x27', '\xEF', '\x3C', '\x13', '\x6A', '\x1A', '\x9E', '\x1A', '\x2D', '\xFE', '\x15', '\x12', '\x7B', '\x3C', '\x86', '\x03', '\x16',
                                        '\xF8', '\x26', '\x0C', '\x7F', '\x31', '\x8E', '\x07', '\x2C', '\xFF', '\x16', '\x12', '\x7A', '\x3F', '\xC5', '\x11', '\x24', '\xEB', '\x49'};
    thread_local EncryptedName<sizeof(stampSource), 0x0F61499B4973EB5DULL> stampName(stampSource);
    stamp->SetImage(stampName.Text());
  }
  this->AddChild(this->stampImage);
}

// OFFSET: 0x009D1C80
// FUNCTION: fn_14009
void CUITab::OnCreate(int width, int height) {
  this->labelText = new CUIStaticText();
  this->labelText->SetSize(98, 40);
  this->labelText->Move(0, 0);
  this->labelText->SetText("");
  this->labelText->SetFitText(0);
  this->labelText->SetAlign(6, 6);
  {
    CUIStaticText* text = this->labelText;
    const std::uint8_t fontSource[] = {0xBA, 0xC0, 0x5F, 0x86, 0x3C, 0x37};
    thread_local EncryptedName<sizeof(fontSource), 0x91493751EF33B5FDULL> fontName(fontSource);
    text->SetFont(fontName.Text());
  }
  this->labelText->SetFontHeight(12);
  this->labelText->SetBold(1);
  this->labelText->SetColor(0x5F3C2C);
  this->labelText->SetShadow(0, 0xFFFFFF);
  this->labelText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labelText);
}

// OFFSET: 0x009D1F00
// FUNCTION: fn_14010
void CUITabInfo::OnCreate(int width, int height) {
  this->titleText = new CUIStaticText();
  this->titleText->SetSize(90, 44);
  this->titleText->Move(32, 0);
  this->titleText->SetText("");
  this->titleText->SetFitText(0);
  this->titleText->SetAlign(0, 6);
  {
    CUIStaticText* text = this->titleText;
    const std::uint8_t fontSource[] = {0x28, 0x08, 0x97, 0x4A, 0x1C, 0x07};
    thread_local EncryptedName<sizeof(fontSource), 0xCD8F077123FB7D6FULL> fontName(fontSource);
    text->SetFont(fontName.Text());
  }
  this->titleText->SetFontHeight(12);
  this->titleText->SetBold(1);
  this->titleText->SetColor(0x815A48);
  this->titleText->SetShadow(0, 0xFFFFFF);
  this->titleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->titleText);

  this->descriptionText = new CUIStaticText();
  this->descriptionText->SetSize(442, 44);
  this->descriptionText->Move(140, 0);
  this->descriptionText->SetText("");
  this->descriptionText->SetFitText(1);
  this->descriptionText->SetAlign(0, 6);
  {
    CUIStaticText* text = this->descriptionText;
    const std::uint8_t fontSource[] = {0x04, 0x5C, 0x3F, 0x4E, 0xB8, 0xBF};
    thread_local EncryptedName<sizeof(fontSource), 0x9949BFD527532943ULL> fontName(fontSource);
    text->SetFont(fontName.Text());
  }
  this->descriptionText->SetFontHeight(12);
  this->descriptionText->SetBold(0);
  this->descriptionText->SetColor(0x2A435D);
  this->descriptionText->SetShadow(0, 0xFFFFFF);
  this->descriptionText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->descriptionText);
}

}

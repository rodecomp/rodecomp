#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

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
    constexpr std::array<std::uint8_t, 6> fontSourceArray = CRYPT_NAME("Gulim", 0xB127A19D775BF751ULL);
    const std::uint8_t (&fontSource)[6] = fontSourceArray._Elems;
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
    const std::array<char, 54> stampSource = CRYPT_CHARS(54, "유저인터페이스\\AdventureGuide\\stamp_completed_sub.bmp", 0x0F61499B4973EB5DULL);
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
    constexpr std::array<std::uint8_t, 6> fontSourceArray = CRYPT_NAME("Gulim", 0x91493751EF33B5FDULL);
    const std::uint8_t (&fontSource)[6] = fontSourceArray._Elems;
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
  this->titleText->SetFont([] {
    constexpr std::array<std::uint8_t, 6> fontSourceArray = CRYPT_NAME("Gulim", 0xCD8F077123FB7D6FULL);
    const std::uint8_t (&fontSource)[6] = fontSourceArray._Elems;
    thread_local EncryptedName<sizeof(fontSource), 0xCD8F077123FB7D6FULL> fontName(fontSource);
    return fontName.Text();
  }());
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
  this->descriptionText->SetFont([] {
    const std::uint8_t fontSource[] = CRYPT_BYTES(6, "Gulim", 0x9949BFD527532943ULL);
    thread_local EncryptedName<sizeof(fontSource), 0x9949BFD527532943ULL> fontName(fontSource);
    return fontName.Text();
  }());
  this->descriptionText->SetFontHeight(12);
  this->descriptionText->SetBold(0);
  this->descriptionText->SetColor(0x2A435D);
  this->descriptionText->SetShadow(0, 0xFFFFFF);
  this->descriptionText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->descriptionText);
}

}

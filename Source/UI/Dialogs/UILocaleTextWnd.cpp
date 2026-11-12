#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

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
  this->labelText->SetFont(OBFUSCATE_TEXT_NAMED("Gulim", 0xB127A19D775BF751ULL, fontName));
  this->labelText->SetFontHeight(12);
  this->labelText->SetBold(1);
  this->labelText->SetColor(0x5F3C2C);
  this->labelText->SetShadow(0, 0xFFFFFF);
  this->labelText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labelText);

  this->stampImage = new CUIStaticImage();
  this->stampImage->SetSize(40, 26);
  this->stampImage->Move(58, 0);
  this->stampImage->SetImage(OBFUSCATE_TEXT_NAMED("유저인터페이스\\AdventureGuide\\stamp_completed_sub.bmp", 0x0F61499B4973EB5DULL, stampName));
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
    text->SetFont(OBFUSCATE_BYTES_TEXT("Gulim", 0x91493751EF33B5FDULL));
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
  this->titleText->SetFont(OBFUSCATE_TEXT_NAMED("Gulim", 0xCD8F077123FB7D6FULL, fontName));
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
  this->descriptionText->SetFont(OBFUSCATE_TEXT_NAMED("Gulim", 0x9949BFD527532943ULL, fontName));
  this->descriptionText->SetFontHeight(12);
  this->descriptionText->SetBold(0);
  this->descriptionText->SetColor(0x2A435D);
  this->descriptionText->SetShadow(0, 0xFFFFFF);
  this->descriptionText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->descriptionText);
}

}

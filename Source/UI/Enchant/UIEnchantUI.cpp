#include "UI/Enchant/EnchantUI.h"
#include "Client/Session.h"
#include "Contents/EnchantMgr.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Utility/Cipher/XorBytes.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>
#include <iterator>
#include <new>

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
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

// GLOBAL: gl_24882
template <>
CEnchantMgr* Singleton<CEnchantMgr>::s_instance = nullptr;

// OFFSET: 0x009F9B40
// OFFSET: 0x009F9C10
// FUNCTION: fn_14814
// FUNCTION: fn_14815
template <class T, class D> void CUIListBoxEx<T, D>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Slot15(int, int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Slot15(int, int);

template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Paint();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Paint();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::Slot35(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::Slot35(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::OnScroll(int);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearItems();
template void CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::ClearItems();
template int CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::AddData(const EnchantUI::CEnchantItemData&, bool);
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::ClearData();
template void CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::UpdateScrollBar();

// OFFSET: 0x006863F0
// FUNCTION: fn_05565
template <>
CEnchantMgr* Singleton<CEnchantMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CEnchantMgr;
  }

  return s_instance;
}

// OFFSET: 0x00A04860
// FUNCTION: fn_14958
CUIEnchantUI::~CUIEnchantUI() {
  CEnchantMgr::GetInstance()->SendCloseEnchant();
}

namespace EnchantUI {
// OFFSET: 0x009F70C0
// FUNCTION: fn_14768
CUIEnchantPage::CUIEnchantPage(UIWindowContext& context)
    : CUIControl(context), unknown_0080{}, gameItem(nullptr), unknown_0088(), unknown_0090(), enchantList(nullptr), unknown_009C{}, enchantInfo(nullptr),
      group(0), unknown_00B0(-1) {
}

// OFFSET: 0x009F72F0
// FUNCTION: fn_14770
CUIMainPage::CUIMainPage(UIWindowContext& context) : CUIControl(context), firstPage(nullptr), secondPage(nullptr), tab(nullptr) {
}
}

// OFFSET: 0x009F7380
// FUNCTION: fn_14771
CUITab::CUITab(UIWindowContext& context) : CUIControl(context), current(nullptr) {
}

// OFFSET: 0x009F7420
// FUNCTION: fn_14772
CUITabButton::CUITabButton(UIWindowContext& context)
    : CUIImageTextButton(context), page(nullptr), useSkinColors(0), normalColor(0xFFFFFF), selectedColor(0xFFFFFF), borderColor(0xCECECE), normalCell{0, 0},
      selectedCell{0, 0}, borderCell{3, 1} {
}

// OFFSET: 0x009F74E0

namespace EnchantUI {
// OFFSET: 0x009F9CE0
// FUNCTION: fn_14816
void CUIEnchantItem::Slot15(int width, int height) {
  this->itemIcon = new CUIGameItem(*this->m_context);
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(2, 3);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->AddChild(this->itemIcon);

  this->name = new CUIStaticText(*this->m_context);
  this->name->SetSize(101, 12);
  this->name->Move(28, 9);
  this->name->SetText("", 0);
  this->name->SetFitText(1);
  this->name->SetAlign(0, 6);
  {
    const std::uint8_t fontSource[] = {0xC8, 0xEC, 0x87, 0x64, 0xD8, 0x7F};
    // GLOBAL: gl_25222
    thread_local EncryptedName<sizeof(fontSource), 0x39FF7FB50DEB998FULL> fontName(fontSource);
    const std::string font(fontName.Text());
    this->name->SetFont(font);
  }
  this->name->SetFontHeight(12);
  this->name->SetBold(1);
  this->name->SetColor(0xC89682);
  this->name->SetShadow(0, 0xFFFFFF);
  this->name->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->name);
}

// OFFSET: 0x00A00880
// FUNCTION: fn_14828
void CUITargetItem::Slot15(int width, int height) {
  this->itemIcon = new CUIGameItem(*this->m_context);
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(6, 13);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->AddChild(this->itemIcon);

  this->unknown_00EC = new CUIStaticImage(*this->m_context);
  this->unknown_00EC->SetSize(12, 12);
  this->unknown_00EC->Move(166, 7);
  {
    const std::uint8_t gradeIconSource[] = {
      0x4F, 0x12, 0x2F, 0x7D, 0xBD, 0x0B, 0x68, 0xB8, 0x49, 0x59, 0x2F, 0x4B, 0xC0, 0x7F, 0xF1, 0x12,
      0xFD, 0xDC, 0x8B, 0xE2, 0x22, 0xA0, 0xC3, 0x16, 0xE7, 0xDC, 0x81, 0xF3, 0x21, 0xA2, 0xDF, 0x14,
      0xEB, 0xD8, 0xB0, 0xEE, 0x1E, 0xAA, 0xC3, 0x44, 0xA1, 0xDF, 0x82, 0xF7, 0x7D
    };
    // GLOBAL: gl_25223
    thread_local EncryptedName<sizeof(gradeIconSource), 0x75ADC57D87EFBD8FULL> gradeIconName(gradeIconSource);
    const std::string gradeIcon(gradeIconName.Text());
    this->unknown_00EC->SetImage(gradeIcon);
  }
  this->AddChild(this->unknown_00EC);

  this->unknown_00F0 = new CUIStaticText(*this->m_context);
  this->unknown_00F0->SetSize(152, 12);
  this->unknown_00F0->Move(38, 23);
  this->unknown_00F0->SetText("", 0);
  this->unknown_00F0->SetFitText(1);
  this->unknown_00F0->SetAlign(0, 0);
  {
    const std::uint8_t fontSource[] = {0x2A, 0xCE, 0xF9, 0x44, 0xFE, 0xFB};
    // GLOBAL: gl_25224
    thread_local EncryptedName<sizeof(fontSource), 0xC517FB932D95BB6DULL> fontName(fontSource);
    const std::string font(fontName.Text());
    this->unknown_00F0->SetFont(font);
  }
  this->unknown_00F0->SetFontHeight(12);
  this->unknown_00F0->SetBold(1);
  this->unknown_00F0->SetColor(0x959595);
  this->unknown_00F0->SetShadow(0, 0xFFFFFF);
  this->unknown_00F0->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00F0);
}

// OFFSET: 0x009F9880
// FUNCTION: fn_14809
void CUIEnchantPage::Slot58(int) {
}

// OFFSET: 0x009F99B0
// FUNCTION: fn_14812
CUIIngredient* CUIEnchantPage::CreateIngredient() {
  CUIIngredient* ingredient = new CUIIngredient(*this->m_context);
  ingredient->SetSize(70, 50);
  ingredient->Move(4, 200);
  this->AddChild(ingredient);
  return ingredient;
}

// OFFSET: 0x009FB800
// FUNCTION: fn_14818
void CUIIngredient::Slot15(int width, int height) {
  this->unknown_0080 = new CUIStaticImage(*this->m_context);
  this->unknown_0080->SetSize(70, 50);
  this->unknown_0080->Move(0, 0);
  {
    const std::uint8_t offImageSource[] = {
      0x5B, 0xF8, 0xC3, 0x63, 0xA7, 0xCB, 0xC0, 0xC8, 0x5D, 0xB3, 0xC3, 0x55, 0xDA, 0xBF, 0x59, 0x40,
      0xF5, 0x34, 0x6B, 0xF8, 0x09, 0x71, 0x50, 0x4C, 0xC7, 0x35, 0x64, 0xC6, 0x02, 0x6B, 0x66, 0x5A,
      0xF4, 0x31, 0x65, 0xB7, 0x05, 0x68, 0x75, 0x05
    };
    // GLOBAL: gl_25225
    thread_local EncryptedName<sizeof(offImageSource), 0x050505679903579BULL> offImageName(offImageSource);
    const std::string offImage(offImageName.Text());
    this->unknown_0080->SetImage(offImage);
  }
  this->AddChild(this->unknown_0080);

  this->unknown_0084 = new CUIStaticImage(*this->m_context);
  this->unknown_0084->SetSize(70, 50);
  this->unknown_0084->Move(0, 0);
  {
    const std::uint8_t onImageSource[] = {
      0x21, 0xD4, 0x39, 0x2D, 0x15, 0xDD, 0xFC, 0x10, 0x27, 0x9F, 0x39, 0x1B, 0x68, 0xA9, 0x65, 0x98,
      0x8F, 0x18, 0x91, 0xB6, 0xBB, 0x67, 0x6C, 0x94, 0xBD, 0x19, 0x9E, 0x88, 0xB0, 0x7D, 0x5A, 0x82,
      0x8E, 0x15, 0xD7, 0xB5, 0xB8, 0x63, 0x39
    };
    // GLOBAL: gl_25226
    thread_local EncryptedName<sizeof(onImageSource), 0xDD3913D5D7F97BE1ULL> onImageName(onImageSource);
    const std::string onImage(onImageName.Text());
    this->unknown_0084->SetImage(onImage);
  }
  this->AddChild(this->unknown_0084);

  this->itemIcon = new CUIGameItem(*this->m_context);
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(6, 10);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->AddChild(this->itemIcon);

  this->ownedText = new CUIStaticText(*this->m_context);
  this->ownedText->SetSize(40, 14);
  this->ownedText->Move(30, 17);
  this->ownedText->SetText("", 0);
  this->ownedText->SetFitText(0);
  this->ownedText->SetAlign(6, 6);
  {
    const std::uint8_t fontSource[] = {0xE0, 0x80, 0x13, 0x50, 0x5C, 0x4B};
    // GLOBAL: gl_25227
    thread_local EncryptedName<sizeof(fontSource), 0xBF2B4B31397FF5A7ULL> fontName(fontSource);
    const std::string font(fontName.Text());
    this->ownedText->SetFont(font);
  }
  this->ownedText->SetFontHeight(12);
  this->ownedText->SetBold(1);
  this->ownedText->SetColor(0xF0F0F0);
  this->ownedText->SetShadow(0, 0xFFFFFF);
  this->ownedText->SetOutline(1, 0x965532);
  this->AddChild(this->ownedText);

  this->requiredText = new CUIStaticText(*this->m_context);
  this->requiredText->SetSize(40, 14);
  this->requiredText->Move(30, 31);
  this->requiredText->SetText("", 0);
  this->requiredText->SetFitText(0);
  this->requiredText->SetAlign(6, 6);
  {
    const std::uint8_t fontSource[] = {0xFA, 0xC8, 0x81, 0x5E, 0xD6, 0xBF};
    // GLOBAL: gl_25228
    thread_local EncryptedName<sizeof(fontSource), 0x83EFBFBB37EDBDBDULL> fontName(fontSource);
    const std::string font(fontName.Text());
    this->requiredText->SetFont(font);
  }
  this->requiredText->SetFontHeight(12);
  this->requiredText->SetBold(1);
  this->requiredText->SetColor(0xF0F0F0);
  this->requiredText->SetShadow(0, 0xFFFFFF);
  this->requiredText->SetOutline(1, 0x965532);
  this->AddChild(this->requiredText);
}

// OFFSET: 0x009F9B30
// FUNCTION: fn_14813
void CUIEnchantPage::Slot54(CUIEnchantItem* item, const CEnchantItemData& data) {
}
}

// OFFSET: 0x009F9710
// FUNCTION: fn_14808
CUIEnchantUI* CreateEnchantUI() {
  return new CUIEnchantUI();
}

namespace EnchantUI {
// OFFSET: 0x009FD340
// FUNCTION: fn_14823
void CUISlot0::Slot15(int width, int height) {
  CUISlot::Slot15(width, height);
  this->radioButton->Move(59, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  this->radioButton->disabled = 0;
  this->radioButton->Invalidate();
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  this->radioButton->checked = false;
  this->radioButton->Invalidate();
  this->itemIcon->Move(69, 11);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->description->Move(0, 44);
  this->description->SetTextOffset(2, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

// OFFSET: 0x009FDC30
// FUNCTION: fn_14824
void CUISlot1::Slot15(int width, int height) {
  CUISlot::Slot15(width, height);
  this->radioButton->Move(59, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  this->radioButton->disabled = 0;
  this->radioButton->Invalidate();
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  this->radioButton->checked = false;
  this->radioButton->Invalidate();
  this->itemIcon->Move(69, 11);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->description->Move(0, 44);
  this->description->SetTextOffset(2, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

// OFFSET: 0x009FE520
// FUNCTION: fn_14825
void CUISlot2::Slot15(int width, int height) {
  CUISlot::Slot15(width, height);
  this->radioButton->Move(3, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  this->radioButton->disabled = 0;
  this->radioButton->Invalidate();
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  this->radioButton->checked = false;
  this->radioButton->Invalidate();
  this->itemIcon->Move(13, 11);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->description->Move(10, 44);
  this->description->SetTextOffset(0, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

// OFFSET: 0x009FEE10
// FUNCTION: fn_14826
void CUISlot3::Slot15(int width, int height) {
  CUISlot::Slot15(width, height);
  this->radioButton->Move(3, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  this->radioButton->disabled = 0;
  this->radioButton->Invalidate();
  this->radioButton->SetImages(
    std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_select.bmp"),
    std::string("유저인터페이스\\EnchantUI\\btn_socket_over.bmp"), std::string("유저인터페이스\\EnchantUI\\btn_socket_out.bmp")
  );
  this->radioButton->checked = false;
  this->radioButton->Invalidate();
  this->itemIcon->Move(13, 11);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->description->Move(10, 44);
  this->description->SetTextOffset(0, 0);
  this->description->SetFont(std::string("Gulim"));
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  this->description->SetScrollBarButtonImages(std::string("유저인터페이스\\scroll0up.bmp"), std::string("유저인터페이스\\scroll0down.bmp"));
  this->description->SetScrollBarBackground(std::string("유저인터페이스\\scroll0mid.bmp"));
  this->description->SetScrollBarThumbImages(
    std::string("유저인터페이스\\scroll0bar_up.bmp"), std::string("유저인터페이스\\scroll0bar_mid.bmp"), std::string("유저인터페이스\\scroll0bar_down.bmp")
  );
}

}

namespace EnchantUI {
// OFFSET: 0x009FC080
// FUNCTION: fn_14819
void CUINormal::Slot15(int width, int height) {
  this->firstPage = new CUINormalRandom(*this->m_context);
  this->firstPage->SetSize(470, 377);
  this->firstPage->Move(0, 0);
  this->AddChild(this->firstPage);

  this->secondPage = new CUINormalPerfect(*this->m_context);
  this->secondPage->SetSize(470, 377);
  this->secondPage->Move(0, 0);
  this->AddChild(this->secondPage);

  this->tab = new CUITab(*this->m_context);
  this->tab->SetSize(76, 20);
  this->tab->Move(7, 1);
  this->AddChild(this->tab);

  CUITabButton* firstButton = new CUITabButton(*this->m_context);
  firstButton->SetSize(38, 20);
  firstButton->Move(0, 0);
  firstButton->disabled = 0;
  firstButton->Invalidate();
  {
    const std::uint8_t disabledSource[] = {
      0xA5, 0xFA, 0x09, 0x85, 0xCD, 0x21, 0xEA, 0x88, 0xA3, 0xB1, 0x09, 0xB3, 0xB0, 0x55, 0x73, 0x00,
      0x0B, 0x36, 0xA1, 0x1E, 0x63, 0x9B, 0x7A, 0x0C, 0x39, 0x21, 0xA8, 0x1D, 0x52, 0x8A, 0x41, 0x26,
      0x3A, 0x27, 0xA8, 0x11, 0x69, 0x80, 0x42, 0x1A, 0x01, 0x3C, 0xBA, 0x1E, 0x6F, 0x83, 0x4A, 0x6B,
      0x07, 0x38, 0xB9, 0x7F
    };
    // GLOBAL: gl_25230
    thread_local EncryptedName<sizeof(disabledSource), 0x452FEF0D7FC95565ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const std::uint8_t pressedSource[] = {
      0x9D, 0xA2, 0x89, 0x73, 0x61, 0xB3, 0x28, 0x52, 0x9B, 0xE9, 0x89, 0x45, 0x1C, 0xC7, 0xB1, 0xDA,
      0x33, 0x6E, 0x21, 0xE8, 0xCF, 0x09, 0xB8, 0xD6, 0x01, 0x79, 0x28, 0xEB, 0xFE, 0x18, 0x83, 0xFC,
      0x02, 0x7F, 0x28, 0xE7, 0xC5, 0x12, 0x80, 0xC0, 0x32, 0x78, 0x3D, 0xA7, 0xC3, 0x10, 0x9D, 0x9F
    };
    // GLOBAL: gl_25231
    thread_local EncryptedName<sizeof(pressedSource), 0x9FED7DA189490D5DULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x73, 0xC8, 0x99, 0x75, 0x1F, 0xE9, 0x5E, 0x4E, 0x75, 0x83, 0x99, 0x43, 0x62, 0x9D, 0xC7, 0xC6,
      0xDD, 0x04, 0x31, 0xEE, 0xB1, 0x53, 0xCE, 0xCA, 0xEF, 0x13, 0x38, 0xED, 0x80, 0x42, 0xF5, 0xE0,
      0xEC, 0x15, 0x38, 0xE1, 0xBB, 0x48, 0xF6, 0xDC, 0xC3, 0x15, 0x3C, 0xFC, 0xAC, 0x42, 0xFF, 0xAD,
      0xD1, 0x0A, 0x29, 0x8F
    };
    // GLOBAL: gl_25232
    thread_local EncryptedName<sizeof(hoverSource), 0x839B27DF8F5967B3ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0xAD, 0xB4, 0x67, 0xEB, 0xB7, 0xA1, 0x0E, 0x3E, 0xAB, 0xFF, 0x67, 0xDD, 0xCA, 0xD5, 0x97, 0xB6,
      0x03, 0x78, 0xCF, 0x70, 0x19, 0x1B, 0x9E, 0xBA, 0x31, 0x6F, 0xC6, 0x73, 0x28, 0x0A, 0xA5, 0x90,
      0x32, 0x69, 0xC6, 0x7F, 0x13, 0x00, 0xA6, 0xAC, 0x02, 0x6E, 0xD3, 0x3F, 0x15, 0x02, 0xBB, 0xF3
    };
    // GLOBAL: gl_25233
    thread_local EncryptedName<sizeof(normalSource), 0xF3CB6F7711A71B6DULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    firstButton->SetImages(normal, hover, pressed, disabled);
  }
  this->tab->AddChild(firstButton);

  CUITabButton* secondButton = new CUITabButton(*this->m_context);
  secondButton->SetSize(38, 20);
  secondButton->Move(38, 0);
  secondButton->disabled = 0;
  secondButton->Invalidate();
  {
    const std::uint8_t disabledSource[] = {
      0x2D, 0xDA, 0xA1, 0xD9, 0x77, 0x2D, 0x94, 0xE0, 0x2B, 0x91, 0xA1, 0xEF, 0x0A, 0x59, 0x0D, 0x68,
      0x83, 0x16, 0x09, 0x42, 0xD9, 0x97, 0x04, 0x64, 0xB1, 0x01, 0x00, 0x41, 0xE8, 0x86, 0x3F, 0x4E,
      0xB2, 0x16, 0x0E, 0x4D, 0xD1, 0x8A, 0x23, 0x40, 0xB2, 0x11, 0x08, 0x50, 0xD6, 0x81, 0x3D, 0x48,
      0xC3, 0x17, 0x0C, 0x53, 0xB7
    };
    // GLOBAL: gl_25234
    thread_local EncryptedName<sizeof(disabledSource), 0x2D51E3B7236175EDULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const std::uint8_t pressedSource[] = {
      0xD7, 0xB4, 0xED, 0x59, 0x7D, 0x25, 0x84, 0xC2, 0xD1, 0xFF, 0xED, 0x6F, 0x00, 0x51, 0x1D, 0x4A,
      0x79, 0x78, 0x45, 0xC2, 0xD3, 0x9F, 0x14, 0x46, 0x4B, 0x6F, 0x4C, 0xC1, 0xE2, 0x8E, 0x2F, 0x6C,
      0x48, 0x78, 0x42, 0xCD, 0xDB, 0x82, 0x33, 0x62, 0x48, 0x74, 0x58, 0xD7, 0x93, 0x89, 0x2C, 0x7F,
      0x17
    };
    // GLOBAL: gl_25235
    thread_local EncryptedName<sizeof(pressedSource), 0x0F41EBBDA32D1B17ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x01, 0xE8, 0x05, 0xED, 0x21, 0x57, 0x10, 0x36, 0x07, 0xA3, 0x05, 0xDB, 0x5C, 0x23, 0x89, 0xBE,
      0xAF, 0x24, 0xAD, 0x76, 0x8F, 0xED, 0x80, 0xB2, 0x9D, 0x33, 0xA4, 0x75, 0xBE, 0xFC, 0xBB, 0x98,
      0x9E, 0x24, 0xAA, 0x79, 0x87, 0xF0, 0xA7, 0x96, 0x9E, 0x37, 0xB7, 0x72, 0x92, 0xEA, 0xB0, 0x9F,
      0xEF, 0x25, 0xA8, 0x67, 0xE1
    };
    // GLOBAL: gl_25236
    thread_local EncryptedName<sizeof(hoverSource), 0xFBD599E117C547C1ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0xA5, 0x5C, 0x17, 0xA7, 0x4D, 0xDD, 0xAA, 0xDA, 0xA3, 0x17, 0x17, 0x91, 0x30, 0xA9, 0x33, 0x52,
      0x0B, 0x90, 0xBF, 0x3C, 0xE3, 0x67, 0x3A, 0x5E, 0x39, 0x87, 0xB6, 0x3F, 0xD2, 0x76, 0x01, 0x74,
      0x3A, 0x90, 0xB8, 0x33, 0xEB, 0x7A, 0x1D, 0x7A, 0x3A, 0x9C, 0xA2, 0x29, 0xA3, 0x71, 0x02, 0x67,
      0x65
    };
    // GLOBAL: gl_25237
    thread_local EncryptedName<sizeof(normalSource), 0x176F138D5DD7F365ULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    secondButton->SetImages(normal, hover, pressed, disabled);
  }
  this->tab->AddChild(secondButton);

  firstButton->page = this->firstPage;
  secondButton->page = this->secondPage;
  this->tab->Select(firstButton);
}

// OFFSET: 0x009FF700
// FUNCTION: fn_14827
void CUISlot::Slot15(int width, int height) {
  this->radioButton = new CUIRadioButton(*this->m_context);
  this->radioButton->SetSize(44, 44);
  this->radioButton->Move(59, 1);
  this->radioButton->SetOnSelect([this](CUIRadioButton* button) { this->SelectSlot(button); });
  this->radioButton->disabled = 0;
  this->radioButton->Invalidate();
  {
    const std::uint8_t disabledSource[] = {
      0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x52,
      0x59, 0x24, 0xC5, 0x58, 0xD1, 0x91, 0xFC, 0x5E, 0x6B, 0x25, 0xD9, 0x57, 0xE0, 0x96, 0xC6, 0x74,
      0x5C, 0x22, 0xD9, 0x66, 0xD0, 0x90, 0xDD, 0x39, 0x55, 0x2A, 0xDD, 0x39
    };
    // GLOBAL: gl_25189
    thread_local EncryptedName<sizeof(disabledSource), 0x17A9E5BF39AD4737ULL> disabledName(disabledSource);
    const std::string disabled(disabledName.Text());
    const std::uint8_t pressedSource[] = {
      0x65, 0x32, 0xAD, 0x03, 0xA3, 0xE9, 0xB2, 0xCE, 0x63, 0x79, 0xAD, 0x35, 0xDE, 0x9D, 0x2B, 0x46,
      0xCB, 0xFE, 0x05, 0x98, 0x0D, 0x53, 0x22, 0x4A, 0xF9, 0xFF, 0x19, 0x97, 0x3C, 0x54, 0x18, 0x60,
      0xCE, 0xF8, 0x19, 0xA6, 0x0C, 0x51, 0x12, 0x71, 0x8B, 0xFF, 0x00, 0x89, 0x63
    };
    // GLOBAL: gl_25190
    thread_local EncryptedName<sizeof(pressedSource), 0x03772763F96D9DA5ULL> pressedName(pressedSource);
    const std::string pressed(pressedName.Text());
    const std::uint8_t hoverSource[] = {
      0x47, 0xBC, 0xAB, 0x85, 0xF9, 0xC7, 0xB4, 0xC6, 0x41, 0xF7, 0xAB, 0xB3, 0x84, 0xB3, 0x2D, 0x4E,
      0xE9, 0x70, 0x03, 0x1E, 0x57, 0x7D, 0x24, 0x42, 0xDB, 0x71, 0x1F, 0x11, 0x66, 0x7A, 0x1E, 0x68,
      0xEC, 0x76, 0x1F, 0x20, 0x4A, 0x6C, 0x1D, 0x6E, 0xE4, 0x67, 0x45, 0x1D, 0x54, 0x79, 0x71
    };
    // GLOBAL: gl_25191
    thread_local EncryptedName<sizeof(hoverSource), 0x0B7109397F6B1387ULL> hoverName(hoverSource);
    const std::string hover(hoverName.Text());
    const std::uint8_t normalSource[] = {
      0x09, 0x5A, 0x9F, 0x71, 0x4F, 0x1B, 0x7E, 0xEE, 0x0F, 0x11, 0x9F, 0x47, 0x32, 0x6F, 0xE7, 0x66,
      0xA7, 0x96, 0x37, 0xEA, 0xE1, 0xA1, 0xEE, 0x6A, 0x95, 0x97, 0x2B, 0xE5, 0xD0, 0xA6, 0xD4, 0x40,
      0xA2, 0x90, 0x2B, 0xD4, 0xE0, 0xA0, 0xCF, 0x0D, 0xAB, 0x98, 0x2F, 0x8B
    };
    // GLOBAL: gl_25192
    thread_local EncryptedName<sizeof(normalSource), 0x23BBD58F8B5FF5C9ULL> normalName(normalSource);
    const std::string normal(normalName.Text());
    this->radioButton->SetImages(normal, hover, pressed, disabled);
  }
  this->radioButton->checked = false;
  this->radioButton->Invalidate();
  this->AddChild(this->radioButton);

  this->itemIcon = new CUIGameItem(*this->m_context);
  this->itemIcon->SetMode(0);
  this->itemIcon->SetScale(100);
  this->itemIcon->Move(69, 11);
  this->itemIcon->unknown_0084 = 1;
  this->itemIcon->unknown_0085 = 1;
  this->AddChild(this->itemIcon);

  this->description = new CUITextViewer(*this->m_context);
  this->description->SetSize(96, 39);
  this->description->Move(0, 44);
  this->description->SetTextOffset(2, 0);
  {
    const std::uint8_t fontSource[] = {0x20, 0xDE, 0xFD, 0xCC, 0xCC, 0x3F};
    // GLOBAL: gl_25193
    thread_local EncryptedName<sizeof(fontSource), 0x13E33FA1A591AB67ULL> fontName(fontSource);
    const std::string font(fontName.Text());
    this->description->SetFont(font);
  }
  this->description->SetFontHeight(12);
  this->description->SetBold(true);
  this->description->SetTextColor(0x965532);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(1);
  this->description->SetBackgroundColor(0xFFFF00FF);
  this->description->SetPadding(0, 0, 0, 0);
  this->description->SetScrollBarSize(13, 39);
  this->description->SetScrollStep(13);
  {
    const std::uint8_t downSource[] = {
      0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xEA,
      0x20, 0x5B, 0x3C, 0x4B, 0xB9, 0x8F, 0x2D, 0xF6, 0x34, 0x47, 0x7D, 0x45, 0xB8, 0xCF, 0x49
    };
    // GLOBAL: gl_25194
    thread_local EncryptedName<sizeof(downSource), 0x9949BFD527532943ULL> downName(downSource);
    const std::string down(downName.Text());
    const std::uint8_t upSource[] = {
      0xAF, 0xD2, 0x3B, 0xD9, 0xB1, 0xC9, 0x4A, 0x00, 0xA9, 0x99, 0x3B, 0xEF, 0xCC, 0xBD, 0xD3, 0xBE,
      0x0C, 0x0F, 0x94, 0x4F, 0x1D, 0x37, 0xFA, 0xBD, 0x41, 0x1F, 0x96, 0x53, 0x71
    };
    // GLOBAL: gl_25195
    thread_local EncryptedName<sizeof(upSource), 0xCD8F077123FB7D6FULL> upName(upSource);
    const std::string up(upName.Text());
    this->description->SetScrollBarButtonImages(up, down);
  }
  {
    const std::uint8_t backgroundSource[] = {
      0xBF, 0xBC, 0x1F, 0x2B, 0x45, 0xA9, 0xBC, 0x20, 0xB9, 0xF7, 0x1F, 0x1D, 0x38, 0xDD, 0x25, 0x9E,
      0x1C, 0x61, 0xB0, 0xBD, 0xE9, 0x57, 0x14, 0x84, 0x1B, 0x3D, 0xBD, 0xBC, 0xF5, 0x67
    };
    // GLOBAL: gl_25196
    thread_local EncryptedName<sizeof(backgroundSource), 0xED796785D1DF137FULL> backgroundName(backgroundSource);
    const std::string background(backgroundName.Text());
    this->description->SetScrollBarBackground(background);
  }
  {
    const std::uint8_t thumbBottomSource[] = {
      0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0x82,
      0x92, 0xB5, 0x56, 0x65, 0x7B, 0x49, 0x2D, 0x90, 0x83, 0x98, 0x5D, 0x66, 0x60, 0x17, 0x61, 0x93,
      0x9C, 0xB7, 0x39
    };
    // GLOBAL: gl_25197
    thread_local EncryptedName<sizeof(thumbBottomSource), 0xF14F79170939C7F1ULL> thumbBottomName(thumbBottomSource);
    const std::string thumbBottom(thumbBottomName.Text());
    const std::uint8_t thumbMiddleSource[] = {
      0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xD6,
      0xFC, 0xB7, 0x02, 0xC7, 0xBB, 0x19, 0x9F, 0xC4, 0xED, 0x9A, 0x00, 0xC2, 0xB3, 0x07, 0x9F, 0xC8,
      0xEF, 0xC5
    };
    // GLOBAL: gl_25198
    thread_local EncryptedName<sizeof(thumbMiddleSource), 0xA5FD29D7AB6DC59FULL> thumbMiddleName(thumbMiddleSource);
    const std::string thumbMiddle(thumbMiddleName.Text());
    const std::uint8_t thumbTopSource[] = {
      0xC9, 0x88, 0xEF, 0x41, 0x7D, 0x93, 0xB8, 0x14, 0xCF, 0xC3, 0xEF, 0x77, 0x00, 0xE7, 0x21, 0xAA,
      0x6A, 0x55, 0x40, 0xD7, 0xD1, 0x6D, 0x1F, 0xB8, 0x7B, 0x78, 0x5A, 0xCB, 0x93, 0x3F, 0x10, 0xA9,
      0x09
    };
    // GLOBAL: gl_25199
    thread_local EncryptedName<sizeof(thumbTopSource), 0xD97D5DBDBB2F2709ULL> thumbTopName(thumbTopSource);
    const std::string thumbTop(thumbTopName.Text());
    this->description->SetScrollBarThumbImages(thumbTop, thumbMiddle, thumbBottom);
  }
  this->AddChild(this->description);
}
}

// OFFSET: 0x00A021D0
// FUNCTION: fn_14838
void CUIRadioButton::SetOnSelect(std::function<void(CUIRadioButton*)> callback) {
  this->onSelect = callback;
}

namespace EnchantUI {
// OFFSET: 0x00A02D80
// FUNCTION: fn_14845
void CUIEnchantPage::Slot59(int value) {
  this->unknown_00B0 = value;
}

// OFFSET: 0x00A05190
// FUNCTION: fn_14970
void CUIUpgradeEnchantPage::Slot58(int value) {
  if (CUIMainPage* mainPage = dynamic_cast<CUIMainPage*>(this->parent)) {
    mainPage->firstPage->Slot59(value);
    mainPage->secondPage->Slot59(value);
  }
}

// OFFSET: 0x00A05290
// FUNCTION: fn_14972
int CUIEnchantPage::FindEmptySlot() {
  const std::vector<int>& slots = this->enchantInfo->slots;
  auto it = std::find_if(slots.begin(), slots.end(), [item = *this->gameItem->GetItemInfo()](int slot) {
    if (slot >= 4) {
      return true;
    }
    int card;
    std::memcpy(&card, item.unknown_001C + slot * 4, sizeof(card));
    return card == 0;
  });
  if (it == slots.end()) {
    return -1;
  }
  return *it;
}

// OFFSET: 0x00A053D0
// FUNCTION: fn_14973
ITEM_INFO* CUITargetItem::GetItemInfo() {
  return this->itemIcon->GetItemInfo();
}

// OFFSET: 0x00A053E0
// FUNCTION: fn_14974
std::optional<EnchantOptionInfo*> CUIEnchantPage::FindOption(uint32_t key) {
  if (this->enchantInfo == nullptr) {
    return std::nullopt;
  }

  auto it = this->enchantInfo->options.find(key);
  if (it != this->enchantInfo->options.end()) {
    return &it->second;
  }
  return std::nullopt;
}

// OFFSET: 0x00A05CE0
// FUNCTION: fn_14980
void CUISlot::Paint() {
  this->ClearSurface(0xFFFF00FF);
}
}

// OFFSET: 0x00A05DF0
// FUNCTION: fn_14983
void CUIEnchantUI::Slot16() {
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
}

namespace EnchantUI {
// OFFSET: 0x00A06050
// FUNCTION: fn_14988
void CUISlot0::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(0);
}

// OFFSET: 0x00A06090
// FUNCTION: fn_14989
void CUISlot1::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(1);
}

// OFFSET: 0x00A060D0
// FUNCTION: fn_14990
void CUISlot2::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(2);
}

// OFFSET: 0x00A06110
// FUNCTION: fn_14991
void CUISlot3::Slot16() {
  CUISlot::Slot16();
  this->radioButton->SetId(3);
}

// OFFSET: 0x00A06150
// FUNCTION: fn_14992
void CUISlot::Slot16() {
  this->itemIcon->hitEnabled = 0;
  this->description->scrollBar->SetHidden(1);
}
}

// OFFSET: 0x00A06190
// FUNCTION: fn_14994
void CUIEnchantUI::Slot25(int x, int y) {
  if (this->titleBar->IsPointInside(x, y)) {
    CUIFrameWnd::Slot25(x, y);
  }
}

namespace EnchantUI {
// OFFSET: 0x00A061C0
// FUNCTION: fn_14995
void CUIEnchantItem::Slot28(int x, int y) {
  this->itemIcon->Slot28(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A061F0
// FUNCTION: fn_14996
void CUITargetItem::Slot28(int x, int y) {
  this->itemIcon->Slot28(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06220
// FUNCTION: fn_14997
void CUIEnchantItem::Slot32(int x, int y) {
  this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A06250
// FUNCTION: fn_14998
void CUISlot::Slot32(int x, int y) {
  if (this->itemIcon->IsPointInside(x, y)) {
    this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
  }
}

// OFFSET: 0x00A06290
// FUNCTION: fn_14999
void CUITargetItem::Slot32(int x, int y) {
  this->itemIcon->Slot32(x - this->itemIcon->geometry.x, y - this->itemIcon->geometry.y);
}

// OFFSET: 0x00A062C0
// FUNCTION: fn_15000
void CUISlot::SelectSlot(UIWindow* button) {
  if (CUIEnchantPage* page = dynamic_cast<CUIEnchantPage*>(this->parent)) {
    page->Slot58(button->geometry.id);
  }
}

// OFFSET: 0x00A06A50
// FUNCTION: fn_15005
void CUITargetItem::SetSelected(bool selected) {
  if (selected) {
    if (this->selected != 1) {
      this->itemIcon->Move(this->itemIcon->geometry.x + 1, this->itemIcon->geometry.y + 1);
      this->unknown_00EC->Move(this->unknown_00EC->geometry.x + 1, this->unknown_00EC->geometry.y + 1);
      this->unknown_00F0->Move(this->unknown_00F0->geometry.x + 1, this->unknown_00F0->geometry.y + 1);
    }
  } else if (this->selected == 1) {
    this->itemIcon->Move(this->itemIcon->geometry.x - 1, this->itemIcon->geometry.y - 1);
    this->unknown_00EC->Move(this->unknown_00EC->geometry.x - 1, this->unknown_00EC->geometry.y - 1);
    this->unknown_00F0->Move(this->unknown_00F0->geometry.x - 1, this->unknown_00F0->geometry.y - 1);
  }
  CUIListItem::SetSelected(selected);
}

// OFFSET: 0x00A04D70
// FUNCTION: fn_14965
void CUINormalPerfect::Slot55() {
  CUIListBoxEx<CUIEnchantItem, CEnchantItemData>* list = this->enchantList;
  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto it = std::next(list->data.begin(), list->selected);
    CEnchantMgr::GetInstance()->RequestPerfectEnchant(
      this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->unknown_0000[1]), static_cast<int16_t>(this->unknown_00B0), it->itemId
    );
  }
}

// OFFSET: 0x00A04E60
// FUNCTION: fn_14966
void CUINormalRandom::Slot55() {
  CEnchantMgr::GetInstance()->RequestRandomEnchant(
    this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->unknown_0000[1]), static_cast<int16_t>(this->unknown_00B0)
  );
}

// OFFSET: 0x00A04F10
// FUNCTION: fn_14967
void CUIReset::Slot55() {
  CEnchantMgr::GetInstance()->RequestResetEnchant(this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->unknown_0000[1]));
}

// OFFSET: 0x00A04FB0
// FUNCTION: fn_14968
void CUIUpgradePerfect::Slot55() {
  if (this->unknown_00B8) {
    CEnchantMgr::GetInstance()->RequestUpgradeEnchant(
      this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->unknown_0000[1]), static_cast<int16_t>(this->unknown_00B0)
    );
    return;
  }

  CUIListBoxEx<CUIEnchantItem, CEnchantItemData>* list = this->enchantList;
  if (static_cast<int>(list->selected) >= 0 && list->selected < list->data.size()) {
    auto it = std::next(list->data.begin(), list->selected);
    CEnchantMgr::GetInstance()->RequestUpgradePerfectEnchant(
      this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->unknown_0000[1]), static_cast<int16_t>(this->unknown_00B0), it->itemId
    );
  }
}

// OFFSET: 0x00A050E0
// FUNCTION: fn_14969
void CUIUpgradeRandom::Slot55() {
  CEnchantMgr::GetInstance()->RequestUpgradeRandomEnchant(
    this->group, static_cast<int16_t>(this->gameItem->GetItemInfo()->unknown_0000[1]), static_cast<int16_t>(this->unknown_00B0)
  );
}
}

// OFFSET: 0x00A08110
// FUNCTION: fn_15017
bool CUIEnchantUI::CanHitTest() {
  return !CEnchantMgr::GetInstance()->m_unknown_0030;
}

// OFFSET: 0x009F6D40
// FUNCTION: fn_14766
template <>
CUIListBoxEx<EnchantUI::CUIEnchantItem, EnchantUI::CEnchantItemData>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x009F6F00
// FUNCTION: fn_14767
template <>
CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x009F7670
// FUNCTION: fn_14774
template <>
CUIListBoxEx<EnchantUI::CUITargetItem, ITEM_INFO>::~CUIListBoxEx() {
}

namespace EnchantUI {
// OFFSET: 0x00A06BE0
// FUNCTION: fn_15007
void CUIEnchantItem::SetFrozen(bool frozen) {
  this->itemIcon->transparent = frozen;
  this->itemIcon->Invalidate();
  this->name->SetColor(frozen ? 0xAAAAAA : 0xC89682);
}

// OFFSET: 0x00A05860
// FUNCTION: fn_14977
void UpdateEnchantItem(CUIEnchantItem* item, const CEnchantItemData* data) {
  ITEM_INFO info;
  info.SetItemId(data->itemId);
  info.identified = 1;
  item->itemIcon->SetItem(info);
  const std::string name = info.DisplayName(0);
  item->name->SetText(name.c_str(), static_cast<unsigned>(name.size()));
  item->SetFrozen(data->frozen);
}

// OFFSET: 0x00A05B90
// FUNCTION: fn_14979
void UpdateTargetItem(CUITargetItem* item, ITEM_INFO* info) {
  item->itemIcon->SetItem(*info);
  if (info->unknown_0088 > 0) {
    item->unknown_00EC->SetImage(std::format("유저인터페이스\\grade_enchant\\grade_icon{}.bmp", info->unknown_0088));
  }
  item->unknown_00EC->Show(info->unknown_0088 > 0);
  const std::string name = info->DisplayName(0);
  item->unknown_00F0->SetText(name.c_str(), name.size());
}
// OFFSET: 0x00A05E20
// FUNCTION: fn_14984
void CUIMainPage::Slot16() {
  CUITabButton* button = this->tab->FindButton(this->firstPage);
  if (button != nullptr) {
    button->disabled = 1;
    button->Invalidate();
  }

  button = this->tab->FindButton(this->secondPage);
  if (button != nullptr) {
    button->disabled = 1;
    button->Invalidate();
  }
}
}

// OFFSET: 0x009F3160
// FUNCTION: fn_14752
void CUIEnchantUI::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(690, 470);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A06170
// FUNCTION: fn_14993
void EnchantUI::CUIUpgradeRandom::Slot16() {
  EnchantUI::CUIEnchantPage::Slot16();
  this->enchantList->unknown_00C4 = 1;
  this->enchantList->Invalidate();
}

namespace EnchantUI {
// OFFSET: 0x00A05CF0
// FUNCTION: fn_14981
void CUIEnchantPage::Slot16() {
  for (int i = 0; i < 8; ++i) {
    CUIIngredient* ingredient = this->CreateIngredient();
    ingredient->Move(ingredient->geometry.x + i % 4 * 74, ingredient->geometry.y + i / 4 * 52);
    this->unknown_0090.push_back(ingredient);
  }

  this->ResetSlots();
}

// OFFSET: 0x00A05E80
// FUNCTION: fn_14985
void CUINormalPerfect::Slot16() {
  CUIEnchantPage::Slot16();
  for (const auto& slot : this->unknown_0088) {
    slot.second->radioButton->disabled = 1;
    slot.second->radioButton->Invalidate();
  }
}

// OFFSET: 0x00A05F10
// FUNCTION: fn_14986
void CUINormalRandom::Slot16() {
  CUIEnchantPage::Slot16();
  for (const auto& slot : this->unknown_0088) {
    slot.second->radioButton->disabled = 1;
    slot.second->radioButton->Invalidate();
  }

  this->enchantList->unknown_00C4 = 1;
  this->enchantList->Invalidate();
}

// OFFSET: 0x00A05FB0
// FUNCTION: fn_14987
void CUIReset::Slot16() {
  CUIEnchantPage::Slot16();
  for (const auto& slot : this->unknown_0088) {
    slot.second->radioButton->disabled = 1;
    slot.second->radioButton->Invalidate();
  }

  this->enchantList->unknown_00C4 = 1;
  this->enchantList->Invalidate();
}

// OFFSET: 0x00A06350
// FUNCTION: fn_15002
void CUIEnchantPage::Slot57() {
  this->enchantList->ClearData();
  for (CUIIngredient* ingredient : this->unknown_0090) {
    ingredient->Show(0);
  }

  const std::string count = std::to_string(0);
  this->unknown_009C->SetText(count.c_str(), count.size());
  this->unknown_00A0->disabled = 1;
  this->unknown_00A0->Invalidate();

  const bool ready = this->Slot56();
  if (CUIMainPage* mainPage = dynamic_cast<CUIMainPage*>(this->parent)) {
    if (CUITabButton* button = mainPage->tab->FindButton(this)) {
      button->disabled = !ready;
      button->Invalidate();
    }
  }
}

// OFFSET: 0x00A06520
// FUNCTION: fn_15003
void CUIReset::Slot57() {
  for (CUIIngredient* ingredient : this->unknown_0090) {
    ingredient->Show(0);
  }

  const std::string count = std::to_string(0);
  this->unknown_009C->SetText(count.c_str(), count.size());
  this->unknown_00A0->disabled = 1;
  this->unknown_00A0->Invalidate();
  this->enchantList->ClearData();
  this->Slot56();
}

// OFFSET: 0x00A06800
// FUNCTION: fn_15004
void CUIEnchantPage::ResetSlots() {
  for (const auto& slot : this->unknown_0088) {
    slot.second->Show(0);
  }

  for (CUIIngredient* ingredient : this->unknown_0090) {
    ingredient->Show(0);
  }

  this->enchantList->ClearData();
  const std::string count = std::to_string(0);
  this->unknown_009C->SetText(count.c_str(), count.size());
  this->unknown_00A0->disabled = 1;
  this->unknown_00A0->Invalidate();
  this->Slot59(-1);
}

// OFFSET: 0x00A06F80
// FUNCTION: fn_15009
bool CUINormalPerfect::Slot56() {
  const int slot = this->FindEmptySlot();
  if (slot == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(slot);
  if (!option) {
    return false;
  }

  this->Slot59(slot);
  if ((*option)->perfectEnchants.size() == 0) {
    return false;
  }

  for (const auto& entry : (*option)->perfectEnchants) {
    const bool lackZeny = g_session.m_zeny < entry.second.cost;
    const bool lackMaterial = std::any_of(entry.second.materials.begin(), entry.second.materials.end(), [](const std::pair<const int, int>& material) {
      return static_cast<unsigned int>(g_session.GetInventoryItemCount(material.first)) < static_cast<unsigned int>(material.second);
    });
    this->enchantList->AddData({entry.first, lackZeny || lackMaterial}, false);
  }

  this->enchantList->UpdateScrollBar();
  return true;
}
}

namespace EnchantUI {
// OFFSET: 0x00A05470
// FUNCTION: fn_14975
void CUINormalPerfect::Slot54(CUIEnchantItem* item, const CEnchantItemData& data) {
  for (CUIIngredient* ingredient : this->unknown_0090) {
    ingredient->Show(0);
  }

  const std::string count = std::to_string(0);
  this->unknown_009C->SetText(count.c_str(), count.size());

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->unknown_00B0);
  if (option && (*option)->perfectEnchants.size() != 0) {
    const int itemId = std::atoi(item->itemIcon->GetItemInfo()->itemId.c_str());
    auto it = (*option)->perfectEnchants.find(itemId);
    if (it != (*option)->perfectEnchants.end()) {
      this->SetCost(it->second);
    }
  }
}

// OFFSET: 0x00A05640
// FUNCTION: fn_14976
void CUIUpgradePerfect::Slot54(CUIEnchantItem* item, const CEnchantItemData& data) {
  for (CUIIngredient* ingredient : this->unknown_0090) {
    ingredient->Show(0);
  }

  const std::string count = std::to_string(0);
  this->unknown_009C->SetText(count.c_str(), count.size());

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->unknown_00B0);
  if (option && (*option)->perfectUpgrades.size() != 0) {
    const int baseItemId = std::atoi(this->unknown_0088[this->unknown_00B0]->itemIcon->GetItemInfo()->itemId.c_str());
    auto base = (*option)->perfectUpgrades.find(baseItemId);
    if (base != (*option)->perfectUpgrades.end()) {
      const int itemId = std::atoi(item->itemIcon->GetItemInfo()->itemId.c_str());
      auto it = base->second.find(itemId);
      if (it != base->second.end()) {
        this->SetCost(it->second);
      }
    }
  }
}

// OFFSET: 0x00A072E0
// FUNCTION: fn_15011
bool CUIReset::Slot56() {
  this->SetCost(*reinterpret_cast<const EnchantPerfectInfo*>(&this->enchantInfo->resetCost));
  if (!this->enchantInfo->resetEnabled) {
    this->unknown_00A0->disabled = 1;
    this->unknown_00A0->Invalidate();
    return false;
  }

  const std::vector<int>& slots = this->enchantInfo->slots;
  const bool enchanted = std::any_of(slots.begin(), slots.end(), [item = *this->gameItem->GetItemInfo()](int slot) {
    if (slot >= 4) {
      return false;
    }
    int card;
    std::memcpy(&card, item.unknown_001C + slot * 4, sizeof(card));
    return card != 0;
  });
  this->unknown_00A0->disabled = !enchanted;
  this->unknown_00A0->Invalidate();
  return enchanted;
}

// OFFSET: 0x00A07BA0
// FUNCTION: fn_15015
void CUIIngredient::SetMaterial(int itemId, int required) {
  ITEM_INFO info;
  info.SetItemId(itemId);
  info.identified = 1;
  this->itemIcon->SetItem(info);

  const int owned = g_session.GetInventoryItemCount(itemId);
  const std::string ownedCount = std::to_string(owned);
  this->ownedText->SetText(ownedCount.c_str(), ownedCount.size());
  const std::string requiredCount = std::format("/{}", required);
  this->requiredText->SetText(requiredCount.c_str(), requiredCount.size());

  const bool enough = owned >= required;
  this->unknown_0084->Show(enough);
  const std::uint32_t color = enough ? 0xF0F0F0 : 0x5A3CE6;
  this->ownedText->SetColor(color);
  this->ownedText->SetOutline(enough, 0x965532);
  this->requiredText->SetColor(color);
  this->requiredText->SetOutline(enough, 0x965532);
}

// OFFSET: 0x00A07F00
// FUNCTION: fn_15016
void CUIEnchantPage::SetCost(const EnchantPerfectInfo& info) {
  auto material = info.materials.begin();
  for (CUIIngredient* ingredient : this->unknown_0090) {
    if (material == info.materials.end()) {
      ingredient->Show(0);
    } else {
      ingredient->Show(1);
      ingredient->SetMaterial(material->first, material->second);
      ++material;
    }
  }

  const std::string cost = std::to_string(info.cost);
  this->unknown_009C->SetText(cost.c_str(), cost.size());

  const bool lackZeny = g_session.m_zeny < info.cost;
  const bool lackMaterial = std::any_of(this->unknown_0090.begin(), this->unknown_0090.end(), [](CUIIngredient* ingredient) {
    return ingredient->geometry.visible && !ingredient->unknown_0084->geometry.visible;
  });
  this->unknown_00A0->disabled = lackZeny || lackMaterial;
  this->unknown_00A0->Invalidate();
}

// OFFSET: 0x00A07100
// FUNCTION: fn_15010
bool CUINormalRandom::Slot56() {
  const int slot = this->FindEmptySlot();
  if (slot == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(slot);
  if (!option) {
    return false;
  }

  this->Slot59(slot);
  if ((*option)->enchants.size() == 0) {
    return false;
  }

  auto grade = (*option)->enchants.find(this->gameItem->GetItemInfo()->unknown_0088);
  if (grade == (*option)->enchants.end()) {
    return false;
  }

  std::list<int> itemIds;
  for (const auto& entry : grade->second) {
    itemIds.push_back(entry.first);
  }
  for (int itemId : itemIds) {
    this->enchantList->AddData({itemId, false}, false);
  }

  this->enchantList->UpdateScrollBar();
  this->SetCost(*reinterpret_cast<const EnchantPerfectInfo*>(*option));
  return true;
}

// OFFSET: 0x00A075E0
// FUNCTION: fn_15013
bool CUIUpgradeRandom::Slot56() {
  if (this->unknown_00B0 == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->unknown_00B0);
  if (!option || (*option)->randomUpgrades.size() == 0) {
    return false;
  }

  const int baseItemId = std::atoi(this->unknown_0088[this->unknown_00B0]->itemIcon->GetItemInfo()->itemId.c_str());
  auto base = (*option)->randomUpgrades.find(baseItemId);
  if (base == (*option)->randomUpgrades.end()) {
    return false;
  }

  std::list<int> itemIds;
  for (const auto& entry : base->second.rates) {
    itemIds.push_back(entry.first);
  }
  for (int itemId : itemIds) {
    this->enchantList->AddData({itemId, false}, false);
  }

  this->enchantList->UpdateScrollBar();
  this->SetCost(*reinterpret_cast<const EnchantPerfectInfo*>(&base->second));
  return true;
}

// OFFSET: 0x00A07440
// FUNCTION: fn_15012
bool CUIUpgradePerfect::Slot56() {
  if (this->unknown_00B0 == -1) {
    return false;
  }

  std::optional<EnchantOptionInfo*> option = this->FindOption(this->unknown_00B0);
  if (!option) {
    return false;
  }

  this->unknown_00B8 = (*option)->isUpgrade;
  if ((*option)->perfectUpgrades.size() == 0) {
    return false;
  }

  const int baseItemId = std::atoi(this->unknown_0088[this->unknown_00B0]->itemIcon->GetItemInfo()->itemId.c_str());
  auto base = (*option)->perfectUpgrades.find(baseItemId);
  if (base == (*option)->perfectUpgrades.end() || base->second.size() == 0) {
    return false;
  }

  for (const auto& entry : base->second) {
    const bool lackZeny = g_session.m_zeny < entry.second.cost;
    const bool lackMaterial = std::any_of(entry.second.materials.begin(), entry.second.materials.end(), [](const std::pair<const int, int>& material) {
      return static_cast<unsigned int>(g_session.GetInventoryItemCount(material.first)) < static_cast<unsigned int>(material.second);
    });
    this->enchantList->AddData({entry.first, lackZeny || lackMaterial}, false);
  }

  this->enchantList->UpdateScrollBar();
  return true;
}
}

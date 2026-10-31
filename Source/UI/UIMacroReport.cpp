#include "UI/UIMacroReport.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
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
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

#pragma pack(push, 1)
struct ReportUserPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t accountId;
  /* 0006 */ int targetId;
  /* 000A */ char targetName[24];
  /* 0022 */ std::uint16_t type;
  /* 0024 */ char message[101];
};
#pragma pack(pop)

static_assert(sizeof(ReportUserPacket) == 0x89);
static_assert(offsetof(ReportUserPacket, type) == 0x22);
static_assert(offsetof(ReportUserPacket, message) == 0x24);

// OFFSET: 0x00A477F0
// FUNCTION: fn_16096
CUIMacroReport::CUIMacroReport()
    : m_background(nullptr), m_titleBar(nullptr), m_messageEdit(nullptr), unknown_00E4(nullptr), unknown_00E8(nullptr), unknown_00EC(nullptr),
      unknown_00F0(nullptr), m_firstTypeButton(nullptr), m_secondTypeButton(nullptr), unknown_00FC(nullptr), unknown_0100(0), m_reportType(0) {
}

// OFFSET: 0x00A47A30
// FUNCTION: fn_16098
void CUIMacroReport::OnButtonClick() {
}

// OFFSET: 0x00A47A50
// FUNCTION: fn_16099
void CUIMacroReport::SendReport() {
  ReportUserPacket packet;
  packet.id = ID_CZ_REQ_REPORT_USER;
  packet.targetId = this->unknown_0100;
  packet.accountId = g_session.m_aid;
  packet.type = static_cast<std::uint16_t>(this->m_reportType);
  strncpy_s(packet.targetName, sizeof(packet.targetName), this->unknown_0108.c_str(), _TRUNCATE);
  {
    const std::string message = this->m_messageEdit->edit->Text();
    strncpy_s(packet.message, sizeof(packet.message), message.c_str(), _TRUNCATE);
  }
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
  this->m_frameContext->windows.CloseWindow(10005);
}

// OFFSET: 0x00A47C30
// FUNCTION: fn_16100
void CUIMacroReport::OnEditSubmit() {
}

// OFFSET: 0x00A47C40
// FUNCTION: fn_16101
void CUIMacroReport::Slot16() {
  this->m_resolveSkin = true;
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);

  if (g_modeMgr.GetGameMode()) {
    this->unknown_0100 = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_02E0;
    this->unknown_0108 = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->unknown_02EC;
  }
}

// OFFSET: 0x00A47CD0
// FUNCTION: fn_16102
void CUIMacroReport::OnReportTypeSelect(CUIRadioButton* button) {
  if (m_firstTypeButton == button) {
    m_reportType = 0;
  } else if (m_secondTypeButton == button) {
    m_reportType = 1;
  }
}

// OFFSET: 0x00A441F0
// FUNCTION: fn_16052
void CUIMacroReport::Reset() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = false;
  this->Resize(0xE7, 0xBD);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A44170
// FUNCTION: fn_16051
CUIFrameWnd* CreateMacroReport() {
  return new CUIMacroReport();
}

// OFFSET: 0x00A44240
// FUNCTION: fn_16053
void CUIMacroReport::Slot15(int width, int height) {
  this->m_background = new CUIStaticImage(*this->m_context);
  this->m_background->SetSize(231, 172);
  this->m_background->Move(0, 17);
  {
    CUIStaticImage* background = this->m_background;
    const std::uint8_t backgroundImageSource[] = {
        0x19, 0x56, 0x21, 0x19, 0xAD, 0xDB, 0x68, 0xE6, 0x1F, 0x1D, 0x21, 0x2F, 0xD0, 0xAF, 0xF1, 0x66, 0xB8, 0x9A, 0x93, 0x8C, 0x3F, 0x70, 0xDD, 0x44,
        0xAB, 0x8D, 0xBD, 0x81, 0x0A, 0x4A, 0xC0, 0x4A, 0xBA, 0x8B, 0x8E, 0x91, 0x08, 0x65, 0xC2, 0x59, 0xAD, 0xD7, 0x83, 0x8E, 0x1D, 0x15};
    thread_local EncryptedName<sizeof(backgroundImageSource), 0x2BAD156DE3E1F9D9ULL> backgroundImageName(backgroundImageSource);
    const std::string backgroundImage(backgroundImageName.Text());
    background->SetImage(backgroundImage);
  }
  this->AddChild(this->m_background);

  this->m_titleBar = new CUITitleBar(*this->m_context);
  this->m_titleBar->SetSize(231, 17);
  this->m_titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->m_titleBar;
    const std::uint8_t titleRightSource[] = {
        0x65, 0x32, 0xAD, 0x03, 0xA3, 0xE9, 0xB2, 0xCE, 0x63, 0x79, 0xAD, 0x35, 0xDE, 0x9D, 0x2B, 0x61, 0xC4, 0xEE, 0x04, 0x9A, 0x3C, 0x4E, 0x19, 0x77,
        0xC0, 0xEF, 0x0B, 0x98, 0x00, 0x42, 0x2B, 0x77, 0xCC, 0xE9, 0x01, 0x9C, 0x01, 0x46, 0x05, 0x5C, 0xD7, 0xF4, 0x0A, 0x91, 0x17, 0x09, 0x15, 0x6E,
        0xD5, 0x9D};
    thread_local EncryptedName<sizeof(titleRightSource), 0x03772763F96D9DA5ULL> titleRightName(titleRightSource);
    const std::string titleRight(titleRightName.Text());
    const std::uint8_t titleMiddleSource[] = {
        0xAD, 0x14, 0x55, 0xD7, 0x53, 0x35, 0xD2, 0x08, 0xAB, 0x5F, 0x55, 0xE1, 0x2E, 0x41, 0x4B, 0xA7, 0x0C, 0xC8, 0xFC, 0x4E, 0xCC, 0x92, 0x79, 0xB1,
        0x08, 0xC9, 0xF3, 0x4C, 0xF0, 0x9E, 0x4B, 0xB1, 0x04, 0xCF, 0xF9, 0x48, 0xF1, 0x9A, 0x65, 0x9A, 0x00, 0xD2, 0xF1, 0x03, 0xF1, 0x96, 0x67, 0xC5};
    thread_local EncryptedName<sizeof(titleMiddleSource), 0xC517FB932D95BB6DULL> titleMiddleName(titleMiddleSource);
    const std::string titleMiddle(titleMiddleName.Text());
    const std::uint8_t titleLeftSource[] = {
        0x09, 0x5A, 0x9F, 0x71, 0x4F, 0x1B, 0x7E, 0xEE, 0x0F, 0x11, 0x9F, 0x47, 0x32, 0x6F, 0xE7, 0x41, 0xA8, 0x86, 0x36, 0xE8, 0xD0, 0xBC, 0xD5, 0x57,
        0xAC, 0x87, 0x39, 0xEA, 0xEC, 0xB0, 0xE7, 0x57, 0xA0, 0x81, 0x33, 0xEE, 0xED, 0xB4, 0xC9, 0x7C, 0xA5, 0x90, 0x39, 0xFF, 0xA1, 0xB7, 0xD6, 0x53,
        0xC9};
    thread_local EncryptedName<sizeof(titleLeftSource), 0x23BBD58F8B5FF5C9ULL> titleLeftName(titleLeftSource);
    const std::string titleLeft(titleLeftName.Text());
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->m_titleBar);

  CUIStaticText* title = new CUIStaticText(*this->m_context);
  title->SetSize(100, 17);
  title->Move(18, 0);
  {
    const std::uint8_t titleTextSource[] = {
        0x47, 0xBC, 0xAB, 0x85, 0x19, 0xB4, 0xB4, 0xBB, 0x6A, 0x13};
    thread_local EncryptedName<sizeof(titleTextSource), 0x0B7109397F6B1387ULL> titleTextName(titleTextSource);
    const char* value = titleTextName.Text();
    title->SetText(value, std::strlen(value));
  }
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const std::uint8_t titleFontSource[] = {
        0x20, 0xDE, 0xFD, 0xCC, 0xCC, 0x3F};
    thread_local EncryptedName<sizeof(titleFontSource), 0x13E33FA1A591AB67ULL> titleFontName(titleFontSource);
    title->SetFont(std::string(titleFontName.Text()));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(1, 0xFFFFFF);
  this->m_titleBar->titleText = title;
  this->m_titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton(*this->m_context);
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  {
    const std::uint8_t menuDisabledSource[] = {
        0xC9, 0x88, 0xEF, 0x41, 0x7D, 0x93, 0xB8, 0x14, 0xCF, 0xC3, 0xEF, 0x77, 0x00, 0xE7, 0x21, 0xBB, 0x68, 0x54, 0x46, 0xD8, 0xE2, 0x34, 0x13, 0xAD,
        0x6C, 0x55, 0x49, 0xDA, 0xDE, 0x38, 0x21, 0xAA, 0x70, 0x54, 0x70, 0xD9, 0xDC, 0x2E, 0x18, 0x86, 0x66, 0x41, 0x49, 0x95, 0xDF, 0x30, 0x0D, 0xD9};
    thread_local EncryptedName<sizeof(menuDisabledSource), 0xD97D5DBDBB2F2709ULL> menuDisabledName(menuDisabledSource);
    const std::string menuDisabled(menuDisabledName.Text());
    const std::uint8_t menuPressedSource[] = {
        0xAF, 0xD2, 0x3B, 0xD9, 0xB1, 0xC9, 0x4A, 0x00, 0xA9, 0x99, 0x3B, 0xEF, 0xCC, 0xBD, 0xD3, 0xAF, 0x0E, 0x0E, 0x92, 0x40, 0x2E, 0x6E, 0xE1, 0xB9,
        0x0A, 0x0F, 0x9D, 0x42, 0x12, 0x62, 0xD3, 0xBE, 0x16, 0x0E, 0xA4, 0x41, 0x10, 0x74, 0xEA, 0x92, 0x00, 0x13, 0xD5, 0x41, 0x1C, 0x77, 0x8F};
    thread_local EncryptedName<sizeof(menuPressedSource), 0xCD8F077123FB7D6FULL> menuPressedName(menuPressedSource);
    const std::string menuPressed(menuPressedName.Text());
    const std::uint8_t menuHoverSource[] = {
        0x83, 0x86, 0x93, 0xDD, 0x15, 0x71, 0x8C, 0x54, 0x85, 0xCD, 0x93, 0xEB, 0x68, 0x05, 0x15, 0xFB, 0x22, 0x5A, 0x3A, 0x44, 0x8A, 0xD6, 0x27, 0xED,
        0x26, 0x5B, 0x35, 0x46, 0xB6, 0xDA, 0x15, 0xEA, 0x3A, 0x5A, 0x0C, 0x45, 0xB4, 0xCC, 0x2C, 0xC6, 0x2C, 0x47, 0x7D, 0x45, 0xB8, 0xCF, 0x49};
    thread_local EncryptedName<sizeof(menuHoverSource), 0x9949BFD527532943ULL> menuHoverName(menuHoverSource);
    const std::string menuHover(menuHoverName.Text());
    const std::uint8_t menuNormalSource[] = {
        0xF7, 0xE8, 0x6D, 0xC3, 0x7F, 0x2B, 0x6C, 0xDA, 0xF1, 0xA3, 0x6D, 0xF5, 0x02, 0x5F, 0xF5, 0x75, 0x56, 0x34, 0xC4, 0x5A, 0xE0, 0x8C, 0xC7, 0x63,
        0x52, 0x35, 0xCB, 0x58, 0xDC, 0x80, 0xF5, 0x64, 0x4E, 0x34, 0xF2, 0x5B, 0xDE, 0x96, 0xCC, 0x48, 0x58, 0x21, 0xCB, 0x17, 0xDD, 0x88, 0xD9, 0x17};
    thread_local EncryptedName<sizeof(menuNormalSource), 0x17A9E5BF39AD4737ULL> menuNormalName(menuNormalSource);
    const std::string menuNormal(menuNormalName.Text());
    menuButton->SetImages(menuNormal, menuHover, menuPressed, menuDisabled);
  }
  this->m_titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton(*this->m_context);
  closeButton->SetSize(11, 11);
  closeButton->Move(217, 3);
  closeButton->SetOnClick([this]() { this->m_frameContext->windows.CloseWindow(10005); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    const std::uint8_t closeDisabledSource[] = {
        0x21, 0xD4, 0x39, 0x2D, 0x15, 0xDD, 0xFC, 0x10, 0x27, 0x9F, 0x39, 0x1B, 0x68, 0xA9, 0x65, 0xBF, 0x80, 0x08, 0x90, 0xB4, 0x8A, 0x7A, 0x57, 0xA9,
        0x84, 0x09, 0x9F, 0xB6, 0xB6, 0x76, 0x65, 0xAE, 0x98, 0x08, 0xA6, 0xB4, 0xB9, 0x7C, 0x4A, 0xB8, 0xBE, 0x14, 0x9F, 0xB1, 0xFB, 0x71, 0x54, 0xAD,
        0xE1};
    thread_local EncryptedName<sizeof(closeDisabledSource), 0xDD3913D5D7F97BE1ULL> closeDisabledName(closeDisabledSource);
    const std::string closeDisabled(closeDisabledName.Text());
    const std::uint8_t closePressedSource[] = {
        0x5F, 0x6A, 0xAD, 0x51, 0x17, 0xE7, 0x38, 0x68, 0x59, 0x21, 0xAD, 0x67, 0x6A, 0x93, 0xA1, 0xC7, 0xFE, 0xB6, 0x04, 0xC8, 0x88, 0x40, 0x93, 0xD1,
        0xFA, 0xB7, 0x0B, 0xCA, 0xB4, 0x4C, 0xA1, 0xD6, 0xE6, 0xB6, 0x32, 0xC8, 0xBB, 0x46, 0x8E, 0xC0, 0xC0, 0xAA, 0x03, 0x85, 0xB5, 0x44, 0x8D, 0xA5};
    thread_local EncryptedName<sizeof(closePressedSource), 0xA5FD29D7AB6DC59FULL> closePressedName(closePressedSource);
    const std::string closePressed(closePressedName.Text());
    const std::uint8_t closeHoverSource[] = {
        0x31, 0x68, 0xF9, 0xF3, 0xD7, 0xB7, 0x8A, 0x3C, 0x37, 0x23, 0xF9, 0xC5, 0xAA, 0xC3, 0x13, 0x93, 0x90, 0xB4, 0x50, 0x6A, 0x48, 0x10, 0x21, 0x85,
        0x94, 0xB5, 0x5F, 0x68, 0x74, 0x1C, 0x13, 0x82, 0x88, 0xB4, 0x66, 0x6A, 0x7B, 0x16, 0x3C, 0x94, 0xAE, 0xA8, 0x57, 0x27, 0x75, 0x14, 0x3F, 0xF1};
    thread_local EncryptedName<sizeof(closeHoverSource), 0xF14F79170939C7F1ULL> closeHoverName(closeHoverSource);
    const std::string closeHover(closeHoverName.Text());
    const std::uint8_t closeNormalSource[] = {
        0xBF, 0xBC, 0x1F, 0x2B, 0x45, 0xA9, 0xBC, 0x20, 0xB9, 0xF7, 0x1F, 0x1D, 0x38, 0xDD, 0x25, 0x8F, 0x1E, 0x60, 0xB6, 0xB2, 0xDA, 0x0E, 0x17, 0x99,
        0x1A, 0x61, 0xB9, 0xB0, 0xE6, 0x02, 0x25, 0x9E, 0x06, 0x60, 0x80, 0xB2, 0xE9, 0x08, 0x0A, 0x88, 0x20, 0x7C, 0xB9, 0xB7, 0xAB, 0x05, 0x14, 0x9D,
        0x7F};
    thread_local EncryptedName<sizeof(closeNormalSource), 0xED796785D1DF137FULL> closeNormalName(closeNormalSource);
    const std::string closeNormal(closeNormalName.Text());
    closeButton->SetImages(closeNormal, closeHover, closePressed, closeDisabled);
  }
  this->m_titleBar->SetSecondButton(closeButton);

  this->m_messageEdit = new CUIEditBox(*this->m_context);
  this->m_messageEdit->SetSize(215, 58);
  this->m_messageEdit->Move(8, 80);
  {
    CUIEditBox* edit = this->m_messageEdit;
    const std::uint8_t placeholderSource[] = {
        0x26, 0x92, 0xB3, 0x74, 0x47, 0xB6, 0xBE, 0xBA, 0x70, 0x97, 0xB8, 0xB9, 0xA7, 0xD1, 0xB2, 0xC7, 0x5C, 0x98, 0xBF, 0x25, 0xD8, 0xE1, 0x2B, 0x05};
    thread_local EncryptedName<sizeof(placeholderSource), 0x050505679903579BULL> placeholderName(placeholderSource);
    edit->SetText(std::string(placeholderName.Text()));
  }
  this->m_messageEdit->SetTextColor(0x808080);
  this->m_messageEdit->SetBackgroundColor(0xFFF3EE);
  this->m_messageEdit->SetBorderColor(0xD6C5BD);
  this->m_messageEdit->SetBorderWidth(1);
  this->m_messageEdit->SetMaximum(100);
  this->m_messageEdit->SetEditBounds(0, 5, 0, 0);
  {
    const std::function<void()> onSubmit = [this]() { this->OnEditSubmit(); };
    this->m_messageEdit->onSubmit = onSubmit;
  }
  this->AddChild(this->m_messageEdit);

  this->unknown_00E4 = new CUIStaticText(*this->m_context);
  this->unknown_00E4->SetSize(165, 12);
  this->unknown_00E4->Move(28, 25);
  {
    CUIStaticText* text = this->unknown_00E4;
    const std::uint8_t firstTypeSource[] = {
        0x07, 0x6F, 0x54, 0xCA, 0x9B, 0x78, 0x2E, 0x34, 0x73, 0x0C, 0x3A, 0x80, 0x1E, 0x9F, 0x2F, 0x44, 0x00, 0x74, 0xCD, 0x8A, 0x7E, 0x0F, 0x02, 0x83};
    thread_local EncryptedName<sizeof(firstTypeSource), 0x83EFBFBB37EDBDBDULL> firstTypeName(firstTypeSource);
    const char* value = firstTypeName.Text();
    text->SetText(value, std::strlen(value));
  }
  this->unknown_00E4->SetFitText(0);
  this->unknown_00E4->SetAlign(0, 0);
  {
    CUIStaticText* text = this->unknown_00E4;
    const std::uint8_t firstTypeFontSource[] = {
        0xE0, 0x80, 0x13, 0x50, 0x5C, 0x4B};
    thread_local EncryptedName<sizeof(firstTypeFontSource), 0xBF2B4B31397FF5A7ULL> firstTypeFontName(firstTypeFontSource);
    text->SetFont(std::string(firstTypeFontName.Text()));
  }
  this->unknown_00E4->SetFontHeight(12);
  this->unknown_00E4->SetBold(0);
  this->unknown_00E4->SetColor(0);
  this->unknown_00E4->SetShadow(0, 0xFFFFFF);
  this->unknown_00E4->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00E4);

  this->unknown_00E8 = new CUIStaticText(*this->m_context);
  this->unknown_00E8->SetSize(165, 12);
  this->unknown_00E8->Move(28, 43);
  {
    CUIStaticText* text = this->unknown_00E8;
    const std::uint8_t secondTypeSource[] = {
        0x2E, 0x07, 0x50, 0x0A, 0xF1, 0x2A, 0x3E, 0x0F, 0x2A, 0x32, 0x5C, 0x67, 0x14, 0x48, 0xC9, 0x91, 0x50, 0x4E, 0x0A, 0xDD};
    thread_local EncryptedName<sizeof(secondTypeSource), 0x2FE993D1DDE1F197ULL> secondTypeName(secondTypeSource);
    const char* value = secondTypeName.Text();
    text->SetText(value, std::strlen(value));
  }
  this->unknown_00E8->SetFitText(0);
  this->unknown_00E8->SetAlign(0, 0);
  {
    CUIStaticText* text = this->unknown_00E8;
    const std::uint8_t secondTypeFontSource[] = {
        0xC8, 0xEC, 0x87, 0x64, 0xD8, 0x7F};
    thread_local EncryptedName<sizeof(secondTypeFontSource), 0x39FF7FB50DEB998FULL> secondTypeFontName(secondTypeFontSource);
    text->SetFont(std::string(secondTypeFontName.Text()));
  }
  this->unknown_00E8->SetFontHeight(12);
  this->unknown_00E8->SetBold(0);
  this->unknown_00E8->SetColor(0);
  this->unknown_00E8->SetShadow(0, 0xFFFFFF);
  this->unknown_00E8->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00E8);

  this->unknown_00EC = new CUIImageButton(*this->m_context);
  this->unknown_00EC->SetSize(58, 20);
  this->unknown_00EC->Move(117, 165);
  this->unknown_00EC->SetOnClick([this]() { this->m_frameContext->windows.CloseWindow(10005); });
  this->unknown_00EC->disabled = 0;
  this->unknown_00EC->Invalidate();
  {
    CUIImageButton* button = this->unknown_00EC;
    const std::uint8_t cancelDisabledSource[] = {
        0xB5, 0x62, 0x2D, 0xA7, 0xAB, 0xAB, 0x20, 0xF8, 0xB3, 0x29, 0x2D, 0x91, 0xD6, 0xDF, 0xB9, 0x57, 0x01, 0xA3, 0xB2, 0x3E, 0x0A, 0x0B, 0x86, 0x50,
        0x19, 0xFF, 0xB2, 0x39, 0x02, 0x16, 0x84, 0x57, 0x19, 0xA8, 0xC3, 0x3F, 0x06, 0x15, 0xE5};
    thread_local EncryptedName<sizeof(cancelDisabledSource), 0x35E5656B5DEDCD75ULL> cancelDisabledName(cancelDisabledSource);
    const std::string cancelDisabled(cancelDisabledName.Text());
    const std::uint8_t cancelHoverSource[] = {
        0x3D, 0x16, 0x7B, 0xDB, 0x5F, 0x33, 0x84, 0x84, 0x3B, 0x5D, 0x7B, 0xED, 0x22, 0x47, 0x1D, 0x2B, 0x89, 0xD7, 0xE4, 0x42, 0xFE, 0x93, 0x22, 0x2C,
        0x91, 0x8B, 0xE4, 0x4E, 0xE9, 0x98, 0x33, 0x67, 0x9F, 0xD4, 0xCB, 0x21};
    thread_local EncryptedName<sizeof(cancelHoverSource), 0x4941FD9F21BBB9FDULL> cancelHoverName(cancelHoverSource);
    const std::string cancelHover(cancelHoverName.Text());
    const std::uint8_t cancelPressedSource[] = {
        0x0D, 0x5E, 0x59, 0x4F, 0x4D, 0x95, 0xC2, 0x60, 0x0B, 0x15, 0x59, 0x79, 0x30, 0xE1, 0x5B, 0xCF, 0xB9, 0x9F, 0xC6, 0xD6, 0xEC, 0x35, 0x64, 0xC8,
        0xA1, 0xC3, 0xC6, 0xC5, 0xFF, 0x3E, 0x74, 0xDE, 0xE3, 0x93, 0xF4, 0xC5, 0x8D};
    thread_local EncryptedName<sizeof(cancelPressedSource), 0xAD075B8DB599F1CDULL> cancelPressedName(cancelPressedSource);
    const std::string cancelPressed(cancelPressedName.Text());
    const std::uint8_t cancelNormalSource[] = {
        0xBF, 0xBA, 0x4D, 0xEB, 0xF7, 0xC1, 0x4E, 0xE2, 0xB9, 0xF1, 0x4D, 0xDD, 0x8A, 0xB5, 0xD7, 0x4D, 0x0B, 0x7B, 0xD2, 0x72, 0x56, 0x61, 0xE8, 0x4A,
        0x13, 0x27, 0xD2, 0x7F, 0x58, 0x7D, 0xE6, 0x4E, 0x13, 0x3B, 0xEF, 0x7C, 0x47, 0x0F};
    thread_local EncryptedName<sizeof(cancelNormalSource), 0x2F8B0F37118D157FULL> cancelNormalName(cancelNormalSource);
    const std::string cancelNormal(cancelNormalName.Text());
    button->SetImages(cancelNormal, cancelPressed, cancelHover, cancelDisabled);
  }
  this->AddChild(this->unknown_00EC);

  this->unknown_00F0 = new CUIImageButton(*this->m_context);
  this->unknown_00F0->SetSize(58, 20);
  this->unknown_00F0->Move(55, 165);
  this->unknown_00F0->SetOnClick([this]() { this->SendReport(); });
  this->unknown_00F0->disabled = 0;
  this->unknown_00F0->Invalidate();
  {
    CUIImageButton* button = this->unknown_00F0;
    const std::uint8_t reportDisabledSource[] = {
        0xA3, 0x5A, 0x21, 0x85, 0xFB, 0xEB, 0xD8, 0x06, 0xA5, 0x11, 0x21, 0xB3, 0x86, 0x9F, 0x41, 0xA9, 0x17, 0x9B, 0xBE, 0x0D, 0x5E, 0x55, 0x72, 0xB9,
        0x17, 0xAA, 0x85, 0x16, 0x48, 0x44, 0x7F, 0xA7, 0x06, 0xDB, 0x83, 0x12, 0x4B, 0x25};
    thread_local EncryptedName<sizeof(reportDisabledSource), 0xCB1D253B7FE1F563ULL> reportDisabledName(reportDisabledSource);
    const std::string reportDisabled(reportDisabledName.Text());
    const std::uint8_t reportHoverSource[] = {
        0x09, 0xBC, 0x39, 0x89, 0x39, 0x17, 0xD0, 0xBC, 0x0F, 0xF7, 0x39, 0xBF, 0x44, 0x63, 0x49, 0x13, 0xBD, 0x7D, 0xA6, 0x01, 0x9C, 0xA9, 0x7A, 0x03,
        0xBD, 0x4C, 0x96, 0x05, 0x9C, 0xAB, 0x3B, 0x13, 0xA4, 0x63, 0xF9};
    thread_local EncryptedName<sizeof(reportHoverSource), 0x7115D9F973F913C9ULL> reportHoverName(reportHoverSource);
    const std::string reportHover(reportHoverName.Text());
    const std::uint8_t reportPressedSource[] = {
        0x3D, 0x78, 0x8D, 0x53, 0x75, 0x41, 0x52, 0x2A, 0x3B, 0x33, 0x8D, 0x65, 0x08, 0x35, 0xCB, 0x85, 0x89, 0xB9, 0x12, 0xDB, 0xD0, 0xFF, 0xF8, 0x95,
        0x89, 0x88, 0x3D, 0xDB, 0xD0, 0xFC, 0xE4, 0xC9, 0x9F, 0xBA, 0x3D, 0xA9};
    thread_local EncryptedName<sizeof(reportPressedSource), 0xE7978FB5A94DD7FDULL> reportPressedName(reportPressedSource);
    const std::string reportPressed(reportPressedName.Text());
    const std::uint8_t reportNormalSource[] = {
        0xAB, 0x16, 0x07, 0xF5, 0xA7, 0x41, 0x38, 0x7A, 0xAD, 0x5D, 0x07, 0xC3, 0xDA, 0x35, 0xA1, 0xD5, 0x1F, 0xD7, 0x98, 0x7D, 0x02, 0xFF, 0x92, 0xC5,
        0x1F, 0xE6, 0xA9, 0x60, 0x15, 0xE2, 0x9C, 0xDB, 0x45, 0xDB, 0xAA, 0x7F, 0x67};
    thread_local EncryptedName<sizeof(reportNormalSource), 0xB7FD8F670FC7B96BULL> reportNormalName(reportNormalSource);
    const std::string reportNormal(reportNormalName.Text());
    button->SetImages(reportNormal, reportPressed, reportHover, reportDisabled);
  }
  this->AddChild(this->unknown_00F0);

  this->m_firstTypeButton = new CUIRadioButton(*this->m_context);
  this->m_firstTypeButton->SetSize(12, 12);
  this->m_firstTypeButton->Move(9, 25);
  this->m_firstTypeButton->SetOnSelect([this](CUIRadioButton* button) { this->OnReportTypeSelect(button); });
  this->m_firstTypeButton->disabled = 0;
  this->m_firstTypeButton->Invalidate();
  {
    CUIRadioButton* button = this->m_firstTypeButton;
    const std::uint8_t firstRadioDisabledSource[] = {
        0xBF, 0x7A, 0x6F, 0xA1, 0xAB, 0x7F, 0xD0, 0xC2, 0xB9, 0x31, 0x6F, 0x97, 0xD6, 0x0B, 0x49, 0x7D, 0x1E, 0xB1, 0xC6, 0x34, 0x09, 0xC5, 0x7B, 0x50,
        0x10, 0xB3, 0xC9, 0x75, 0x09, 0xDC, 0x65, 0x0F};
    thread_local EncryptedName<sizeof(firstRadioDisabledSource), 0x0F15B16B5BAFD57FULL> firstRadioDisabledName(firstRadioDisabledSource);
    const std::string firstRadioDisabled(firstRadioDisabledName.Text());
    const std::uint8_t firstRadioHoverSource[] = {
        0xE3, 0x5E, 0x17, 0xED, 0x17, 0x87, 0x26, 0xB0, 0xE5, 0x15, 0x17, 0xDB, 0x6A, 0xF3, 0xBF, 0x0F, 0x42, 0x95, 0xBE, 0x78, 0xB5, 0x3D, 0x8D, 0x22,
        0x4C, 0x97, 0xB1, 0x39, 0xB5, 0x24, 0x93, 0x7D};
    thread_local EncryptedName<sizeof(firstRadioHoverSource), 0x7DE349D717D7F123ULL> firstRadioHoverName(firstRadioHoverSource);
    const std::string firstRadioHover(firstRadioHoverName.Text());
    const std::uint8_t firstRadioPressedSource[] = {
        0xDD, 0xAA, 0x53, 0xD3, 0xB1, 0xED, 0x92, 0x28, 0xDB, 0xE1, 0x53, 0xE5, 0xCC, 0x99, 0x0B, 0x97, 0x7C, 0x61, 0xFA, 0x46, 0x13, 0x57, 0x39, 0xBA,
        0x72, 0x6B, 0xBD, 0x4B, 0x1C, 0x53, 0x57};
    thread_local EncryptedName<sizeof(firstRadioPressedSource), 0xE55723712993051DULL> firstRadioPressedName(firstRadioPressedSource);
    const std::string firstRadioPressed(firstRadioPressedName.Text());
    const std::uint8_t firstRadioNormalSource[] = {
        0x8F, 0xC8, 0x2D, 0x7F, 0xC9, 0xF9, 0xFA, 0xB4, 0x89, 0x83, 0x2D, 0x49, 0xB4, 0x8D, 0x63, 0x0B, 0x2E, 0x03, 0x84, 0xEA, 0x6B, 0x43, 0x51, 0x26,
        0x20, 0x01, 0x8B, 0xAB, 0x6B, 0x5A, 0x4F, 0x79};
    thread_local EncryptedName<sizeof(firstRadioNormalSource), 0x793F370985ED674FULL> firstRadioNormalName(firstRadioNormalSource);
    const std::string firstRadioNormal(firstRadioNormalName.Text());
    button->SetImages(firstRadioNormal, firstRadioPressed, firstRadioHover, firstRadioDisabled);
  }
  this->m_firstTypeButton->checked = true;
  this->m_firstTypeButton->Invalidate();
  this->AddChild(this->m_firstTypeButton);

  this->m_secondTypeButton = new CUIRadioButton(*this->m_context);
  this->m_secondTypeButton->SetSize(12, 12);
  this->m_secondTypeButton->Move(9, 43);
  this->m_secondTypeButton->SetOnSelect([this](CUIRadioButton* button) { this->OnReportTypeSelect(button); });
  this->m_secondTypeButton->disabled = 0;
  this->m_secondTypeButton->Invalidate();
  {
    CUIRadioButton* button = this->m_secondTypeButton;
    const std::uint8_t secondRadioDisabledSource[] = {
        0xAD, 0xB4, 0x67, 0xEB, 0xB7, 0xA1, 0x0E, 0x3E, 0xAB, 0xFF, 0x67, 0xDD, 0xCA, 0xD5, 0x97, 0x81, 0x0C, 0x7F, 0xCE, 0x7E, 0x15, 0x1B, 0xA5, 0xAC,
        0x02, 0x7D, 0xC1, 0x3F, 0x15, 0x02, 0xBB, 0xF3};
    thread_local EncryptedName<sizeof(secondRadioDisabledSource), 0xF3CB6F7711A71B6DULL> secondRadioDisabledName(secondRadioDisabledSource);
    const std::string secondRadioDisabled(secondRadioDisabledName.Text());
    const std::uint8_t secondRadioHoverSource[] = {
        0x73, 0xC8, 0x99, 0x75, 0x1F, 0xE9, 0x5E, 0x4E, 0x75, 0x83, 0x99, 0x43, 0x62, 0x9D, 0xC7, 0xF1, 0xD2, 0x03, 0x30, 0xE0, 0xBD, 0x53, 0xF5, 0xDC,
        0xDC, 0x01, 0x3F, 0xA1, 0xBD, 0x4A, 0xEB, 0x83};
    thread_local EncryptedName<sizeof(secondRadioHoverSource), 0x839B27DF8F5967B3ULL> secondRadioHoverName(secondRadioHoverSource);
    const std::string secondRadioHover(secondRadioHoverName.Text());
    const std::uint8_t secondRadioPressedSource[] = {
        0x21, 0x36, 0x47, 0xB5, 0x41, 0x0D, 0xE4, 0xBE, 0x27, 0x7D, 0x47, 0x83, 0x3C, 0x79, 0x7D, 0x01, 0x80, 0xFD, 0xEE, 0x20, 0xE3, 0xB7, 0x4F, 0x2C,
        0x8E, 0xF7, 0xA9, 0x2D, 0xEC, 0xB3, 0x21};
    thread_local EncryptedName<sizeof(secondRadioPressedSource), 0x7321C3814F8799E1ULL> secondRadioPressedName(secondRadioPressedSource);
    const std::string secondRadioPressed(secondRadioPressedName.Text());
    const std::uint8_t secondRadioNormalSource[] = {
        0x8F, 0x0E, 0x0F, 0xCF, 0xA3, 0x19, 0x84, 0xB4, 0x89, 0x45, 0x0F, 0xF9, 0xDE, 0x6D, 0x1D, 0x0B, 0x2E, 0xC5, 0xA6, 0x5A, 0x01, 0xA3, 0x2F, 0x26,
        0x20, 0xC7, 0xA9, 0x1B, 0x01, 0xBA, 0x31, 0x79};
    thread_local EncryptedName<sizeof(secondRadioNormalSource), 0x7941D76335CFA14FULL> secondRadioNormalName(secondRadioNormalSource);
    const std::string secondRadioNormal(secondRadioNormalName.Text());
    button->SetImages(secondRadioNormal, secondRadioPressed, secondRadioHover, secondRadioDisabled);
  }
  this->m_secondTypeButton->checked = false;
  this->m_secondTypeButton->Invalidate();
  this->AddChild(this->m_secondTypeButton);

  this->unknown_00FC = new CUIStaticText(*this->m_context);
  this->unknown_00FC->SetSize(210, 13);
  this->unknown_00FC->Move(9, 142);
  {
    CUIStaticText* text = this->unknown_00FC;
    const std::uint8_t warningSource[] = {
        0xD4, 0xB7, 0x7D, 0xB6, 0x2D, 0x51, 0xE8, 0xFA, 0x8E, 0x75, 0x74, 0xBC, 0x2D, 0x2E, 0x89, 0x85, 0x82, 0xED, 0x6F, 0x5F, 0xB4, 0x31, 0xEF, 0xFE,
        0x45, 0xE9, 0x3F, 0x5F, 0xCD, 0x39, 0x92, 0x85, 0xD1, 0x9A, 0x7D, 0xA6, 0x23, 0xEF};
    thread_local EncryptedName<sizeof(warningSource), 0x452FEF0D7FC95565ULL> warningName(warningSource);
    const char* value = warningName.Text();
    text->SetText(value, std::strlen(value));
  }
  this->unknown_00FC->SetFitText(0);
  this->unknown_00FC->SetAlign(0, 0);
  {
    CUIStaticText* text = this->unknown_00FC;
    const std::uint8_t warningFontSource[] = {
        0x1A, 0x78, 0x25, 0xE0, 0xCC, 0x7D};
    thread_local EncryptedName<sizeof(warningFontSource), 0x9FED7DA189490D5DULL> warningFontName(warningFontSource);
    text->SetFont(std::string(warningFontName.Text()));
  }
  this->unknown_00FC->SetFontHeight(12);
  this->unknown_00FC->SetBold(0);
  this->unknown_00FC->SetColor(0x2D35F7);
  this->unknown_00FC->SetShadow(0, 0xFFFFFF);
  this->unknown_00FC->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->unknown_00FC);
}

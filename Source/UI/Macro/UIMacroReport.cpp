#include "UI/Macro/UIMacroReport.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
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
#include "UI/Window/UIRegisterMgr.h"
#include "Utility/Cipher/LocalEncryptedName.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>

namespace {

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

}

// INLINED
inline std::string EditBoxText(CUIEditBox* editBox) {
  return editBox->edit->Text();
}

#pragma pack(push, 1)
struct ReportUserPacket {
  /* 0000 */ short id;
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
    : m_background(nullptr), m_titleBar(nullptr), m_messageEdit(nullptr), firstTypeText(nullptr), secondTypeText(nullptr), cancelButton(nullptr),
      reportButton(nullptr), m_firstTypeButton(nullptr), m_secondTypeButton(nullptr), warningText(nullptr), targetId(0), m_reportType(0) {
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
  packet.targetId = this->targetId;
  packet.accountId = g_session.m_aid;
  packet.type = static_cast<std::uint16_t>(this->m_reportType);
  strncpy_s(packet.targetName, sizeof(packet.targetName), this->targetName.c_str(), _TRUNCATE);
  strncpy_s(packet.message, sizeof(packet.message), EditBoxText(this->m_messageEdit).c_str(), _TRUNCATE);
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
  g_windowMgr.CloseWindow(WID_MACROREPORT);
}

// OFFSET: 0x00A47C30
// FUNCTION: fn_16100
void CUIMacroReport::OnEditSubmit() {
}

// OFFSET: 0x00A47C40
// FUNCTION: fn_16101
void CUIMacroReport::OnInit() {
  this->m_resolveSkin = true;
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);

  if (g_modeMgr.GetGameMode()) {
    this->targetId = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->menuTargetAid;
    this->targetName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->menuTargetName;
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
void CUIMacroReport::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->m_resolveSkin = false;
  this->Resize(0xE7, 0xBD);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
}

// OFFSET: 0x00A44170
// FUNCTION: fn_16051
CUIFrameWnd* CreateMacroReport() {
  return new CUIMacroReport();
}

// OFFSET: 0x004955B0
// FUNCTION: fn_45550
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateMacroReport;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10005, factory);
  return true;
}();

// OFFSET: 0x00A44240
// FUNCTION: fn_16053
void CUIMacroReport::OnCreate(int width, int height) {
  this->m_background = new CUIStaticImage();
  this->m_background->SetSize(231, 172);
  this->m_background->Move(0, 17);
  {
    CUIStaticImage* background = this->m_background;
    const char backgroundImageSource[] = {
        '\x19', '\x56', '\x21', '\x19', '\xAD', '\xDB', '\x68', '\xE6', '\x1F', '\x1D', '\x21', '\x2F', '\xD0', '\xAF', '\xF1', '\x66', '\xB8', '\x9A', '\x93', '\x8C', '\x3F', '\x70', '\xDD', '\x44',
        '\xAB', '\x8D', '\xBD', '\x81', '\x0A', '\x4A', '\xC0', '\x4A', '\xBA', '\x8B', '\x8E', '\x91', '\x08', '\x65', '\xC2', '\x59', '\xAD', '\xD7', '\x83', '\x8E', '\x1D', '\x15'};
    const std::string backgroundImage(DecryptName<0x2BAD156DE3E1F9D9ULL>(backgroundImageSource));
    background->SetImage(backgroundImage);
  }
  this->AddChild(this->m_background);

  this->m_titleBar = new CUITitleBar();
  this->m_titleBar->SetSize(231, 17);
  this->m_titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->m_titleBar;
    const char titleRightSource[] = {
        '\x65', '\x32', '\xAD', '\x03', '\xA3', '\xE9', '\xB2', '\xCE', '\x63', '\x79', '\xAD', '\x35', '\xDE', '\x9D', '\x2B', '\x61', '\xC4', '\xEE', '\x04', '\x9A', '\x3C', '\x4E', '\x19', '\x77',
        '\xC0', '\xEF', '\x0B', '\x98', '\x00', '\x42', '\x2B', '\x77', '\xCC', '\xE9', '\x01', '\x9C', '\x01', '\x46', '\x05', '\x5C', '\xD7', '\xF4', '\x0A', '\x91', '\x17', '\x09', '\x15', '\x6E',
        '\xD5', '\x9D'};
    const std::string titleRight(DecryptName<0x03772763F96D9DA5ULL>(titleRightSource));
    const char titleMiddleSource[] = {
        '\xAD', '\x14', '\x55', '\xD7', '\x53', '\x35', '\xD2', '\x08', '\xAB', '\x5F', '\x55', '\xE1', '\x2E', '\x41', '\x4B', '\xA7', '\x0C', '\xC8', '\xFC', '\x4E', '\xCC', '\x92', '\x79', '\xB1',
        '\x08', '\xC9', '\xF3', '\x4C', '\xF0', '\x9E', '\x4B', '\xB1', '\x04', '\xCF', '\xF9', '\x48', '\xF1', '\x9A', '\x65', '\x9A', '\x00', '\xD2', '\xF1', '\x03', '\xF1', '\x96', '\x67', '\xC5'};
    const std::string titleMiddle(DecryptName<0xC517FB932D95BB6DULL>(titleMiddleSource));
    const char titleLeftSource[] = {
        '\x09', '\x5A', '\x9F', '\x71', '\x4F', '\x1B', '\x7E', '\xEE', '\x0F', '\x11', '\x9F', '\x47', '\x32', '\x6F', '\xE7', '\x41', '\xA8', '\x86', '\x36', '\xE8', '\xD0', '\xBC', '\xD5', '\x57',
        '\xAC', '\x87', '\x39', '\xEA', '\xEC', '\xB0', '\xE7', '\x57', '\xA0', '\x81', '\x33', '\xEE', '\xED', '\xB4', '\xC9', '\x7C', '\xA5', '\x90', '\x39', '\xFF', '\xA1', '\xB7', '\xD6', '\x53',
        '\xC9'};
    const std::string titleLeft(DecryptName<0x23BBD58F8B5FF5C9ULL>(titleLeftSource));
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->m_titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  {
    const char titleTextSource[] = {
        '\x47', '\xBC', '\xAB', '\x85', '\x19', '\xB4', '\xB4', '\xBB', '\x6A', '\x13'};
    title->SetText(DecryptName<0x0B7109397F6B1387ULL>(titleTextSource));
  }
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    const char titleFontSource[] = {
        '\x20', '\xDE', '\xFD', '\xCC', '\xCC', '\x3F'};
    title->SetFont(std::string(DecryptName<0x13E33FA1A591AB67ULL>(titleFontSource)));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(1, 0xFFFFFF);
  CUITitleBar* titleBar = this->m_titleBar;
  titleBar->titleText = title;
  titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton();
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  {
    const char menuDisabledSource[] = {
        '\xC9', '\x88', '\xEF', '\x41', '\x7D', '\x93', '\xB8', '\x14', '\xCF', '\xC3', '\xEF', '\x77', '\x00', '\xE7', '\x21', '\xBB', '\x68', '\x54', '\x46', '\xD8', '\xE2', '\x34', '\x13', '\xAD',
        '\x6C', '\x55', '\x49', '\xDA', '\xDE', '\x38', '\x21', '\xAA', '\x70', '\x54', '\x70', '\xD9', '\xDC', '\x2E', '\x18', '\x86', '\x66', '\x41', '\x49', '\x95', '\xDF', '\x30', '\x0D', '\xD9'};
    const std::string menuDisabled(DecryptName<0xD97D5DBDBB2F2709ULL>(menuDisabledSource));
    const char menuPressedSource[] = {
        '\xAF', '\xD2', '\x3B', '\xD9', '\xB1', '\xC9', '\x4A', '\x00', '\xA9', '\x99', '\x3B', '\xEF', '\xCC', '\xBD', '\xD3', '\xAF', '\x0E', '\x0E', '\x92', '\x40', '\x2E', '\x6E', '\xE1', '\xB9',
        '\x0A', '\x0F', '\x9D', '\x42', '\x12', '\x62', '\xD3', '\xBE', '\x16', '\x0E', '\xA4', '\x41', '\x10', '\x74', '\xEA', '\x92', '\x00', '\x13', '\xD5', '\x41', '\x1C', '\x77', '\x8F'};
    const std::string menuPressed(DecryptName<0xCD8F077123FB7D6FULL>(menuPressedSource));
    const char menuHoverSource[] = {
        '\x83', '\x86', '\x93', '\xDD', '\x15', '\x71', '\x8C', '\x54', '\x85', '\xCD', '\x93', '\xEB', '\x68', '\x05', '\x15', '\xFB', '\x22', '\x5A', '\x3A', '\x44', '\x8A', '\xD6', '\x27', '\xED',
        '\x26', '\x5B', '\x35', '\x46', '\xB6', '\xDA', '\x15', '\xEA', '\x3A', '\x5A', '\x0C', '\x45', '\xB4', '\xCC', '\x2C', '\xC6', '\x2C', '\x47', '\x7D', '\x45', '\xB8', '\xCF', '\x49'};
    const std::string menuHover(DecryptName<0x9949BFD527532943ULL>(menuHoverSource));
    const char menuNormalSource[] = {
        '\xF7', '\xE8', '\x6D', '\xC3', '\x7F', '\x2B', '\x6C', '\xDA', '\xF1', '\xA3', '\x6D', '\xF5', '\x02', '\x5F', '\xF5', '\x75', '\x56', '\x34', '\xC4', '\x5A', '\xE0', '\x8C', '\xC7', '\x63',
        '\x52', '\x35', '\xCB', '\x58', '\xDC', '\x80', '\xF5', '\x64', '\x4E', '\x34', '\xF2', '\x5B', '\xDE', '\x96', '\xCC', '\x48', '\x58', '\x21', '\xCB', '\x17', '\xDD', '\x88', '\xD9', '\x17'};
    const std::string menuNormal(DecryptName<0x17A9E5BF39AD4737ULL>(menuNormalSource));
    menuButton->SetImages(menuNormal, menuHover, menuPressed, menuDisabled);
  }
  this->m_titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(217, 3);
  closeButton->SetOnClick([this]() { g_windowMgr.CloseWindow(WID_MACROREPORT); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    const char closeDisabledSource[] = {
        '\x21', '\xD4', '\x39', '\x2D', '\x15', '\xDD', '\xFC', '\x10', '\x27', '\x9F', '\x39', '\x1B', '\x68', '\xA9', '\x65', '\xBF', '\x80', '\x08', '\x90', '\xB4', '\x8A', '\x7A', '\x57', '\xA9',
        '\x84', '\x09', '\x9F', '\xB6', '\xB6', '\x76', '\x65', '\xAE', '\x98', '\x08', '\xA6', '\xB4', '\xB9', '\x7C', '\x4A', '\xB8', '\xBE', '\x14', '\x9F', '\xB1', '\xFB', '\x71', '\x54', '\xAD',
        '\xE1'};
    const std::string closeDisabled(DecryptName<0xDD3913D5D7F97BE1ULL>(closeDisabledSource));
    const char closePressedSource[] = {
        '\x5F', '\x6A', '\xAD', '\x51', '\x17', '\xE7', '\x38', '\x68', '\x59', '\x21', '\xAD', '\x67', '\x6A', '\x93', '\xA1', '\xC7', '\xFE', '\xB6', '\x04', '\xC8', '\x88', '\x40', '\x93', '\xD1',
        '\xFA', '\xB7', '\x0B', '\xCA', '\xB4', '\x4C', '\xA1', '\xD6', '\xE6', '\xB6', '\x32', '\xC8', '\xBB', '\x46', '\x8E', '\xC0', '\xC0', '\xAA', '\x03', '\x85', '\xB5', '\x44', '\x8D', '\xA5'};
    const std::string closePressed(DecryptName<0xA5FD29D7AB6DC59FULL>(closePressedSource));
    const char closeHoverSource[] = {
        '\x31', '\x68', '\xF9', '\xF3', '\xD7', '\xB7', '\x8A', '\x3C', '\x37', '\x23', '\xF9', '\xC5', '\xAA', '\xC3', '\x13', '\x93', '\x90', '\xB4', '\x50', '\x6A', '\x48', '\x10', '\x21', '\x85',
        '\x94', '\xB5', '\x5F', '\x68', '\x74', '\x1C', '\x13', '\x82', '\x88', '\xB4', '\x66', '\x6A', '\x7B', '\x16', '\x3C', '\x94', '\xAE', '\xA8', '\x57', '\x27', '\x75', '\x14', '\x3F', '\xF1'};
    const std::string closeHover(DecryptName<0xF14F79170939C7F1ULL>(closeHoverSource));
    const char closeNormalSource[] = {
        '\xBF', '\xBC', '\x1F', '\x2B', '\x45', '\xA9', '\xBC', '\x20', '\xB9', '\xF7', '\x1F', '\x1D', '\x38', '\xDD', '\x25', '\x8F', '\x1E', '\x60', '\xB6', '\xB2', '\xDA', '\x0E', '\x17', '\x99',
        '\x1A', '\x61', '\xB9', '\xB0', '\xE6', '\x02', '\x25', '\x9E', '\x06', '\x60', '\x80', '\xB2', '\xE9', '\x08', '\x0A', '\x88', '\x20', '\x7C', '\xB9', '\xB7', '\xAB', '\x05', '\x14', '\x9D',
        '\x7F'};
    const std::string closeNormal(DecryptName<0xED796785D1DF137FULL>(closeNormalSource));
    closeButton->SetImages(closeNormal, closeHover, closePressed, closeDisabled);
  }
  this->m_titleBar->SetSecondButton(closeButton);

  this->m_messageEdit = new CUIEditBox();
  this->m_messageEdit->SetSize(215, 58);
  this->m_messageEdit->Move(8, 80);
  {
    CUIEditBox* edit = this->m_messageEdit;
    const char placeholderSource[] = {
        '\x26', '\x92', '\xB3', '\x74', '\x47', '\xB6', '\xBE', '\xBA', '\x70', '\x97', '\xB8', '\xB9', '\xA7', '\xD1', '\xB2', '\xC7', '\x5C', '\x98', '\xBF', '\x25', '\xD8', '\xE1', '\x2B', '\x05'};
    edit->SetText(std::string(DecryptName<0x050505679903579BULL>(placeholderSource)));
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

  this->firstTypeText = new CUIStaticText();
  this->firstTypeText->SetSize(165, 12);
  this->firstTypeText->Move(28, 25);
  {
    CUIStaticText* text = this->firstTypeText;
    const char firstTypeSource[] = {
        '\x07', '\x6F', '\x54', '\xCA', '\x9B', '\x78', '\x2E', '\x34', '\x73', '\x0C', '\x3A', '\x80', '\x1E', '\x9F', '\x2F', '\x44', '\x00', '\x74', '\xCD', '\x8A', '\x7E', '\x0F', '\x02', '\x83'};
    text->SetText(DecryptName<0x83EFBFBB37EDBDBDULL>(firstTypeSource));
  }
  this->firstTypeText->SetFitText(0);
  this->firstTypeText->SetAlign(0, 0);
  {
    CUIStaticText* text = this->firstTypeText;
    const char firstTypeFontSource[] = {
        '\xE0', '\x80', '\x13', '\x50', '\x5C', '\x4B'};
    text->SetFont(std::string(DecryptName<0xBF2B4B31397FF5A7ULL>(firstTypeFontSource)));
  }
  this->firstTypeText->SetFontHeight(12);
  this->firstTypeText->SetBold(0);
  this->firstTypeText->SetColor(0);
  this->firstTypeText->SetShadow(0, 0xFFFFFF);
  this->firstTypeText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->firstTypeText);

  this->secondTypeText = new CUIStaticText();
  this->secondTypeText->SetSize(165, 12);
  this->secondTypeText->Move(28, 43);
  {
    CUIStaticText* text = this->secondTypeText;
    const char secondTypeSource[] = {
        '\x2E', '\x07', '\x50', '\x0A', '\xF1', '\x2A', '\x3E', '\x0F', '\x2A', '\x32', '\x5C', '\x67', '\x14', '\x48', '\xC9', '\x91', '\x50', '\x4E', '\x0A', '\xDD'};
    text->SetText(DecryptName<0x2FE993D1DDE1F197ULL>(secondTypeSource));
  }
  this->secondTypeText->SetFitText(0);
  this->secondTypeText->SetAlign(0, 0);
  {
    CUIStaticText* text = this->secondTypeText;
    const char secondTypeFontSource[] = {
        '\xC8', '\xEC', '\x87', '\x64', '\xD8', '\x7F'};
    text->SetFont(std::string(DecryptName<0x39FF7FB50DEB998FULL>(secondTypeFontSource)));
  }
  this->secondTypeText->SetFontHeight(12);
  this->secondTypeText->SetBold(0);
  this->secondTypeText->SetColor(0);
  this->secondTypeText->SetShadow(0, 0xFFFFFF);
  this->secondTypeText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->secondTypeText);

  this->cancelButton = new CUIImageButton();
  this->cancelButton->SetSize(58, 20);
  this->cancelButton->Move(117, 165);
  this->cancelButton->SetOnClick([this]() { g_windowMgr.CloseWindow(WID_MACROREPORT); });
  this->cancelButton->disabled = 0;
  this->cancelButton->Invalidate();
  {
    CUIImageButton* button = this->cancelButton;
    const char cancelDisabledSource[] = {
        '\xB5', '\x62', '\x2D', '\xA7', '\xAB', '\xAB', '\x20', '\xF8', '\xB3', '\x29', '\x2D', '\x91', '\xD6', '\xDF', '\xB9', '\x57', '\x01', '\xA3', '\xB2', '\x3E', '\x0A', '\x0B', '\x86', '\x50',
        '\x19', '\xFF', '\xB2', '\x39', '\x02', '\x16', '\x84', '\x57', '\x19', '\xA8', '\xC3', '\x3F', '\x06', '\x15', '\xE5'};
    const std::string cancelDisabled(DecryptName<0x35E5656B5DEDCD75ULL>(cancelDisabledSource));
    const char cancelHoverSource[] = {
        '\x3D', '\x16', '\x7B', '\xDB', '\x5F', '\x33', '\x84', '\x84', '\x3B', '\x5D', '\x7B', '\xED', '\x22', '\x47', '\x1D', '\x2B', '\x89', '\xD7', '\xE4', '\x42', '\xFE', '\x93', '\x22', '\x2C',
        '\x91', '\x8B', '\xE4', '\x4E', '\xE9', '\x98', '\x33', '\x67', '\x9F', '\xD4', '\xCB', '\x21'};
    const std::string cancelHover(DecryptName<0x4941FD9F21BBB9FDULL>(cancelHoverSource));
    const char cancelPressedSource[] = {
        '\x0D', '\x5E', '\x59', '\x4F', '\x4D', '\x95', '\xC2', '\x60', '\x0B', '\x15', '\x59', '\x79', '\x30', '\xE1', '\x5B', '\xCF', '\xB9', '\x9F', '\xC6', '\xD6', '\xEC', '\x35', '\x64', '\xC8',
        '\xA1', '\xC3', '\xC6', '\xC5', '\xFF', '\x3E', '\x74', '\xDE', '\xE3', '\x93', '\xF4', '\xC5', '\x8D'};
    const std::string cancelPressed(DecryptName<0xAD075B8DB599F1CDULL>(cancelPressedSource));
    const char cancelNormalSource[] = {
        '\xBF', '\xBA', '\x4D', '\xEB', '\xF7', '\xC1', '\x4E', '\xE2', '\xB9', '\xF1', '\x4D', '\xDD', '\x8A', '\xB5', '\xD7', '\x4D', '\x0B', '\x7B', '\xD2', '\x72', '\x56', '\x61', '\xE8', '\x4A',
        '\x13', '\x27', '\xD2', '\x7F', '\x58', '\x7D', '\xE6', '\x4E', '\x13', '\x3B', '\xEF', '\x7C', '\x47', '\x0F'};
    const std::string cancelNormal(DecryptName<0x2F8B0F37118D157FULL>(cancelNormalSource));
    button->SetImages(cancelNormal, cancelPressed, cancelHover, cancelDisabled);
  }
  this->AddChild(this->cancelButton);

  this->reportButton = new CUIImageButton();
  this->reportButton->SetSize(58, 20);
  this->reportButton->Move(55, 165);
  this->reportButton->SetOnClick([this]() { this->SendReport(); });
  this->reportButton->disabled = 0;
  this->reportButton->Invalidate();
  {
    CUIImageButton* button = this->reportButton;
    const char reportDisabledSource[] = {
        '\xA3', '\x5A', '\x21', '\x85', '\xFB', '\xEB', '\xD8', '\x06', '\xA5', '\x11', '\x21', '\xB3', '\x86', '\x9F', '\x41', '\xA9', '\x17', '\x9B', '\xBE', '\x0D', '\x5E', '\x55', '\x72', '\xB9',
        '\x17', '\xAA', '\x85', '\x16', '\x48', '\x44', '\x7F', '\xA7', '\x06', '\xDB', '\x83', '\x12', '\x4B', '\x25'};
    const std::string reportDisabled(DecryptName<0xCB1D253B7FE1F563ULL>(reportDisabledSource));
    const char reportHoverSource[] = {
        '\x09', '\xBC', '\x39', '\x89', '\x39', '\x17', '\xD0', '\xBC', '\x0F', '\xF7', '\x39', '\xBF', '\x44', '\x63', '\x49', '\x13', '\xBD', '\x7D', '\xA6', '\x01', '\x9C', '\xA9', '\x7A', '\x03',
        '\xBD', '\x4C', '\x96', '\x05', '\x9C', '\xAB', '\x3B', '\x13', '\xA4', '\x63', '\xF9'};
    const std::string reportHover(DecryptName<0x7115D9F973F913C9ULL>(reportHoverSource));
    const char reportPressedSource[] = {
        '\x3D', '\x78', '\x8D', '\x53', '\x75', '\x41', '\x52', '\x2A', '\x3B', '\x33', '\x8D', '\x65', '\x08', '\x35', '\xCB', '\x85', '\x89', '\xB9', '\x12', '\xDB', '\xD0', '\xFF', '\xF8', '\x95',
        '\x89', '\x88', '\x3D', '\xDB', '\xD0', '\xFC', '\xE4', '\xC9', '\x9F', '\xBA', '\x3D', '\xA9'};
    const std::string reportPressed(DecryptName<0xE7978FB5A94DD7FDULL>(reportPressedSource));
    const char reportNormalSource[] = {
        '\xAB', '\x16', '\x07', '\xF5', '\xA7', '\x41', '\x38', '\x7A', '\xAD', '\x5D', '\x07', '\xC3', '\xDA', '\x35', '\xA1', '\xD5', '\x1F', '\xD7', '\x98', '\x7D', '\x02', '\xFF', '\x92', '\xC5',
        '\x1F', '\xE6', '\xA9', '\x60', '\x15', '\xE2', '\x9C', '\xDB', '\x45', '\xDB', '\xAA', '\x7F', '\x67'};
    const std::string reportNormal(DecryptName<0xB7FD8F670FC7B96BULL>(reportNormalSource));
    button->SetImages(reportNormal, reportPressed, reportHover, reportDisabled);
  }
  this->AddChild(this->reportButton);

  this->m_firstTypeButton = new CUIRadioButton();
  this->m_firstTypeButton->SetSize(12, 12);
  this->m_firstTypeButton->Move(9, 25);
  this->m_firstTypeButton->SetOnSelect([this](CUIRadioButton* button) { this->OnReportTypeSelect(button); });
  this->m_firstTypeButton->disabled = 0;
  this->m_firstTypeButton->Invalidate();
  {
    CUIRadioButton* button = this->m_firstTypeButton;
    const char firstRadioDisabledSource[] = {
        '\xBF', '\x7A', '\x6F', '\xA1', '\xAB', '\x7F', '\xD0', '\xC2', '\xB9', '\x31', '\x6F', '\x97', '\xD6', '\x0B', '\x49', '\x7D', '\x1E', '\xB1', '\xC6', '\x34', '\x09', '\xC5', '\x7B', '\x50',
        '\x10', '\xB3', '\xC9', '\x75', '\x09', '\xDC', '\x65', '\x0F'};
    const std::string firstRadioDisabled(DecryptName<0x0F15B16B5BAFD57FULL>(firstRadioDisabledSource));
    const char firstRadioHoverSource[] = {
        '\xE3', '\x5E', '\x17', '\xED', '\x17', '\x87', '\x26', '\xB0', '\xE5', '\x15', '\x17', '\xDB', '\x6A', '\xF3', '\xBF', '\x0F', '\x42', '\x95', '\xBE', '\x78', '\xB5', '\x3D', '\x8D', '\x22',
        '\x4C', '\x97', '\xB1', '\x39', '\xB5', '\x24', '\x93', '\x7D'};
    const std::string firstRadioHover(DecryptName<0x7DE349D717D7F123ULL>(firstRadioHoverSource));
    const char firstRadioPressedSource[] = {
        '\xDD', '\xAA', '\x53', '\xD3', '\xB1', '\xED', '\x92', '\x28', '\xDB', '\xE1', '\x53', '\xE5', '\xCC', '\x99', '\x0B', '\x97', '\x7C', '\x61', '\xFA', '\x46', '\x13', '\x57', '\x39', '\xBA',
        '\x72', '\x6B', '\xBD', '\x4B', '\x1C', '\x53', '\x57'};
    const std::string firstRadioPressed(DecryptName<0xE55723712993051DULL>(firstRadioPressedSource));
    const char firstRadioNormalSource[] = {
        '\x8F', '\xC8', '\x2D', '\x7F', '\xC9', '\xF9', '\xFA', '\xB4', '\x89', '\x83', '\x2D', '\x49', '\xB4', '\x8D', '\x63', '\x0B', '\x2E', '\x03', '\x84', '\xEA', '\x6B', '\x43', '\x51', '\x26',
        '\x20', '\x01', '\x8B', '\xAB', '\x6B', '\x5A', '\x4F', '\x79'};
    const std::string firstRadioNormal(DecryptName<0x793F370985ED674FULL>(firstRadioNormalSource));
    button->SetImages(firstRadioNormal, firstRadioPressed, firstRadioHover, firstRadioDisabled);
  }
  this->m_firstTypeButton->checked = true;
  this->m_firstTypeButton->Invalidate();
  this->AddChild(this->m_firstTypeButton);

  this->m_secondTypeButton = new CUIRadioButton();
  this->m_secondTypeButton->SetSize(12, 12);
  this->m_secondTypeButton->Move(9, 43);
  this->m_secondTypeButton->SetOnSelect([this](CUIRadioButton* button) { this->OnReportTypeSelect(button); });
  this->m_secondTypeButton->disabled = 0;
  this->m_secondTypeButton->Invalidate();
  {
    CUIRadioButton* button = this->m_secondTypeButton;
    const char secondRadioDisabledSource[] = {
        '\xAD', '\xB4', '\x67', '\xEB', '\xB7', '\xA1', '\x0E', '\x3E', '\xAB', '\xFF', '\x67', '\xDD', '\xCA', '\xD5', '\x97', '\x81', '\x0C', '\x7F', '\xCE', '\x7E', '\x15', '\x1B', '\xA5', '\xAC',
        '\x02', '\x7D', '\xC1', '\x3F', '\x15', '\x02', '\xBB', '\xF3'};
    const std::string secondRadioDisabled(DecryptName<0xF3CB6F7711A71B6DULL>(secondRadioDisabledSource));
    const char secondRadioHoverSource[] = {
        '\x73', '\xC8', '\x99', '\x75', '\x1F', '\xE9', '\x5E', '\x4E', '\x75', '\x83', '\x99', '\x43', '\x62', '\x9D', '\xC7', '\xF1', '\xD2', '\x03', '\x30', '\xE0', '\xBD', '\x53', '\xF5', '\xDC',
        '\xDC', '\x01', '\x3F', '\xA1', '\xBD', '\x4A', '\xEB', '\x83'};
    const std::string secondRadioHover(DecryptName<0x839B27DF8F5967B3ULL>(secondRadioHoverSource));
    const char secondRadioPressedSource[] = {
        '\x21', '\x36', '\x47', '\xB5', '\x41', '\x0D', '\xE4', '\xBE', '\x27', '\x7D', '\x47', '\x83', '\x3C', '\x79', '\x7D', '\x01', '\x80', '\xFD', '\xEE', '\x20', '\xE3', '\xB7', '\x4F', '\x2C',
        '\x8E', '\xF7', '\xA9', '\x2D', '\xEC', '\xB3', '\x21'};
    const std::string secondRadioPressed(DecryptName<0x7321C3814F8799E1ULL>(secondRadioPressedSource));
    const char secondRadioNormalSource[] = {
        '\x8F', '\x0E', '\x0F', '\xCF', '\xA3', '\x19', '\x84', '\xB4', '\x89', '\x45', '\x0F', '\xF9', '\xDE', '\x6D', '\x1D', '\x0B', '\x2E', '\xC5', '\xA6', '\x5A', '\x01', '\xA3', '\x2F', '\x26',
        '\x20', '\xC7', '\xA9', '\x1B', '\x01', '\xBA', '\x31', '\x79'};
    const std::string secondRadioNormal(DecryptName<0x7941D76335CFA14FULL>(secondRadioNormalSource));
    button->SetImages(secondRadioNormal, secondRadioPressed, secondRadioHover, secondRadioDisabled);
  }
  this->m_secondTypeButton->checked = false;
  this->m_secondTypeButton->Invalidate();
  this->AddChild(this->m_secondTypeButton);

  this->warningText = new CUIStaticText();
  this->warningText->SetSize(210, 13);
  this->warningText->Move(9, 142);
  {
    CUIStaticText* text = this->warningText;
    const char warningSource[] = {
        '\xD4', '\xB7', '\x7D', '\xB6', '\x2D', '\x51', '\xE8', '\xFA', '\x8E', '\x75', '\x74', '\xBC', '\x2D', '\x2E', '\x89', '\x85', '\x82', '\xED', '\x6F', '\x5F', '\xB4', '\x31', '\xEF', '\xFE',
        '\x45', '\xE9', '\x3F', '\x5F', '\xCD', '\x39', '\x92', '\x85', '\xD1', '\x9A', '\x7D', '\xA6', '\x23', '\xEF'};
    text->SetText(DecryptName<0x452FEF0D7FC95565ULL>(warningSource));
  }
  this->warningText->SetFitText(0);
  this->warningText->SetAlign(0, 0);
  {
    CUIStaticText* text = this->warningText;
    const char warningFontSource[] = {
        '\x1A', '\x78', '\x25', '\xE0', '\xCC', '\x7D'};
    const std::string warningFont(DecryptName<0x9FED7DA189490D5DULL>(warningFontSource));
    text->SetFont(warningFont);
  }
  this->warningText->SetFontHeight(12);
  this->warningText->SetBold(0);
  this->warningText->SetColor(0x2D35F7);
  this->warningText->SetShadow(0, 0xFFFFFF);
  this->warningText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->warningText);
}

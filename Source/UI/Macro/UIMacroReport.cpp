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
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>

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
    const std::string backgroundImage(OBFUSCATE_TEXT("유저인터페이스\\MacroReport\\bg_macroreport.bmp", 0x2BAD156DE3E1F9D9ULL));
    background->SetImage(backgroundImage);
  }
  this->AddChild(this->m_background);

  this->m_titleBar = new CUITitleBar();
  this->m_titleBar->SetSize(231, 17);
  this->m_titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->m_titleBar;
    const std::string titleRight(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0x03772763F96D9DA5ULL));
    const std::string titleMiddle(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0xC517FB932D95BB6DULL));
    const std::string titleLeft(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_left.bmp", 0x23BBD58F8B5FF5C9ULL));
    titleBar->SetImages(titleLeft, titleMiddle, titleRight);
  }
  this->AddChild(this->m_titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  {
    title->SetText(OBFUSCATE_TEXT("유저 신고", 0x0B7109397F6B1387ULL));
  }
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    title->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x13E33FA1A591AB67ULL)));
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
    const std::string menuDisabled(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0xD97D5DBDBB2F2709ULL));
    const std::string menuPressed(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0xCD8F077123FB7D6FULL));
    const std::string menuHover(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x9949BFD527532943ULL));
    const std::string menuNormal(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x17A9E5BF39AD4737ULL));
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
    const std::string closeDisabled(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xDD3913D5D7F97BE1ULL));
    const std::string closePressed(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0xA5FD29D7AB6DC59FULL));
    const std::string closeHover(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0xF14F79170939C7F1ULL));
    const std::string closeNormal(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xED796785D1DF137FULL));
    closeButton->SetImages(closeNormal, closeHover, closePressed, closeDisabled);
  }
  this->m_titleBar->SetSecondButton(closeButton);

  this->m_messageEdit = new CUIEditBox();
  this->m_messageEdit->SetSize(215, 58);
  this->m_messageEdit->Move(8, 80);
  {
    CUIEditBox* edit = this->m_messageEdit;
    edit->SetText(std::string(OBFUSCATE_TEXT("신고 내용을 입력하세요.", 0x050505679903579BULL)));
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
    text->SetText(OBFUSCATE_TEXT("불법 프로그램 의심 신고", 0x83EFBFBB37EDBDBDULL));
  }
  this->firstTypeText->SetFitText(0);
  this->firstTypeText->SetAlign(0, 0);
  {
    CUIStaticText* text = this->firstTypeText;
    text->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0xBF2B4B31397FF5A7ULL)));
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
    text->SetText(OBFUSCATE_TEXT("버그 및 시스템 악용", 0x2FE993D1DDE1F197ULL));
  }
  this->secondTypeText->SetFitText(0);
  this->secondTypeText->SetAlign(0, 0);
  {
    CUIStaticText* text = this->secondTypeText;
    text->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x39FF7FB50DEB998FULL)));
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
    const std::string cancelDisabled(OBFUSCATE_TEXT("유저인터페이스\\btn_cancel2_disable.bmp", 0x35E5656B5DEDCD75ULL));
    const std::string cancelHover(OBFUSCATE_TEXT("유저인터페이스\\btn_cancel2_over.bmp", 0x4941FD9F21BBB9FDULL));
    const std::string cancelPressed(OBFUSCATE_TEXT("유저인터페이스\\btn_cancel2_press.bmp", 0xAD075B8DB599F1CDULL));
    const std::string cancelNormal(OBFUSCATE_TEXT("유저인터페이스\\btn_cancel2_normal.bmp", 0x2F8B0F37118D157FULL));
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
    const std::string reportDisabled(OBFUSCATE_TEXT("유저인터페이스\\btn_report_disable.bmp", 0xCB1D253B7FE1F563ULL));
    const std::string reportHover(OBFUSCATE_TEXT("유저인터페이스\\btn_report_over.bmp", 0x7115D9F973F913C9ULL));
    const std::string reportPressed(OBFUSCATE_TEXT("유저인터페이스\\btn_report_press.bmp", 0xE7978FB5A94DD7FDULL));
    const std::string reportNormal(OBFUSCATE_TEXT("유저인터페이스\\btn_report_normal.bmp", 0xB7FD8F670FC7B96BULL));
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
    const std::string firstRadioDisabled(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x0F15B16B5BAFD57FULL));
    const std::string firstRadioHover(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x7DE349D717D7F123ULL));
    const std::string firstRadioPressed(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0xE55723712993051DULL));
    const std::string firstRadioNormal(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x793F370985ED674FULL));
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
    const std::string secondRadioDisabled(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0xF3CB6F7711A71B6DULL));
    const std::string secondRadioHover(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x839B27DF8F5967B3ULL));
    const std::string secondRadioPressed(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_on.bmp", 0x7321C3814F8799E1ULL));
    const std::string secondRadioNormal(OBFUSCATE_TEXT("유저인터페이스\\radiobtn_off.bmp", 0x7941D76335CFA14FULL));
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
    text->SetText(OBFUSCATE_TEXT("기능 악용 시 제재를 받을 수 있습니다.", 0x452FEF0D7FC95565ULL));
  }
  this->warningText->SetFitText(0);
  this->warningText->SetAlign(0, 0);
  {
    CUIStaticText* text = this->warningText;
    const std::string warningFont(OBFUSCATE_TEXT("Gulim", 0x9FED7DA189490D5DULL));
    text->SetFont(warningFont);
  }
  this->warningText->SetFontHeight(12);
  this->warningText->SetBold(0);
  this->warningText->SetColor(0x2D35F7);
  this->warningText->SetShadow(0, 0xFFFFFF);
  this->warningText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->warningText);
}

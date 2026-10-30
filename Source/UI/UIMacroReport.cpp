#include "UI/UIMacroReport.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Renderer.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

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

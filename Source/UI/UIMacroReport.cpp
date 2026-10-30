#include "UI/UIMacroReport.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"

// OFFSET: 0x00A47A30
// FUNCTION: fn_16098
void CUIMacroReport::OnButtonClick() {
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

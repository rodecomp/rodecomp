#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Client/Registry.h"

#include <windows.h>

// OFFSET: 0x007D4520
// FUNCTION: fn_09578
CharSlotInfo::CharSlotInfo() = default;

// OFFSET: 0x007D4A00
// FUNCTION: fn_09583
CharSlotInfo::~CharSlotInfo() = default;

// OFFSET: 0x007D5000
// FUNCTION: fn_09586
void Nullsub243() {
}

// OFFSET: 0x007D5130

// OFFSET: 0x007D9090
// FUNCTION: fn_09595
void UINewSelectCharWnd::DisableButton(int id) {
  switch (id) {
  case 416:
    this->m_button416->Enable(1);
    break;
  case 417:
    this->m_button417->Enable(1);
    break;
  }
}

// OFFSET: 0x007D97B0
// FUNCTION: fn_09599
void UINewSelectCharWnd::SaveCurrentSlot() {
  HKEY key;
  DWORD disposition = 0;
  if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key, &disposition) == ERROR_SUCCESS) {
    DWORD slot = this->m_slotsPerPage * this->m_page + this->m_pageSlot;
    RegSetValueExA(key, "CURSLOT", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&slot), sizeof(slot));
    RegCloseKey(key);
  }
}

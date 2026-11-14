#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Network/Packets/NoticeEnums.h"
#include "Client/Session/Session.h"

#include "Contents/Tables/MessageTable.h"
#include "Contents/Sound/SoundMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Script/LuaScript.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cstring>
#include <string>

struct EntryQueueAdmissionPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint8_t result;
  /* 0003 */ char queueName[24];
  /* 001B */ char characterName[24];
};

struct EntryQueueApplyPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t type;
  /* 0004 */ char queueName[24];
};

struct EntryQueueNamePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char queueName[24];
};

// GLOBAL: gl_24868
CEntryQueueMgr* g_entryQueueMgr;
// GLOBAL: gl_24869
std::map<unsigned int, EntryQueueInfo*> g_entryQueueList;

CEntryQueueMgr::~CEntryQueueMgr() {
  for (std::map<unsigned int, EntryQueueInfo*>::iterator it = g_entryQueueList.begin(); it != g_entryQueueList.end(); ++it) {
    if (it->second) {
      delete it->second;
    }
  }
}

// OFFSET: 0x00BA1F80
// FUNCTION: fn_19660
void CEntryQueueMgr::SendAdmissionReply(int result) {
  if (!this->current) {
    if (result == 2) {
      this->ready = 0;
    }
    return;
  }

  EntryQueueAdmissionPacket packet;
  packet.id = ID_CZ_REPLY_LOBBY_ADMISSION;
  strcpy_s(packet.queueName, sizeof(packet.queueName), this->current->name);
  strcpy_s(packet.characterName, sizeof(packet.characterName), this->characterName.text);
  packet.result = static_cast<std::uint8_t>(result);
  if (result == 2) {
    this->ready = 0;
  }

  GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
}

// OFFSET: 0x00BA2030
// FUNCTION: fn_19661
void CEntryQueueMgr::SendApply(std::uint16_t type, const char* name) {
  EntryQueueApplyPacket packet;
  packet.id = ID_CZ_REQ_ENTRY_QUEUE_APPLY;
  packet.type = type;
  strncpy_s(packet.queueName, sizeof(packet.queueName), name, 23);
  GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
}

// OFFSET: 0x00BA20A0
// FUNCTION: fn_19662
void CEntryQueueMgr::SendCancel() {
  if (this->current) {
    EntryQueueNamePacket packet;
    packet.id = ID_CZ_REQ_ENTRY_QUEUE_CANCEL;
    strcpy_s(packet.queueName, sizeof(packet.queueName), this->current->name);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
  }
}

// OFFSET: 0x00BA2110
// FUNCTION: fn_19663
void CEntryQueueMgr::SendRankingRequest() {
  if (this->current) {
    EntryQueueNamePacket packet;
    packet.id = ID_CZ_REQ_ENTRY_QUEUE_RANKING;
    strcpy_s(packet.queueName, sizeof(packet.queueName), this->current->name);
    GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.id), &packet);
  }
}

// OFFSET: 0x00BA2180
// FUNCTION: fn_19664
void DestroyEntryQueueMgr() {
  if (g_entryQueueMgr) {
    delete g_entryQueueMgr;
    g_entryQueueMgr = nullptr;
  }
}

// OFFSET: 0x00BA2250
// FUNCTION: fn_19665
void CEntryQueueMgr::SetQueueName(const char* name) {
  std::memcpy(this->queueName.text, name, sizeof(this->queueName.text));
}

// OFFSET: 0x00BA2270
// FUNCTION: fn_19666
void CEntryQueueMgr::SetCharacterName(const char* name) {
  std::memcpy(this->characterName.text, name, sizeof(this->characterName.text));
}

// OFFSET: 0x00BA2310
// FUNCTION: fn_19667
void CEntryQueueMgr::OnApplyResult(const EntryQueueApplyResult* packet) {
  switch (packet->result) {
  case BG_APPLY_ACCEPT: {
    if (!g_windowMgr.CloseWindow(WID_ENTRYQUEUESTANDBYWND)) {
      g_windowMgr.EnsureWindow(WID_ENTRYQUEUESTANDBYWND);
    }

    strcpy_s(this->queueName.text, sizeof(this->queueName.text), packet->queueName);
    g_soundMgr->PlayWave("se_btg_request.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);

    std::string text("[");
    if (this->current) {
      text += this->current->displayName;
    }
    text += "]";
    text += LookupMessage(MSI_BATTLEFIELD_MSG_UNKNOWN_TYPE);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x4763FF, 0, 0, 0);
    this->ready = 1;
    return;
  }
  case BG_APPLY_QUEUE_FINISHED:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_MAXOVER), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_INVALID_NAME:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_JOIN_NOTLEVEL), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_INVALID_APP:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_JOIN_OVERLAP), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_PLAYER_COUNT:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_RESTART), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_PLAYER_LEVEL:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_NOTJOB), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_DUPLICATE:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_JOIN_ONLYBOSS), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_RECONNECT:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_BUSY_PARTYMEMBER), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_PLAYER_CLASS:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_CANCEL_JOINWAIT), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case BG_APPLY_PARTYGUILD_LEADER:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_WRONG_NAME), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case 15:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_NOTRANK), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  }
}

// OFFSET: 0x00BA2660
// FUNCTION: fn_19668
void CEntryQueueMgr::OnCancelResult(const EntryQueueNotice* notice) {
  switch (notice->result) {
  case 1:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_FAIL_CHOICE), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    g_windowMgr.CloseWindow(WID_ENTRYQUEUESTANDBYWND);
    this->ready = 0;
    break;
  case 3:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_CANCEL_JOIN), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case 11:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_DOYOU_JOIN), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case 14:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_HELP1), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    g_windowMgr.CloseWindow(WID_ENTRYQUEUESTANDBYWND);
    this->ready = 0;
    break;
  }

  std::string text("[");
  text += this->current->displayName;
  text += "]";
  text += LookupMessage(MSI_BATTLEFIELD_HELP2);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
  this->current = nullptr;
}

// OFFSET: 0x00BA28B0
// FUNCTION: fn_19669
void CEntryQueueMgr::ResetCurrent(int unused) {
  this->current = nullptr;
  this->ready = 0;
}

// OFFSET: 0x00BA28C0
// FUNCTION: fn_19670
void CEntryQueueMgr::IgnorePacket(const unsigned char* packet) {
}

// OFFSET: 0x00BA28D0
// FUNCTION: fn_19671
void CEntryQueueMgr::OnStatus(const EntryQueueStatus* packet) {
  this->ready = 1;
  UIWindow* window = g_windowMgr.LookupWindow(WID_ENTRYQUEUESTANDBYWND);
  if (!window && !g_windowMgr.CloseWindow(WID_ENTRYQUEUESTANDBYWND)) {
    window = g_windowMgr.EnsureWindow(WID_ENTRYQUEUESTANDBYWND);
  }

  if (!this->current) {
    for (auto it = g_entryQueueList.begin(); it != g_entryQueueList.end(); ++it) {
      if (std::string(it->second->name) == std::string(packet->queueName)) {
        this->current = it->second;
        break;
      }
    }
  }

  if (window && this->current) {
    static_cast<UIEntryQueueStandByWnd*>(window)->SetInfo(
      packet->queueName, this->current->maxLevel + this->current->minLevel, packet->ranking
    );
  }

  g_windowMgr.CloseWindow(WID_JOINTO_BATTLEFIELDWND);
}

// OFFSET: 0x00BA2C70
// FUNCTION: fn_19672
void CEntryQueueMgr::OnReady(const EntryQueueReady* packet) {
  strcpy_s(this->queueName.text, sizeof(this->queueName.text), packet->queueName);
  strcpy_s(this->characterName.text, sizeof(this->characterName.text), packet->characterName);
  if (g_windowMgr.CloseWindow(WID_ENTRYQUEUEREQUESTWND)) {
    return;
  }

  g_windowMgr.EnsureWindow(WID_ENTRYQUEUEREQUESTWND);
  g_soundMgr->PlayWave("se_btg_ready.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);

  std::string text("[");
  text += this->current->displayName;
  text += "]";
  text += LookupMessage(MSI_BATTLEFIELD_HELP3);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text.c_str()), 0x4763FF, 0, 0, 0);
}

// OFFSET: 0x00BA2E50
// FUNCTION: fn_19673
void CEntryQueueMgr::OnResultNotice(const EntryQueueNotice* notice) {
  switch (notice->result) {
  case 3:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_CANCEL_JOIN), 0, false, false, 0, "Message", 0x118, 0x78, 0);
    break;
  case 11:
    g_windowMgr.Confirm(LookupMessage(MSI_BATTLEFIELD_MSG_DOYOU_JOIN), 0, false, false, 0, "Message", 0x118, 0x78, 0);
  }
}

// OFFSET: 0x00BA2EB0
// FUNCTION: fn_19674
void CEntryQueueMgr::OnNotify(const EntryQueueNotice* notice) {
  switch (notice->result) {
  case 1:
    g_soundMgr->PlayWave("se_btg_forward.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
  }
}

// INLINED
CEntryQueueMgr::CEntryQueueMgr() : current(nullptr), ready(0) {
  std::memset(&this->queueName, 0, sizeof(this->queueName) + sizeof(this->characterName));
  g_session.m_hotkeyScript->Call("ReadEntryQueueList", "");
}

// OFFSET: 0x00BA1DF0
// FUNCTION: fn_19659
bool CreateEntryQueueMgr() {
  if (!g_entryQueueMgr) {
    g_entryQueueMgr = new (std::nothrow) CEntryQueueMgr();
    if (g_entryQueueMgr) {
      return true;
    }
  }

  return false;
}

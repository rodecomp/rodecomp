#include "Client/Mode/CGameMode.h"
#include "Client/Application/Launcher.h"
#include "Client/Session.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/ClanInfo.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/GuildMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/RuneSystem.h"
#include "Contents/MessageTable.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "UI/UIGradeEnchantWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <utility>
#include <direct.h>
#include <io.h>
#include <windows.h>

#pragma pack(push, 1)
struct ClanInfoPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int16_t length;
  /* 0004 */ int clanId;
  /* 0008 */ char clanName[24];
  /* 0020 */ char masterName[24];
  /* 0038 */ char mapName[16];
  /* 0048 */ unsigned char allyCount;
  /* 0049 */ unsigned char hostileCount;
  /* 004A */ char relationNames[1][24];
};

struct ActorClanNamePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ std::uint32_t aid;
  /* 0008 */ char name[1];
};

struct ClanMemberCountPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t connectedMembers;
  /* 0004 */ std::uint16_t totalMembers;
};

struct RuneSetGradeEntry {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ std::uint16_t grade;
  /* 0006 */ std::uint16_t gradeLevel;
};

struct RuneRewardStepEntry {
  /* 0000 */ std::uint32_t id;
  /* 0004 */ unsigned char step;
};

struct RuneListPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::uint16_t length;
  /* 0004 */ unsigned char unknown_0004;
  /* 0005 */ std::uint16_t tagId;
  /* 0007 */ std::uint16_t count;
};

struct RuneActivationPacket : RuneListPacket {
  /* 0009 */ std::uint32_t ids[1];
};

struct RuneSetGradesPacket : RuneListPacket {
  /* 0009 */ RuneSetGradeEntry entries[1];
};

struct RuneRewardStepsPacket : RuneListPacket {
  /* 0009 */ RuneRewardStepEntry entries[1];
};
struct PackageItemTestEntry {
  /* 0000 */ int itemId;
  /* 0004 */ std::uint16_t unknown_0004;
  /* 0006 */ int count;
};
#pragma pack(pop)

struct GameOptionEntry {
  /* 0000 */ int id;
  /* 0004 */ int value;
};

// GLOBAL: gl_24865
bool g_gameOptionFlag;

// GLOBAL: gl_24927
int g_pendingAid;

// GLOBAL: gl_24928
int g_pendingGid;

// GLOBAL: gl_24929
unsigned char g_toggleNoticeState;

// GLOBAL: gl_25094
bool g_playerMessage82Pending;

// OFFSET: 0x00D3B690
// FUNCTION: fn_21781
void CGameMode::ReceivePartyBookingRegisterResult(const unsigned char* packet) {
  switch (*reinterpret_cast<const std::int16_t*>(packet + 2)) {
  case 2:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1828), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1829), 0, true, false, 0, "Message", 280, 120, 0);
    [[fallthrough]];
  case 0:
  case 1:
    g_windowMgr.CloseWindow(164);
    break;
  }
}

// OFFSET: 0x00D3B7F0
// FUNCTION: fn_21783
void CGameMode::ReceivePartyBookingSearchResult(const unsigned char* packet) {
  const int size = *reinterpret_cast<const std::int16_t*>(packet + 2) - 5;
  const int count = size / sizeof(PARTY_BOOKING_AD_INFO);
  PARTY_BOOKING_AD_INFO* infos = new PARTY_BOOKING_AD_INFO[count];
  std::memcpy(infos, packet + 5, size);
  g_session.m_partyBooking.m_filterEnabled = true;
  UIWindow* window = g_windowMgr.LookupWindow(169);
  if (!window) {
    return;
  }

  if (size == 0) {
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(1827), 0, true, false, 0, "Message", 280, 120, 0);
    g_session.m_partyBooking.m_lastPage = false;
    window->Invalidate();
    return;
  }

  g_session.m_partyBooking.AddEntries(infos, count, packet[4]);
  window->Invalidate();
}

// OFFSET: 0x00D3B8D0
// FUNCTION: fn_21784
void CGameMode::ReceivePartyBookingDelete(const unsigned char* packet) {
  g_session.m_partyBooking.Remove(*reinterpret_cast<const int*>(packet + 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(169)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B910
// FUNCTION: fn_21785
void CGameMode::ReceivePartyBookingInsert(const unsigned char* packet) {
  g_session.m_partyBooking.OnAdvertisement(*reinterpret_cast<const PARTY_BOOKING_AD_INFO*>(packet + 2));
  if (UIWindow* window = g_windowMgr.LookupWindow(169)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B960
// FUNCTION: fn_21786
void CGameMode::ReceivePartyBookingUpdate(const unsigned char* packet) {
  g_session.m_partyBooking.SetJobs(*reinterpret_cast<const int*>(packet + 2), reinterpret_cast<const short*>(packet + 6));
  if (UIWindow* window = g_windowMgr.LookupWindow(169)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00D3B9A0
// FUNCTION: fn_21787
void CGameMode::ReceiveToggleNotice(const unsigned char* packet) {
  g_toggleNoticeState = packet[2];
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(g_toggleNoticeState == 0 ? 1326 : 1325)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D3BF00
// FUNCTION: fn_21791
void CGameMode::ReceivePendingResult(const unsigned char* packet) {
  std::string text;
  std::uint32_t color = 0xFF;
  switch (*reinterpret_cast<const std::int16_t*>(packet + 10)) {
  case 0:
  case 8:
    g_pendingAid = *reinterpret_cast<const int*>(packet + 2);
    g_pendingGid = *reinterpret_cast<const int*>(packet + 6);
    if (g_windowMgr.unknown_0250) {
      g_windowMgr.unknown_0250->Invalidate();
    }
    if (*reinterpret_cast<const std::int16_t*>(packet + 10) == 8) {
      text = LookupMessage(1814);
    } else {
      color = 0xFFFF00;
      text = LookupMessage(54);
    }
    break;
  case 1:
    text = LookupMessage(1227);
    break;
  case 2:
    text = LookupMessage(1228);
    break;
  case 3:
    text = LookupMessage(56);
    break;
  case 4:
    text = LookupMessage(1229);
    break;
  case 5:
    text = LookupMessage(1230);
    break;
  case 6:
    text = LookupMessage(1254);
    break;
  case 7:
    text = LookupMessage(1813);
    break;
  case 9:
    text = LookupMessage(1572);
    break;
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), color, 0, 0, 0);
  }
}

// OFFSET: 0x00D3C4F0

// OFFSET: 0x00D3CC10
// FUNCTION: fn_21796
void CGameMode::ReceivePetAct(unsigned char* packet) {
  CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 2));
  if (!actor) {
    return;
  }

  int* data = reinterpret_cast<int*>(packet + 6);
  const int kind = *data % 10;
  const int value = *data / 10;
  if (kind == 2) {
    const std::int64_t emotion = value;
    const std::int64_t basePack = g_cashEmotionMgr->BasePackId();
    actor->SendMsg(
      nullptr, 160, 0, static_cast<int>(basePack), static_cast<int>(basePack >> 32), static_cast<int>(emotion), static_cast<int>(emotion >> 32), 0, 0, 0,
      0, 0, 0, 0, 0
    );
    return;
  }

  const int talkId = value / 10;
  *data = talkId;
  char talk[256];
  std::memset(talk, 0, sizeof(talk));
  g_session.GetPetTalkText(talkId / 10, value % 10, talkId % 10, talk);
  if (talk[0]) {
    char speech[256];
    g_session.FormatMonsterSpeech(talk, speech, *reinterpret_cast<const std::uint32_t*>(packet + 2), 1 - kind);
    if (std::strcmp(speech, "") != 0) {
      const std::int64_t text = reinterpret_cast<std::intptr_t>(speech);
      actor->SendMsg(nullptr, 7, 0, static_cast<int>(text), static_cast<int>(text >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      if (!g_serviceOptions.Get(114) && !g_serviceOptions.Get(148)) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(speech), 0xFAFAFA, 0, 0, 0);
      }
    }
  }
}

// OFFSET: 0x00D3CEC0
// FUNCTION: fn_21798
void CGameMode::PlayBgm(unsigned char type, std::string name) {
  if (!g_serviceOptions.Get(7)) {
    return;
  }

  g_soundMgr->StopStreams();
  switch (type) {
  case 0:
    g_soundMgr->PlayStream(FormatOwnedString("bgm\\%s.mp3", name.c_str()).c_str(), 0, false);
    g_soundMgr->secondaryEnabled = 0;
    break;
  case 1:
    g_soundMgr->PlayStream(FormatOwnedString("bgm\\%s.mp3", name.c_str()).c_str(), 1, false);
    g_soundMgr->secondaryEnabled = 1;
    break;
  case 2:
    g_soundMgr->SetDefaultName(g_session.GetBgmFileName(std::string(this->mapName).c_str()));
    g_soundMgr->secondaryEnabled = 0;
    break;
  }
}

// OFFSET: 0x00D3D0F0
// FUNCTION: fn_21799
void CGameMode::ReceiveNpcBgm(const unsigned char* packet) {
  if (g_serviceOptions.Get(7)) {
    const std::string path = std::string("bgm\\") + reinterpret_cast<const char*>(packet + 2) + std::string(".mp3");
    g_soundMgr->PlayStream(path.c_str(), 1, true);
    g_soundMgr->secondaryEnabled = 1;
  }
}

// OFFSET: 0x00D3D2E0
// FUNCTION: fn_21800
void CGameMode::IgnoreRemovedPacket1(const unsigned char*) {
}

// OFFSET: 0x00D3D2F0
// FUNCTION: fn_21801
void CGameMode::ReceivePlayerMessage82(const unsigned char* packet) {
  CPlayer* player = this->scene->player;
  if (!player) {
    return;
  }

  player->SendMsg(nullptr, 0x52, 0, *reinterpret_cast<const int*>(packet + 6) * 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  g_playerMessage82Pending = true;
}

// OFFSET: 0x00D3DF40
// FUNCTION: fn_21807
void CGameMode::ReceiveResultNotice00FC(const unsigned char* packet) {
  this->unknown_00FC = 0;

  int messageId;
  switch (packet[2]) {
  case 0:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x43;
    break;
  case 1:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 7;
    break;
  case 2:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x44;
    break;
  case 4:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x37;
    break;
  case 5:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x1B0;
    break;
  case 6:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x1B1;
    break;
  case 7:
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    messageId = 0x1B2;
    break;
  default:
    return;
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(messageId)), 0xFF, 0, 0, 0);
}

// OFFSET: 0x00D3E0B0
// FUNCTION: fn_21808
void CGameMode::IgnoreRemovedPacket2(const unsigned char*) {
}

// OFFSET: 0x00D3E0C0
// FUNCTION: fn_21809
void CGameMode::ReceiveBillingInfo(const unsigned char* packet) {
  char text[512];
  std::memset(text, 0, sizeof(text));
  if (g_serviceType == 3) {
    switch (*reinterpret_cast<const int*>(packet + 2)) {
    case 1:
      std::sprintf(text, LookupMessage(1124), *reinterpret_cast<const int*>(packet + 6) / 86400 + 1);
      break;
    case 2: {
      const int seconds = *reinterpret_cast<const int*>(packet + 10);
      std::sprintf(text, LookupMessage(1125), seconds / 3600, seconds % 3600 / 60, seconds % 60);
      break;
    }
    }
  } else {
    __time32_t now;
    _time32(&now);
    _localtime32(&now);
    const __time32_t* expireTime = reinterpret_cast<const __time32_t*>(packet + 6);
    const tm* expire = _localtime32(expireTime);
    const int type = *reinterpret_cast<const int*>(packet + 2);
    if (type >= 0x8000 && type <= 0x80FF) {
      if (type & 0x20) {
        std::strcat(text, "(PC방-실버) ");
      }
      if (type & 0x40) {
        std::strcat(text, "(PC방-골드) ");
      }
      if (type & 0x80) {
        std::strcat(text, "(PC방-스페셜) ");
      }
      if (type & 0x01) {
        std::strcat(text, "프리미엄 정액 요금제");
        if (type & 0x10) {
          std::strcat(text, " + Advanced Package");
        } else {
          if (type & 0x02) {
            std::strcat(text, " + Exp UP");
          }
          if (type & 0x04) {
            std::strcat(text, " + Item UP");
          }
          if (type & 0x08) {
            std::strcat(text, " + Death Down");
          }

          char period[256];
          std::memset(period, 0, sizeof(period));
          const int remaining = *reinterpret_cast<const int*>(packet + 10);
          if (*expireTime > now) {
            if (remaining > 0) {
              std::sprintf(
                period, " (종료일시 : %04d/%02d/%02d , 남은시간 : %d 시간 %d 분 %d 초)", expire->tm_year + 1900, expire->tm_mon + 1,
                expire->tm_mday, remaining / 3600, remaining / 60 % 60, remaining % 60
              );
            } else if (remaining == 0) {
              std::sprintf(period, " (종료일시 : %04d/%02d/%02d )", expire->tm_year + 1900, expire->tm_mon + 1, expire->tm_mday);
            }
          } else if (remaining > 0) {
            std::sprintf(period, " (남은시간 : %d 시간 %d 분 %d 초)", remaining / 3600, remaining / 60 % 60, remaining % 60);
          }
          std::strcat(text, period);
        }
      }
    } else {
      switch (type) {
      case 100: {
        const int remaining = *reinterpret_cast<const int*>(packet + 10);
        if (*expireTime > now) {
          if (remaining > 0) {
            std::sprintf(
              text, "귀하는 혼합요금제 사용자 입니다. (종료일시 : %04d/%02d/%02d , 남은시간 : %d 시간 %d 분 %d 초)",
              expire->tm_year + 1900, expire->tm_mon + 1, expire->tm_mday, remaining / 3600, remaining / 60 % 60, remaining % 60
            );
          } else if (remaining == 0) {
            std::sprintf(
              text, "귀하는 정액요금제 사용자 입니다. (종료일시 : %04d 년 %02d 월 %02d 일 %02d 시 %02d 분 %02d 초)",
              expire->tm_year + 1900, expire->tm_mon + 1, expire->tm_mday, expire->tm_hour, expire->tm_min, expire->tm_sec
            );
          }
        } else if (remaining > 0) {
          std::sprintf(
            text, "귀하는 정량요금제 사용자 입니다. (남은시간 : %d 시간 %d 분 %d 초)", remaining / 3600, remaining / 60 % 60, remaining % 60
          );
        }
        break;
      }
      case 1000:
        std::sprintf(text, "PC방의 요금제 정보는 요청할 수 없습니다.");
        break;
      case 10000:
        std::sprintf(text, "귀하는 사크라이 2시간 무료요금제 사용자 입니다.");
        break;
      default:
        std::sprintf(text, "과금정보 요청에 실패 했습니다.");
        break;
      }
    }
  }

  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFFFF, 0, 0, 0);
}

struct ActorMessage32 {
  /* 0000 */ std::string text;
  /* 0018 */ std::string extra;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int unknown_0038;
  /* 003C */ int unknown_003C;
};

// OFFSET: 0x00D3E8B0
// FUNCTION: fn_21810
void CGameMode::ReceiveServerRateNotice(const unsigned char* packet) {
  g_session.unknown_54A0 = *reinterpret_cast<const int*>(packet + 2);
  g_session.unknown_54A4 = packet[6];
  g_session.unknown_54A8 = packet[7];
  if (g_windowMgr.unknown_030C) {
    reinterpret_cast<UIWindow*>(g_windowMgr.unknown_030C)->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }

  char text[128];
  std::memset(text, 0, sizeof(text));
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  switch (g_session.unknown_54A0) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(292), LookupMessage(287));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(292), LookupMessage(288));
    break;
  case 2:
    std::sprintf(text, "%s - %s : %s(%s)", LookupMessage(291), LookupMessage(292), LookupMessage(287), LookupMessage(1535));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }

  switch (g_session.unknown_54A4) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(293), LookupMessage(287));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(293), LookupMessage(290));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }

  switch (g_session.unknown_54A8) {
  case 0:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(738), LookupMessage(287));
    break;
  case 1:
    std::sprintf(text, "%s - %s : %s", LookupMessage(291), LookupMessage(738), LookupMessage(288));
    break;
  }
  if (std::strlen(text) != 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text), 0xFFFF, 12, 0, 0);
  }
}

// OFFSET: 0x00D3F130
// FUNCTION: fn_21811
void CGameMode::ReceiveActorMessage32(const unsigned char* packet) {
  this->unknown_01AC[*reinterpret_cast<const std::uint32_t*>(packet + 8)] = *reinterpret_cast<const std::uint32_t*>(packet + 4);
  CRenderObject* actor = this->scene->GetGameActorByAID(*reinterpret_cast<const std::uint32_t*>(packet + 4));
  if (!actor) {
    return;
  }

  ActorMessage32 message;
  message.text.assign(reinterpret_cast<const char*>(packet + 17), *reinterpret_cast<const std::int16_t*>(packet + 2) - 17);
  message.unknown_0030 = packet[16];
  message.unknown_0034 = *reinterpret_cast<const std::int16_t*>(packet + 14);
  message.unknown_0038 = *reinterpret_cast<const std::int16_t*>(packet + 12);
  message.unknown_003C = *reinterpret_cast<const int*>(packet + 8);
  if (message.text.size() > 60) {
    message.text.resize(60);
  }

  const std::int64_t argument = reinterpret_cast<std::intptr_t>(&message);
  actor->SendMsg(nullptr, 32, 0, static_cast<int>(argument), static_cast<int>(argument >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OFFSET: 0x00D3F3E0
// FUNCTION: fn_21812
void CGameMode::ReceiveRequestResult(const unsigned char* packet) {
  std::string text;
  switch (*reinterpret_cast<const std::int16_t*>(packet + 6)) {
  case 0:
  case 8: {
    UIWindow* window = g_windowMgr.LookupWindow(318);
    if (window) {
      window->SendMsg(nullptr, 122, *reinterpret_cast<const int*>(packet + 8), *reinterpret_cast<const int*>(packet + 12), 0, 0);
    }
    if (*reinterpret_cast<const std::int16_t*>(packet + 6) == 8) {
      text = LookupMessage(1814);
    }
    break;
  }
  case 9:
    text = LookupMessage(1944);
    break;
  case 10:
    text = LookupMessage(1945);
    break;
  case 11:
    text = LookupMessage(1946);
    break;
  case 12:
    text = LookupMessage(1947);
    break;
  case 303:
    text = "- " + IntToString(303, 0) + " : " + LookupMessage(3940);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 0, false, false, 0, "Error", 280, 120, 0);
    break;
  case 304:
    text = "- " + IntToString(304, 0) + " : " + LookupMessage(3941);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 0, false, false, 0, "Error", 280, 120, 0);
    break;
  default:
    text = LookupMessage(57);
    break;
  }

  if (!text.empty()) {
    g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFF, 0, 0, 0);
  }
}

// OFFSET: 0x00D3F760
// FUNCTION: fn_21813
void CGameMode::ReceiveStatusNotice(const unsigned char* packet) {
  int message;
  switch (*reinterpret_cast<const int*>(packet + 2)) {
  case 21:
    message = 889;
    break;
  case 22:
    message = 890;
    break;
  case 23:
    message = 891;
    break;
  case 24:
    message = 892;
    break;
  case 25:
    message = 893;
    break;
  case 28:
    message = 894;
    break;
  case 29:
    message = 895;
    break;
  case 30:
    message = 896;
    break;
  case 31:
    message = 897;
    break;
  case 32:
    message = 898;
    break;
  case 40:
    message = 915;
    break;
  default:
    return;
  }
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(message)), 0x9B9BFF, 8, 0, 0);
}

// OFFSET: 0x00D40080
// FUNCTION: fn_21816
void CGameMode::ReceiveWindow181Message(const unsigned char* packet) {
  UIWindow* window = g_windowMgr.LookupWindow(181);
  if (window) {
    window->SendMsg(nullptr, 60, 1, packet[2], 0, 0);
  }
}

// OFFSET: 0x00D40120
// FUNCTION: fn_21817
void CGameMode::ReceiveNamedResultNotice(const unsigned char* packet) {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  std::string name(reinterpret_cast<const char*>(packet + 2));
  switch (packet[26]) {
  case 0:
    switch (packet[27]) {
    case 0:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3389)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3390)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3391)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  case 1:
    switch (packet[27]) {
    case 0:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3392)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3393)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((name + LookupMessage(3394)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  }
}

// OFFSET: 0x00D40360
// FUNCTION: fn_21818
void CGameMode::ReceiveResultNotice0118(const unsigned char* packet) {
  g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
  switch (packet[2]) {
  case 0:
    switch (packet[3]) {
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(194)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(195)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  case 1:
    switch (packet[3]) {
    case 0:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(196)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 1:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(197)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>((this->unknown_0118 + LookupMessage(198)).c_str()), 0xFFFF, 0, 0, 0);
      break;
    }
    break;
  }
}

// OFFSET: 0x00D652E0
// FUNCTION: fn_21998
void CGameMode::ReceivePackageItemTest(const unsigned char* packet) {
  const PackageItemTestEntry* entry = reinterpret_cast<const PackageItemTestEntry*>(packet + 8);
  char packageName[260];
  std::memset(packageName, 0, sizeof(packageName));
  ITEM_INFO item;
  item.SetItemId(*reinterpret_cast<const int*>(packet + 4));
  int capacity = sizeof(packageName);
  ItemDescriptions{g_itemDescriptions}.Name(item, packageName, capacity, 0);

  char path[2048];
  std::memset(path, 0, sizeof(path));
  int suffix = 0;
  std::sprintf(path, "%s\\PackageItemTest", g_applicationDirectory);
  if (_access(path, 0) != 0 && _mkdir(path) != 0) {
    return;
  }

  SYSTEMTIME time;
  GetLocalTime(&time);
  std::sprintf(path, "%s\\PackageItemTest\\%s(%d_%d_%d).txt", g_applicationDirectory, packageName, time.wYear, time.wMonth, time.wDay);
  while (_access(path, 0) == 0) {
    ++suffix;
    std::sprintf(path, "%s\\PackageItemTest\\%s(%d_%d_%d)_%d.txt", g_applicationDirectory, packageName, time.wYear, time.wMonth, time.wDay, suffix);
  }

  std::FILE* file = std::fopen(path, "w");
  if (!file) {
    return;
  }

  int total = 0;
  const int count = static_cast<unsigned int>(*reinterpret_cast<const std::int16_t*>(packet + 2) - 8) / sizeof(PackageItemTestEntry);
  std::fprintf(file, "============================START============================\n");
  for (int i = 0; i < count; ++i) {
    char itemName[260];
    std::memset(itemName, 0, sizeof(itemName));
    const PackageItemTestEntry current = *entry;
    item.SetItemId(current.itemId);
    int nameCapacity = sizeof(itemName);
    ItemDescriptions{g_itemDescriptions}.Name(item, itemName, nameCapacity, 0);
    std::fprintf(file, "%s\t\t\t%d\t%c%d%c\n", itemName, current.count, '(', current.unknown_0004, ')');
    total += current.count;
    ++entry;
  }

  std::fprintf(file, "============================END============================\n");
  std::fprintf(file, "Unpacked %d Packages\n", total);
  std::fclose(file);
}

#pragma pack(push, 1)
struct SummonHpInitPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int32_t summonId;
  /* 0006 */ std::int32_t hp;
  /* 000A */ std::int32_t maxHp;
};

struct SummonHpUpdatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ std::int32_t summonId;
  /* 0006 */ std::uint16_t varId;
  /* 0008 */ std::int32_t value;
};
#pragma pack(pop)

// OFFSET: 0x00D655E0
// FUNCTION: fn_21999
void CGameMode::ReceiveSummonHpInit(const unsigned char* buffer) {
  const SummonHpInitPacket* packet = reinterpret_cast<const SummonHpInitPacket*>(buffer);
  g_session.unknown_0784[packet->summonId] = packet->hp;
  g_session.unknown_078C[packet->summonId] = packet->maxHp;

  CRenderObject* actor = this->scene->GetGameActorByAID(packet->summonId);
  if (actor != nullptr) {
    const std::int64_t id = packet->summonId;
    actor->SendMsg(nullptr, 34, 0, static_cast<int>(id), static_cast<int>(id >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D65680
// FUNCTION: fn_22000
void CGameMode::ReceiveSummonHpUpdate(const unsigned char* buffer) {
  const SummonHpUpdatePacket* packet = reinterpret_cast<const SummonHpUpdatePacket*>(buffer);
  if (g_session.FindUnknown078C(packet->summonId) == -1) {
    return;
  }

  switch (packet->varId) {
  case 5:
    g_session.unknown_0784[packet->summonId] = packet->value;
    break;
  case 6:
    g_session.unknown_078C[packet->summonId] = packet->value;
    break;
  }

  CRenderObject* actor = this->scene->GetGameActorByAID(packet->summonId);
  if (actor != nullptr) {
    const std::int64_t id = packet->summonId;
    actor->SendMsg(nullptr, 34, 0, static_cast<int>(id), static_cast<int>(id >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D65730
// FUNCTION: fn_22001
void CGameMode::ReceiveClanLeave(const unsigned char* packet) {
  g_clanInfo->Clear();
  this->nameCard.SetValue3(g_session.m_aid, g_clanInfo->m_clanName);
  if (this->scene->player) {
    this->scene->player->SetUnknown0305(0, 0);
  }

  g_windowMgr.RefreshClanWnd();
}

// OFFSET: 0x00D658C0
// FUNCTION: fn_22005
void CGameMode::ReceiveClanInfo(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ClanInfoPacket*>(data);

  g_clanInfo->Clear();
  g_clanInfo->m_hasClan = true;
  g_clanInfo->m_clanId = packet->clanId;

  char clanName[24] = {};
  strcpy_s(clanName, sizeof(clanName), packet->clanName);
  g_clanInfo->m_clanName = clanName;
  this->nameCard.SetValue3(g_session.m_aid, g_clanInfo->m_clanName);

  char masterName[24] = {};
  strcpy_s(masterName, sizeof(masterName), packet->masterName);
  g_clanInfo->m_masterName = masterName;

  char mapName[16] = {};
  strcpy_s(mapName, sizeof(mapName), packet->mapName);
  g_clanInfo->m_mapName = mapName;

  const char* relationName = packet->relationNames[0];
  for (int i = 0; i < packet->allyCount; ++i) {
    char name[24] = {};
    StringFormat(name, sizeof(name), static_cast<std::size_t>(-1), "%s", relationName);
    std::pair<int, std::string> relation(0, name);
    g_clanInfo->m_relations.push_back(relation);
    relationName += sizeof(name);
  }

  for (int i = 0; i < packet->hostileCount; ++i) {
    char name[24] = {};
    StringFormat(name, sizeof(name), static_cast<std::size_t>(-1), "%s", relationName);
    std::pair<int, std::string> relation(1, name);
    g_clanInfo->m_relations.push_back(relation);
    relationName += sizeof(name);
  }

  g_windowMgr.RefreshClanWnd();
}

// OFFSET: 0x00D66200
// FUNCTION: fn_22009
void CGameMode::ReceiveGameOptions(int count, const GameOptionEntry* entries) {
  for (int i = 0; i < count; ++i) {
    if (entries[i].id == 1) {
      g_gameOptionFlag = entries[i].value != 0;
    }
  }
}

// OFFSET: 0x00D66370
// FUNCTION: fn_22011
void CGameMode::ReceiveActorClanName(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ActorClanNamePacket*>(data);

  this->nameCard.SetValue3(packet->aid, std::string(packet->name, packet->name + (packet->length - 8)));
}

// OFFSET: 0x00D66840
// FUNCTION: fn_22015
void CGameMode::ReceiveDeferredReset(const unsigned char*) {
  g_session.m_options.Set(5, 0);
  GetRagConnection().ResetDeferred();
}

// OFFSET: 0x00D66A00
// FUNCTION: fn_22017
void CGameMode::ReceiveClanMemberCount(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const ClanMemberCountPacket*>(data);

  g_clanInfo->m_connectedMembers = packet->connectedMembers;
  g_clanInfo->m_totalMembers = packet->totalMembers;
}

// OFFSET: 0x00D67040
// FUNCTION: fn_22023
void CGameMode::ReceiveRuneActivation(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RuneActivationPacket*>(data);
  if (!packet) {
    return;
  }

  unsigned int count = packet->count;
  const unsigned int size = packet->length - 9;
  if (size != count * sizeof(std::uint32_t)) {
    count = size / sizeof(std::uint32_t);
  }

  for (unsigned int i = 0; i < count; ++i) {
    g_runeSystemMgr->SetRuneGrade(packet->tagId, 1, packet->ids[i], 0, 0);
  }

  CTagData* tag = g_runeSystemMgr->GetTag(packet->tagId);
  if (!tag) {
    return;
  }

  tag->UpdateActiveCount(1);
  UIWindow* window = g_windowMgr.LookupWindow(361);
  if (window) {
    window->SendMsg(window, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D670F0
// FUNCTION: fn_22024
void CGameMode::ReceiveRuneSetGrades(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RuneSetGradesPacket*>(data);
  if (!packet) {
    return;
  }

  unsigned int count = packet->count;
  const unsigned int size = packet->length - 9;
  if (size != count * sizeof(RuneSetGradeEntry)) {
    count = size / sizeof(RuneSetGradeEntry);
  }

  for (unsigned int i = 0; i < count; ++i) {
    const RuneSetGradeEntry& entry = packet->entries[i];
    g_runeSystemMgr->SetRuneGrade(packet->tagId, 0, entry.id, entry.grade, entry.gradeLevel);
  }

  CTagData* tag = g_runeSystemMgr->GetTag(packet->tagId);
  if (!tag) {
    return;
  }

  tag->UpdateActiveCount(0);
  tag->m_locked = true;
  UIWindow* window = g_windowMgr.LookupWindow(361);
  if (window) {
    window->SendMsg(window, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D671A0
// FUNCTION: fn_22025
void CGameMode::ReceiveRuneRewardResult(int result, int tagId, std::int64_t id, int step) {
  switch (result) {
  case 0: {
    g_runeSystemMgr->SetRewardStep(tagId, id, step);
    UIWindow* window = g_windowMgr.LookupWindow(361);
    if (window) {
      if (step > 0) {
        window->SendMsg(window, 178, 224, step - 1, 0, 0);
      }

      window->SendMsg(window, 60, 0, 0, 0, 0);
    }
    break;
  }
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4202)), 0x64FFFF, 0, 0, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4210)), 0x64FFFF, 0, 0, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4211)), 0x64FFFF, 0, 0, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4212)), 0x64FFFF, 0, 0, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4213)), 0x64FFFF, 0, 0, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4214)), 0x64FFFF, 0, 0, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4215)), 0x64FFFF, 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00D672E0
// FUNCTION: fn_22026
void CGameMode::ReceiveRuneRewardSteps(const unsigned char* data) {
  const auto* packet = reinterpret_cast<const RuneRewardStepsPacket*>(data);
  if (!packet) {
    return;
  }

  for (int i = 0; i < packet->count; ++i) {
    const RuneRewardStepEntry& entry = packet->entries[i];
    g_runeSystemMgr->SetRewardStep(packet->tagId, entry.id, entry.step);
  }

  UIWindow* window = g_windowMgr.LookupWindow(361);
  if (window) {
    window->SendMsg(window, 60, 0, 0, 0, 0);
  }
}
// OFFSET: 0x00D66CE0
// FUNCTION: fn_22022
int CGameMode::HandleRuneResult(int result) {
  switch (result) {
  case 0:
    return 0;
  case 1:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF4)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF4), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 2:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF5)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF5), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 3:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF6)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF6), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 4:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF7)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF7), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 5:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF8)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF8), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 6:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFF9)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFF9), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 7:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFA)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFA), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 8:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFB)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFB), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 10:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFD)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFC), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 11:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFD)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFD), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  case 12:
  case 13:
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xFFE)), 0xFF, 0, 0, 0);
    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(0xFFE), 0, true, false, 0, "Message", 280, 120, 0);
    break;
  default:
    return result;
  }

  return result;
}

// OFFSET: 0x00D67540
// FUNCTION: fn_22028
void CGameMode::ApplyRuneSetResult(int result, int tagId, int runeSetId) {
  if (this->HandleRuneResult(result) == 0) {
    g_runeSystemMgr->SetCurRuneSet(tagId, runeSetId, -1, -1);
  }

  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (window) {
    window->SendMsg(window, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D67590
// FUNCTION: fn_22029
void CGameMode::ApplyRuneSetResultWithRequest(int result, int tagId, int runeSetId, int grade, int slotCount) {
  if (this->HandleRuneResult(result) == 0) {
    g_runeSystemMgr->SetCurRuneSet(tagId, runeSetId, grade, slotCount);
  }

  g_runeSystemMgr->SendRequest(0x0BCB, nullptr, tagId);

  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (window) {
    window->SendMsg(window, 0x3C, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00D677C0
// FUNCTION: fn_22033
void CGameMode::ReceiveGameOptionResult(int command, int result, int enabled) {
  if (result != 1) {
    return;
  }

  if (command == 1) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xF15)), 0x64FFFF, 0, 0, 0);
  }

  g_gameOptionFlag = enabled != 0;
  g_windowMgr.sub_AAB090();
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(g_gameOptionFlag ? 0xF17 : 0xF16)), 0x64FFFF, 0, 0, 0);
}

// OFFSET: 0x00D679E0
// FUNCTION: fn_22037
void CGameMode::ReceiveGradeEnchantResult(const unsigned char* packet) {
  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, *reinterpret_cast<const std::int16_t*>(packet + 2));
  if (item.unknown_0000[1]) {
    const int result = *reinterpret_cast<const int*>(packet + 6);
    if (result == 0 || result == 2) {
      if (result == 0) {
        item.unknown_0060 = 0;
      }
      item.unknown_0088 = *reinterpret_cast<const std::int16_t*>(packet + 4);
      g_session.UpdateInventoryItem(item, 1);
    }
  }

  auto* window = static_cast<UIGradeEnchantWnd*>(g_windowMgr.LookupWindow(343));
  if (window) {
    const int result = *reinterpret_cast<const int*>(packet + 6);
    switch (result) {
    case 0:
      window->SetResult(2, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1607, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1608, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)->Slot16(2800);
      break;
    case 1:
    case 2:
    case 3:
    case 4:
      window->SetResult(2, result);
      g_effectMgr->Spawn(g_session.m_aid, 1607, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      g_effectMgr->Spawn(g_session.m_aid, 1609, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)->Slot16(2800);
      break;
    }
  }
}

// OFFSET: 0x00D686F0
// FUNCTION: fn_22042
void CGameMode::ReceiveGuildEmblemVersion(std::uint32_t aid, int guildId, int emblemVersion) {
  if (this->scene) {
    CRenderObject* actor = this->scene->GetGameActorByAID(aid);
    if (actor) {
      static_cast<CGameActor*>(actor)->Slot48(guildId);
    }
  }

  if (guildId) {
    g_guildMgr->SetEmblemVersion(guildId, emblemVersion);
  }
}

// OFFSET: 0x00D658B0
// FUNCTION: fn_22004
int CGameMode::RejectRequest(int, int, int, int, int) {
  return -1;
}

// OFFSET: 0x00D65FA0
// FUNCTION: fn_22006
void CGameMode::EnsureWindow259(const unsigned char*) {
  g_windowMgr.EnsureWindow(0x103);
}

// OFFSET: 0x00D66A50
// FUNCTION: fn_22019
void CGameMode::IgnoreRemovedPacket12(const unsigned char*, int, int, int, int) {
}

// OFFSET: 0x00D67670
// FUNCTION: fn_22031
void CGameMode::ToggleWindow169(unsigned char enable) {
  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (enable == 0) {
    if (window) {
      g_windowMgr.CloseWindow(0x169);
    }
  } else if (!window) {
    g_windowMgr.EnsureWindow(0x169);
  }
}

#pragma pack(push, 1)
struct RequestWindow169Packet {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ unsigned char unknown_0002;
};
#pragma pack(pop)

// OFFSET: 0x00D67600
// FUNCTION: fn_22030
void CGameMode::RequestWindow169(unsigned char enable) {
  UIWindow* window = g_windowMgr.LookupWindow(0x169);
  if (enable == 0) {
    if (window) {
      g_windowMgr.CloseWindow(0x169);
    }
    return;
  }

  if (window) {
    return;
  }

  RequestWindow169Packet packet;
  packet.id = 0x0BE0;
  packet.unknown_0002 = 0;
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00D66A60
// FUNCTION: fn_22020
void CGameMode::ShowNoticeText(std::string text) {
  const std::string message = FormatOwnedString(LookupMessage(0xF18), text.c_str());
  g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(message.c_str()), 0x64FFFF, 0, 0, 0);
}

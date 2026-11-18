#pragma once

#include "File/CommonObject.h"
#include "Client/Replay/Replay.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Window/WindowId.h"
#include "Network/Packets/PacketId.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Session/Session.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <windows.h>
#include <timeapi.h>
#include "Contents/EntryQueue/EntryQueueMgr.h"
#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/ClientState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Messenger/MessengerMember.h"

template __declspec(noinline) void std::vector<unsigned char>::assign<const unsigned char*, 0>(const unsigned char*, const unsigned char*);
template __declspec(noinline) void std::list<ITEM_INFO>::_Construct_range_unchecked<ITEM_INFO*>(ITEM_INFO*, ITEM_INFO*);

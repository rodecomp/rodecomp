#pragma once

#include "Client/Mode/ModeMessage.h"
#include "UI/Login/UIChangeNameWnd.h"
#include "UI/Window/UIEnums.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Navigation/NavigationLink.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cstdio>
#include <cstring>
#include <string>

template __declspec(noinline) std::string::size_type std::string::find(const char, const std::string::size_type) const noexcept;

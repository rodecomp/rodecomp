#pragma once

#include "UI/Window/WindowId.h"
#include "Render/Text/FontCache.h"
#include "UI/Window/UIEnums.h"
#include "Render/Text/TextContext.h"
#include "UI/Items/UIChangeMaterialWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ChangeMaterialMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Utility/StackAlloc.h"
#include "Utility/Text/FormatString.h"
#include "Client/Mode/ModeMgr.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <malloc.h>
#include <string>

template __declspec(noinline) std::vector<ChangeMaterialEntry>::vector(const std::vector<ChangeMaterialEntry>&);
template __declspec(noinline) std::vector<ChangeMaterialProduct>::vector(const std::vector<ChangeMaterialProduct>&);
template __declspec(noinline) std::vector<MaterialReq>::vector(const std::vector<MaterialReq>&);
template __declspec(noinline) std::vector<MaterialResult>::vector(const std::vector<MaterialResult>&);

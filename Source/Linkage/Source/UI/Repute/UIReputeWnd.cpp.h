#pragma once

#include <string>
#include <vector>
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Repute/UIReputeWnd.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Repute/ReputeMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIComboBoxEx.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Resource/Json/JsonCppLoader.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <new>

extern template std::vector<std::string>::~vector();
template CReputeMgr* Singleton<CReputeMgr>::GetInstance();

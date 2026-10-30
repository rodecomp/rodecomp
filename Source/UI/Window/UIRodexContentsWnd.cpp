#include "UI/Window/UIRodexContentsWnd.h"

#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00801CC0
// FUNCTION: fn_10014
void UIRodexContentsWnd::SetContentsText(const char* text) {
  this->m_contents->SetText(text);
}

// OFFSET: 0x00801CE0

// OFFSET: 0x00801F80

// OFFSET: 0x00802050

// OFFSET: 0x00802070

// OFFSET: 0x008023F0

// OFFSET: 0x008026E0

// OFFSET: 0x00802910

// OFFSET: 0x00803760

// OFFSET: 0x00803790

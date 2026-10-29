#include "UI/Items/MergeItemWnd.h"

#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <windows.h>

struct MergeItemPacketHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t length;
};

// OFFSET: 0x007DEF00

// OFFSET: 0x007DEF80

// OFFSET: 0x007DF3D0

// OFFSET: 0x007DFB20

// OFFSET: 0x007DFD00
// FUNCTION: fn_09669
void CMergeItemWnd::Slot19() {
}

// OFFSET: 0x007DFD10

// OFFSET: 0x007DFD60

// OFFSET: 0x007E0060

// OFFSET: 0x007E04B0

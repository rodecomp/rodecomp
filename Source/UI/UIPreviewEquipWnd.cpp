#include "UI/UIPreviewEquipWnd.h"

#include "Client/Session.h"
#include "Contents/Actors/CharacterRender.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/UserData/NormalItemSlots.h"

// OFFSET: 0x007F2F90
// FUNCTION: fn_09879
UIPreviewEquipWnd::UIPreviewEquipWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 1), saved(nullptr), m_render(nullptr), m_leftButton(nullptr), m_rightButton(nullptr), unknown_00C4(0) {
}

// OFFSET: 0x007F3090
// FUNCTION: fn_09880
UIPreviewEquipWnd::~UIPreviewEquipWnd() {
  delete this->m_render;
}

// OFFSET: 0x007F43E0
// FUNCTION: fn_09887
void UIPreviewEquipWnd::RefreshHeadgearLocations() {
  if (this->m_render->unknown_0028) {
    ITEM_INFO equipped = g_session.GetItem17F0(0);
    if (this->m_render->unknown_0028 == equipped.unknown_0070) {
      this->m_render->unknown_0034 = 1;
      if (equipped.unknown_0000[2] == 0x301) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }

    ITEM_INFO costume = g_session.GetItem2B50(0);
    if (this->m_render->unknown_0028 == costume.unknown_0070) {
      this->m_render->unknown_0034 = 1;
      if (costume.unknown_0000[2] == 0x1C00) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }
  }

  if (this->m_render->unknown_002C) {
    ITEM_INFO equipped = g_session.GetItem17F0(8);
    if (this->m_render->unknown_002C == equipped.unknown_0070) {
      if (equipped.unknown_0000[2] == 0x100) {
        this->m_render->unknown_0038 = 0x100;
      }

      if (equipped.unknown_0000[2] == 0x101) {
        this->m_render->unknown_0038 = 0x101;
        this->m_render->unknown_0034 = 0x101;
      }

      if (equipped.unknown_0000[2] == 0x300) {
        this->m_render->unknown_0038 = 0x300;
        this->m_render->unknown_003C = 0x300;
      }

      if (equipped.unknown_0000[2] == 0x301) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }

    ITEM_INFO costume = g_session.GetItem2B50(8);
    if (this->m_render->unknown_002C == costume.unknown_0070) {
      if (costume.unknown_0000[2] == 0x400) {
        this->m_render->unknown_0038 = 0x100;
      }

      if (costume.unknown_0000[2] == 0x1400) {
        this->m_render->unknown_0038 = 0x101;
        this->m_render->unknown_0034 = 0x101;
      }

      if (costume.unknown_0000[2] == 0xC00) {
        this->m_render->unknown_0038 = 0x300;
        this->m_render->unknown_003C = 0x300;
      }

      if (costume.unknown_0000[2] == 0x1C00) {
        this->m_render->unknown_0034 = 0x301;
        this->m_render->unknown_0038 = 0x301;
        this->m_render->unknown_003C = 0x301;
      }
    }
  }

  if (this->m_render->unknown_0030) {
    ITEM_INFO equipped = g_session.GetItem17F0(9);
    if (this->m_render->unknown_0030 == equipped.unknown_0070) {
      if (equipped.unknown_0000[2] == 0x200) {
        this->m_render->unknown_003C = 0x200;
      }

      if (equipped.unknown_0000[2] == 0x201) {
        this->m_render->unknown_003C = 0x201;
        this->m_render->unknown_0034 = 0x201;
      }
    }

    ITEM_INFO costume = g_session.GetItem2B50(9);
    if (this->m_render->unknown_0030 == costume.unknown_0070) {
      if (costume.unknown_0000[2] == 0x800) {
        this->m_render->unknown_003C = 0x200;
      }

      if (costume.unknown_0000[2] == 0x1800) {
        this->m_render->unknown_003C = 0x201;
        this->m_render->unknown_0034 = 0x201;
      }
    }
  }
}

// OFFSET: 0x007F4830
// FUNCTION: fn_09888
void UIPreviewEquipWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007F3DB0
// FUNCTION: fn_09884
void UIPreviewEquipWnd::Slot19() {
  this->Invalidate();
}

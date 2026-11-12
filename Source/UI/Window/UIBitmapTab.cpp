#include "UI/Window/UIEnums.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Window/UIBitmapTab.h"
#include "Utility/Text/FormatString.h"

template __declspec(noinline) void std::vector<BitmapTabEntry>::clear() noexcept;

// OFFSET: 0x0085BBC0
// FUNCTION: fn_10797
void UIBitmapTab::Paint() {
  int y = 0;

  for (unsigned index = 0; index < this->tabs.size(); ++index) {
    const std::string name = FormatOwnedString(this->tabs[index].name.c_str(), 'a' + (this->geometry.state == index));
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
    if (bitmap) {
      this->DrawBitmap(0, y, bitmap, false);
      y += bitmap->image.height;
    }
  }
}

// OFFSET: 0x0085EA90
// FUNCTION: fn_10838
void UIBitmapTab::OnLBtnDown(int, int y) {
  if (!this->parent) {
    return;
  }

  int bottom = 0;
  for (unsigned index = 0; index < this->tabs.size(); ++index) {
    bottom += this->tabs[index].height;
    if (y < bottom) {
      if (this->geometry.state != index) {
        this->geometry.state = index;
        this->Invalidate();
        this->parent->SendMsg(this, UM_TAB, this->geometry.state, 0, 0, 0);
      }

      return;
    }
  }
}

// OFFSET: 0x00855190
// FUNCTION: fn_10752
void UIBitmapTab::DragAndDrop(int argument1, int argument2, std::intptr_t argument3) {
  if (this->parent) {
    this->parent->DragAndDrop(argument1, argument2, argument3);
  }
}

// OFFSET: 0x00863300
// FUNCTION: fn_10908
int UIBitmapTab::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  switch (message) {
  case UM_SETSTATE:
    this->geometry.state = argument1;
    this->Invalidate();
    break;
  case UM_CLEAR_ITEM:
    this->tabs.clear();
    break;
  case UM_SET_BITMAP:
    if (argument1) {
      const char* tabName = reinterpret_cast<const char*>(argument1);
      const std::string name = FormatOwnedString(tabName, 'a');
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
      if (bitmap) {
        this->tabs.push_back(BitmapTabEntry{bitmap->image.height, tabName});
      }
    }
    break;
  case UM_RESIZE:
    if (!this->tabs.empty()) {
      int width = 0;
      int height = 0;
      for (unsigned index = 0; index < this->tabs.size(); ++index) {
        const std::string name = FormatOwnedString(this->tabs[index].name.c_str(), 'a');
        CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
        if (bitmap) {
          width = bitmap->image.width;
          height += bitmap->image.height;
        }
      }

      this->Resize(width, height);
    }
    break;
  }

  return 0;
}

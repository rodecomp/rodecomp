#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIBitmapTab.h"
#include "Utility/Text/FormatString.h"

// OFFSET: 0x0085BBC0
// FUNCTION: fn_10797
void UIBitmapTab::Paint() {
  int y = 0;

  for (unsigned index = 0; index < this->tabs.size(); ++index) {
    const std::string name = FormatOwnedString(this->tabs[index].name.c_str(), 'a' + (this->geometry.unknown_0030 == index));
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
    if (bitmap) {
      this->DrawBitmap(0, y, &bitmap->image, false);
      y += bitmap->image.height;
    }
  }
}

// OFFSET: 0x0085EA90
// FUNCTION: fn_10838
void UIBitmapTab::Slot25(int, int y) {
  if (!this->parent) {
    return;
  }

  int bottom = 0;
  for (unsigned index = 0; index < this->tabs.size(); ++index) {
    bottom += this->tabs[index].height;
    if (bottom > y) {
      if (this->geometry.unknown_0030 != index) {
        this->geometry.unknown_0030 = index;
        this->Invalidate();
        this->parent->SendMsg(this, 22, this->geometry.unknown_0030, 0, 0, 0);
      }

      return;
    }
  }
}

// OFFSET: 0x00855190
// FUNCTION: fn_10752
void UIBitmapTab::Slot10(int argument1, int argument2, std::intptr_t argument3) {
  if (this->parent) {
    this->parent->Slot10(argument1, argument2, argument3);
  }
}

// OFFSET: 0x00863300
// FUNCTION: fn_10908
int UIBitmapTab::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  switch (message) {
  case 13:
    this->geometry.unknown_0030 = argument1;
    this->Invalidate();
    break;
  case 14:
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
  case 75:
    this->tabs.clear();
    break;
  case 78:
    if (argument1) {
      const char* tabName = reinterpret_cast<const char*>(argument1);
      const std::string name = FormatOwnedString(tabName, 'a');
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
      if (bitmap) {
        this->tabs.push_back(BitmapTabEntry{bitmap->image.height, tabName});
      }
    }
    break;
  }

  return 0;
}

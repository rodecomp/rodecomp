#include "UI/Window/UIPetEvolutionWnd.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Window/WindowBorder.h"

#include <cstdio>
#include <cstring>
#include <string>

// OFFSET: 0x009AB0E0
// FUNCTION: fn_13762
UIPetEvolutionWnd::~UIPetEvolutionWnd() {
}

// OFFSET: 0x009AB170

// OFFSET: 0x009AB320
// FUNCTION: fn_13764
void UIPetEvolutionWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2567), 0);

  char text[260];
  std::memset(text, 0, sizeof(text));
  {
    std::string unused;

    int job = g_session.GetPetJobByEggItem(this->m_petEggId);
    if (job != -1) {
      g_session.GetPetIllustName(job, g_session.unknown_1038, text);
      CBitmapRes* illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      if (!illust) {
        illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("userinterface\\illust\\pet_noimage.bmp")));
      }
      if (illust) {
        this->FillRegion(12, 29, illust->image.width, illust->image.height, 0x9B9B9B);
        this->DrawBitmap(10, 27, &illust->image, false);
        DrawRectangleBorder(*this, 10, 27, illust->image.width, illust->image.height, 0);
      }
    }

    CBitmapRes* arrow = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\img_next.bmp")));
    if (arrow) {
      this->DrawBitmap(
        static_cast<int>((this->geometry.width - arrow->image.width) * 0.5), static_cast<int>((134 - arrow->image.height) * 0.5 + 27.0),
        &arrow->image, true
      );
    }

    job = g_session.GetPetJobByEggItem(this->m_evolvedEggId);
    if (job != -1) {
      g_session.GetPetIllustName(job, g_session.unknown_1038, text);
      CBitmapRes* illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      if (!illust) {
        illust = dynamic_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("userinterface\\illust\\pet_noimage.bmp")));
      }
      if (illust) {
        this->FillRegion(this->geometry.width - illust->image.width - 8, 29, illust->image.width, illust->image.height, 0x9B9B9B);
        this->DrawBitmap(this->geometry.width - illust->image.width - 10, 27, &illust->image, false);
        DrawRectangleBorder(*this, this->geometry.width - illust->image.width - 10, 27, illust->image.width, illust->image.height, 0);
      }
    }
  }

  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 10, 200, LookupMessage(2569), 0, 0, 12, 0, 0, 0);

  int index = 0;
  for (auto it = this->m_materials.begin(); it != this->m_materials.end(); ++it, ++index) {
    std::memset(text, 0, sizeof(text));
    std::sprintf(text, "%dea", it->second);
    UITextButton* button = this->m_materialButtons[index];
    this->DrawTextAt(
      this->m_controls->fonts, this->m_controls->locale, button->geometry.width + button->geometry.x + 10, button->geometry.y + 1, text, 0, 0, 12, 0, 1, 0
    );
  }

  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 10, this->geometry.height - 52, LookupMessage(2570), 0, 0, 12, 0, 0, 0);
}

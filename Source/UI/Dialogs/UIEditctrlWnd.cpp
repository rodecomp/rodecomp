#include "Client/Mode/CMode.h"
#include "UI/Dialogs/UIEditctrlWnd.h"

#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Render/Textures/SpritePixels.h"
#include "Contents/MessageTable.h"
#include "Contents/NavigationManager.h"
#include "Contents/TownInfoMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UISpriteButton.h"
#include "UI/Dialogs/UIMiniMapWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// GLOBAL: gl_24906
const char* g_miniMapMemoPath = "%s\\Memo\\$$%s.txt";

// GLOBAL: gl_25072
const char* g_miniMapMemoLineFormat = "%d\t%d\t%s\r\n";

// OFFSET: 0x0099B0C0
// FUNCTION: fn_13549
UIEditctrlWnd::UIEditctrlWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x0099B120
// FUNCTION: fn_13550
UIMiniMapWnd::UIMiniMapWnd()
    : UIFrameWnd(0) {
  this->dragging = 0;
  this->unknown_0114 = 0;
  this->unknown_011C = nullptr;
  this->unknown_0120 = nullptr;
}

// OFFSET: 0x0099B540
// FUNCTION: fn_13554
void UIMiniMapWnd::AddTownMarks() {
  std::string mapName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->mapName;
  this->warpMarks.clear();

  for (int index = 0;; ++index) {
    TownInfo info = g_townInfoMgr->GetTownInfo(mapName, index);
    if (info.image.empty()) {
      break;
    }

    if (info.type == 8) {
      MiniMapMark mark;
      mark.x = static_cast<int>(static_cast<float>(info.x) * this->scale - 6.0f + this->originX);
      mark.y = static_cast<int>(this->originY - (static_cast<float>(info.y) * this->scale + 6.0f));
      this->warpMarks.push_back(mark);
    } else {
      UIBitmapButton* button = new UIBitmapButton();
      button->SetBitmap(info.image.c_str(), 0, 0);
      button->SetBitmap(info.image.c_str(), 1, 0);
      button->SetBitmap(info.image.c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->SetId(index + 1000);
      button->SetTooltip(info.name.c_str());
      button->Move(
        static_cast<int>(static_cast<float>(info.x) * this->scale - 6.0f + this->originX),
        static_cast<int>(this->originY - (static_cast<float>(info.y) * this->scale + 6.0f))
      );
      this->AddChild(button);
      this->townButtons.emplace(index, button);
    }
  }
}

// OFFSET: 0x0099BA80
// FUNCTION: fn_13555
int UIMiniMapWnd::LoadMemos() {
  for (int index = 0; index < 20; ++index) {
    this->memoButtons[index]->Move(-100, -100);
    this->memoButtons[index]->SetTooltip("");
  }

  int count = 0;
  const std::string path = FormatOwnedString(g_miniMapMemoPath, g_applicationDirectory, g_session.m_navigationPlayerMap);
  FILE* file = std::fopen(path.c_str(), "rb");
  if (file) {
    char separators[] = "\t ,\t\r\n";
    char line[1024];
    std::memset(line, 0, sizeof(line));
    std::vector<std::string> tokens;

    int lineCount = 0;
    while (!std::feof(file)) {
      if (++lineCount >= 40 || count >= 20) {
        break;
      }

      std::fgets(line, sizeof(line), file);
      for (char* token = std::strtok(line, separators); token; token = std::strtok(nullptr, separators)) {
        tokens.push_back(std::string(token));
      }

      if (tokens.size() >= 2) {
        const int x = std::atoi(tokens[0].c_str());
        const int y = std::atoi(tokens[1].c_str());
        if (x <= 0 || this->geometry.width < x || y <= 0 || this->geometry.height < y) {
          break;
        }

        std::string text;
        for (unsigned index = 2; index < tokens.size(); ++index) {
          if (text.size() > 30) {
            break;
          }
          text += " ";
          text += tokens[index];
        }

        tokens.clear();
        this->memoButtons[count]->Move(x, y);
        this->memoButtons[count]->SetTooltip(text.c_str());
        ++count;
      }
    }

    std::fclose(file);
    this->Invalidate();
  }

  return count;
}

// OFFSET: 0x0099C090
// FUNCTION: fn_13556
void UIEditctrlWnd::Slot15(int, int) {
  this->AddCloseButton(LookupMessage(2741), 0);
  this->AddNumberedButton(184, 205, 19, "minimap\\memo_ok", LookupMessage(2754), true);

  this->edit = new UIEditCtrl();
  this->edit->SetSize(200, 18);
  this->edit->Move(5, 19);
  this->edit->maximum = 20;
  this->edit->Background(232, 232, 232);
  this->edit->Border(200, 100, 100);
  this->edit->flat = 0;
  const char* tooltip = LookupMessage(2753);
  if (tooltip) {
    this->edit->tooltip = tooltip;
  }
  this->AddChild(this->edit);
  g_windowMgr.SetFocus(this->edit);
}

// OFFSET: 0x0099C200
// FUNCTION: fn_13557
void UIMiniMapWnd::Slot15(int width, int height) {
  const int bottom = this->geometry.height;
  const float terrainWidth = static_cast<float>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->TerrainWidth());
  const float terrainHeight = static_cast<float>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->TerrainHeight());
  this->originX = 2.0f;
  this->originY = 531.0f;
  this->scale = 512.0f / std::max(terrainWidth, terrainHeight);
  const float margin = std::fabs(terrainWidth - terrainHeight) * 0.5f * this->scale;
  if (terrainHeight > terrainWidth) {
    this->originX = margin + 2.0f;
  } else if (terrainWidth > terrainHeight) {
    this->originY = 531.0f - margin;
  }

  this->unknown_011C = this->AddCheckBox(361, 100, 12, LookupMessage(2742), 20, bottom - 19, 1, LookupMessage(2743));
  this->unknown_0120 = this->AddCheckBox(194, 100, 12, LookupMessage(2744), 120, bottom - 19, 1, LookupMessage(2745));
  this->unknown_0124 = this->AddCheckBox(373, 100, 12, LookupMessage(2746), 220, bottom - 19, 1, LookupMessage(2747));
  this->AddCloseButton(LookupMessage(2741), 0);
  this->AddNumberedButton(356, this->geometry.width - 120, bottom - 23, "minimap\\memo_ready", LookupMessage(2748), true);
  this->AddTownMarks();
  this->unknown_012C = this->AddNumberedButton(205, -100, -100, "minimap\\boss", LookupMessage(2749), true);
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ResendMinimapMarkers();
  for (int index = 0; index < 20; ++index) {
    this->memoButtons[index] = this->AddNumberedButton(index + 601, -100, -100, "minimap\\memopoint", nullptr, true);
  }

  this->LoadMemos();
  this->m_playerHead = new UIHeadsprButton();
  this->UpdatePlayerHead();
  this->m_playerHead->Move(-100, -100);
  this->m_playerHead->unknown_00BC = 0;
  this->m_playerHead->unknown_00C0 = 0;
  this->m_playerHead->unknown_00C4 = 0;
  this->m_playerHead->SetTooltip(LookupMessage(2750));
  this->m_playerHead->SetId(200);
  this->AddChild(this->m_playerHead);
}

// OFFSET: 0x0099C530
// FUNCTION: fn_13558
void UIEditctrlWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(true, LookupMessage(2755), 0);
}

// OFFSET: 0x0099C820
// FUNCTION: fn_13560
void UIMiniMapWnd::Slot25(int x, int y) {
  if (y <= 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (x >= 2 && y >= 19 && this->geometry.width - 2 >= x && this->geometry.height - 19 >= y && this->dragging) {
    this->dragging = 0;
    g_windowMgr.miniMapClickY = y - 6;
    g_windowMgr.miniMapClickX = x - 6 < 0 ? 0 : x - 6;
    g_windowMgr.EnsureWindow(274);
  }
}

// OFFSET: 0x0099C8A0
// FUNCTION: fn_13561
void UIMiniMapWnd::ResetCursor(int, int) {
  if (this->dragging) {
    g_modeMgr.gameMode->SetCursorAction(1);
  } else {
    g_modeMgr.gameMode->SetCursorAction(0);
  }
}

// OFFSET: 0x0099CA60
// FUNCTION: fn_13562
int UIEditctrlWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 184: {
      std::string text = FormatOwnedString(g_miniMapMemoPath, g_applicationDirectory, g_session.m_navigationPlayerMap);
      FILE* file = std::fopen(text.c_str(), "ab");
      if (file) {
        std::fseek(file, 0, SEEK_END);
        text = FormatOwnedString(g_miniMapMemoLineFormat, g_windowMgr.miniMapClickX, g_windowMgr.miniMapClickY, this->edit->Text());
        std::fwrite(text.c_str(), text.size(), 1, file);
        std::fclose(file);
        if (g_windowMgr.window_4F1CC) {
          static_cast<UIMiniMapWnd*>(g_windowMgr.window_4F1CC)->LoadMemos();
        }
        g_windowMgr.CloseWindow(274);
      }
      return 0;
    }
    case 201:
      g_windowMgr.CloseWindow(274);
      return 0;
    }
    return 0;
  case 0:
    if (this->edit == sender && this->edit->Length() > 0) {
      this->SendMsg(this->edit, 6, 184, 0, 0, 0);
      return 0;
    }
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0099DB10
// FUNCTION: fn_13564
void UIMiniMapWnd::UpdatePlayerHead() {
  UIHeadsprButton* head = this->m_playerHead;
  if (!head) {
    return;
  }

  head->SetActName(g_session.GetPlayerHeadActName().c_str());
  if (g_session.m_headPalette != 0) {
    this->m_playerHead->SetPalette(g_session.GetPlayerHeadPalName().c_str());
  }

  const std::string sprite = g_session.GetPlayerHeadSprName();
  this->m_playerHead->SetSprite(sprite.c_str());
  CSprRes* resource = dynamic_cast<CSprRes*>(ResourceMgr::Instance().Load(sprite.c_str()));
  if (resource) {
    const SpriteRleImage* image = resource->images[0][0];
    this->m_playerHead->SetSize(image->width, image->height);
  } else {
    this->m_playerHead->SetSize(2, 2);
    this->m_playerHead->Move(-100, -100);
  }
}

// OFFSET: 0x0099DC90
// FUNCTION: fn_13565
void UIMiniMapWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.x;
    this->m_saved->height = this->geometry.y;
    this->m_saved->unknown_0010 = this->geometry.width;
    this->m_saved->unknown_0014 = this->geometry.height;
  }
  this->m_saved->unknown_0018 = this->unknown_011C->geometry.unknown_0030 != 0;
  this->m_saved->unknown_001C = this->unknown_0120->geometry.unknown_0030 != 0;
  this->m_saved->unknown_0020 = this->unknown_0124->geometry.unknown_0030 != 0;
}

// OFFSET: 0x0099C560
// FUNCTION: fn_13559
void UIMiniMapWnd::Paint() {
  int column = 0;
  int row = 0;
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->PlayerCell(&column, &row);
  const std::string title =
    FormatOwnedString("%s ( %s ) %d %d", g_session.GetPlayerMapDisplayName(), g_session.m_navigationPlayerMap, column, row);
  this->DrawPanelBackground(this->DrawTitleBar(true, title.c_str(), 0), 0, "");
  this->DrawButtonBar(1, 0, 1);
  this->FillRegion(2, 19, 512, 512, 0xFFAAAAAA);

  const std::string mapName = FormatOwnedString("유저인터페이스\\map\\%s.bmp", g_session.m_navigationPlayerMap);
  auto* map = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(mapName.c_str())));
  if (map) {
    this->DrawBitmap(2, 19, map, true);
    this->geometry.surface->Brighten(2, 19, 512, 512, -50);
  }

  auto* warp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\minimap\\warp.bmp")));
  for (unsigned index = 0; index < this->warpMarks.size(); ++index) {
    if (warp) {
      this->DrawBitmap(this->warpMarks[index].x, this->warpMarks[index].y, warp, true);
    }
  }

  if (this->unknown_0124->geometry.unknown_0030 != 0) {
    auto* guild = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\minimap\\guild.bmp")));
    for (unsigned index = 0; index < this->guildMarks.size(); ++index) {
      const int x = this->guildMarks[index].x;
      if (x > 0) {
        const int y = this->guildMarks[index].y;
        if (y > 0 && guild) {
          this->DrawBitmap(x, y, guild, true);
        }
      }
    }

    auto* party = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\minimap\\Party.bmp")));
    for (unsigned index = 0; index < this->partyMarks.size(); ++index) {
      const int x = this->partyMarks[index].x;
      if (x > 0) {
        const int y = this->partyMarks[index].y;
        if (y > 0 && party) {
          this->DrawBitmap(x, y, party, true);
        }
      }
    }
  }
}

// OFFSET: 0x0099CC20
// FUNCTION: fn_13563
int UIMiniMapWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 194: {
      const bool enabled = this->unknown_0120->geometry.unknown_0030 != 0;
      for (auto it = this->townButtons.begin(); it != this->townButtons.end(); ++it) {
        it->second->Enable(enabled);
      }
      this->Invalidate();
      return 0;
    }
    case 200:
      return 0;
    case 201:
      g_windowMgr.CloseWindow(273);
      return 0;
    case 356:
      this->dragging = 1;
      return 0;
    case 361: {
      const bool enabled = this->unknown_011C->geometry.unknown_0030 != 0;
      for (auto it = this->questButtons.begin(); it != this->questButtons.end(); ++it) {
        it->second->Enable(enabled);
      }
      this->Invalidate();
      return 0;
    }
    case 373:
      this->Invalidate();
      return 0;
    default:
      if (argument1 >= 601 && argument1 < 621) {
        if (g_windowMgr.Confirm(LookupMessage(2751), 2, true, false, 0, "Message", 280, 120, 0) == 187) {
          this->memoButtons[argument1 - 601]->SetTooltip("");
          this->memoButtons[argument1 - 601]->Move(-100, -100);

          std::string text = FormatOwnedString(g_miniMapMemoPath, g_applicationDirectory, g_session.m_navigationPlayerMap);
          FILE* file = std::fopen(text.c_str(), "wb");
          if (file) {
            for (int index = 0; index < 20; ++index) {
              UIBitmapButton* memo = this->memoButtons[index];
              if (memo->geometry.x > 0 && memo->geometry.y > 0) {
                text = FormatOwnedString(g_miniMapMemoLineFormat, memo->geometry.x, memo->geometry.y, std::string(memo->m_tooltip).c_str());
                std::fwrite(text.c_str(), text.size(), 1, file);
              }
            }
            std::fclose(file);
          }

          this->unknown_0114 = this->LoadMemos();
          this->Invalidate();
        }
      }

      if (argument1 >= 1000 && argument1 < 2000) {
        std::string mapName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->MapName();
        TownInfo town = g_townInfoMgr->GetTownInfo(mapName, argument1 - 1000);
        if (!town.image.empty()) {
          const std::size_t extension = mapName.find_first_of(".");
          if (extension != std::string::npos) {
            mapName = mapName.substr(0, extension);
          }
          g_navigationManager.StartNavigation(mapName.c_str(), 0, 5, true, town.x, town.y, 1002);
        }
        return 0;
      }

      if (argument1 > 2000) {
        std::string mapName = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->MapName();
        const auto mark = this->questMarks.find(argument1);
        const std::size_t extension = mapName.find_first_of(".");
        if (extension != std::string::npos) {
          mapName = mapName.substr(0, extension);
        }
        g_navigationManager.StartNavigation(mapName.c_str(), 0, 5, true, mark->second.x, mark->second.y, 1002);
        return 0;
      }
      return 0;
    }

  case 34:
    this->m_saved = reinterpret_cast<MiniMapWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->Move(this->m_saved->width, this->m_saved->height);
    this->unknown_011C->SetChecked(this->m_saved->unknown_0018);
    this->unknown_0120->SetChecked(this->m_saved->unknown_001C);
    this->unknown_0124->SetChecked(this->m_saved->unknown_0020);
    return 0;

  case 481:
    this->m_playerHead->Move(static_cast<int>(static_cast<float>(argument1) * this->scale - 13.0f + this->originX),
      static_cast<int>(this->originY - (static_cast<float>(argument2) * this->scale + 13.0f)));
    this->Invalidate();
    return 0;

  case 482:
    this->unknown_012C->Move(static_cast<int>(static_cast<float>(argument1) * this->scale - 12.0f + this->originX),
      static_cast<int>(this->originY - (static_cast<float>(argument2) * this->scale + 12.0f)));
    this->Invalidate();
    return 0;

  case 483:
    if (static_cast<unsigned>(argument1) < 3 && argument2 >= 0) {
      std::vector<MiniMapMark>& marks = (&this->partyMarks)[argument1];
      if (static_cast<unsigned>(argument2) < marks.size()) {
        const int half = argument1 == 2 ? 8 : 6;
        marks[argument2].x = static_cast<int>(static_cast<float>(argument3) * this->scale - half + this->originX);
        marks[argument2].y = static_cast<int>(this->originY - (static_cast<float>(argument4) * this->scale + half));
        this->Invalidate();
        return 0;
      }
    }
    return 0;

  case 484:
    if (argument2 >= 0) {
      std::vector<MiniMapMark>& marks = (&this->partyMarks)[argument1];
      if (static_cast<unsigned>(argument2) != marks.size()) {
        marks.resize(argument2, MiniMapMark{-100, -100});
        this->Invalidate();
        return 0;
      }
    }
    return 0;

  case 485: {
    const int id = static_cast<int>(this->questButtons.size()) + 2001;
    const std::string name = FormatOwnedString("유저인터페이스\\minimap\\Quest_%d_%d.bmp", argument1, argument2);
    UIBitmapButton* button = new UIBitmapButton();
    button->SetBitmap(name.c_str(), 0, 0);
    button->SetBitmap(name.c_str(), 1, 0);
    button->SetBitmap(name.c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->SetId(id);
    button->SetTooltip(LookupMessage(2752));
    button->Move(static_cast<int>(static_cast<float>(argument3) * this->scale - 6.0f + this->originX),
      static_cast<int>(this->originY - (static_cast<float>(argument4) * this->scale + 6.0f)));
    this->AddChild(button);
    this->questButtons.insert(std::make_pair(id, button));
    this->questMarks.insert(std::make_pair(id, MiniMapMark{argument3, argument4}));
    this->Invalidate();
    return 0;
  }

  case 486:
    for (auto entry = this->townButtons.begin(); entry != this->townButtons.end(); ++entry) {
      this->RemoveChild(entry->second, 1);
    }
    this->questButtons.clear();
    this->questMarks.clear();
    this->Invalidate();
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

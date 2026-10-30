#include "UI/Dialogs/UIEditctrlWnd.h"

#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Contents/TownInfoMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Dialogs/UIMiniMapWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// GLOBAL: gl_24906
const char* g_miniMapMemoPath = "%s\\Memo\\$$%s.txt";

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
      UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
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

// OFFSET: 0x0099C530
// FUNCTION: fn_13558
void UIEditctrlWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2755), 0);
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
    this->m_context->cursor.Set(1, this->m_context->cursorTick);
  } else {
    this->m_context->cursor.Set(0, this->m_context->cursorTick);
  }
}

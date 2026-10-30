#include "UI/Dialogs/UIEditctrlWnd.h"

#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Contents/TownInfoMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Dialogs/UIMiniMapWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

// GLOBAL: gl_24906
const char* g_miniMapMemoPath = "%s\\Memo\\$$%s.txt";

// GLOBAL: gl_25072
const char* g_miniMapMemoLineFormat = "%d\t%d\t%s\r\n";

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

// OFFSET: 0x0099C090
// FUNCTION: fn_13556
void UIEditctrlWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, LookupMessage(2741), 0);
  this->AddNumberedButton(*this->m_controls, 184, 205, 19, "minimap\\memo_ok", LookupMessage(2754), true);

  this->edit = new UIEditCtrl(*this->m_context, *this->m_editContext);
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
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->edit);
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

// OFFSET: 0x0099CA60
// FUNCTION: fn_13562
int UIEditctrlWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 0) {
    if (this->edit == sender && this->edit->Length() > 0) {
      this->SendMsg(this->edit, 6, 184, 0, 0, 0);
      return 0;
    }
  } else if (message == 6) {
    if (argument1 == 184) {
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
    if (argument1 == 201) {
      g_windowMgr.CloseWindow(274);
      return 0;
    }
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
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

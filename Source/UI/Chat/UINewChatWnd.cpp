#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/UIEnums.h"

#include "Client/Input/Mouse.h"
#include "Client/Mode/CMode.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/MessageTable.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Text/NavigationLink.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <mbstring.h>

#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

// GLOBAL: gl_25380
static const std::string s_chatItemLinkOpening = "<ITEML>";

// GLOBAL: gl_25450
static const std::string s_itemLinkClosing = "</ITEML>";

// OFFSET: 0x00936360
// FUNCTION: fn_12750
int UINewChatWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (g_replayAnimationScaling != 0 && Replay::Instance().header.options.recordRestricted == 0) {
    return 1;
  }

  switch (message) {
  case 0x9A:
    for (std::map<int, SubChatWndInfo>::iterator it = g_session.m_noSubChatWnds.begin(); it != g_session.m_noSubChatWnds.end(); ++it) {
      static_cast<UIChatHisBox*>(this->m_tabs[it->first])->ResolvePlayerTags(reinterpret_cast<const char*>(a));
    }
    return 0;
  case 0x75: {
    if (!g_session.m_canDragWnd || g_session.m_noSubChatWnds.size() <= 1) {
      return 0;
    }
    std::map<int, SubChatWndInfo>& names = g_session.m_noSubChatWnds;
    std::map<int, SubChatWndInfo>::iterator found = names.find(this->m_currentTab);
    if (found == names.end()) {
      return 0;
    }
    int detached = static_cast<int>(g_session.m_subChatWnds.size());
    g_session.m_subChatWnds[detached] = found->second;
    const int key = found->first;
    names.erase(found);
    if (key != names.size()) {
      std::map<int, SubChatWndInfo> saved(names);
      names.clear();
      int index = 0;
      for (std::map<int, SubChatWndInfo>::iterator it = saved.begin(); it != saved.end(); ++it, ++index) {
        names[index] = it->second;
      }
    }

    this->m_tabControl->RemoveTab(this->m_currentTab);
    this->m_tabWidth = this->m_tabControl->geometry.width / names.size();
    UISubChatHisWnd* window = static_cast<UISubChatHisWnd*>(g_windowMgr.OpenSubChatWindow(static_cast<int>(g_windowMgr.retainedWindows.size()), -1, -1, 0, 0));
    if (window == nullptr) {
      return 0;
    }

    std::vector<UIChatHisBox*>::iterator tab = this->m_tabs.begin() + this->m_currentTab;
    if (tab != this->m_tabs.end()) {
      window->ChatBox()->CopyHistory(*static_cast<UIChatHisBox*>(*tab));
      this->RemoveChild(*tab, 1);
      this->m_tabs.erase(tab);
    }

    RECT rectangle;
    rectangle.left = 3;
    rectangle.top = 0;
    rectangle.right = this->m_tabWidth + 3 + this->m_tabControl->geometry.width;
    rectangle.bottom = this->m_tabControl->geometry.height;
    this->FillSurface(0xFFFF00FF, &rectangle);
    if (this->m_currentTab > 0) {
      --this->m_currentTab;
      this->m_tabControl->unknown_007C = this->m_currentTab;
    }
    this->RefreshTabs(true);

    if (sender != this) {
      int x = 0;
      int y = 0;
      this->ToScreen(x, y);
      g_windowMgr.ReleaseCapture();
      window->Move(x, y);
      window->OnLBtnDown(g_mouse.x - this->m_tabWidth * a - x, g_mouse.y - y);
    }
    this->ApplyHeaderLayout();
    return 0;
  }
  case 0x76: {
    if (!g_session.m_canDragWnd || this->m_tabControl->geometry.width + this->m_tabWidth > this->geometry.width - 0x5A) {
      return 0;
    }
    int index = static_cast<UISubChatHisWnd*>(sender)->m_index;
    std::map<int, SubChatWndInfo>& windows = g_session.m_subChatWnds;
    std::map<int, SubChatWndInfo>::iterator found = windows.find(index);
    if (found == windows.end()) {
      return 0;
    }

    int count = static_cast<int>(g_session.m_noSubChatWnds.size());
    if (this->TextWidth(found->second.name.c_str(), 0, 0, 12, 0, 0) * (count + 1)
        > this->geometry.width - 0x5A) {
      g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x6F0)), 0xFF, 0, 0, 0);
      g_windowMgr.Promote(sender);
      return 0;
    }

    g_session.m_noSubChatWnds[count] = found->second;
    this->m_tabControl->AddTab(found->second.name.c_str(), "");
    this->m_tabWidth = this->m_tabControl->geometry.width / g_session.m_noSubChatWnds.size();
    const int key = found->first;
    windows.erase(found);
    if (key != windows.size()) {
      std::map<int, SubChatWndInfo> saved(windows);
      windows.clear();
      int renumbered = 0;
      for (std::map<int, SubChatWndInfo>::iterator it = saved.begin(); it != saved.end(); ++it, ++renumbered) {
        windows[renumbered] = it->second;
      }
    }

    int height = this->geometry.height - this->m_inputOffsetY - this->m_inputAreaHeight - 4;
    if (height < 0) {
      height = this->m_inputAreaHeight;
    }
    UIChatHisBox* box = new UIChatHisBox();
    box->Configure(0, 0, this->geometry.width - 8, height, 0);
    box->Move(-100 - this->geometry.width, -100);
    this->AddChild(box);
    box->CopyHistory(*static_cast<UISubChatHisWnd*>(g_windowMgr.retainedWindows[index])->ChatBox());
    this->m_tabs.push_back(box);
    g_windowMgr.ReleaseRetainedWindow(index);
    if (index != g_windowMgr.retainedWindows.size()) {
      std::map<int, UIWindow*> saved(g_windowMgr.retainedWindows);
      g_windowMgr.retainedWindows.clear();
      int renumbered = 0;
      for (std::map<int, UIWindow*>::iterator it = saved.begin(); it != saved.end(); ++it, ++renumbered) {
        static_cast<UISubChatHisWnd*>(it->second)->m_index = renumbered;
        g_windowMgr.retainedWindows[renumbered] = it->second;
      }
    }

    RECT rectangle;
    rectangle.left = 3;
    rectangle.top = 0;
    rectangle.right = this->m_tabControl->geometry.width + 3;
    rectangle.bottom = this->m_tabControl->geometry.height;
    this->FillSurface(0xFFFF00FF, &rectangle);
    this->m_currentTab = count;
    this->m_tabControl->unknown_007C = count;
    this->RefreshTabs(true);
    this->ApplyHeaderLayout();
    return 0;
  }
  case 0x74:
    if (this->geometry.height <= this->m_inputOffsetY + this->m_inputAreaHeight) {
      return 0;
    }
    if (sender == this->m_tabControl) {
      if (a != 0) {
        g_session.m_noSubChatWnds[b].name = reinterpret_cast<const char*>(a);
        this->m_tabWidth = this->m_tabControl->geometry.width / g_session.m_noSubChatWnds.size();
        g_windowMgr.SetFocus(this->m_inputEdit);
        if (UIWindow* window = g_windowMgr.LookupWindow(0x84)) {
          window->SendMsg(this, 6, 0xC1, this->m_currentTab, 1, 2);
        }
      }
      RECT rectangle;
      rectangle.left = 3;
      rectangle.top = 0;
      rectangle.right = (c != 0 ? c : this->m_tabControl->geometry.width) + 3;
      rectangle.bottom = this->m_tabControl->geometry.height;
      this->FillSurface(0xFFFF00FF, &rectangle);
      this->ApplyHeaderLayout();
      this->Invalidate();
      return 0;
    } else {
      RECT rectangle;
      rectangle.left = 5;
      rectangle.top = this->m_inputOffsetY + 2;
      rectangle.right = this->geometry.width - 6;
      rectangle.bottom = this->geometry.height - this->m_inputAreaHeight - 2;
      this->FillSurface(0xFFFF00FF, &rectangle);
      return 0;
    }
  case UM_TAB:
    if (a != this->m_currentTab) {
      this->m_currentTab = a;
      this->RefreshTabs(true);
    } else {
      this->RefreshTabs(false);
    }
    return 0;
  case UM_DELETE_WHISPER_NAME: {
    std::string name(reinterpret_cast<const char*>(a));
    const int count = static_cast<int>(static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->history.size());
    int index = 0;
    for (std::vector<std::string>::iterator it = static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->history.begin(); it != static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->history.end(); ++it, ++index) {
      if (*it == name) {
        UIWhisperEditCtrl* whisper = static_cast<UIWhisperEditCtrl*>(this->unknown_00C0);
        whisper->history.erase(it);
        if (static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->historyIndex == index) {
          static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->historyIndex = count - 1;
        } else if (static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->historyIndex > index) {
          static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->historyIndex = static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->historyIndex - 1;
        }
        return 0;
      }
    }
    return 0;
  }
  case UM_COMBOBOX_SELECTED:
    switch (this->unknown_00F0) {
    case 1:
      *reinterpret_cast<int*>(&g_session.unknown_5490[0x8]) = a;
      switch (a) {
      case 0:
        static_cast<UIChatEditCtrl*>(this->m_chatEdit)->TextColor(RGB(0, 0, 0), 0xFFFFFFFF);
        break;
      case 1:
        static_cast<UIChatEditCtrl*>(this->m_chatEdit)->TextColor(RGB(128, 0, 128), 0xFFFFFFFF);
        break;
      case 2:
        static_cast<UIChatEditCtrl*>(this->m_chatEdit)->TextColor(RGB(0, 128, 128), 0xFFFFFFFF);
        break;
      case 4:
        static_cast<UIChatEditCtrl*>(this->m_chatEdit)->TextColor(RGB(148, 0, 211), 0xFFFFFFFF);
        break;
      }
      this->Invalidate();
      return 0;
    case 0: {
      const int count = static_cast<int>(static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->history.size());
      if (a >= 0 && a <= count) {
        static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->SetText(a == count ? "" : static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->history[a].c_str());
        static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->historyIndex = a;
      }
      g_windowMgr.SetFocus(static_cast<UIWhisperEditCtrl*>(this->unknown_00C0));
      break;
    }
    default:
      return 0;
    }
  case UM_GET_EDIT_FOCUS:
    g_windowMgr.SetFocus(static_cast<UIChatEditCtrl*>(this->m_chatEdit));
    return 0;
  case UM_ADDCHAT:
  case UM_ADDCHAT2:
    if (message != 0x73) {
      std::vector<std::string> lines;
      char name[25] = {};
      if (d != 0) {
        std::strcpy(name, reinterpret_cast<const char*>(d));
      }

      switch (c) {
      case 1:
      case 3:
      case 4:
      case 21: {
        std::string text(reinterpret_cast<const char*>(a));
        const int length = static_cast<int>(std::strlen(reinterpret_cast<const char*>(a)));
        const int position = static_cast<int>(text.find(" :"));
        if (position > 0 && position < length && position < 24) {
          std::memcpy(name, reinterpret_cast<const char*>(a), position);
          name[position] = '\0';
        }
        break;
      }
      }

      if (g_serviceType == 10 && std::strlen(name) > 24) {
        return 0;
      }

      if (std::strcmp(name, g_session.DecodePlayerName()) == 0) {
        name[0] = '\0';
      }

      bool hasLink = false;
      std::string plain = RemoveTextColors(reinterpret_cast<const char*>(a));
      if (g_navigationLinkParser->HasLink(plain)) {
        hasLink = true;
        lines.push_back(plain);
      } else {
        WrapPlainText(
          RemoveTextColors(reinterpret_cast<const char*>(a)).c_str(), lines, 0x5E
        );
      }

      const bool everyTab = c == 25 || (c == 0 && b == 0xFF);
      int index = 0;
      for (std::map<int, SubChatWndInfo>::iterator it = g_session.m_noSubChatWnds.begin(); it != g_session.m_noSubChatWnds.end(); ++it, ++index) {
        if ((it->second.options[c] || everyTab) && this->m_tabs[index] != nullptr) {
          static_cast<UIChatHisBox*>(this->m_tabs[index])->AddLinkRecord(reinterpret_cast<const char*>(a), b, name);
          for (unsigned int line = 0; line < lines.size(); ++line) {
            UIWindow* tab = this->m_tabs[index];
            const int width = this->TextWidth(lines[line].c_str(), 0, 0, 12, 0, 0);
            if (width <= tab->geometry.width - 20 || hasLink) {
              static_cast<UIChatHisBox*>(this->m_tabs[index])->AddText(lines[line].c_str(), b, name);
            } else {
              static_cast<UIChatHisBox*>(this->m_tabs[index])->AddTextWrapped(lines[line].c_str(), b, name);
            }
          }
        }
      }

      index = 0;
      for (std::map<int, SubChatWndInfo>::iterator it = g_session.m_subChatWnds.begin(); it != g_session.m_subChatWnds.end(); ++it, ++index) {
        if (it->second.options[c] || everyTab) {
          g_windowMgr.retainedWindows[index]->SendMsg(this, 0x25, a, b, reinterpret_cast<std::intptr_t>(name), 0);
        }
      }
    }
    return 0;
  case UM_SETINFO:
    if (g_windowMgr.unknown_0030 < 0 || g_windowMgr.unknown_0034 < 0 || g_windowMgr.unknown_0030 > g_renderer->viewport.width - this->geometry.width
        || g_windowMgr.unknown_0034 > g_renderer->viewport.height - this->geometry.height) {
      g_windowMgr.unknown_0030 = 0;
      g_windowMgr.unknown_0034 = g_renderer->viewport.height - this->geometry.height;
    }
    this->geometry.x = g_windowMgr.unknown_0030;
    this->geometry.y = g_windowMgr.unknown_0034;
    this->ResizeRows(g_windowMgr.unknown_004C);
    this->Move(this->geometry.x, this->geometry.y);
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_OPTION:
      if (UIWindow* window = g_windowMgr.EnsureWindow(0x84)) {
        window->SendMsg(this, 6, 0xC1, this->m_currentTab, 1, 0);
      }
      return 0;
    case 0x15B:
      g_windowMgr.SelectWhisperName(reinterpret_cast<const char*>(b));
      return 0;
    case 0x165:
      *reinterpret_cast<unsigned char*>(&g_windowMgr.unknown_03F4) = 1;
      this->m_currentTab = 0;
      if (this->unknown_0118 == 0) {
        this->unknown_0118 = 1;
      }
      this->ResizeRows(this->unknown_0118);
      this->Invalidate();
      return 0;
    case 0x166:
      this->SendMsg(this, 0x75, this->m_currentTab, 0, 0, 0);
      return 0;
    case ID_TALKTOWHOCOMBO: {
      this->unknown_00F0 = 1;
      if (g_windowMgr.unknown_0214 != nullptr) {
        g_windowMgr.CloseWindow(0x1A);
      }
      UIWindow* menu = g_windowMgr.EnsureWindow(0x1A);
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0x55)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0x56)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0x1B5)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0x939)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(LookupMessage(0xFC0)), 0, 0, 0);
      menu->SendMsg(nullptr, 0x28, *reinterpret_cast<int*>(&g_session.unknown_5490[0x8]), 0, 0, 0);
      menu->Resize(0x70, 0x30);
      menu->SendMsg(nullptr, 0x53, 1, 0, 0, 0);
      int x = this->geometry.width - 0x73;
      int y = this->geometry.height - this->m_inputAreaHeight;
      this->ToScreen(x, y);
      menu->Move(x + 1, y - 0x2F);
      return 0;
    }
    case ID_WHISPERCOMBO: {
      this->unknown_00F0 = 0;
      if (g_windowMgr.unknown_0214 != nullptr) {
        g_windowMgr.CloseWindow(0x1A);
      }
      const int count = static_cast<int>(static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->history.size());
      if (count == 0) {
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xC0)), 0xFF, 0, 0, 0);
        return 0;
      }

      UIWindow* menu = g_windowMgr.EnsureWindow(0x1A);
      for (int i = 0; i < count; ++i) {
        menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->history[i].c_str()), 0, 0, 0);
      }
      menu->SendMsg(nullptr, 0x1F, reinterpret_cast<std::intptr_t>(""), 0, 0, 0);
      int height = (count + 1) * 16;
      if (height > 0x50) {
        height = 0x50;
      }
      menu->SendMsg(nullptr, 0x28, static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->historyIndex, 0, 0, 0);
      menu->Resize(0x70, height);
      menu->SendMsg(nullptr, 0x53, 1, 0, 0, 0);
      int x = 0;
      int y = this->geometry.height - this->m_inputAreaHeight;
      this->ToScreen(x, y);
      menu->Move(x + 1, y - height + 1);
      return 0;
    }
    case ID_OK: {
      if (!g_windowMgr.inputSuggestionsEnabled && g_windowMgr.chatModeChanged) {
        return 0;
      }

      char buffer[0x16A];
      std::memset(buffer, 0, sizeof(buffer));
      std::string text(static_cast<UIChatEditCtrl*>(this->m_chatEdit)->Text());
      const std::size_t lineEnd = text.find('\n', 0);
      if (lineEnd != std::string::npos) {
        text = text.substr(0, lineEnd);
      }

      UIChatEditCtrl* edit = static_cast<UIChatEditCtrl*>(this->m_chatEdit);
      bool rejected = HasItemLinkTag(std::string(edit->Text()));
      if (!rejected) {
        UIItemTagOnChat* tags = static_cast<UIChatEditCtrl*>(this->m_chatEdit)->unknown_0144;
        if (tags != nullptr && tags->unknown_0140.size() != 0) {
          text = std::string(tags->unknown_0140);
          static_cast<UIChatEditCtrl*>(this->m_chatEdit)->unknown_0144->unknown_0140.clear();
          static_cast<UIChatEditCtrl*>(this->m_chatEdit)->unknown_0144->ClearTags();
          rejected = static_cast<UIChatEditCtrl*>(this->m_chatEdit)->LinkedText().size() != 0;
        }
        if (!rejected) {
          if (static_cast<UIChatEditCtrl*>(this->m_chatEdit)->LinkedText().size() != 0) {
            text = static_cast<UIChatEditCtrl*>(this->m_chatEdit)->LinkedText();
          }
          edit = static_cast<UIChatEditCtrl*>(this->m_chatEdit);
          rejected = HasNaviLinkTag(std::string(edit->Text()));
        }
      }

      if (rejected) {
        static_cast<UIChatEditCtrl*>(this->m_chatEdit)->SetText("");
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBE2)), 0xFF, 0, 0, 0);
        return 0;
      }

      StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), "%s", text.c_str());
      static_cast<UIChatEditCtrl*>(this->m_chatEdit)->PushHistory();
      this->m_inputEdit = static_cast<UIChatEditCtrl*>(this->m_chatEdit);
      g_windowMgr.SetFocus(static_cast<UIChatEditCtrl*>(this->m_chatEdit));
      static_cast<UIChatEditCtrl*>(this->m_chatEdit)->linkedText.clear();
      if (std::strlen(buffer) == 0) {
        return 0;
      }

      if (g_serviceType == 1 && !ValidateCharacterName(buffer)) {
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xBE)), 0xFF, 0, 0, 0);
        return 0;
      }

      if (static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->Length() != 0 && buffer[0] != '/') {
        g_windowMgr.unknown_04DC = buffer;
        g_modeMgr.gameMode->SendMsg(0xB, reinterpret_cast<int>(static_cast<UIWhisperEditCtrl*>(this->unknown_00C0)->Text()), 0, 0, 0);
        return 0;
      }

      if (g_session.ProcessEmotionShortcut(buffer) != 0) {
        return 0;
      }

      std::string parts[3];
      int talkType;
      const int offset = g_session.GetTalkType(buffer, &talkType, parts);
      if (talkType == 0x37 || talkType == 0x1A || talkType == 0x12) {
        if (_mbsstr(reinterpret_cast<const unsigned char*>(buffer), reinterpret_cast<const unsigned char*>(s_chatItemLinkOpening.c_str())) != nullptr
            || _mbsstr(reinterpret_cast<const unsigned char*>(buffer), reinterpret_cast<const unsigned char*>(s_itemLinkClosing.c_str())) != nullptr) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0xAFC)), 0x64FFFF, 0, 0, 0);
          return 0;
        }
      }
      if (offset != -1) {
        g_windowMgr.SetUnknown04DC(buffer + offset);
      }
      g_modeMgr.gameMode->SendMsg(0x2A, talkType, reinterpret_cast<int>(parts), 0, 0);
      return 0;
    }
    case ID_HIS:
      this->SendMsg(nullptr, 2, 0, 0, 0, 0);
      return 0;
    case 0x176: {
      int noSubCount = static_cast<int>(g_session.m_noSubChatWnds.size());
      int subCount = static_cast<int>(g_session.m_subChatWnds.size());
      const int total = noSubCount + subCount;
      if (total == this->unknown_0110) {
        g_windowMgr.Confirm(LookupMessage(0x62A), 0, true, false, 0, "Message", 0x118, 0x78, 0);
        return 0;
      }

      SubChatWndInfo info = {};
      info.name = "NewTab_";
      char number[16];
      info.name += _itoa(total + 1, number, 10);
      if ((noSubCount + 1) * this->m_tabWidth > this->geometry.width - 0x5A) {
        g_session.m_subChatWnds[subCount] = info;
        g_windowMgr.OpenSubChatWindow(static_cast<int>(g_windowMgr.retainedWindows.size()), -1, -1, 0, 0);
      } else {
        g_session.m_noSubChatWnds[noSubCount] = info;
        this->m_tabControl->AddTab(info.name.c_str(), "");
        this->m_tabWidth = this->m_tabControl->geometry.width / g_session.m_noSubChatWnds.size();
        UIChatHisBox* box = new UIChatHisBox();
        static_cast<UIChatHisBox*>(box)->Configure(0, 0, this->geometry.width - 8, this->geometry.height - this->m_inputAreaHeight - this->m_inputOffsetY - 4, 0);
        box->Move(-100 - this->geometry.width, -100);
        this->AddChild(box);
        this->m_tabs.push_back(box);

        RECT rectangle;
        rectangle.left = 3;
        rectangle.top = 0;
        rectangle.right = this->m_tabControl->geometry.width + 3;
        rectangle.bottom = this->m_tabControl->geometry.height;
        this->FillSurface(0xFFFF00FF, &rectangle);
        this->m_currentTab = noSubCount;
        this->m_tabControl->unknown_007C = noSubCount;
        this->RefreshTabs(true);
        this->ApplyHeaderLayout();
      }
      return 0;
    }
    case ID_MINI: {
      std::map<int, SubChatWndInfo>& names = g_session.m_noSubChatWnds;
      if (names.size() <= 1) {
        return 0;
      }
      std::map<int, SubChatWndInfo>::iterator found = names.find(this->m_currentTab);
      if (found == names.end()) {
        return 0;
      }
      const int key = found->first;
      names.erase(found);
      if (key != names.size()) {
        std::map<int, SubChatWndInfo> saved(names);
        names.clear();
        int index = 0;
        for (std::map<int, SubChatWndInfo>::iterator it = saved.begin(); it != saved.end(); ++it, ++index) {
          names[index] = it->second;
        }
      }

      std::vector<UIChatHisBox*>::iterator tab = this->m_tabs.begin() + this->m_currentTab;
      if (tab != this->m_tabs.end()) {
        this->RemoveChild(*tab, 1);
        this->m_tabs.erase(tab);
      }
      this->m_tabControl->RemoveTab(this->m_currentTab);
      this->m_tabWidth = this->m_tabControl->geometry.width / names.size();

      RECT rectangle;
      rectangle.left = 3;
      rectangle.top = 0;
      rectangle.right = this->m_tabWidth + this->m_tabControl->geometry.width + 3;
      rectangle.bottom = this->m_tabControl->geometry.height;
      this->FillSurface(0xFFFF00FF, &rectangle);
      if (this->m_currentTab > 0) {
        --this->m_currentTab;
        this->m_tabControl->unknown_007C = this->m_currentTab;
      }
      this->RefreshTabs(true);
      this->ApplyHeaderLayout();
      if (UIWindow* window = g_windowMgr.LookupWindow(0x84)) {
        window->SendMsg(this, 6, 0xC1, this->m_currentTab, 1, 1);
      }
      return 0;
    }
    default:
      return 0;
    }
  case UM_RESIZE:
    if (a == 2 && static_cast<unsigned char>(g_windowMgr.unknown_03F4) != 0) {
      const int top = this->m_inputOffsetY + 0x38;
      if (c < top) {
        if (this->geometry.unknown_0030 != 0) {
          this->ResizeRows(0);
        }
      } else {
        int rows = (c - top) / 14 + 1;
        if (rows > 15) {
          rows = 15;
        }
        if (this->geometry.unknown_0030 != rows) {
          this->ResizeRows(rows);
        }
      }
    }
    return 0;
  case UM_TOGGLEPUSH:
    if (static_cast<unsigned char>(g_windowMgr.unknown_03F4) != 0) {
      const int step = a == 0 ? 1 : -1;
      int rows = step;
      if (this->geometry.unknown_0030 != 0) {
        rows = static_cast<int>(this->geometry.unknown_0030) + step * 3;
      }
      if (rows > 0) {
        this->ResizeRows(rows / 3 * 3 + 1);
      } else {
        this->ResizeRows(this->geometry.unknown_0030 != 0 ? 0 : rows);
      }
    }
    return 0;
  case UM_PAGEDOWNPUSH: {
    UIListBox* tab = static_cast<UIListBox*>(this->m_tabs[this->m_currentTab]);
    if (tab != nullptr && tab->verticalOverflow != 0) {
      if (HIBYTE(GetKeyState(VK_CONTROL))) {
        tab->SendMsg(nullptr, 7, tab->LineCount(), 0, 0, 0);
      } else {
        tab->SendMsg(nullptr, 10, 0, 0, 0, 0);
      }
    }
    return 0;
  }
  case UM_PAGEUPPUSH: {
    UIListBox* tab = static_cast<UIListBox*>(this->m_tabs[this->m_currentTab]);
    if (tab != nullptr && tab->verticalOverflow != 0) {
      if (HIBYTE(GetKeyState(VK_CONTROL))) {
        tab->SendMsg(nullptr, 7, -tab->LineCount(), 0, 0, 0);
      } else {
        tab->SendMsg(nullptr, 9, 0, 0, 0, 0);
      }
    }
    return 0;
  }
  case UM_CHANGESTATUS:
    this->ResizeRows(a);
    return 0;
  case UM_OPENCHATWND:
    if (static_cast<unsigned char>(g_windowMgr.unknown_03F4) != 0 && this->geometry.unknown_0030 == 0) {
      this->ResizeRows(1);
    }
    return 0;
  }

  return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

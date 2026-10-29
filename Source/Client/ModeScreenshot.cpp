#include "Client/ModeScreenshot.h"

#include "Client/Application/Launcher.h"
#include "Client/Mode/CMode.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstring>
#include <direct.h>
#include <io.h>
#include <windows.h>

// GLOBAL: gl_24743
int g_screenshotIndex;

// OFFSET: 0x00AD63F0
// FUNCTION: fn_17830
void SanitizeScreenshotName(std::string& name) {
  for (std::size_t i = 0; i < name.size(); ++i) {
    const char c = name[i];
    if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
      name[i] = '_';
    }
  }
}

// OFFSET: 0x00AD6A40
// FUNCTION: fn_17848
void CMode::TakeScreenshot() {
  if (!this->screenshotRequested) {
    return;
  }

  SYSTEMTIME time;
  GetLocalTime(&time);

  char text[128];
  std::memset(text, 0, sizeof(text));

  const char* dayNames[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  if (g_serviceType != 0) {
    std::sprintf(
      text, "%d/%02d/%02d/%s  %02d: %02d: %02d", time.wYear, time.wMonth, time.wDay, dayNames[time.wDayOfWeek],
      time.wHour, time.wMinute, time.wSecond
    );
  } else {
    int gid = static_cast<int>(g_session.m_gid);
    if (gid != 0) {
      char code[12] = {};
      int divisor = 100000000;
      int length = 0;
      do {
        int digit = 0;
        while (gid >= divisor) {
          gid -= divisor;
          ++digit;
        }
        divisor /= 10;

        if (length > 0) {
          ++length;
        } else if (digit > 0) {
          length = 1;
        }

        if (digit == 0) {
          if (length > 0) {
            code[length - 1] = 'R';
          }
        } else if (digit == 1) {
          code[length - 1] = 'O';
        } else if (digit == 2) {
          code[length - 1] = 'H';
        } else if (digit == 3) {
          code[length - 1] = 'U';
        } else if (digit == 4) {
          code[length - 1] = 'T';
        } else if (digit == 5) {
          code[length - 1] = 'N';
        } else if (digit == 6) {
          code[length - 1] = 'A';
        } else if (digit == 7) {
          code[length - 1] = 'S';
        } else if (digit == 8) {
          code[length - 1] = 'E';
        } else if (digit == 9) {
          code[length - 1] = 'W';
        }
      } while (divisor > 0);

      std::sprintf(
        text, "%d/%02d/%02d/%s  %02d: %02d: %02d     %s", time.wYear, time.wMonth, time.wDay,
        dayNames[time.wDayOfWeek], time.wHour, time.wMinute, time.wSecond, code
      );
    }
  }

  DrawBackBufferText(5, g_renderer->viewport.height - 16, text, 0);
  DrawBackBufferText(3, g_renderer->viewport.height - 16, text, 0);
  DrawBackBufferText(4, g_renderer->viewport.height - 15, text, 0);
  DrawBackBufferText(4, g_renderer->viewport.height - 17, text, 0);
  DrawBackBufferText(4, g_renderer->viewport.height - 16, text, 0xFFFFFF);

  this->screenshotRequested = 0;

  std::string directory = FormatOwnedString("%s\\ScreenShot", g_applicationDirectory);
  if (_access(directory.c_str(), 0) == 0 || _mkdir(directory.c_str()) == 0) {
    std::string characterName(g_session.m_serverName);
    SanitizeScreenshotName(characterName);
    SanitizeScreenshotName(this->screenshotName);

    std::string base = directory + std::string("\\") + this->screenshotName + std::string("screen");
    std::string extension(".jpg");
    base += characterName;

    std::string path = base + FormatOwnedString("%03d", g_screenshotIndex++) + extension;
    while (_access(path.c_str(), 0) == 0) {
      path = base + FormatOwnedString("%03d", g_screenshotIndex++) + extension;
    }

    g_renderer->SaveLogo(path);
    path += " is Saved.";
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(path.c_str()), 0xFF00, 0, 0, 0);
    this->screenshotName = "";
  }
}

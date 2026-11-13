#ifndef RAGNAROK_SOURCE_UI_NAVIGATION_NAVIGATIONLINK_H
#define RAGNAROK_SOURCE_UI_NAVIGATION_NAVIGATIONLINK_H

#include <string>
#include <utility>
#include <vector>

struct NavigationLinkParser {
  NavigationLinkParser();
  ~NavigationLinkParser();

  bool HasUnownedItemLink(std::string text);
  bool HasLink(std::string text);
  __declspec(noinline) std::string ExpandMessages(std::string text);
  std::string EncodeNavigationNumber(int value);
  bool ParseNavigationLink(std::string value, std::string& name, int& x, int& y);
  const std::string ExtractNavigationLink(std::string value);
  std::pair<bool, std::string> RemoveLineBreakTag(std::string value);
  void SplitMessageArguments(const char* text, std::vector<std::string>& arguments);
  std::string FormatMessageArguments(std::string value, int messageId);
  std::string FormatNavigationLink(std::string value, bool brackets);

  /* 0000 */ bool m_ready;
};

extern NavigationLinkParser* g_navigationLinkParser;

// INLINED
inline NavigationLinkParser* GetNavigationLinkParser() {
  return g_navigationLinkParser;
}
extern std::string g_naviLinkTagBegin;
extern std::string g_naviLinkTagEnd;

bool CreateNavigationLinkParser();
void DestroyNavigationLinkParser();
int __stdcall DecodeNavigationNumber(std::string value);
void DisplayNavigationLink(std::string& output, std::string value);

#endif

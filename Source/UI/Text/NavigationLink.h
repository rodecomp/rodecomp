#ifndef RAGNAROK_SOURCE_UI_TEXT_NAVIGATIONLINK_H
#define RAGNAROK_SOURCE_UI_TEXT_NAVIGATIONLINK_H

#include <string>
#include <utility>

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

  /* 0000 */ bool m_ready;
};

extern NavigationLinkParser* g_navigationLinkParser;
extern std::string g_naviLinkTagBegin;
extern std::string g_naviLinkTagEnd;

bool CreateNavigationLinkParser();
void DestroyNavigationLinkParser();
std::string FormatMessageArguments(std::string value, int messageId);
int DecodeNavigationNumber(std::string value);
std::string FormatNavigationLink(std::string value, bool brackets);
void DisplayNavigationLink(std::string& output, std::string value);

#endif

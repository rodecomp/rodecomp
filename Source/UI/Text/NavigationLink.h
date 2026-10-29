#ifndef RAGNAROK_SOURCE_UI_TEXT_NAVIGATIONLINK_H
#define RAGNAROK_SOURCE_UI_TEXT_NAVIGATIONLINK_H

#include <string>

struct NavigationLinkParser {
  NavigationLinkParser() : m_ready(true) {}

  /* 0000 */ bool m_ready;
};

extern NavigationLinkParser* g_navigationLinkParser;

bool CreateNavigationLinkParser();
void DestroyNavigationLinkParser();
std::string FormatMessageArguments(std::string value, int messageId);
int DecodeNavigationNumber(std::string value);
std::string EncodeNavigationNumber(int value);
bool ParseNavigationLink(std::string value, std::string& name, int& x, int& y);
std::string FormatNavigationLink(std::string value, bool brackets);
void DisplayNavigationLink(std::string& output, std::string value);
void ExtractNavigationLink(std::string& output, std::string value);

#endif

#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMLINK_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMLINK_H

#include <cstdint>
#include <string>
#include <vector>

struct ITEM_INFO;

extern std::string g_itemLinkTagBegin;
extern std::string g_itemLinkTagEnd;

int ItemLinkMessageId(const std::string& text);

#endif

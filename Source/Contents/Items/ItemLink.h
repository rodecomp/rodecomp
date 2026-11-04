#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMLINK_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMLINK_H

#include <cstdint>
#include <string>
#include <vector>

struct ITEM_INFO;

extern std::string g_itemLinkTagBegin;
extern std::string g_itemLinkTagEnd;

void DecodeItemLinkFields(const std::string& text, std::vector<std::int32_t>& fields);
bool ItemFromLinkFields(const std::vector<std::int32_t>& fields, ITEM_INFO& item);
int ItemLinkMessageId(const std::string& text);

#endif

#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMLINK_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_ITEMLINK_H

#include <cstdint>
#include <string>
#include <vector>

struct ItemDescriptions;
struct ItemInfo;

void DecodeItemLinkFields(const std::string& text, std::vector<std::int32_t>& fields);
bool ItemFromLinkFields(const std::vector<std::int32_t>& fields, ItemInfo& item, ItemDescriptions& descriptions);
int ItemLinkMessageId(const std::string& text);

#endif

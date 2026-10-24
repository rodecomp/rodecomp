#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMLINK_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMLINK_H

#include <cstdint>
#include <string>
#include <vector>

struct ItemDescriptions;
struct ItemInfo;

// Decoder retains a 260-word work array, including untouched prior entries.
void DecodeItemLinkFields(const std::string& text, std::vector<std::int32_t>& fields);
bool ItemFromLinkFields(const std::vector<std::int32_t>& fields, ItemInfo& item, ItemDescriptions& descriptions);
int ItemLinkMessageId(const std::string& text);

#endif

#ifndef RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADEREXTERNAL_H
#define RAGNAROK_SOURCE_RESOURCE_JSON_JSONCPPLOADEREXTERNAL_H

extern "C" {
void BsonDecodeOptionsConstruct();
void JsonDecodeOptionsConstruct();
void JsonOptionsCommonDestroy();
void JsonDefaultParsingHandler();
void JsonSwap();
void JsonDestroy();
void JsonCopyConstruct();
void* __cdecl JsonNull();
}

#endif

#ifndef RAGNAROK_SOURCE_RESOURCE_XMLNODE_H
#define RAGNAROK_SOURCE_RESOURCE_XMLNODE_H

#include <cstdint>
#include <string>

struct XmlNode {
  explicit XmlNode(const char* name);
  ~XmlNode();
  XmlNode* AppendSibling(XmlNode* node);
  void Clear();
  XmlNode* FindChild(const char* name);
  XmlNode* FindSibling(const char* name);

  /* 0000 */ const char* name;
  /* 0004 */ std::uint32_t unknown_0004;
  /* 0008 */ std::uint32_t unknown_0008;
  /* 000C */ std::string text;
  /* 0024 */ XmlNode* sibling;
  /* 0028 */ XmlNode* child;
};

struct XmlDocument : XmlNode {
  void Reset();

  /* 002C */ void* buffer;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(XmlNode) == 44);
static_assert(sizeof(XmlDocument) == 48);
#endif

#endif

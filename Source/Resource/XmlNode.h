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
  const std::string& Text();
  std::string DecodeEntity(const char* begin, const char* end);

  /* 0000 */ const char* name;
  /* 0004 */ char* unknown_0004;
  /* 0008 */ char* unknown_0008;
  /* 000C */ std::string text;
  /* 0024 */ XmlNode* sibling;
  /* 0028 */ XmlNode* child;
};

struct XmlDocument : XmlNode {
  // INLINED
  XmlDocument() : XmlNode("XMLDocument"), buffer(nullptr) {
  }
  void Reset();
  char* TerminateName(char* begin, char* end);
  char* ParseElements(XmlNode* parent, char* cursor, char* end);
  bool Parse(const char* begin, const char* end);

  /* 002C */ void* buffer;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(XmlNode) == 44);
static_assert(sizeof(XmlDocument) == 48);
#endif

#endif

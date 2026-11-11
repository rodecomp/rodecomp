#include "Resource/Xml/XmlNode.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00AFA470
// FUNCTION: fn_18291
XmlNode::XmlNode(const char* name) : name(name), sourceBegin(0), sourceEnd(0), sibling(nullptr), child(nullptr) {
}

// OFFSET: 0x00AFA500
// FUNCTION: fn_18292
XmlNode::~XmlNode() {
  this->Clear();
}

// OFFSET: 0x00AFA590
// FUNCTION: fn_18293
XmlNode* XmlNode::AppendSibling(XmlNode* node) {
  XmlNode* current = this;
  while (current->sibling != nullptr) {
    current = current->sibling;
  }

  current->sibling = node;
  return node;
}

// OFFSET: 0x00AFA5C0
// FUNCTION: fn_18294
void XmlDocument::Reset() {
  this->Clear();
  if (this->buffer != nullptr) {
    delete[] static_cast<char*>(this->buffer);
    this->buffer = nullptr;
  }
}

// OFFSET: 0x00AFA5F0
// FUNCTION: fn_18295
void XmlNode::Clear() {
  if (this->sibling != nullptr) {
    delete this->sibling;
    this->sibling = nullptr;
  }

  if (this->child != nullptr) {
    delete this->child;
    this->child = nullptr;
  }

  this->sourceBegin = 0;
  this->sourceEnd = 0;
}

// OFFSET: 0x00AFA6B0
// FUNCTION: fn_18296
XmlNode* XmlNode::FindChild(const char* name) {
  XmlNode* first = this->child;
  if (first == nullptr) {
    return nullptr;
  }

  if (std::strcmp(first->name, name) == 0) {
    return first;
  }

  return first->FindSibling(name);
}

// OFFSET: 0x00AFA710
// FUNCTION: fn_18297
XmlNode* XmlNode::FindSibling(const char* name) {
  for (XmlNode* current = this->sibling; current != nullptr; current = current->sibling) {
    if (std::strcmp(current->name, name) == 0) {
      return current;
    }
  }

  return nullptr;
}

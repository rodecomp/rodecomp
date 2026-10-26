#include "Resource/XmlNode.h"

#include <cstdlib>
#include <cstring>

// FUNCTION: fn_18291
XmlNode::XmlNode(const char* name) : name(name), unknown_0004(0), unknown_0008(0), sibling(nullptr), child(nullptr) {
}

// FUNCTION: fn_18292
XmlNode::~XmlNode() {
  this->Clear();
}

// FUNCTION: fn_18293
XmlNode* XmlNode::AppendSibling(XmlNode* node) {
  XmlNode* current = this;
  while (current->sibling != nullptr) {
    current = current->sibling;
  }

  current->sibling = node;
  return node;
}

// FUNCTION: fn_18294
void XmlDocument::Reset() {
  this->Clear();
  if (this->buffer != nullptr) {
    std::free(this->buffer);
    this->buffer = nullptr;
  }
}

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

  this->unknown_0008 = 0;
  this->unknown_0004 = 0;
}

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

// FUNCTION: fn_18297
XmlNode* XmlNode::FindSibling(const char* name) {
  for (XmlNode* current = this->sibling; current != nullptr; current = current->sibling) {
    if (std::strcmp(current->name, name) == 0) {
      return current;
    }
  }

  return nullptr;
}

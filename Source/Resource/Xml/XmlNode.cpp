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

// OFFSET: 0x00AFA560
// FUNCTION: fn_46710
XmlNode* XmlNode::AppendChild(XmlNode* node) {
  if (this->child == nullptr) {
    this->child = node;
    return node;
  }

  return this->child->AppendSibling(node);
}

// OFFSET: 0x00AFA590
// FUNCTION: fn_18293
XmlNode* XmlNode::AppendSibling(XmlNode* node) {
  if (this->sibling == nullptr) {
    this->sibling = node;
    return node;
  }

  return this->sibling->AppendSibling(node);
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

// OFFSET: 0x00AFA770
// FUNCTION: fn_18298
const std::string& XmlNode::Text() {
  if (this->text.size() == 0) {
    char* cursor = this->sourceBegin;
    while (*cursor != '>' && *cursor != '\0') {
      ++cursor;
    }

    char* end = this->sourceEnd;
    ++cursor;
    while (cursor != end) {
      for (XmlNode* node = this->child; node != nullptr; node = node->child) {
        if (cursor == node->sourceBegin) {
          cursor = node->sourceEnd;
        }

        for (XmlNode* sibling = node->sibling; sibling != nullptr; sibling = sibling->sibling) {
          if (cursor == sibling->sourceBegin) {
            cursor = sibling->sourceEnd;
          }
        }
      }

      const char value = *cursor;
      if (value == '<') {
        break;
      }

      if (value == '\r' || value == '\n') {
        ++cursor;
        continue;
      }

      char* next = cursor + 1;
      if (value == '&') {
        if (std::strcmp(this->name, "registrationweb") != 0) {
          this->text += this->DecodeEntity(next, end);
          while (cursor != end && *cursor != ';') {
            ++cursor;
          }
          ++cursor;
        } else {
          this->text.append(1, '&');
          cursor = next;
        }
      } else {
        this->text.append(1, value);
        cursor = next;
      }
    }
  }

  return this->text;
}

// OFFSET: 0x00AFA940
// FUNCTION: fn_18299
std::string XmlNode::DecodeEntity(const char* begin, const char* end) {
  if (std::memcmp(begin, "lt", 2) == 0) {
    return std::string(1, '<');
  }

  if (std::memcmp(begin, "gt", 2) == 0) {
    return std::string(1, '>');
  }

  if (std::memcmp(begin, "amp", 3) == 0) {
    return std::string(1, '&');
  }

  return std::string();
}

// OFFSET: 0x00AFAA50
// FUNCTION: fn_18300
char* XmlDocument::TerminateName(char* begin, char* end) {
  char* cursor = begin;
  while (cursor != end && *cursor != ' ' && *cursor != '\t' && *cursor != '>' && *cursor != '/') {
    ++cursor;
  }

  *cursor = '\0';
  return begin;
}

// OFFSET: 0x00AFAA90
// FUNCTION: fn_18301
char* XmlDocument::ParseElements(XmlNode* parent, char* cursor, char* end) {
  while (cursor != end) {
    const char value = *cursor;
    if (value == '<') {
      char* last = end - 1;
      if (cursor[1] == '/') {
        char* close = cursor;
        while (close != last && *close != '>') {
          ++close;
        }

        char* next = close + 1;
        if (std::strcmp(this->TerminateName(cursor + 2, end), parent->name) == 0) {
          parent->sourceEnd = next;
          return next;
        }

        cursor = next;
        continue;
      }

      if (cursor[1] == '?') {
        while (cursor != last && *cursor != '>') {
          ++cursor;
        }

        ++cursor;
        continue;
      }

      if (cursor[1] == '!') {
        while (cursor != last && *cursor != '>') {
          ++cursor;
        }

        ++cursor;
        continue;
      }

      char* close = cursor;
      while (close != last && *close != '>') {
        ++close;
      }

      char* next = close + 1;
      XmlNode* node = new XmlNode(this->TerminateName(cursor + 1, end));
      parent->AppendChild(node);

      if (next[-2] == '/') {
        cursor = next;
        continue;
      }

      node->sourceBegin = cursor;
      cursor = this->ParseElements(node, next, end);
    } else {
      if (value == '&' && std::strcmp(parent->name, "registrationweb") != 0) {
        while (cursor != end && *cursor != ';') {
          ++cursor;
        }
      }

      ++cursor;
    }
  }

  return cursor;
}

// OFFSET: 0x00AFAD00
// FUNCTION: fn_18302
bool XmlDocument::Parse(const char* begin, const char* end) {
  if (this->child != nullptr) {
    this->Reset();
  }

  const int length = static_cast<int>(end - begin);
  this->buffer = new char[length + 1];
  std::memcpy(this->buffer, begin, length);
  static_cast<char*>(this->buffer)[length] = '\0';
  this->sourceBegin = static_cast<char*>(this->buffer);
  this->sourceEnd = this->sourceBegin + length;
  this->ParseElements(this, this->sourceBegin, this->sourceEnd);
  return this->child != nullptr;
}

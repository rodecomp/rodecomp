#include "Resource/Xml/XmlNode.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x00AFA770
// FUNCTION: fn_18298
const std::string& XmlNode::Text() {
  if (this->text.size() == 0) {
    char* cursor = this->sourceBegin;
    while (*cursor != '>' && *cursor != '\0') {
      ++cursor;
    }

    char* end = this->sourceEnd;
    for (++cursor; cursor != end; ++cursor) {
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
        continue;
      }

      if (value == '&') {
        if (std::strcmp(this->name, "registrationweb") != 0) {
          this->text += this->DecodeEntity(cursor + 1, end);
          while (cursor != end && *cursor != ';') {
            ++cursor;
          }
        } else {
          this->text.append(1, '&');
        }
      } else {
        this->text.append(1, value);
      }
    }
  }

  return this->text;
}

// OFFSET: 0x00AFA940
// FUNCTION: fn_18299
std::string XmlNode::DecodeEntity(const char* begin, const char* end) {
  if (begin[0] == 'l' && begin[1] == 't') {
    return std::string(1, '<');
  }

  if (begin[0] == 'g' && begin[1] == 't') {
    return std::string(1, '>');
  }

  if (begin[0] == 'a' && begin[1] == 'm' && begin[2] == 'p') {
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
      if (parent->child == nullptr) {
        parent->child = node;
      } else {
        parent->child->AppendSibling(node);
      }

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

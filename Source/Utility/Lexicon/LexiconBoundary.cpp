#include "Utility/Lexicon/LexiconBoundary.h"

#include "Utility/Lexicon/LexiconSerializer.h"
#include "File/File.h"

#include <cstring>

// GLOBAL: gl_24545
std::uint32_t g_lexiconByteClasses[256] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0x1000000, 0x400, 0x400, 0x400, 0x400, 0x400, 0x400, 0x400,
  0x10, 0x20, 0x400, 0x400, 0x400, 0x400, 0x404, 0x400,
  0x10002, 0x10002, 0x10002, 0x10002, 0x10002, 0x10002, 0x10002, 0x10002,
  0x10002, 0x10002, 0x400, 0x400, 0x10, 0x400, 0x20, 0x40,
  0x400, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002,
  0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002,
  0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002, 0x100002,
  0x100002, 0x100002, 0x100002, 0x10, 0x400, 0x20, 0x400, 0x400,
  0x400, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002,
  0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002,
  0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002, 0x200002,
  0x200002, 0x200002, 0x200002, 0x10, 0x400, 0x20, 0x400, 0,
  0x20001, 0x101, 0x101, 0x101, 0x101, 0x400, 0x8101, 0x8101,
  0x8101, 0x8101, 0x4101, 0x8101, 0x8101, 0x8101, 0x8101, 0x4101,
  0x20001, 0x414, 0x424, 0x414, 0x424, 0x400, 0x400, 0x400,
  0x4101, 0x4101, 0x4101, 0x8101, 0x8101, 0x8101, 0x8101, 0x4101,
  0x1000000, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001,
  0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001,
  0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001,
  0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001,
  0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001,
  0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x20001, 0x201,
  0x2001, 0x4101, 0x2001, 0x2001, 0x101, 0x101, 0x101, 0x101,
  0x101, 0x101, 0x4101, 0x2000000, 0x400, 0, 0, 0x400,
  0x1001, 0x1001, 0x1001, 0x1001, 0x1001, 0x2001, 0x1, 0x4101,
  0x8101, 0x8101, 0x8101, 0x8101, 0x4101, 0x4101, 0x4101, 0x400,
  0x10001, 0x10001, 0x10001, 0x10001, 0x10001, 0x10001, 0x10001, 0x10001,
  0x10001, 0x400, 0x400, 0x101, 0x101, 0x101, 0, 0,
};

// INLINED
static bool HasClass(std::uint32_t classes, int bit) {
  return ((classes >> bit) & 1) != 0;
}

// INLINED
static bool MatchesClasses(const std::uint32_t& before, const std::uint32_t& after, unsigned char options) {
  if ((options & 2) != 0) {
    if (((before & 0x470) != 0) == ((after & 0x470) != 0) &&
        ((HasClass(before, 12) && HasClass(after, 0)) || ((before & 0x470) != 0 && (after & 0x470) != 0) ||
         (HasClass(before, 1) && HasClass(after, 1)))) {
      return true;
    }
  } else {
    if (!HasClass(after, 4) || HasClass(before, 4)) {
      if ((after & 0x60) != 0 || HasClass(before, 4) || (HasClass(before, 12) && HasClass(after, 0)) ||
          (HasClass(before, 10) && (after & 0x10002) != 0) || HasClass(after, 10)) {
        return true;
      }
    }
  }

  return false;
}

// OFFSET: 0x00AC4EA0
// FUNCTION: fn_17634
bool LexiconBoundary::Matches(unsigned char previous, unsigned char current, unsigned char options) const {
  if (previous == 0xDD) {
    return true;
  }

  if (HasClass(this->flags[previous], 0) && (this->flags[current] & 0x2100) != 0) {
    return true;
  }

  if (current == 32) {
    return true;
  }

  if (previous != 32) {
    const std::uint32_t& after = this->flags[current];
    const std::uint32_t& before = this->flags[previous];
    return MatchesClasses(before, after, options);
  }

  return false;
}

// OFFSET: 0x00AC3D50
// FUNCTION: fn_45867
LexiconSerializer::LexiconSerializer() {
  this->root = nullptr;
  this->wordCount = 0;
  this->serializedSize = 0;
  this->nodeCount = 0;
  this->buffer = nullptr;
  std::memset(&this->header, 0, sizeof(this->header));
}

// OFFSET: 0x00AC3E30
// FUNCTION: fn_45871
LexiconSerializer::~LexiconSerializer() {
  if (this->buffer != nullptr) {
    delete this->buffer;
  }
  this->Destroy(this->root);
}

// OFFSET: 0x00AC3D90
// FUNCTION: fn_45870
LexiconBoundary::~LexiconBoundary() {
  delete this->serializer;
}

// OFFSET: 0x00AC4A40
// FUNCTION: fn_17632
bool LexiconBoundary::Load() {
  delete this->serializer;
  this->serializer = new LexiconSerializer;
  this->serializer->Load("..\\ftwrk.lex");
  std::memcpy(this->flags, g_lexiconByteClasses, sizeof(this->flags));
  return this->serializer != nullptr;
}

// OFFSET: 0x00AC3FF0
// FUNCTION: fn_17630
void LexiconSerializer::Destroy(LexiconNode* node) {
  if (node != nullptr) {
    this->Destroy(node->child);
    this->Destroy(node->sibling);
    delete node;
  }
}

// OFFSET: 0x00AC3FC0
// FUNCTION: fn_45868
unsigned char LexiconSerializer::UnbranchedLength(const LexiconNode* node) const {
  unsigned char length = 0;
  for (; node != nullptr && node->sibling == nullptr; node = node->child) {
    ++length;
  }
  return length;
}

// OFFSET: 0x00AC4030
// FUNCTION: fn_45869
int LexiconSerializer::SiblingCount(const LexiconNode* node) const {
  int count = 0;
  for (; node != nullptr; node = node->sibling) {
    ++count;
  }
  return count;
}

// OFFSET: 0x00AC4D40
// FUNCTION: fn_17633
std::uint32_t LexiconSerializer::Serialize(LexiconNode* node, std::uint32_t offset, unsigned char terminal) {
  int siblings = this->SiblingCount(node);

  std::uint32_t next = offset + static_cast<std::uint32_t>(siblings) * 3u + 1u;
  while (siblings > 0) {
    this->buffer[offset++] = static_cast<unsigned char>((terminal << 7) | (next >> 16));
    this->buffer[offset++] = static_cast<unsigned char>(next >> 8);
    this->buffer[offset++] = static_cast<unsigned char>(next);

    const unsigned char chain = this->UnbranchedLength(node);

    if (chain > 1) {
      this->buffer[next++] = chain;

      LexiconNode* current = node;
      unsigned char lastTerminal;
      for (unsigned char index = 0; index < chain; ++index) {
        this->buffer[next++] = current->value;
        lastTerminal = current->terminal;
        if (lastTerminal == 1) {
          this->buffer[next++] = 2;
        }

        current = current->child;
      }

      if (current != nullptr) {
        const unsigned char last = this->buffer[next - 1];
        if (last < 0x20) {
          this->buffer[next - 1] = last | 4;
        } else {
          this->buffer[next++] = 4;
        }

        next = this->Serialize(current, next, lastTerminal);
      }
    } else {
      this->buffer[next++] = node->value;
      if (node->child != nullptr) {
        next = this->Serialize(node->child, next, node->terminal);
      } else {
        this->buffer[next++] = 0xC0;
      }
    }

    node = node->sibling;
    --siblings;
  }

  this->buffer[offset] = static_cast<unsigned char>((terminal << 7) | 0x40);
  return next;
}

// OFFSET: 0x00AC4FA0
// FUNCTION: fn_17635
bool LexiconSerializer::Load(const char* path) {
  bool result = false;
  CFile file;
  if (file.Open(path, 0, true, false)) {
    if (file.Read(&this->header, sizeof(this->header)) && this->header.total == this->header.nodeCount + this->header.wordCount * 2) {
      this->buffer = new unsigned char[this->header.size];
      if (this->buffer && file.Read(this->buffer, this->header.size)) {
        result = true;
        this->wordCount = this->header.wordCount;
        this->nodeCount = this->header.nodeCount;
        this->serializedSize = this->header.size;
        this->header.loaded = result;
      }
    }

    if (!result) {
      if (this->buffer) {
        delete this->buffer;
        this->buffer = nullptr;
      }

      this->header.loaded = result;
    }
  } else {
    result = false;
  }

  return result;
}

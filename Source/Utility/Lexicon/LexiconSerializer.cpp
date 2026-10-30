#include "Utility/Lexicon/LexiconSerializer.h"

#include "File/File.h"

// OFFSET: 0x00AC3FF0
// FUNCTION: fn_17630
void LexiconSerializer::Destroy(LexiconNode* node) {
  if (node != nullptr) {
    this->Destroy(node->child);
    this->Destroy(node->sibling);
    delete node;
  }
}

// OFFSET: 0x00AC4D40
// FUNCTION: fn_17633
std::uint32_t LexiconSerializer::Serialize(LexiconNode* node, std::uint32_t offset, unsigned char terminal) {
  int siblings = 0;
  for (LexiconNode* current = node; current != nullptr; current = current->sibling) {
    ++siblings;
  }

  std::uint32_t next = offset + static_cast<std::uint32_t>(siblings) * 3u + 1u;
  while (siblings > 0) {
    this->buffer[offset] = static_cast<unsigned char>((terminal << 7) | (next >> 16));
    this->buffer[offset + 1] = static_cast<unsigned char>(next >> 8);
    this->buffer[offset + 2] = static_cast<unsigned char>(next);
    offset += 3;

    unsigned char chain = 0;
    for (LexiconNode* current = node; current != nullptr && current->sibling == nullptr; current = current->child) {
      ++chain;
    }

    if (chain > 1) {
      this->buffer[next++] = chain;
      LexiconNode* current = node;
      unsigned char lastTerminal;
      for (int remaining = chain; remaining != 0; --remaining) {
        this->buffer[next++] = current->value;
        lastTerminal = current->terminal;
        if (lastTerminal == 1) {
          this->buffer[next++] = 2;
        }

        current = current->child;
      }

      if (current != nullptr) {
        const unsigned char last = this->buffer[next - 1];
        if (last >= 0x20) {
          this->buffer[next++] = 4;
        } else {
          this->buffer[next - 1] = last | 4;
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

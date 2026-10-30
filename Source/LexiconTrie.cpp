#include "Utility/Lexicon/LexiconSerializer.h"

// OFFSET: 0x00AC3EB0
// FUNCTION: fn_45446
bool LexiconSerializer::Insert(const unsigned char* word, int length) {
  int index = 0;
  if (length == 0) {
    return false;
  }

  if (this->wordCount == 0) {
    this->root = new LexiconNode;
    this->root->child = nullptr;
    this->root->sibling = nullptr;
    this->root->value = word[0];
    this->root->terminal = 0;
    ++this->nodeCount;
  }

  LexiconNode* node = this->root;
  for (;;) {
    if (node->value != word[index]) {
      LexiconNode** link = &node->sibling;
      node = *link;
      if (node != nullptr && node->value <= word[index]) {
        continue;
      }

      node = new LexiconNode;
      ++this->nodeCount;
      node->value = word[index];
      node->sibling = *link;
      node->child = nullptr;
      node->terminal = 0;
      *link = node;
    }

    for (;;) {
      if (++index == length) {
        if (node->terminal != 0) {
          return false;
        }

        node->terminal = 1;
        ++this->wordCount;
        return true;
      }

      LexiconNode** link = &node->child;
      node = *link;
      if (node != nullptr && node->value <= word[index]) {
        break;
      }

      node = new LexiconNode;
      ++this->nodeCount;
      node->value = word[index];
      node->sibling = *link;
      node->child = nullptr;
      node->terminal = 0;
      *link = node;
    }
  }
}

#include "../../headers/BTree.h"

using namespace BTreeNS;

BTreeNodeCell::BTreeNodeCell(int _key, int _page, int _offset)
        : child(MAX_SIZE), key(_key), page(_page), offset(_offset) {};

bool BTreeNodeCell::isLastElementInNode() { return child != MAX_SIZE && key == MAX_SIZE; }

bool BTreeNodeCell::isDefault() {
    return child == MAX_SIZE && key == MAX_SIZE && page == MAX_SIZE && offset == MAX_SIZE;
}

void BTreeNodeCell::swapCellValues(std::shared_ptr<BTreeNodeCell> toSwap) {
    std::swap(this->key, toSwap->key);
    std::swap(this->page, toSwap->page);
    std::swap(this->offset, toSwap->offset);
}

void BTreeNodeCell::swapChildren(std::shared_ptr<BTreeNodeCell> toSwap) { std::swap(this->child, toSwap->child); }

int BTreeNodeCell::getKey() { return key; }

int BTreeNodeCell::getChild() { return child; }

int BTreeNodeCell::getPage() { return page; }

int BTreeNodeCell::getOffset() { return offset; }


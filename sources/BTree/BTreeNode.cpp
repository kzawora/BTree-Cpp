#include "../../headers/BTree.h"
using namespace BTreeNS;

// BTreeNode::

BTreeNode::BTreeNode(int _index, int _parent, bool _leaf)
        : index(_index), parent(_parent), leaf(_leaf) {
    cells.push_back(std::make_shared<BTreeNodeCell>());
}

int BTreeNode::size() { return cells.size() - 1; }

void BTreeNode::insert(std::shared_ptr<BTreeNodeCell> newCell) {
    if (cells.size() > BTREE_2D + 1)
        throw "Cannot insert cell into node! Node has no free space!";

    for (size_t i = 0; i < cells.size(); i++) {
        if (this->cells[i]->isLastElementInNode() || this->cells[i]->getKey() > newCell->getKey()) {
            cells.insert(cells.begin() + i, newCell);
            return;
        }
    }
}

// TODO: test this
std::shared_ptr<BTreeNodeCell> BTreeNode::pop(int _key) {
    for (size_t i = 0; i < cells.size(); i++) {
        if (cells[i]->getKey() == _key) {
            auto poppedCell = cells[i];
            cells.erase(cells.begin() + i);
            return poppedCell;
        }
    }
    return nullptr;
}

// TODO: test this
std::shared_ptr<BTreeNodeCell> BTreeNode::getCellByKey(int _key) {
    if (cells.size() < 2) {
        throw "Cell not found!";
    }
    auto prev_key = 0;
    for (auto cell : cells) {
        if (prev_key <= _key && _key <= cell->getKey())
            return cell;
        prev_key = cell->getKey();
    }
    throw "Cell not found!";
}

std::shared_ptr<BTreeNodeCell> BTreeNode::getCellByChild(int _cell) {
    for (auto cell : cells) {
        if (cell->getChild() == _cell)
            return cell;
    }
    return nullptr;
}

// TODO
std::shared_ptr<bytearray> BTreeNode::serialize() {
    std::vector<int> data;
    data.push_back(parent);
    int dummies = BTREE_ELEMENTS_PER_PAGE - cells.size();
    for (auto cell : cells) {
        data.push_back(cell->getChild());
        data.push_back(cell->getKey());
        data.push_back(cell->getPage());
        data.push_back(cell->getOffset());
    }
    for (int i = 0; i < dummies * 4; i++) {
        data.push_back(MAX_SIZE);
    }
    std::shared_ptr<bytearray> arr = std::make_shared<bytearray>(BTREE_PAGE_SIZE);
    memcpy(arr->arr, &data[0], BTREE_PAGE_SIZE);
    return arr;
}

std::shared_ptr<BTreeNode> BTreeNode::deserialize(std::shared_ptr<bytearray> data, int index) {
    std::shared_ptr<BTreeNode> newNode = std::make_shared<BTreeNode>(index);
    std::vector<int> newData(BTREE_ELEMENTS_PER_PAGE * 4 + 1);
    memcpy(&newData[0], data->arr, BTREE_PAGE_SIZE);
    newNode->parent = newData[0];
    newNode->cells.clear();
    for (int i = 1; i < newData.size(); i += 4) {
        std::shared_ptr<BTreeNodeCell> cell =
                std::make_shared<BTreeNodeCell>(newData[i + 1], newData[i + 2], newData[i + 3]);
        cell->child = newData[i];
        if (cell->isDefault())
            break;
        newNode->cells.push_back(cell);
    }
    if (newNode->cells.size() < 1 || !newNode->cells.back()->isLastElementInNode())
        newNode->cells.push_back(std::make_shared<BTreeNodeCell>());
    return newNode;
}


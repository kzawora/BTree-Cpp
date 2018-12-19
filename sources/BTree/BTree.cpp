#include "../../headers/BTree.h"
#include "../../headers/Storage.h"

using namespace BTreeNS;

BTree::BTree(std::string _name) {
    metadata = std::vector<int>(7);
    name = _name;
    btree = std::make_shared<BTreeStorage>(name);
    data = std::make_shared<DataStorage>(name);
}

BTree::~BTree() {
    flushMetadata();
}

void BTree::flush() {
    flushMetadata();
    btree->flush();
    data->flush();
}

void BTree::loadMetadata() {

}

void BTree::flushMetadata() {
    metadata[4] = data->nextpage; // data_next_page
    metadata[5] = data->nextoffset; // data_next_offset
    metadata[6] = btree->nextnode; // btree_next_node
    // zapisywanie metadanych do bytearray;
//  metadataStorage->setPage(0,)
}

void BTree::syncMetadataStorage() {
    data->nextpage = metadata[4]; // data_next_page
    data->nextoffset = metadata[5]; // data_next_offset
    btree->nextnode = metadata[6]; // btree_next_node
}

std::shared_ptr<BTreeNode> BTree::createTree() {
    metadata[0] = 0; // elements
    metadata[1] = 1; // nodes
    metadata[2] = 1; // height
    metadata[3] = 0; // root
    metadata[4] = 0; // data_next_page
    metadata[5] = 0; // data_next_offset
    metadata[6] = 0; // btree_next_node
    syncMetadataStorage();
    btree->clear();
    data->clear();
    auto root = btree->newNode();
    root->leaf = true;
    metadata[3] = root->index;

    return root;
}

std::shared_ptr<BTreeNode> BTree::newNode() {
    metadata[1]++; // nodes
    return btree->newNode();
}

std::tuple<std::shared_ptr<BTreeNode>, bool> BTree::getNodeForKey(int key, bool test) {
    return std::tuple<std::shared_ptr<BTreeNode>, bool>();
}

void BTree::insertCellIntoNode(std::shared_ptr<BTreeNodeCell> cell, std::shared_ptr<BTreeNode> node) {
    if (node->size > BTREE_2D)
        throw "Not enough space in node!";
    node->insert(cell);
    // overflow
    if (node->size > BTREE_2D) {
        bool status = compensate(node);
        if(!status)
            splitNode(node);
    }
}

std::vector<int> BTree::getSiblings(std::shared_ptr<BTreeNode> node) {
    if (node->parent == MAX_SIZE) // if node is root
        return std::vector<int>();
    std::vector<int> siblings;
    auto parentNode = btree->get(node->parent);
    if (parentNode->cells.size() < 2)
        return std::vector<int>();
    else if (parentNode->cells[0]->child == node->index) // rightmost index;
        siblings.push_back(node->cells[1]->child);
    else if (parentNode->cells.back()->child == node->index)  // leftmost TODO: check correctness
        siblings.push_back(node->cells[node->cells.size()-2]->child);
    else {
        for (int i = 0; i < parentNode->cells.size(); i++) {
            if(node->index == parentNode->cells[i]->child) {
                siblings.push_back(parentNode->cells[i-1]->child);
                siblings.push_back(parentNode->cells[i+1]->child);
                break;
            }
        }
    }
}

bool BTree::compensate(std::shared_ptr<BTreeNode> node) {
    return false;
}

void BTree::rotateNodes(std::shared_ptr<BTreeNode> node1, std::shared_ptr<BTreeNode> node2) {
}

void BTree::splitNode(std::shared_ptr<BTreeNode>) {

}

void BTree::rebalance(std::shared_ptr<BTreeNode> node) {

}

void BTree::mergeNodes(std::shared_ptr<BTreeNode> node1, std::shared_ptr<BTreeNode> node2) {

}

void BTree::repairNodeAfterDeletion(std::shared_ptr<BTreeNode> node) {

}

void BTree::deleteKeyFromNode(int key, std::shared_ptr<BTreeNode> node) {

}

std::shared_ptr<Record> BTree::get(int key) {
    return std::shared_ptr<Record>();
}

void BTree::set(int key, std::shared_ptr<Record> rec) {

}

void BTree::del(int key) {

}

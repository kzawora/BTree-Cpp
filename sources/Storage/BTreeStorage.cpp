#include "../../headers/Storage.h"
#include "../../headers/CommonHeader.h"


BTreeStorage::BTreeStorage(std::string name) {
    name.append(".btree");
    storage = std::make_shared<Storage>(name, BTREE_PAGE_SIZE);
    nextnode = storage->getPageCount();
}

void BTreeStorage::clear() {
    storage->clear();
    nextnode = 0;
    freenodes.clear();
}

std::shared_ptr<BTreeNS::BTreeNode> BTreeStorage::get(int index) {
    std::shared_ptr<bytearray> page = storage->getPage(index);
    std::shared_ptr<BTreeNS::BTreeNode> node = BTreeNS::BTreeNode::deserialize(page, index);
    return node;
};

void BTreeStorage::set(int index, std::shared_ptr<BTreeNS::BTreeNode> node) {
    if (nextnode == index) {
        nextnode++;
    }
    storage->setPage(index, node->serialize());
}

std::shared_ptr<BTreeNS::BTreeNode> BTreeStorage::newNode() {
    int index;
    if (freenodes.size() > 0) {
        index = freenodes.back();
        freenodes.pop_back();
    } else
        index = nextnode;
    std::shared_ptr<BTreeNS::BTreeNode> node = std::make_shared<BTreeNS::BTreeNode>(index);
    set(index, node);
    return node;
}

void BTreeStorage::flush() { storage->flush(); }

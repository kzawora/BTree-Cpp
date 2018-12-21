#include "../../headers/Storage.h"
#include "../../headers/CommonHeader.h"
#include <tuple>
#include <vector>

#define MAX_CACHE_SIZE 8

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

void BTreeStorage::addToCache(int index, std::shared_ptr<BTreeNS::BTreeNode> node) {
    for (int i = 0; i < btreecache.size(); i++) {
        if (std::get<0>(btreecache[i]) == index) {
            auto tup = btreecache[i];
            btreecache.erase(btreecache.begin() + i);
            btreecache.push_back(tup);
            return;
        }
    }
    auto tup = std::make_tuple(index, node);
    if (btreecache.size() >= MAX_CACHE_SIZE) {
        auto tup2 = btreecache[0];
        auto node2 = std::get<1>(tup2);
        set(node2->index, node2);
        btreecache.erase(btreecache.begin());
    }
    btreecache.push_back(tup);
}

std::shared_ptr<BTreeNS::BTreeNode> BTreeStorage::get(int index) {
    std::shared_ptr<BTreeNS::BTreeNode> node;

    for (int i = 0; i < btreecache.size(); i++) {
        if (std::get<0>(btreecache[i]) == index) {
            auto tup = btreecache[i];
            btreecache.erase(btreecache.begin() + i);
            btreecache.push_back(tup);
            return std::get<1>(tup);
        }
    }
    std::shared_ptr<bytearray> page = storage->getPage(index);
    node = BTreeNS::BTreeNode::deserialize(page, index);

    auto tup = std::make_tuple(index, node);
    if (btreecache.size() >= MAX_CACHE_SIZE) {
        auto tup2 = btreecache[0];
        auto node2 = std::get<1>(tup2);
        set(node2->index, node2);
        btreecache.erase(btreecache.begin());
    }
    btreecache.push_back(tup);

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
    addToCache(index, node);
    set(index, node);
    return get(index);
}

void BTreeStorage::flush() {
    for (auto tup : btreecache) {
        auto node = std::get<1>(tup);
        set(node->index, node);
    }
    storage->flush();
}

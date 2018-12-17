#include "BTree.cpp"
#include "Record.cpp"
#include "Utils.cpp"
#include <fstream>
#include <string>
#include <vector>
#pragma once

class Storage {
    std::string filename;
    int pagesize, filesize, pages;
    std::fstream file;

  public:
    Storage(std::string _name, int _pagesize) : filename(_name), pagesize(_pagesize) {
        std::ifstream file_temp(filename, std::fstream::in | std::fstream::out | std::ios::app | std::ios::binary |
                                              std::ios::ate);
        filesize = static_cast<size_t>(file_temp.tellg());
        file_temp.close();
        file.open(filename, std::ios::in | std::ios::binary | std::ios::out | std::ios::ate);
        if (filesize % pagesize != 0)
            throw "Size does not match!";
        pages = filesize / pagesize;
    }
    ~Storage() {
        file.flush();
        file.close();
    }
    void clear() {
        file.close();
        file.open(filename, std::ofstream::out | std::ofstream::trunc);
        file.close();
        file.open(filename, std::ios::in | std::ios::binary | std::ios::out | std::ios::ate);
        pages = 0;
    }
    std::shared_ptr<bytearray> getPage(int index) {
        if (index > pages)
            throw "Index out of bounds!";

        std::shared_ptr<bytearray> arr = std::make_shared<bytearray>(pagesize);
        if (pages > 0) {
            file.seekg(index * pagesize, std::ios::beg);
            file.read(arr->arr, pagesize);
        }
        return arr;
    }
    void flush() { file.flush(); }
    void setPage(int index, std::shared_ptr<bytearray> data) {

        if (index > pages)
            throw "Index too big!";
        if (data->size != pagesize)
            throw "Page data doesn't match page size!";

        if (index < pages) { // replace page
            file.seekg(index * pagesize, std::ios::beg);
            file.write(data->arr, data->size);
        } else { // append page
            file.seekg(0, std::ios::end);
            file.write(data->arr, data->size);
            pages++;
        }
        file.flush();
    }
    int getPageCount() { return pages; }
};

class BTreeStorage {
    std::shared_ptr<Storage> storage;
    int nextnode;
    std::vector<int> freenodes;
    std::shared_ptr<bytearray> page;

  public:
    BTreeStorage(std::string name) {
        name.append(".btree");
        storage = std::make_shared<Storage>(name, BTREE_PAGE_SIZE);
        nextnode = storage->getPageCount();
    }
    ~BTreeStorage() = default;
    void clear() {
        storage->clear();
        nextnode = 0;
        freenodes.clear();
    }
    std::shared_ptr<BTree::BTreeNode> get(int index) {
        std::shared_ptr<bytearray> page = storage->getPage(index);
        std::shared_ptr<BTree::BTreeNode> node = BTree::BTreeNode::deserialize(page, index);
        return node;
    };
    void set(int index, std::shared_ptr<BTree::BTreeNode> node) {
        if (nextnode == index) {
            nextnode++;
        }
        storage->setPage(index, node->serialize());
    }

    std::shared_ptr<BTree::BTreeNode> newNode() {
        int index;
        if (freenodes.size() > 0) {
            index = freenodes.back();
            freenodes.pop_back();
        } else
            index = nextnode;
        std::shared_ptr<BTree::BTreeNode> node = std::make_shared<BTree::BTreeNode>(index);
        set(index, node);
        return node;
    }
    void flush() { storage->flush(); }
};

class DataStorage {
    std::shared_ptr<Storage> storage;
    int nextpage, nextoffset;

  public:
    DataStorage(std::string name) : nextoffset(0) {
        name.append(".data");
        storage = std::make_shared<Storage>(name, RECORD_PAGE_SIZE);
        nextpage = storage->getPageCount();
    }
    void clear() {
        storage->clear();
        nextpage = 0;
        nextoffset = 0;
    }
    std::shared_ptr<Record> get(int pageNo, int offset) {
        std::shared_ptr<bytearray> page = storage->getPage(pageNo);
		std::shared_ptr<bytearray> recordData = std::make_shared<bytearray>(RECORD_SIZE);
        memcpy(recordData->arr, page->arr + offset, RECORD_SIZE);
        return Record::deserialize(recordData);
    }
    // TODO: poprawic to
    void set(int pageNo, int offset, std::shared_ptr<Record> rec) {
        std::shared_ptr<bytearray> page = storage->getPage(pageNo);
        std::shared_ptr<bytearray> recordData = rec->serialize();
        if (page == nullptr) {
            page = std::make_shared<bytearray>(RECORD_PAGE_SIZE);
        }
        memcpy(page->arr + offset, recordData->arr, RECORD_SIZE);
        storage->setPage(pageNo, page);
    }
    void insert() {}
    void flush() { storage->flush(); }
};
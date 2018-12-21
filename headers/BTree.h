#ifndef SBD2_BTREE_H
#define SBD2_BTREE_H

#include "CommonHeader.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Record.h"

class Storage;

class DataStorage;

class BTreeStorage;

namespace BTreeNS {
    class BTreeNodeCell {
        int key;
        int page;
        int offset;

    public:
        int child; // TODO przeniesc do private

        BTreeNodeCell(int _key = MAX_SIZE, int _page = MAX_SIZE, int _offset = MAX_SIZE);

        bool isLastElementInNode();

        bool isDefault();

        void swapKeys(std::shared_ptr<BTreeNodeCell> toSwap);

        void swapChildren(std::shared_ptr<BTreeNodeCell> toSwap);

        int getKey();

        int getChild();

        int getPage();

        int getOffset();

        ~BTreeNodeCell() = default;
    };

    class BTreeNode {
    public:
        int index;
        int parent;
        bool leaf;
        std::vector<std::shared_ptr<BTreeNodeCell>> cells;

        //	public:
        BTreeNode(int _index = MAX_SIZE, int _parent = MAX_SIZE, bool _leaf = false);

        int size();

        std::shared_ptr<BTreeNodeCell> insert(std::shared_ptr<BTreeNodeCell> newCell);

        std::shared_ptr<BTreeNodeCell> pop(int _key);

        std::shared_ptr<BTreeNodeCell> getCellByKey(int _key);

        std::shared_ptr<BTreeNodeCell> getCellByChild(int _cell);

        std::shared_ptr<bytearray> serialize();

        static std::shared_ptr<BTreeNode> deserialize(std::shared_ptr<bytearray> data, int index);
    };

    class BTree {
        std::shared_ptr<DataStorage> data;
        std::string name;
        std::shared_ptr<Storage> metadataStorage;

    public:
        std::vector<int> metadata;
        std::shared_ptr<BTreeStorage> btree;

        BTree(std::string _name);

        ~BTree();

        void flush();

        void flushNode(std::shared_ptr<BTreeNode> node);

        void loadMetadata();

        void flushMetadata();

        void syncMetadataStorage();

        void printData();

        std::shared_ptr<BTreeNode> createTree();

        std::shared_ptr<BTreeNode> newNode();

        void refreshReference(std::shared_ptr<BTreeNode> node);

        std::tuple<std::shared_ptr<BTreeNode>, bool> getNodeForKey(int key, bool test = true);

        void insertCellIntoNode(std::shared_ptr<BTreeNodeCell> cell, std::shared_ptr<BTreeNode> node);

        std::vector<int> getSiblings(std::shared_ptr<BTreeNode> node);

        bool compensate(std::shared_ptr<BTreeNode> node);

        void rotateNodes(std::shared_ptr<BTreeNode> node1, std::shared_ptr<BTreeNode> node2);

        void splitNode(std::shared_ptr<BTreeNode>);

        bool rebalance(std::shared_ptr<BTreeNode> node);

        void mergeNodes(std::shared_ptr<BTreeNode> node1, std::shared_ptr<BTreeNode> node2);

        void repairNodeAfterDeletion(std::shared_ptr<BTreeNode> node);

        void deleteKeyFromNode(int key, std::shared_ptr<BTreeNode> node);

        std::shared_ptr<Record> get(int key);

        void set(int key, std::shared_ptr<Record> rec);

        void del(int key);

    };
} // namespace BTreeNS
#endif //SBD2_BTREE_H

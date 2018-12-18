//
// Created by Konrad on 18.12.2018.
//

#ifndef SBD2_BTREE_H
#define SBD2_BTREE_H

#include "CommonHeader.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Record.h"

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

        void insert(std::shared_ptr<BTreeNodeCell> newCell);

        std::shared_ptr<BTreeNodeCell> pop(int _key);

        std::shared_ptr<BTreeNodeCell> getCellByKey(int _key);

        std::shared_ptr<BTreeNodeCell> getCellByChild(int _cell);

        std::shared_ptr<bytearray> serialize();

        static std::shared_ptr<BTreeNode> deserialize(std::shared_ptr<bytearray> data, int index);
    };

    class BTreeMetadata {
    public:
        int element{0};
        int nodes{1};
        int height{1};
        int root{0};
        int btree_next_node{0};
        std::vector<std::shared_ptr<BTreeNode>> btree_free_nodes;
    };

    class BTree {
        std::shared_ptr<BTreeMetadata> metadata;
//    std::shared_ptr<DataStorage> data;
//    std::shared_ptr<BTreeStorage> btree;

        BTree();

        ~BTree();

        void flush();

        void set(int key, std::shared_ptr<Record> rec);

        BTreeNode createTree();
    };
} // namespace BTreeNS
#endif //SBD2_BTREE_H

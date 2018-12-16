#include <iostream>
#include <vector>
#include "Utils.cpp"
#pragma once


namespace BTree {
	class BTreeNodeCell {
		int key;
		int page;
		int offset;
	public:
		int child;	// TODO przeniesc do private

		BTreeNodeCell(int _key = MAX_SIZE, int _page = MAX_SIZE, int _offset = MAX_SIZE) : child(MAX_SIZE), key(_key), page(_page), offset(_offset) {};

		bool isLastElementInNode() {
			return child != MAX_SIZE && key == MAX_SIZE;
		}

		bool isDefault() {
			return child == MAX_SIZE && key == MAX_SIZE && page == MAX_SIZE && offset == MAX_SIZE;
		}

		void swapKeys(BTreeNodeCell* toSwap) {
			std::swap(this->key, toSwap->key);
			std::swap(this->page, toSwap->page);
			std::swap(this->offset, toSwap->offset);
		}
		void swapChildren(BTreeNodeCell* toSwap) {
			std::swap(this->child, toSwap->child);
		}
		int getKey() {
			return key;
		}
		int getChild() {
			return child;
		}
		int getPage() {
			return page;
		}
		int getOffset() {
			return offset;
		}

		~BTreeNodeCell() = default;
	};

	class BTreeNode {
	public:
		int index;
		int parent;
		bool leaf;
		std::vector<BTreeNodeCell*> cells;
		//	public:
		BTreeNode(int _index = MAX_SIZE, int _parent = MAX_SIZE, bool _leaf = false) : index(_index), parent(_parent), leaf(_leaf) {
			cells.push_back(new BTreeNodeCell());
		}
		int size() {
			return cells.size() - 1;
		}

		void insert(BTreeNodeCell* newCell) {
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
		BTreeNodeCell* pop(int _key) {
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
		BTreeNodeCell* getCellByKey(int _key) {
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

		BTreeNodeCell* getCellByChild(int _cell) {
			for (auto cell : cells) {
				if (cell->getChild() == _cell)
					return cell;
			}
		}

		// TODO
		bytearray * serialize() {
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
			bytearray * arr = new bytearray(BTREE_PAGE_SIZE);
			memcpy(arr->arr, &data[0], BTREE_PAGE_SIZE);
			return arr;
		}
		static BTreeNode* deserialize(bytearray* data, int index) {
			BTreeNode * newNode = new BTreeNode(index);
			std::vector<int> newData(BTREE_ELEMENTS_PER_PAGE * 4 + 1);
			memcpy(&newData[0], data->arr, BTREE_PAGE_SIZE);
			newNode->parent = newData[0];
			newNode->cells.clear();
			for (int i = 1; i < newData.size(); i += 4) {
				BTreeNodeCell *cell = new BTreeNodeCell(newData[i + 1], newData[i + 2], newData[i + 3]);
				cell->child = newData[i];
				if (cell->isDefault())
					break;
				newNode->cells.push_back(cell);
			}
			if (newNode->cells.size() < 1 || !newNode->cells.back()->isLastElementInNode())
				newNode->cells.push_back(new BTreeNodeCell());
			return newNode;
		}
	};

	class BTreeMetadata {
	public:
		int element{ 0 };
		int nodes{ 1 };
		int height{ 1 };
		int root{ 0 };
		int btree_next_node{ 0 };
		std::vector<BTreeNode*> btree_free_nodes;
	};

	class BTree {
		BTreeMetadata metadata;
		BTree() {}
		~BTree() {}

		void flush() {}

		BTreeNode createTree() {

		}

	};
}
#include <iostream>
#include <vector>

#define BTREE_D (2)
#define BTREE_2D (2 * BTREE_D)
#define BTREE_ELEMENTS_PER_PAGE (BTREE_2D + 2) // BTREE_2D keys per page + 1 for final child pointer + 1 for overflows.
#define BTREE_PAGE_SIZE ((BTREE_ELEMENTS_PER_PAGE * 4) + 1) * 8
#define MAX_SIZE ~(1 << 31)
namespace BTree {
	class BTreeNodeCell {
		int child;
		int key;
		int page;
		int offset;
	public:
		BTreeNodeCell(int _key = MAX_SIZE, int _page = MAX_SIZE, int _offset = MAX_SIZE) : child(MAX_SIZE), key(_key), page(page), offset(offset) {};

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

		~BTreeNodeCell() = default;
	};

	class BTreeNode {
		int index;
		int parent;
		bool leaf;
		std::vector<BTreeNodeCell*> cells;
	public:
		BTreeNode(int _index = MAX_SIZE, int _parent = MAX_SIZE, bool _leaf = false) : index(_index), parent(_parent), leaf(_leaf) {}
		int size() {
			return cells.size() - 1;
		}

		void insert(BTreeNodeCell* newCell) {
			if (this->size() <= BTREE_2D + 1)
				throw "Cannot insert cell into node! Node has no free space!";

			for (size_t i = 0; i < cells.size(); i++) {
				if (this->cells[i]->isLastElementInNode() || this->cells[i]->getKey() > newCell->getKey()) {
					cells.push_back(newCell);
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
		char * /*byte array*/ serialize() {}
		static BTreeNode deserialize(/*arg*/) {}
	};

	class BTree {
		BTree() {}
		~BTree() {}

		void flush() {}

		BTreeNode createTree() {}

	};
}
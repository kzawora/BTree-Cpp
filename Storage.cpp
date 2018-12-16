#include <string>
#include <fstream>
#include <vector>
#include "Utils.cpp"
#include "BTree.cpp"
#pragma once

class Storage {
	std::string filename;
	int pagesize, filesize, pages;
	std::fstream file;
public:
	Storage(std::string _name, int _pagesize) : filename(_name), pagesize(_pagesize) {
		std::ifstream file_temp(filename, std::fstream::in | std::fstream::out | std::ios::binary | std::ios::ate);
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
	bytearray* getPage(int index) {
		if (index > pages)
			throw "Index out of bounds!";
		file.seekg(index * pagesize, std::ios::beg);
		bytearray* arr = new bytearray(pagesize);
		file.read(arr->arr, pagesize);
		return arr;
	}

	void setPage(int index, bytearray* data) {

		if (index > pages)
			throw "Index too big!";
		if (data->size != pagesize)
			throw "Page data doesn't match page size!";

		if (index < pages) {	// replace page
			file.seekg(index*pagesize, std::ios::beg);
			file.write(data->arr, data->size);
		}
		else {	// append page
			file.seekg(0, std::ios::end);
			file.write(data->arr, data->size);
			pages++;
		}
		file.flush();
	}
	int getPageCount() {
		return pages;
	}
};

class BTreeStorage {
	Storage * storage;
	int nextnode, freenodes;
	std::vector<int> freenodes;
	bytearray* page;

	BTreeStorage(std::string name) {
		name.append(".btree");
		storage = new Storage(name, BTREE_PAGE_SIZE);
		nextnode = storage->getPageCount();
	}
	~BTreeStorage() {
		delete storage;
	}
	BTree::BTreeNode get() {};
	void set(int index, BTree::BTreeNode) {
	//	if()
	}
};

class DataStorage {

};
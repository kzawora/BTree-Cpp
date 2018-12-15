#include "BTree.cpp"
#include "Record.cpp"
#pragma once
int main() {
	for (int i = 0; i < 10; i++) {
		Record r = Record::generate();
		r.print();
	}
	return 0;
}
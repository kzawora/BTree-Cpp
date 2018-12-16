#include "BTree.cpp"
#include "Record.cpp"
#include "Storage.cpp"
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#pragma once
int shell_cmd(std::string cmd) {
	if (cmd == "") {
		std::cout << "You need to type a command." << std::endl;
		return 0;
	}
	std::istringstream iss(cmd);
	std::vector<std::string> words(std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>());
	if (words[0] == "help") {
		std::cout << "help is on the way" << std::endl;
	}
	else if (words[0] == "add") {
		if (words.size() < 3) {
			std::cout << "[ERROR] add requires index and list of numbers." << std::endl;
			return 0;
		}
		auto index = std::stoi(words[1]);
		std::vector<double> values;
		for (int i = 2; i < words.size(); i++) {
			values.push_back(std::stod(words[i]));
		}
		Record r(index, values);
		std::cout << "Adding " << r << "at index " << index << "." << std::endl;
	}
	else if (words[0] == "delete") {
		if (words.size() < 2) {
			std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
			return 0;
		}
		auto count = std::stoi(words[1]);
		std::cout << "Deleting record with index " << count << "." << std::endl;
	}
	else if (words[0] == "random") {
		if (words.size() < 2) {
			std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
			return 0;
		}
		auto count = std::stoi(words[1]);
		std::cout << "Randomly generating " << count << " records." << std::endl;
	}
	else if (words[0] == "dump") {
		std::cout << "dumping..." << std::endl;
	}
	else if (words[0] == "show") {
		std::cout << "show..." << std::endl;
	}
	else if (words[0] == "exit") {
		std::cout << "exiting..." << std::endl;
		return -1;
	}
	else {
		std::cout << "Unrecognized command: " << words[0] << std::endl;
	}
	return 0;
}

int main() {
	/*	for (int i = 0; i < 10; i++) {
		Record r = Record::generate();
		r.print();
	}*/
	std::string input;
	Storage * st = new Storage("file2.bin", 16);
	bytearray* data = new bytearray(16);

//	st->clear();

	for (int i = 0; i < 30; i++)
		st->setPage(i, data);
	
	data->arr[3] = 'a';
	data->arr[2] = 'p';
	data->arr[1] = 'u';
	data->arr[0] = 'd';

	st->setPage(1, data);

	st->setPage(29, data);

	data->arr[0] = 'z';
	st->setPage(3, data);

	bytearray* read = st->getPage(1);
	std::cout << read->arr << std::endl;
	delete read;
	read = st->getPage(3);
	std::cout << read->arr;
	delete read;
	
	st->setPage(0, data);

	delete st;
	delete data;

	/*
	while (true) {
		std::cout << "> ";
		getline(std::cin, input);
		int status = shell_cmd(input);
		if (status == -1) break;
	}
	*/
	return 0;
}
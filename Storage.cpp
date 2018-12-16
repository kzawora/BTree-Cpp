#include <string>
#include <fstream>
#include "Utils.cpp"

class Storage {
	std::string filename;
	int pagesize, filesize, pages;
	std::fstream file;
public:
	Storage(std::string _name, int _pagesize) : filename(_name), pagesize(_pagesize) {
		std::ifstream file_temp(filename, std::ios::binary | std::ios::ate);
		filesize = static_cast<size_t>(file_temp.tellg());
		file_temp.close();
		file.open(filename, std::ios::in | std::ios::binary || std::ios::out | std::ios::app);
		if (filesize%pagesize != 0)
			throw "Size does not match!";
		pages = filesize / pagesize;
	}
	~Storage() {
		file.flush();
		file.close();
	}
	void clear() {
		file.clear();
		file.flush();
		pages = 0;
	}
	bytearray* getPage(int index) {
		if (index > pages)
			throw "Index out of bounds!";
		file.seekg(index * pagesize);
		bytearray* arr = new bytearray(pagesize);
		file.read(reinterpret_cast<char *>(&arr->arr), pagesize);
		return arr;
	}
};

class BTreeStorage {

};

class DataStorage {

};
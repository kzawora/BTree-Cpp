#include "../../headers/Storage.h"
#include "../../headers/CommonHeader.h"
int Storage::reads = 0;
int Storage::writes = 0;

Storage::Storage(std::string _name, int _pagesize) : filename(_name), pagesize(_pagesize) {
    std::ifstream file_temp(filename, std::fstream::in | std::fstream::out | std::ios::app | std::ios::binary |
                                      std::ios::ate);
    filesize = static_cast<size_t>(file_temp.tellg());
    file_temp.close();
    file.open(filename, std::ios::in | std::ios::binary | std::ios::out | std::ios::ate);
    if (filesize % pagesize != 0)
        throw "Size does not match!";
    pages = filesize / pagesize;
}

Storage::~Storage() {
    file.flush();
    file.close();
}

void Storage::clear() {
    file.close();
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open(filename, std::ios::in | std::ios::binary | std::ios::out | std::ios::ate);
    pages = 0;
}

std::shared_ptr<bytearray> Storage::getPage(int index) {
    if (index > pages)
        throw "Index out of bounds!";

    std::shared_ptr<bytearray> arr = std::make_shared<bytearray>(pagesize);
    if (pages > 0) {
        file.seekg(index * pagesize, std::ios::beg);
        file.read(arr->arr, pagesize);
    }
    reads++;
    return arr;
}

void Storage::flush() { file.flush(); }

void Storage::setPage(int index, std::shared_ptr<bytearray> data) {

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
    writes++;
}

int Storage::getPageCount() { return pages; }


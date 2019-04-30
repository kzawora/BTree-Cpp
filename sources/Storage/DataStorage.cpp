#include "../../headers/Storage.h"
#include "../../headers/CommonHeader.h"

DataStorage::DataStorage(std::string name) : nextoffset(0) {
    name.append(".data");
    storage = std::make_shared<Storage>(name, RECORD_PAGE_SIZE);
    nextpage = storage->getPageCount();
}

void DataStorage::clear() {
    storage->clear();
    nextpage = 0;
    nextoffset = 0;
}

std::shared_ptr<Record> DataStorage::get(int pageNo, int offset) {
    std::shared_ptr<bytearray> page = storage->getPage(pageNo);
    std::shared_ptr<bytearray> recordData = std::make_shared<bytearray>(RECORD_SIZE);
    memcpy(recordData->arr, page->arr + offset, RECORD_SIZE);
    return Record::deserialize(recordData);
}

// TODO: poprawic to
void DataStorage::set(int pageNo, int offset, std::shared_ptr<Record> rec) {
    std::shared_ptr<bytearray> page;
    std::shared_ptr<bytearray> recordData = rec->serialize();
    if (storage->getPageCount() == pageNo) {
        page = std::make_shared<bytearray>(RECORD_PAGE_SIZE);
    } else {
        page = storage->getPage(pageNo);
    }
    memcpy(page->arr + offset, recordData->arr, RECORD_SIZE);
    storage->setPage(pageNo, page);
}

std::tuple<int, int> DataStorage::insert(std::shared_ptr<Record> rec) {
    int page = nextpage;
    int offset = nextoffset;
    nextoffset += RECORD_SIZE;
    if (nextoffset == RECORD_PAGE_SIZE) {
        nextpage++;
        nextoffset = 0;
    }
    set(page, offset, rec);
    return std::tuple<int, int>(page, offset);
}

void DataStorage::flush() { storage->flush(); }

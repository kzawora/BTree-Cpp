#ifndef SBD2_RECORD_H
#define SBD2_RECORD_H

#include "CommonHeader.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <memory>



class Record {
    int index;
    std::vector<double> values;

public:
    Record();

    Record(int _index, std::vector<double> vals);

    void print();

    friend std::ostream &operator<<(std::ostream &stream, const Record &record);

    static std::shared_ptr<Record> generate();

    std::shared_ptr<bytearray> serialize();

    static std::shared_ptr<Record> deserialize(std::shared_ptr<bytearray> data);

};

#endif //SBD2_RECORD_H

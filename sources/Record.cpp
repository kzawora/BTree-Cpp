#include "../headers/Record.h"

Record::Record() = default;

Record::Record(int _index, std::vector<double> vals) : index(_index) {
    for (size_t i = 0; i < MAX_RECORD_SIZE; i++) {
        if (i < vals.size())
            values.push_back(vals[i]);
        else
            values.push_back(std::nan(0));
    }
}

void Record::print() { std::cout << *this << std::endl; }

std::ostream &operator<<(std::ostream &stream, const Record &record) {
    stream << "RECORD " << record.index << ": ";
    for (auto i = record.values.begin(); i != record.values.end(); ++i)
        if (!std::isnan(*i))
            stream << *i << ' ';
    return stream;
}

std::shared_ptr<Record> Record::generate() {
    std::shared_ptr<Record> r = std::make_shared<Record>();
    std::default_random_engine gen(static_cast<unsigned int>(clock())); // clock()
    std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT, DIST_UPPER_LIMIT);
    auto generator = [&]() { return dist(gen); };
    for (int i = 0; i < MAX_RECORD_SIZE; i++) {
        r->values.push_back(generator());
    }
    return r;
}

std::shared_ptr<bytearray> Record::serialize() {
    std::vector<double> data;
    int dummies = MAX_RECORD_SIZE - values.size();
    for (auto value : values) {
        data.push_back(value);
    }
    for (int i = 0; i < dummies; i++) {
        data.push_back(std::nan(0));
    }
    std::shared_ptr<bytearray> arr = std::make_shared<bytearray>(RECORD_SIZE);
    memcpy(arr->arr, &index, sizeof(int));
    memcpy(arr->arr + sizeof(int), &data[0], MAX_RECORD_SIZE * sizeof(double));
    return arr;
}

std::shared_ptr<Record> Record::deserialize(std::shared_ptr<bytearray> data) {
    int index = 0;
    double numbers[MAX_RECORD_SIZE];
    memcpy(&index, data->arr, sizeof(int));
    memcpy(&numbers, data->arr + sizeof(int), MAX_RECORD_SIZE * sizeof(double));
    std::vector<double> dataVec(numbers, numbers + sizeof(numbers) / sizeof(numbers[0]));
    std::shared_ptr<Record> newRecord = std::make_shared<Record>(index, dataVec);
    return newRecord;
}

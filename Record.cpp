#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include <ctime>
#include "Utils.cpp"

#define MAX_RECORD_SIZE 15
#define DIST_LOWER_LIMIT -10
#define DIST_UPPER_LIMIT 10
class Record {
	int index;
	std::vector<double> values;
public:
	Record() = default;
	Record(int _index, std::vector<double> vals) : index(_index) {
		for (size_t i = 0; i < MAX_RECORD_SIZE; i++) {
			if (i < vals.size()) values.push_back(vals[i]);
			else values.push_back(std::nan(0));
		}
	}
	void print() {
		std::cout << *this << std::endl;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Record& record) {
		stream << "RECORD " << record.index << ": ";
		for (auto i = record.values.begin(); i != record.values.end(); ++i)
			if (!std::isnan(*i))
				stream << *i << ' ';
		return stream;
	}

	static Record generate() {
		Record r;
		std::default_random_engine gen(static_cast<unsigned int>(clock())); // clock()
		std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
			DIST_UPPER_LIMIT);
		auto generator = [&]() { return dist(gen); };
		for (int i = 0; i < MAX_RECORD_SIZE; i++) {
			r.values.push_back(generator());
		}
		return r;
	}

	bytearray* serialize() {
		
	}
	static Record deserialize(/*arg*/) {}
};